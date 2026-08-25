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

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

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
	class LocalStorage:
		public Storage,
		public utility::logging::Loggable<LocalStorage,
										  utility::logging::DefaultLogger>
	{
		private:
		std::filesystem::path
			_storageFilePath;		  ///< Backing SQLite database file
		sqlite3 *_database;			  ///< SQLite connection handle
		mutable std::mutex _mutex;	  ///< Synchronizes SQLite access

		/**
		 * @brief Initialize database schema.
		 * @throws std::runtime_error If schema initialization fails, an
		 * exception is thrown with details about the failure.
		 */
		void initializeSchema(void);

		/**
		 * @brief Execute a SQL statement that does not return rows.
		 * @param sql SQL statement to execute.
		 * @return True on success, false otherwise.
		 * @throws std::runtime_error If execution fails, an exception is thrown
		 * with details about the failure.
		 */
		bool executeStatement(const std::string &sql) const noexcept(false);

		public:
		using Storage::setItem;

		/**
		 * @brief Construct a new LocalStorage object.
		 * @param storageFilePath Backing SQLite database file path.
		 * @throws std::runtime_error If database connection fails, an exception
		 * is thrown with details about the failure.
		 * @note If the specified database file does not exist, it will be
		 * created automatically.
		 */
		explicit LocalStorage(const std::filesystem::path &storageFilePath =
								  defaultStoragePath()) noexcept(false);

		/**
		 * @brief Virtual destructor.
		 */
		~LocalStorage(void) override;

		/**
		 * @brief Store a value for a key.
		 * @param key Storage key.
		 * @param value String value to store.
		 * @throws std::runtime_error If the operation fails, an exception is
		 * thrown with details about the failure.
		 */
		void setItem(const std::string &key,
					 const std::string &value) noexcept(false) override;

		/**
		 * @brief Retrieve a value for a key.
		 * @param key Storage key.
		 * @return Stored value when found, std::nullopt otherwise.
		 * @throws std::runtime_error If the operation fails, an exception is
		 * thrown with details about the failure.
		 */
		std::optional<std::string>
			getItem(const std::string &key) noexcept(false) override;

		/**
		 * @brief Remove a key and its value.
		 * @param key Storage key.
		 * @throws std::runtime_error If the operation fails, an exception is
		 * thrown with details about the failure.
		 */
		void removeItem(const std::string &key) noexcept(false) override;

		/**
		 * @brief Remove all keys and values.
		 * @throws std::runtime_error If the operation fails, an exception is
		 * thrown with details about the failure.
		 */
		void clear(void) noexcept(false) override;

		/**
		 * @brief Get the default local storage path.
		 * @return Default storage path.
		 * @throws std::runtime_error If the default path cannot be determined,
		 * an exception is thrown with details about the failure.
		 */
		static std::filesystem::path defaultStoragePath(void) noexcept(false);
	};

}	 // namespace guillaume
