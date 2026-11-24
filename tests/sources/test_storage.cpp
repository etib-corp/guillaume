/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "test_storage.hpp"

#include <chrono>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "test_environment.hpp"

#if !defined(_WIN32)
	#include <sys/wait.h>
	#include <unistd.h>
#endif

namespace guillaume::tests
{

	namespace
	{
		/// An environment made of the given variables only.
		LocalStorage::EnvironmentReader
			environmentOf(std::map<std::string, std::string> variables)
		{
			return [variables = std::move(variables)](
					   const std::string &name) -> std::optional<std::string> {
				const auto found = variables.find(name);
				if (found == variables.end()) {
					return std::nullopt;
				}
				return found->second;
			};
		}

		const std::filesystem::path fallbackDirectory = "fallback";
	}	 // namespace

	void TestStorage::SetUp(void)
	{
		const auto timestamp =
			std::chrono::steady_clock::now().time_since_epoch().count();
		_localStoragePath = std::filesystem::temp_directory_path()
			/ ("guillaume-storage-test-" + std::to_string(timestamp) + ".db");

		SessionStorage sessionStorage;
		sessionStorage.clear();
	}

	void TestStorage::TearDown(void)
	{
		std::error_code errorCode;
		std::filesystem::remove(_localStoragePath, errorCode);

		SessionStorage sessionStorage;
		sessionStorage.clear();
	}

	TEST_F(TestStorage, LocalStorageSetAndGetItem)
	{
		LocalStorage localStorage(_localStoragePath);

		localStorage.setItem("username", "clement");

		const auto value = localStorage.getItem("username");
		ASSERT_TRUE(value.has_value());
		EXPECT_EQ(*value, "clement");
	}

	TEST_F(TestStorage, LocalStorageReturnsNulloptForMissingKey)
	{
		LocalStorage localStorage(_localStoragePath);

		const auto value = localStorage.getItem("missing");
		EXPECT_FALSE(value.has_value());
	}

	TEST_F(TestStorage, LocalStorageRemoveItem)
	{
		LocalStorage localStorage(_localStoragePath);
		localStorage.setItem("token", "abc123");

		localStorage.removeItem("token");

		EXPECT_FALSE(localStorage.getItem("token").has_value());
	}

	TEST_F(TestStorage, LocalStorageClearRemovesAllItems)
	{
		LocalStorage localStorage(_localStoragePath);
		localStorage.setItem("first", "one");
		localStorage.setItem("second", "two");

		localStorage.clear();

		EXPECT_FALSE(localStorage.getItem("first").has_value());
		EXPECT_FALSE(localStorage.getItem("second").has_value());
	}

	TEST_F(TestStorage, LocalStoragePersistsBetweenInstances)
	{
		{
			LocalStorage firstInstance(_localStoragePath);
			firstInstance.setItem("theme", "dark");
		}

		LocalStorage secondInstance(_localStoragePath);
		const auto value = secondInstance.getItem("theme");

		ASSERT_TRUE(value.has_value());
		EXPECT_EQ(*value, "dark");
	}

	TEST_F(TestStorage, LocalStorageSupportsTypedValues)
	{
		LocalStorage localStorage(_localStoragePath);

		localStorage.setItem("volume", 75);
		localStorage.setItem("gamma", 1.25);
		localStorage.setItem("enabled", true);

		const auto rawVolume = localStorage.getItem("volume");
		ASSERT_TRUE(rawVolume.has_value());
		EXPECT_EQ(*rawVolume, "75");

		const auto volume  = localStorage.getItemAs<int>("volume");
		const auto gamma   = localStorage.getItemAs<double>("gamma");
		const auto enabled = localStorage.getItemAs<bool>("enabled");

		ASSERT_TRUE(volume.has_value());
		ASSERT_TRUE(gamma.has_value());
		ASSERT_TRUE(enabled.has_value());
		EXPECT_EQ(*volume, 75);
		EXPECT_DOUBLE_EQ(*gamma, 1.25);
		EXPECT_TRUE(*enabled);
	}

	TEST_F(TestStorage, SessionStorageSetAndGetItem)
	{
		SessionStorage sessionStorage;

		sessionStorage.setItem("language", "fr");

		const auto value = sessionStorage.getItem("language");
		ASSERT_TRUE(value.has_value());
		EXPECT_EQ(*value, "fr");
	}

	TEST_F(TestStorage, SessionStorageRemoveItem)
	{
		SessionStorage sessionStorage;
		sessionStorage.setItem("cacheKey", "value");

		sessionStorage.removeItem("cacheKey");

		EXPECT_FALSE(sessionStorage.getItem("cacheKey").has_value());
	}

	TEST_F(TestStorage, SessionStorageClearRemovesAllItems)
	{
		SessionStorage sessionStorage;
		sessionStorage.setItem("a", "1");
		sessionStorage.setItem("b", "2");

		sessionStorage.clear();

		EXPECT_FALSE(sessionStorage.getItem("a").has_value());
		EXPECT_FALSE(sessionStorage.getItem("b").has_value());
	}

	TEST_F(TestStorage, SessionStorageIsSharedInCurrentSession)
	{
		SessionStorage firstInstance;
		SessionStorage secondInstance;

		firstInstance.setItem("mode", "editor");

		const auto value = secondInstance.getItem("mode");
		ASSERT_TRUE(value.has_value());
		EXPECT_EQ(*value, "editor");
	}

	TEST_F(TestStorage, SessionStorageSupportsTypedValues)
	{
		SessionStorage sessionStorage;

		sessionStorage.setItem("attempts", 3);
		sessionStorage.setItem("ratio", 0.5f);
		sessionStorage.setItem("ok", false);

		const auto rawAttempts = sessionStorage.getItem("attempts");
		ASSERT_TRUE(rawAttempts.has_value());
		EXPECT_EQ(*rawAttempts, "3");

		const auto attempts = sessionStorage.getItemAs<int>("attempts");
		const auto ratio	= sessionStorage.getItemAs<float>("ratio");
		const auto ok		= sessionStorage.getItemAs<bool>("ok");

		ASSERT_TRUE(attempts.has_value());
		ASSERT_TRUE(ratio.has_value());
		ASSERT_TRUE(ok.has_value());
		EXPECT_EQ(*attempts, 3);
		EXPECT_FLOAT_EQ(*ratio, 0.5f);
		EXPECT_FALSE(*ok);
	}

	TEST_F(TestStorage, LocalStorageIsPersistentWhenTheFileOpens)
	{
		LocalStorage localStorage(_localStoragePath);

		EXPECT_TRUE(localStorage.isPersistent());
	}

	TEST_F(TestStorage, LocalStorageFallsBackToMemoryWhenTheFileCannotOpen)
	{
		// A directory is not a valid database file.
		const auto unusable = std::filesystem::temp_directory_path();

		LocalStorage localStorage(unusable);

		EXPECT_FALSE(localStorage.isPersistent());

		// The fallback still behaves like a storage.
		localStorage.setItem("key", "value");
		ASSERT_TRUE(localStorage.getItem("key").has_value());
		EXPECT_EQ(*localStorage.getItem("key"), "value");
	}

	TEST_F(TestStorage, LocalStorageFallsBackWhenTheDirectoryCannotBeCreated)
	{
		// The parent of the database path is a regular file.
		const auto blocker = _localStoragePath;
		{
			LocalStorage create(blocker);
		}
		ASSERT_TRUE(std::filesystem::is_regular_file(blocker));

		std::unique_ptr<LocalStorage> localStorage;
		ASSERT_NO_THROW(localStorage = std::make_unique<LocalStorage>(
							blocker / "nested" / "storage.db"));

		EXPECT_FALSE(localStorage->isPersistent());
	}

	TEST_F(TestStorage, UserDataDirectoryOnWindowsIsLocalAppData)
	{
		const std::string localAppData = "C:\\Users\\ada\\AppData\\Local";

		EXPECT_EQ(LocalStorage::userDataDirectory(
					  LocalStorage::Platform::Windows,
					  environmentOf({ { "LOCALAPPDATA", localAppData },
									  { "HOME", "/home/ada" } }),
					  fallbackDirectory),
				  std::filesystem::path(localAppData));
	}

	TEST_F(TestStorage, UserDataDirectoryOnLinuxPrefersXdgDataHome)
	{
		EXPECT_EQ(LocalStorage::userDataDirectory(
					  LocalStorage::Platform::Linux,
					  environmentOf({ { "XDG_DATA_HOME", "/data/ada" },
									  { "HOME", "/home/ada" } }),
					  fallbackDirectory),
				  std::filesystem::path("/data/ada"));
	}

	TEST_F(TestStorage, UserDataDirectoryOnLinuxDefaultsToLocalShare)
	{
		const auto expected =
			std::filesystem::path("/home/ada") / ".local" / "share";

		EXPECT_EQ(LocalStorage::userDataDirectory(
					  LocalStorage::Platform::Linux,
					  environmentOf({ { "HOME", "/home/ada" } }),
					  fallbackDirectory),
				  expected);
		// An empty or relative XDG_DATA_HOME is ignored.
		EXPECT_EQ(LocalStorage::userDataDirectory(
					  LocalStorage::Platform::Linux,
					  environmentOf(
						  { { "XDG_DATA_HOME", "" }, { "HOME", "/home/ada" } }),
					  fallbackDirectory),
				  expected);
		EXPECT_EQ(LocalStorage::userDataDirectory(
					  LocalStorage::Platform::Linux,
					  environmentOf({ { "XDG_DATA_HOME", "relative/data" },
									  { "HOME", "/home/ada" } }),
					  fallbackDirectory),
				  expected);
	}

	TEST_F(TestStorage, UserDataDirectoryOnMacOsIsApplicationSupport)
	{
		EXPECT_EQ(LocalStorage::userDataDirectory(
					  LocalStorage::Platform::MacOS,
					  environmentOf({ { "HOME", "/Users/ada" },
									  { "XDG_DATA_HOME", "/data/ada" } }),
					  fallbackDirectory),
				  std::filesystem::path("/Users/ada") / "Library"
					  / "Application Support");
	}

	TEST_F(TestStorage, UserDataDirectoryFallsBackWithoutTheVariables)
	{
		for (const auto platform:
			 { LocalStorage::Platform::Windows, LocalStorage::Platform::Linux,
			   LocalStorage::Platform::MacOS }) {
			EXPECT_EQ(LocalStorage::userDataDirectory(
						  platform, environmentOf({}), fallbackDirectory),
					  fallbackDirectory);
			EXPECT_EQ(
				LocalStorage::userDataDirectory(
					platform,
					environmentOf({ { "LOCALAPPDATA", "" }, { "HOME", "" } }),
					fallbackDirectory),
				fallbackDirectory);
			EXPECT_EQ(LocalStorage::userDataDirectory(platform, nullptr,
													  fallbackDirectory),
					  fallbackDirectory);
		}
	}

	TEST_F(TestStorage, DefaultStoragePathIsNotInTheWorkingDirectory)
	{
		const auto path = LocalStorage::defaultStoragePath();

		EXPECT_TRUE(path.is_absolute());
		EXPECT_NE(path.parent_path(), std::filesystem::current_path());
		EXPECT_EQ(path.filename(), "local-storage.db");
		EXPECT_EQ(path.parent_path().filename(), "guillaume");
	}

	TEST_F(TestStorage, DefaultStoragePathStaysInTheTestDataDirectory)
	{
		// The test program redirects the per-user data directory (see
		// TestEnvironment): a default storage must land inside it.
		const auto path = LocalStorage::defaultStoragePath();
		const auto relative =
			path.lexically_relative(TestEnvironment::getDataDirectory());

		ASSERT_FALSE(relative.empty());
		EXPECT_NE(*relative.begin(), "..");
	}

	TEST_F(TestStorage, DefaultLocalStorageCreatesItsDirectory)
	{
		const auto path = LocalStorage::defaultStoragePath();

		LocalStorage localStorage;

		EXPECT_TRUE(localStorage.isPersistent());
		EXPECT_TRUE(std::filesystem::is_regular_file(path));
	}

	TEST_F(TestStorage, LocalStorageReportsAFailedStatement)
	{
		LocalStorage localStorage(_localStoragePath);

		// Run each statement once so that it is prepared and cached: the
		// failure below then comes from the step, not from the preparation.
		localStorage.setItem("key", "value");
		ASSERT_TRUE(localStorage.getItem("key").has_value());
		localStorage.removeItem("key");

		// Another connection removes the table from under the storage.
		sqlite3 *other = nullptr;
		ASSERT_EQ(sqlite3_open(_localStoragePath.string().c_str(), &other),
				  SQLITE_OK);
		ASSERT_EQ(sqlite3_exec(other, "DROP TABLE local_storage;", nullptr,
							   nullptr, nullptr),
				  SQLITE_OK);
		sqlite3_close(other);

		EXPECT_THROW(localStorage.setItem("key", "value"), std::runtime_error);
		EXPECT_THROW(localStorage.getItem("key"), std::runtime_error);
		EXPECT_THROW(localStorage.removeItem("key"), std::runtime_error);
	}

	TEST_F(TestStorage, LocalStorageWorksAgainAfterAFailedStatement)
	{
		LocalStorage localStorage(_localStoragePath);
		localStorage.setItem("key", "value");

		sqlite3 *other = nullptr;
		ASSERT_EQ(sqlite3_open(_localStoragePath.string().c_str(), &other),
				  SQLITE_OK);
		ASSERT_EQ(sqlite3_exec(other, "DROP TABLE local_storage;", nullptr,
							   nullptr, nullptr),
				  SQLITE_OK);
		EXPECT_THROW(localStorage.setItem("key", "value"), std::runtime_error);

		ASSERT_EQ(sqlite3_exec(other,
							   "CREATE TABLE local_storage ("
							   "key TEXT PRIMARY KEY, value TEXT NOT NULL);",
							   nullptr, nullptr, nullptr),
				  SQLITE_OK);
		sqlite3_close(other);

		EXPECT_NO_THROW(localStorage.setItem("key", "again"));
		ASSERT_TRUE(localStorage.getItem("key").has_value());
		EXPECT_EQ(*localStorage.getItem("key"), "again");
	}

	TEST_F(TestStorage, LocalStorageReadDoesNotKeepTheFileLocked)
	{
		LocalStorage reader(_localStoragePath);
		LocalStorage writer(_localStoragePath);

		reader.setItem("key", "value");
		ASSERT_TRUE(reader.getItem("key").has_value());

		// A read that found a row used to leave its statement active, and
		// with it a read lock that made the other connection's write fail
		// once the busy timeout had run out.
		const auto start = std::chrono::steady_clock::now();
		EXPECT_NO_THROW(writer.setItem("other", "written"));
		const auto elapsed = std::chrono::steady_clock::now() - start;

		EXPECT_LT(elapsed, std::chrono::seconds(2));
		ASSERT_TRUE(writer.getItem("other").has_value());
		EXPECT_EQ(*writer.getItem("other"), "written");
	}

#if !defined(_WIN32)
	TEST_F(TestStorage, LocalStorageSurvivesConcurrentFirstOpenByProcesses)
	{
		// Several processes opening the same file for the first time used
		// to fail with SQLITE_BUSY while the schema was created. Processes,
		// not threads: that is the real case (an app started twice, tests
		// run in parallel).
		constexpr int processCount = 8;
		std::vector<pid_t> children;

		for (int index = 0; index < processCount; ++index) {
			const pid_t child = fork();
			ASSERT_GE(child, 0);
			if (child == 0) {
				int status = 0;
				try {
					LocalStorage localStorage(_localStoragePath);
					localStorage.setItem("key" + std::to_string(index),
										 "value");
				} catch (...) {
					status = 1;
				}
				_exit(status);
			}
			children.push_back(child);
		}

		int failures = 0;
		for (const pid_t child: children) {
			int status = 0;
			waitpid(child, &status, 0);
			if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
				++failures;
			}
		}

		EXPECT_EQ(failures, 0);
	}
#endif

}	 // namespace guillaume::tests
