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
struct sqlite3_stmt;

namespace guillaume
{

	/**
	 * @brief Key-value storage kept in a file: the place for data that must
	 * be found again at the next start (preferences, a saved session).
	 *
	 * The items live in an SQLite database file, by default
	 * `.guillaume-local-storage.db` in the current working directory. Two
	 * LocalStorage objects opened on the same path see the same items. The
	 * typed overloads of Storage (numbers, booleans, getItemAs) are
	 * available.
	 *
	 * @code
	 * guillaume::LocalStorage storage("settings.db");
	 *
	 * storage.setItem("theme", "dark");
	 * const auto theme = storage.getItem("theme");	   // "dark", also after
	 * 												   // a restart
	 * @endcode
	 *
	 * Things to watch out for:
	 * - When the file cannot be opened or created, the storage logs an
	 *   error and works in memory instead: nothing is saved. Check
	 *   isPersistent() when that matters.
	 * - Every operation takes a lock, so one object can be used from
	 *   several threads. Another process holding the file is waited for,
	 *   up to five seconds.
	 * - The class owns a database connection and is not meant to be copied.
	 *
	 * @see Storage
	 * @see SessionStorage
	 */
	class LocalStorage:
		public Storage,
		public utility::logging::Loggable<LocalStorage,
										  utility::logging::DefaultLogger>
	{
		private:
		std::filesystem::path
			_storageFilePath;	 ///< Path of the SQLite database file asked for
		sqlite3 *_database;		 ///< SQLite connection (file or in-memory)
		mutable std::mutex _mutex;	  ///< Guards every use of the connection
		bool _persistent {
			true
		};	  ///< False when the file could not be opened and the items
			  ///< are kept in memory only
		sqlite3_stmt *_setItemStmt {
			nullptr
		};	  ///< Statement of setItem, prepared on first use and reused
		sqlite3_stmt *_getItemStmt {
			nullptr
		};	  ///< Statement of getItem, prepared on first use and reused
		sqlite3_stmt *_removeItemStmt {
			nullptr
		};	  ///< Statement of removeItem, prepared on first use and reused

		/**
		 * @brief Get a prepared statement, compiling it the first time.
		 * @param stmt The slot that caches the statement. When it is null,
		 * the SQL is compiled and the slot is filled; otherwise the cached
		 * statement is returned and `sql` is ignored.
		 * @param sql The SQL text of the statement.
		 * @return The prepared statement, owned by the slot.
		 * @throws std::runtime_error When SQLite cannot compile the SQL.
		 */
		sqlite3_stmt *prepareStatement(sqlite3_stmt *&stmt,
									   const char *sql) const;

		/**
		 * @brief Release the three cached statements. Required before the
		 * connection is closed.
		 */
		void finalizeStatements(void);

		/**
		 * @brief Create the `local_storage` table (key, value) when the
		 * database does not have it yet.
		 * @throws std::runtime_error When the table cannot be created.
		 */
		void initializeSchema(void);

		/**
		 * @brief Run an SQL statement that returns no row.
		 * @param sql The SQL text to run.
		 * @return Always true: a failure is reported by the exception.
		 * @throws std::runtime_error When SQLite reports an error; the
		 * message holds the SQL and SQLite's reason.
		 */
		bool executeStatement(const std::string &sql) const noexcept(false);

		public:
		using Storage::setItem;

		/**
		 * @brief Open the storage file, creating it and its parent
		 * directories when they do not exist.
		 *
		 * When the file cannot be opened or created (read-only location, for
		 * instance), an error is logged and the storage works on an
		 * in-memory database: isPersistent() then returns false.
		 * @param storageFilePath Path of the SQLite database file. Defaults
		 * to defaultStoragePath().
		 * @throws std::runtime_error When neither the file nor the in-memory
		 * database can be opened, or when the table cannot be created.
		 */
		explicit LocalStorage(const std::filesystem::path &storageFilePath =
								  defaultStoragePath()) noexcept(false);

		/**
		 * @brief Close the database. The items stay in the file.
		 */
		~LocalStorage(void) override;

		/**
		 * @brief Store a string under a key, replacing the item already
		 * stored under that key, if any.
		 *
		 * The item is written to the file before the call returns.
		 * @param key The name of the item.
		 * @param value The text to store. Text after an embedded null
		 * character is not stored.
		 * @throws std::runtime_error When the statement cannot be prepared.
		 */
		void setItem(const std::string &key,
					 const std::string &value) noexcept(false) override;

		/**
		 * @brief Read the text stored under a key.
		 * @param key The name of the item.
		 * @return The stored text, or `std::nullopt` when no item has that
		 * key.
		 * @throws std::runtime_error When the statement cannot be prepared.
		 */
		std::optional<std::string>
			getItem(const std::string &key) noexcept(false) override;

		/**
		 * @brief Delete the item stored under a key. Does nothing when there
		 * is none.
		 * @param key The name of the item.
		 * @throws std::runtime_error When the statement cannot be prepared.
		 */
		void removeItem(const std::string &key) noexcept(false) override;

		/**
		 * @brief Delete every item of the storage file.
		 * @throws std::runtime_error When SQLite reports an error.
		 */
		void clear(void) noexcept(false) override;

		/**
		 * @brief Get the path used when the constructor is given none.
		 * @return `.guillaume-local-storage.db` in the current working
		 * directory.
		 * @throws std::filesystem::filesystem_error When the current
		 * working directory cannot be determined.
		 */
		static std::filesystem::path defaultStoragePath(void) noexcept(false);

		/**
		 * @brief Tell whether the items are written to the storage file.
		 *
		 * When the storage file cannot be opened (read-only or missing
		 * location), the storage works on an in-memory database so the
		 * application keeps running, but nothing survives a restart.
		 * @return True when backed by the file, false when in-memory.
		 */
		bool isPersistent(void) const noexcept;
	};

}	 // namespace guillaume
