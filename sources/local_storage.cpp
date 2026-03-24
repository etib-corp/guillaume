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

		if (sqlite3_open(_storageFilePath.string().c_str(), &_database)
			!= SQLITE_OK) {
			if (_database) {
				sqlite3_close(_database);
				_database = nullptr;
			}
			return;
		}

		initializeSchema();
	}

	LocalStorage::~LocalStorage(void)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_database) {
			sqlite3_close(_database);
			_database = nullptr;
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
			return;
		}

		static constexpr const char *statement =
			"INSERT INTO local_storage (key, value) VALUES (?, ?) "
			"ON CONFLICT(key) DO UPDATE SET value = excluded.value;";

		sqlite3_stmt *query = nullptr;
		if (sqlite3_prepare_v2(_database, statement, -1, &query, nullptr)
			!= SQLITE_OK) {
			return;
		}

		sqlite3_bind_text(query, 1, key.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(query, 2, value.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_step(query);
		sqlite3_finalize(query);
	}

	std::optional<std::string> LocalStorage::getItem(const std::string &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			return std::nullopt;
		}

		static constexpr const char *statement =
			"SELECT value FROM local_storage WHERE key = ? LIMIT 1;";

		sqlite3_stmt *query = nullptr;
		if (sqlite3_prepare_v2(_database, statement, -1, &query, nullptr)
			!= SQLITE_OK) {
			return std::nullopt;
		}

		sqlite3_bind_text(query, 1, key.c_str(), -1, SQLITE_TRANSIENT);

		std::optional<std::string> result = std::nullopt;
		if (sqlite3_step(query) == SQLITE_ROW) {
			const auto *value = sqlite3_column_text(query, 0);
			if (value) {
				result = reinterpret_cast<const char *>(value);
			}
		}

		sqlite3_finalize(query);
		return result;
	}

	void LocalStorage::removeItem(const std::string &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			return;
		}

		static constexpr const char *statement =
			"DELETE FROM local_storage WHERE key = ?;";

		sqlite3_stmt *query = nullptr;
		if (sqlite3_prepare_v2(_database, statement, -1, &query, nullptr)
			!= SQLITE_OK) {
			return;
		}

		sqlite3_bind_text(query, 1, key.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_step(query);
		sqlite3_finalize(query);
	}

	void LocalStorage::clear(void)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_database) {
			return;
		}

		executeStatement("DELETE FROM local_storage;");
	}

	void LocalStorage::initializeSchema(void)
	{
		executeStatement("CREATE TABLE IF NOT EXISTS local_storage ("
						 "key TEXT PRIMARY KEY, "
						 "value TEXT NOT NULL"
						 ");");
	}

	bool LocalStorage::executeStatement(const std::string &sql) const
	{
		if (!_database) {
			return false;
		}

		char *errorMessage = nullptr;
		const int code = sqlite3_exec(_database, sql.c_str(), nullptr, nullptr,
									  &errorMessage);
		if (errorMessage) {
			sqlite3_free(errorMessage);
		}
		return code == SQLITE_OK;
	}

}	 // namespace guillaume
