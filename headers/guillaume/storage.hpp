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
	 * @brief A key-value store of strings, modelled on the web Storage API
	 * (`setItem`, `getItem`, `removeItem`, `clear`).
	 *
	 * Two implementations exist: LocalStorage keeps the items in a file and
	 * finds them again at the next start; SessionStorage keeps them in
	 * memory until the process ends. Everything is stored as text: the
	 * typed setItem() overloads convert to a string and getItemAs() converts
	 * back.
	 *
	 * @code
	 * guillaume::SessionStorage storage;
	 *
	 * storage.setItem("volume", 0.8f);		   // stored as "0.800000"
	 * storage.setItem("muted", false);		   // stored as "false"
	 * storage.setItem("user", "ada");
	 *
	 * const float volume =
	 * 	storage.getItemAs<float>("volume").value_or(1.0f);
	 * if (!storage.getItem("token")) {
	 * 	// no item under that key
	 * }
	 * @endcode
	 *
	 * @note A class that implements the interface adds
	 * `using Storage::setItem;`, otherwise its own setItem() hides the
	 * typed overloads.
	 * @see LocalStorage
	 * @see SessionStorage
	 */
	class Storage
	{
		public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~Storage(void) = default;

		/**
		 * @brief Store a string under a key, replacing the item already
		 * stored under that key, if any.
		 *
		 * Implemented by each storage; the other overloads end up here.
		 * @hidecallgraph
		 * @param key The name of the item.
		 * @param value The text to store. It may be empty.
		 */
		virtual void setItem(const std::string &key,
							 const std::string &value) = 0;

		/**
		 * @brief Store a copy of a string view under a key.
		 * @hidecallgraph
		 * @param key The name of the item.
		 * @param value The text to store.
		 */
		void setItem(const std::string &key, std::string_view value);

		/**
		 * @brief Store a copy of a C string under a key.
		 * @hidecallgraph
		 * @param key The name of the item.
		 * @param value The null-terminated text to store. A null pointer
		 * stores an empty string.
		 */
		void setItem(const std::string &key, const char *value);

		/**
		 * @brief Store a boolean under a key, as the text "true" or
		 * "false".
		 * @hidecallgraph
		 * @tparam ValueType `bool` (possibly const or a reference).
		 * @param key The name of the item.
		 * @param value The boolean to store.
		 */
		template<typename ValueType>
			requires(std::is_same_v<std::remove_cvref_t<ValueType>, bool>)
		void setItem(const std::string &key, ValueType value);

		/**
		 * @brief Store a number under a key, as its decimal text.
		 *
		 * The text is produced by `std::to_string`: a floating-point value
		 * keeps six digits after the decimal point, and a `char` is stored
		 * as its numeric code.
		 * @hidecallgraph
		 * @tparam ValueType An integer or floating-point type other than
		 * `bool`.
		 * @param key The name of the item.
		 * @param value The number to store.
		 */
		template<typename ValueType>
			requires(std::is_arithmetic_v<ValueType>
					 && !std::is_same_v<ValueType, bool>)
		void setItem(const std::string &key, ValueType value);

		/**
		 * @brief Read the text stored under a key.
		 * @param key The name of the item.
		 * @return The stored text, or `std::nullopt` when no item has that
		 * key. An item holding an empty string is returned as an empty
		 * string, not as `std::nullopt`.
		 */
		virtual std::optional<std::string> getItem(const std::string &key) = 0;

		/**
		 * @brief Read the item stored under a key and convert it to a type.
		 *
		 * - `std::string`: the text as stored.
		 * - `bool`: "true" or "1" gives true, "false" or "0" gives false.
		 * - integer types: the whole text must be a number that fits the
		 *   type.
		 * - floating-point types: the whole text must be a number in range,
		 *   as read by `std::strtold`. An empty text reads as 0.
		 *
		 * @tparam ValueType The type to convert to: `std::string`, `bool`,
		 * an integer type or a floating-point type. Any other type does not
		 * compile.
		 * @param key The name of the item.
		 * @return The converted value, or `std::nullopt` when no item has
		 * that key or when its text cannot be converted to `ValueType`.
		 */
		template<typename ValueType>
		std::optional<ValueType> getItemAs(const std::string &key);

		/**
		 * @brief Delete the item stored under a key. Does nothing when there
		 * is none.
		 * @param key The name of the item.
		 */
		virtual void removeItem(const std::string &key) = 0;

		/**
		 * @brief Delete every item of the storage.
		 */
		virtual void clear(void) = 0;
	};

}	 // namespace guillaume

// Include the Storage template implementation
#include "guillaume/storage.tpp"