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

#include "test_environment.hpp"

#include <chrono>
#include <cstdlib>
#include <random>
#include <string>
#include <system_error>

namespace guillaume::tests
{

	namespace
	{
		void setVariable(const char *name, const std::string &value)
		{
#if defined(_WIN32)
			_putenv_s(name, value.c_str());
#else
			setenv(name, value.c_str(), 1);
#endif
		}

		// Registered while the test program starts, so that it also applies
		// with the `main` of gtest_main. GoogleTest owns the object.
		const ::testing::Environment *const registered =
			::testing::AddGlobalTestEnvironment(new TestEnvironment());
	}	 // namespace

	const std::filesystem::path &TestEnvironment::getDataDirectory(void)
	{
		return _dataDirectory;
	}

	void TestEnvironment::SetUp(void)
	{
		const auto timestamp =
			std::chrono::steady_clock::now().time_since_epoch().count();
		std::random_device random;

		_dataDirectory = std::filesystem::temp_directory_path()
			/ ("guillaume-test-data-" + std::to_string(timestamp) + "-"
			   + std::to_string(random()));
		std::filesystem::create_directories(_dataDirectory);

		// Every variable LocalStorage::defaultStoragePath may read.
		setVariable("LOCALAPPDATA", _dataDirectory.string());
		setVariable("XDG_DATA_HOME", _dataDirectory.string());
		setVariable("HOME", _dataDirectory.string());
	}

	void TestEnvironment::TearDown(void)
	{
		std::error_code ignored;
		std::filesystem::remove_all(_dataDirectory, ignored);
	}

	std::filesystem::path TestEnvironment::_dataDirectory;

}	 // namespace guillaume::tests
