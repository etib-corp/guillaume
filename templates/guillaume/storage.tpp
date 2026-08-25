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

#include "guillaume/storage.hpp"

namespace guillaume
{
	template<typename ValueType>
		requires(std::is_same_v<std::remove_cvref_t<ValueType>, bool>)
	void Storage::setItem(const std::string &key, ValueType value)
	{
		setItem(key, value ? std::string("true") : std::string("false"));
	}

	template<typename ValueType>
		requires(std::is_arithmetic_v<ValueType>
				 && !std::is_same_v<ValueType, bool>)
	void Storage::setItem(const std::string &key, ValueType value)
	{
		setItem(key, std::to_string(value));
	}

	template<typename ValueType>
	std::optional<ValueType> Storage::getItemAs(const std::string &key)
	{
		const auto value = getItem(key);
		if (!value.has_value()) {
			return std::nullopt;
		}

		if constexpr (std::is_same_v<ValueType, std::string>) {
			return value;
		} else if constexpr (std::is_same_v<ValueType, bool>) {
			if (*value == "true" || *value == "1") {
				return true;
			}
			if (*value == "false" || *value == "0") {
				return false;
			}
			return std::nullopt;
		} else if constexpr (std::is_integral_v<ValueType>) {
			ValueType parsed {};
			const char *begin = value->data();
			const char *end	  = begin + value->size();
			const auto result = std::from_chars(begin, end, parsed);
			if (result.ec != std::errc() || result.ptr != end) {
				return std::nullopt;
			}
			return parsed;
		} else if constexpr (std::is_floating_point_v<ValueType>) {
			char *endPointer = nullptr;
			errno			 = 0;
			const long double parsed =
				std::strtold(value->c_str(), &endPointer);

			if (errno == ERANGE
				|| endPointer != value->c_str() + value->size()) {
				return std::nullopt;
			}

			return static_cast<ValueType>(parsed);
		} else {
			static_assert(std::is_same_v<ValueType, std::string>
							  || std::is_same_v<ValueType, bool>
							  || std::is_integral_v<ValueType>
							  || std::is_floating_point_v<ValueType>,
						  "Storage::getItemAs only supports string, bool, "
						  "integral and floating-point types.");
		}
	}
}	 // namespace guillaume