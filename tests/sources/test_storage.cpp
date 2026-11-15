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
#include <memory>
#include <string>
#include <vector>

#if !defined(_WIN32)
	#include <sys/wait.h>
	#include <unistd.h>
#endif

namespace guillaume::tests
{

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
