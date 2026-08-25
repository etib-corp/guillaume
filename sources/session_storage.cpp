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

#include "guillaume/session_storage.hpp"

namespace guillaume
{

	std::unordered_map<std::string, std::string> SessionStorage::_storage;
	std::mutex SessionStorage::_mutex;

	void SessionStorage::setItem(const std::string &key,
								 const std::string &value)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_storage[key] = value;
	}

	std::optional<std::string> SessionStorage::getItem(const std::string &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		auto it = _storage.find(key);
		if (it == _storage.end()) {
			return std::nullopt;
		}
		return it->second;
	}

	void SessionStorage::removeItem(const std::string &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_storage.erase(key);
	}

	void SessionStorage::clear(void)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_storage.clear();
	}

}	 // namespace guillaume
