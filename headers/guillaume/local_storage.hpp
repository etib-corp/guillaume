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
#include <mutex>
#include <optional>
#include <string>

#include "storage.hpp"

struct sqlite3;

namespace guillaume
{

	/**
	 * @brief Persistent key-value storage backed by an SQLite database.
	 *
	 * Values stored in this class survive across application restarts as long
	 * as the same database file path is used.
	 */
	class LocalStorage: public Storage
	{
		private:
		std::filesystem::path
			_storageFilePath;		  ///< Backing SQLite database file
		sqlite3 *_database;			  ///< SQLite connection handle
		mutable std::mutex _mutex;	  ///< Synchronizes SQLite access

		/**
		 * @brief Initialize database schema.
		 */
		void initializeSchema(void);

		/**
		 * @brief Execute a SQL statement that does not return rows.
		 * @param sql SQL statement to execute.
		 * @return True on success, false otherwise.
		 */
		bool executeStatement(const std::string &sql) const;

		public:
		using Storage::setItem;

		/**
		 * @brief Construct a new LocalStorage object.
		 * @param storageFilePath Backing SQLite database file path.
		 */
		explicit LocalStorage(const std::filesystem::path &storageFilePath =
								  defaultStoragePath());

		/**
		 * @brief Virtual destructor.
		 */
		~LocalStorage(void) override;

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

		/**
		 * @brief Get the default local storage path.
		 * @return Default storage path.
		 */
		static std::filesystem::path defaultStoragePath(void);
	};

}	 // namespace guillaume
