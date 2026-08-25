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

#include <cerrno>
#include <charconv>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

namespace guillaume
{

	/**
	 * @brief Interface for key-value storage backends.
	 *
	 * This abstraction defines the minimal operations required by storage
	 * implementations used by the framework.
	 */
	class Storage
	{
		public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~Storage(void) = default;

		/**
		 * @brief Store a value for a key.
		 * @hidecallgraph
		 * @param key Storage key.
		 * @param value String value to store.
		 */
		virtual void setItem(const std::string &key,
							 const std::string &value) = 0;

		/**
		 * @brief Store a string view value for a key.
		 * @hidecallgraph
		 * @param key Storage key.
		 * @param value String view value to store.
		 */
		void setItem(const std::string &key, std::string_view value);

		/**
		 * @brief Store a C-string value for a key.
		 * @hidecallgraph
		 * @param key Storage key.
		 * @param value C-string value to store.
		 */
		void setItem(const std::string &key, const char *value);

		/**
		 * @brief Store a boolean value for a key.
		 * @hidecallgraph
		 * @tparam ValueType Boolean type.
		 * @param key Storage key.
		 * @param value Boolean value to store.
		 */
		template<typename ValueType>
			requires(std::is_same_v<std::remove_cvref_t<ValueType>, bool>)
		void setItem(const std::string &key, ValueType value);

		/**
		 * @brief Store a numeric value for a key.
		 * @hidecallgraph
		 * @tparam ValueType Numeric type.
		 * @param key Storage key.
		 * @param value Numeric value to store.
		 */
		template<typename ValueType>
			requires(std::is_arithmetic_v<ValueType>
					 && !std::is_same_v<ValueType, bool>)
		void setItem(const std::string &key, ValueType value);

		/**
		 * @brief Retrieve a value for a key.
		 * @param key Storage key.
		 * @return Stored value when found, std::nullopt otherwise.
		 */
		virtual std::optional<std::string> getItem(const std::string &key) = 0;

		/**
		 * @brief Retrieve a value for a key and convert it to a requested type.
		 * @tparam ValueType Type to convert to.
		 * @param key Storage key.
		 * @return Converted value when found and valid, std::nullopt otherwise.
		 */
		template<typename ValueType>
		std::optional<ValueType> getItemAs(const std::string &key);

		/**
		 * @brief Remove a key and its value.
		 * @param key Storage key.
		 */
		virtual void removeItem(const std::string &key) = 0;

		/**
		 * @brief Remove all keys and values.
		 */
		virtual void clear(void) = 0;
	};

}	 // namespace guillaume

/// Include the Storage template implementation
#include "guillaume/storage.tpp"