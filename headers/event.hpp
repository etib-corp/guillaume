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
#include <memory>
#include <string>

class Component;

/**
 * @brief Represents a UI event with type, target, and optional data
 *
 * The Event class encapsulates user interactions and system events that can
 * be propagated through the component hierarchy. Events have a type (e.g., "click"),
 * a target component, and optional type-erased data payload.
 */
class Event {
private:
  std::string _type;
  std::shared_ptr<Component> _target;
  std::any _data;

public:
  /**
   * @brief Construct an event
   * @param type The event type (e.g., "click", "hover", "change")
   * @param target The component that is the target of this event
   * @param data Optional data payload associated with the event
   */
  Event(const std::string &type, std::shared_ptr<Component> target,
        const std::any &data = std::any())
      : _type(type), _target(target), _data(data) {}

  ~Event(void) = default;

  /**
   * @brief Get the event type
   * @return The event type string
   */
  const std::string &getType(void) const { return _type; }

  /**
   * @brief Get the target component
   * @return Shared pointer to the target component
   */
  std::shared_ptr<Component> getTarget(void) const { return _target; }

  /**
   * @brief Get the event data with type casting
   * @tparam T The type to cast the data to
   * @return The event data cast to type T, or default-constructed T if no data
   */
  template <typename T> T getData(void) const {
    if (_data.has_value()) {
      return std::any_cast<T>(_data);
    }
    return T();
  }

  /**
   * @brief Check if the event has data
   * @return true if data is present, false otherwise
   */
  bool hasData(void) const { return _data.has_value(); }
};
