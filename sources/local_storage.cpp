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

#include "guillaume/local_storage.hpp"

#include <sqlite3.h>

namespace guillaume
{

	LocalStorage::LocalStorage(const std::filesystem::path &storageFilePath)
		: _storageFilePath(storageFilePath)
		, _database(nullptr)
	{
		const auto parentPath = _storageFilePath.parent_path();
		if (!parentPath.empty()) {
			std::filesystem::create_directories(parentPath);
		}

		if (sqlite3_open_v2(_storageFilePath.string().c_str(), &_database,
							SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)
			!= SQLITE_OK) {
			if (_database) {
				sqlite3_close(_database);
				_database = nullptr;
			}
			// Fallback to in-memory database if file-based storage fails
			if (sqlite3_open(":memory:", &_database) != SQLITE_OK) {
				if (_database) {
					sqlite3_close(_database);
					_database = nullptr;
				}
				throw std::runtime_error(
					"Failed to open local storage database (file and in-memory "
					"fallback): "
					+ _storageFilePath.string() + " - reason: "
					+ (_database != nullptr
						   ? std::string(sqlite3_errmsg(_database))
						   : std::string("database not available")));
			}
		}

		initializeSchema();
	}

	LocalStorage::~LocalStorage(void)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		finalizeStatements();
		if (_database) {
			sqlite3_close(_database);
			_database = nullptr;
		}
	}

	sqlite3_stmt *LocalStorage::prepareStatement(sqlite3_stmt *&stmt,
												 const char *sql) const
	{
		if (stmt != nullptr) {
			return stmt;
		}
		if (sqlite3_prepare_v2(_database, sql, -1, &stmt, nullptr)
			!= SQLITE_OK) {
			throw std::runtime_error("Failed to prepare statement: "
									 + std::string(sqlite3_errmsg(_database)));
		}
		return stmt;
	}

	void LocalStorage::finalizeStatements(void)
	{
		if (_setItemStmt != nullptr) {
			sqlite3_finalize(_setItemStmt);
			_setItemStmt = nullptr;
		}
		if (_getItemStmt != nullptr) {
			sqlite3_finalize(_getItemStmt);
			_getItemStmt = nullptr;
		}
		if (_removeItemStmt != nullptr) {
			sqlite3_finalize(_removeItemStmt);
			_removeItemStmt = nullptr;
		}
	}

	std::filesystem::path LocalStorage::defaultStoragePath(void)
	{
		return std::filesystem::current_path() / ".guillaume-local-storage.db";
	}

	void LocalStorage::setItem(const std::string &key, const std::string &value)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			throw std::runtime_error("Failed to set item in local storage: "
									 + _storageFilePath.string() + " - reason: "
									 + "database not available");
		}

		static constexpr const char *statement =
			"INSERT INTO local_storage (key, value) VALUES (?, ?) "
			"ON CONFLICT(key) DO UPDATE SET value = excluded.value;";

		sqlite3_stmt *query = prepareStatement(_setItemStmt, statement);
		sqlite3_reset(query);
		sqlite3_clear_bindings(query);

		sqlite3_bind_text(query, 1, key.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(query, 2, value.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_step(query);
	}

	std::optional<std::string> LocalStorage::getItem(const std::string &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			throw std::runtime_error("Failed to get item from local storage: "
									 + _storageFilePath.string() + " - reason: "
									 + "database not available");
		}

		static constexpr const char *statement =
			"SELECT value FROM local_storage WHERE key = ? LIMIT 1;";

		sqlite3_stmt *query = prepareStatement(_getItemStmt, statement);
		sqlite3_reset(query);
		sqlite3_clear_bindings(query);

		sqlite3_bind_text(query, 1, key.c_str(), -1, SQLITE_TRANSIENT);

		std::optional<std::string> result = std::nullopt;
		if (sqlite3_step(query) == SQLITE_ROW) {
			const auto *value = sqlite3_column_text(query, 0);
			if (value) {
				result = reinterpret_cast<const char *>(value);
			}
		}

		return result;
	}

	void LocalStorage::removeItem(const std::string &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			throw std::runtime_error(
				"Failed to remove item from local storage: "
				+ _storageFilePath.string()
				+ " - reason: database not available");
		}

		static constexpr const char *statement =
			"DELETE FROM local_storage WHERE key = ?;";

		sqlite3_stmt *query = prepareStatement(_removeItemStmt, statement);
		sqlite3_reset(query);
		sqlite3_clear_bindings(query);

		sqlite3_bind_text(query, 1, key.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_step(query);
	}

	void LocalStorage::clear(void)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			throw std::runtime_error("Failed to clear local storage: "
									 + _storageFilePath.string()
									 + " - reason: database not available");
		}

		if (!executeStatement("DELETE FROM local_storage;")) {
			throw std::runtime_error("Failed to clear local storage: "
									 + _storageFilePath.string());
		}
	}

	void LocalStorage::initializeSchema(void)
	{
		if (!executeStatement("CREATE TABLE IF NOT EXISTS local_storage ("
							  "key TEXT PRIMARY KEY, "
							  "value TEXT NOT NULL"
							  ");")) {
			throw std::runtime_error(
				"Failed to initialize local storage schema: "
				+ _storageFilePath.string());
		}
	}

	bool LocalStorage::executeStatement(const std::string &sql) const
	{
		char *errorMessage = nullptr;
		const int code = sqlite3_exec(_database, sql.c_str(), nullptr, nullptr,
									  &errorMessage);
		if (code != SQLITE_OK) {
			std::string msg = "Failed to execute statement: ";
			msg += sql;
			if (errorMessage) {
				msg += " - reason: ";
				msg += errorMessage;
				sqlite3_free(errorMessage);
			}
			throw std::runtime_error(msg);
		}

		return true;
	}

}	 // namespace guillaume
