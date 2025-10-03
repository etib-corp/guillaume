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
#include <functional>
#include <map>
#include <string>

/**
 * @brief Manages dynamic component state with type-safe storage
 *
 * The State class provides a key-value store for component data that can change
 * over time. It supports type-safe getters/setters and callbacks for state changes.
 */
class State {
private:
  std::map<std::string, std::any> _data;
  std::function<void()> _onStateChange;

public:
  State(void) : _data(), _onStateChange(nullptr) {}
  ~State(void) = default;

  /**
   * @brief Replace the entire state with new values
   * @param newState Map of key-value pairs to set as the new state
   */
  void setState(const std::map<std::string, std::any> &newState) {
    for (const auto &[key, value] : newState) {
      _data[key] = value;
    }
    if (_onStateChange) {
      _onStateChange();
    }
  }

  /**
   * @brief Get a state value by key with type casting
   * @tparam T The type to cast the value to
   * @param key The key to retrieve
   * @return The value cast to type T, or default-constructed T if key doesn't exist
   */
  template <typename T> T get(const std::string &key) const {
    auto it = _data.find(key);
    if (it != _data.end()) {
      return std::any_cast<T>(it->second);
    }
    return T();
  }

  /**
   * @brief Set a state value by key
   * @tparam T The type of the value to store
   * @param key The key to set
   * @param value The value to store
   */
  template <typename T> void set(const std::string &key, const T &value) {
    _data[key] = value;
    if (_onStateChange) {
      _onStateChange();
    }
  }

  /**
   * @brief Register a callback to be called when state changes
   * @param callback Function to call on state changes
   */
  void setOnStateChange(std::function<void()> callback) {
    _onStateChange = callback;
  }

  /**
   * @brief Get the entire state data map
   * @return Const reference to the map containing all state key-value pairs
   */
  const std::map<std::string, std::any> &getData(void) const { return _data; }
};
