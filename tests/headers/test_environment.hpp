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

#pragma once

#include <filesystem>

#include <gtest/gtest.h>

namespace guillaume::tests
{

	/**
	 * @brief Keeps the whole test program away from the per-user data
	 * directory of the person running it.
	 *
	 * A LocalStorage built without a path (the one of every SceneManager,
	 * hence of every Application) opens a file under that directory. Before
	 * the first test, the environment variables it is worked out from are
	 * pointed at a temporary directory of this process, which is deleted
	 * after the last test.
	 */
	class TestEnvironment: public ::testing::Environment
	{
		public:
		/**
		 * @brief Get the temporary directory that stands for the per-user
		 * data directory while the tests run.
		 * @return The directory; empty before SetUp().
		 */
		static const std::filesystem::path &getDataDirectory(void);

		void SetUp(void) override;
		void TearDown(void) override;

		private:
		static std::filesystem::path
			_dataDirectory;	   ///< Stand-in for the per-user data directory
	};

}	 // namespace guillaume::tests
