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
	 * @brief Key-value storage that lasts as long as the process: the place
	 * for data shared between scenes that need not survive a restart.
	 *
	 * All SessionStorage objects of a process read and write the same
	 * items: creating a second instance gives another handle on the same
	 * data, not a second store, and destroying an instance deletes nothing.
	 * The typed overloads of Storage (numbers, booleans, getItemAs) are
	 * available.
	 *
	 * @code
	 * guillaume::SessionStorage first;
	 * guillaume::SessionStorage second;
	 *
	 * first.setItem("step", 3);
	 * second.getItemAs<int>("step");	 // 3
	 * @endcode
	 *
	 * @note Every operation takes a lock, so the storage can be used from
	 * several threads. clear() empties it for the whole process.
	 * @see Storage
	 * @see LocalStorage
	 */
	class SessionStorage: public Storage
	{
		private:
		static std::unordered_map<std::string, std::string>
			_storage;				 ///< The items, shared by every instance
		static std::mutex _mutex;	 ///< Guards every access to _storage

		public:
		using Storage::setItem;

		/**
		 * @brief Create a handle on the process-wide session items.
		 */
		SessionStorage(void) = default;

		/**
		 * @brief Destroy the handle. The items are kept.
		 */
		~SessionStorage(void) override = default;

		/**
		 * @brief Store a string under a key, replacing the item already
		 * stored under that key, if any.
		 * @param key The name of the item.
		 * @param value The text to store.
		 */
		void setItem(const std::string &key, const std::string &value) override;

		/**
		 * @brief Read the text stored under a key.
		 * @param key The name of the item.
		 * @return A copy of the stored text, or `std::nullopt` when no item
		 * has that key.
		 */
		std::optional<std::string> getItem(const std::string &key) override;

		/**
		 * @brief Delete the item stored under a key. Does nothing when there
		 * is none.
		 * @param key The name of the item.
		 */
		void removeItem(const std::string &key) override;

		/**
		 * @brief Delete every session item of the process, including those
		 * written through other SessionStorage objects.
		 */
		void clear(void) override;
	};

}	 // namespace guillaume
