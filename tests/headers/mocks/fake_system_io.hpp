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

#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <utility/system_io/default_system_io.hpp>
#include <utility/system_io/file.hpp>

namespace guillaume::tests
{
	/**
	 * @brief SystemIO test double that serves in-memory file contents.
	 *
	 * Paths registered in @ref files are answered with a `utility::File`
	 * carrying the configured content; every other path falls back to the
	 * default filesystem-backed implementation. This lets font/codepoints
	 * assets be faked without touching the disk.
	 */
	class FakeSystemIO: public utility::DefaultSystemIO
	{
		public:
		std::map<std::string, std::string> files;	 ///< In-memory files

		/**
		 * @brief Register a virtual path backed by a file on disk.
		 * @param virtualPath The path requested by the code under test.
		 * @param realPath The path of the file to read the content from.
		 */
		void addFileFromDisk(const std::string &virtualPath,
							 const std::string &realPath)
		{
			std::ifstream file(realPath, std::ios::binary);
			std::ostringstream stream;
			stream << file.rdbuf();
			files[virtualPath] = stream.str();
		}

		/**
		 * @brief Load a file, returning an in-memory asset when registered.
		 * @param path The path of the file to load.
		 * @return The loaded file, or nullptr when unavailable.
		 */
		std::shared_ptr<utility::File> add(const std::string &path) override
		{
			const auto file = files.find(path);
			if (file != files.end()) {
				return std::make_shared<utility::File>(path, file->second);
			}
			return utility::DefaultSystemIO::add(path);
		}
	};

}	 // namespace guillaume::tests
