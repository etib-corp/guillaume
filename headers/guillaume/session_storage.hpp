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

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "storage.hpp"

namespace guillaume
{

	/**
	 * @brief Session-scoped key-value storage.
	 *
	 * Values in this storage are shared between SessionStorage instances in the
	 * same running process and are cleared when the process ends.
	 */
	class SessionStorage: public Storage
	{
		private:
		static std::unordered_map<std::string, std::string>
			_storage;				 ///< Shared in-memory storage map
		static std::mutex _mutex;	 ///< Synchronizes access to shared storage

		public:
		using Storage::setItem;

		/**
		 * @brief Default constructor.
		 */
		SessionStorage(void) = default;

		/**
		 * @brief Virtual destructor.
		 */
		~SessionStorage(void) override = default;

		/**
		 * @brief Store a value for a key.
		 * @param key Storage key.
		 * @param value String value to store.
		 */
		void setItem(const std::string &key, const std::string &value) override;

		/**
		 * @brief Retrieve a value for a key.
		 * @param key Storage key.
		 * @return Stored value when found, std::nullopt otherwise.
		 */
		std::optional<std::string> getItem(const std::string &key) override;

		/**
		 * @brief Remove a key and its value.
		 * @param key Storage key.
		 */
		void removeItem(const std::string &key) override;

		/**
		 * @brief Remove all keys and values.
		 */
		void clear(void) override;
	};

}	 // namespace guillaume
