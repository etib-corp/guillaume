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

namespace guillaume {

/**
 * @brief Interface for key-value storage backends.
 *
 * This abstraction defines the minimal operations required by storage
 * implementations used by the framework.
 */
class Storage {
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
    virtual void setItem(const std::string &key, const std::string &value) = 0;

    /**
     * @brief Store a string view value for a key.
     * @hidecallgraph
     * @param key Storage key.
     * @param value String view value to store.
     */
    void setItem(const std::string &key, std::string_view value) {
        setItem(key, std::string(value));
    }

    /**
     * @brief Store a C-string value for a key.
     * @hidecallgraph
     * @param key Storage key.
     * @param value C-string value to store.
     */
    void setItem(const std::string &key, const char *value) {
        setItem(key, value ? std::string(value) : std::string());
    }

    /**
     * @brief Store a boolean value for a key.
        * @hidecallgraph
     * @tparam ValueType Boolean type.
     * @param key Storage key.
     * @param value Boolean value to store.
     */
    template <typename ValueType>
        requires(std::is_same_v<std::remove_cvref_t<ValueType>, bool>)
    void setItem(const std::string &key, ValueType value) {
        setItem(key, value ? std::string("true") : std::string("false"));
    }

    /**
     * @brief Store a numeric value for a key.
        * @hidecallgraph
     * @tparam ValueType Numeric type.
     * @param key Storage key.
     * @param value Numeric value to store.
     */
    template <typename ValueType>
        requires(std::is_arithmetic_v<ValueType> &&
                 !std::is_same_v<ValueType, bool>)
    void setItem(const std::string &key, ValueType value) {
        setItem(key, std::to_string(value));
    }

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
    template <typename ValueType>
    std::optional<ValueType> getItemAs(const std::string &key) {
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
            ValueType parsed{};
            const char *begin = value->data();
            const char *end = begin + value->size();
            const auto result = std::from_chars(begin, end, parsed);
            if (result.ec != std::errc() || result.ptr != end) {
                return std::nullopt;
            }
            return parsed;
        } else if constexpr (std::is_floating_point_v<ValueType>) {
            char *endPointer = nullptr;
            errno = 0;
            const long double parsed =
                std::strtold(value->c_str(), &endPointer);

            if (errno == ERANGE ||
                endPointer != value->c_str() + value->size()) {
                return std::nullopt;
            }

            return static_cast<ValueType>(parsed);
        } else {
            static_assert(std::is_same_v<ValueType, std::string> ||
                              std::is_same_v<ValueType, bool> ||
                              std::is_integral_v<ValueType> ||
                              std::is_floating_point_v<ValueType>,
                          "Storage::getItemAs only supports string, bool, "
                          "integral and floating-point types.");
        }
    }

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

} // namespace guillaume
