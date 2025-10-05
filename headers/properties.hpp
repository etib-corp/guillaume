/*
 Copyright (c) 2025 ETIB Corporation

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

#include <any>
#include <map>
#include <stdexcept>
#include <string>

/**
 * @brief Manages immutable component properties
 *
 * The Properties class provides read-only access to component configuration
 * data passed from parent components. Properties cannot be modified after
 * construction.
 */
class Properties {
private:
  std::map<std::string, std::any> _attributes;

public:
  /**
   * @brief Default constructor for empty properties
   */
  Properties(void) : _attributes() {}

  /**
   * @brief Construct properties from a map of attributes
   * @param attributes Map of key-value pairs to store as properties
   */
  Properties(const std::map<std::string, std::any> &attributes)
      : _attributes(attributes) {}
  ~Properties(void) = default;

  /**
   * @brief Get a property value by key with type casting
   * @tparam T The type to cast the value to
   * @param key The key to retrieve
   * @return The value cast to type T, or default-constructed T if key doesn't
   * exist
   */
  template <typename T> T get(const std::string &key) const {
    auto it = _attributes.find(key);
    if (it != _attributes.end()) {
      return std::any_cast<T>(it->second);
    }
    return T();
  }

  /**
   * @brief Check if a property key exists
   * @param key The key to check
   * @return true if the key exists, false otherwise
   */
  bool has(const std::string &key) const {
    return _attributes.find(key) != _attributes.end();
  }

  /**
   * @brief Get all property attributes
   * @return Const reference to the map containing all property key-value pairs
   */
  const std::map<std::string, std::any> &getAttributes(void) const {
    return _attributes;
  }
};
