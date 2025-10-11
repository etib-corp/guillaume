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

#include "component.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include <functional>
#include <string>

/**
 * @class Button
 * @brief Interactive button component.
 *
 * The Button class represents a clickable button with a label and onClick
 * handler.
 */
class Button : public Component {
private:
  std::string _label;             ///< The button's label text
  std::function<void()> _onClick; ///< Callback function for click events

public:
  /**
   * @brief Constructs a Button object.
   *
   * @param label The button's label text
   */
  Button(const std::string &label)
      : Component(), _label(label), _onClick(nullptr) {
    // Store label in state
    _state.set<std::string>("label", label);
  }

  /**
   * @brief Constructs a Button object with properties.
   *
   * @param label The button's label text
   * @param properties The properties for the button
   */
  Button(const std::string &label, const Properties &properties)
      : Component(properties), _label(label), _onClick(nullptr) {
    // Store label in state
    _state.set<std::string>("label", label);
  }

  /**
   * @brief Destroys the Button object.
   */
  ~Button(void) override = default;

  /**
   * @brief Sets the onClick callback function.
   *
   * @param callback The callback function to be called when the button is
   * clicked
   */
  void setOnClick(std::function<void()> callback) { _onClick = callback; }

  /**
   * @brief Gets the button's label.
   *
   * @return const std::string& The button's label text
   */
  const std::string &getLabel(void) const { return _label; }

  /**
   * @brief Sets the button's label.
   *
   * @param label The new label text
   */
  void setLabel(const std::string &label) {
    _label = label;
    _state.set<std::string>("label", label);
  }

  /**
   * @brief Renders the button.
   *
   * @return std::shared_ptr<Component> Pointer to the rendered component
   */
  std::shared_ptr<Component> render(void) override {
    // Update label from state if it has changed
    if (_state.getData().find("label") != _state.getData().end()) {
      _label = _state.get<std::string>("label");
    }

    // Clear existing primitives and regenerate
    _primitives.clear();

  // Create a rectangle primitive for the button background (centered at (50,15,0), width 100, height 30, no rotation)
  auto rectPrimitive = std::make_shared<Rectangle>(Point(50, 15, 0), 100, 30, Point(0, 0, 0));
  _primitives.push_back(rectPrimitive);

    // Create a text primitive for the button label
    auto textPrimitive = std::make_shared<Text>(_label, Point(10, 15, 0));
    _primitives.push_back(textPrimitive);

    return shared_from_this();
  }

  /**
   * @brief Handles events for the button.
   *
   * If the event is a click event, the onClick callback is called.
   *
   * @param event The event to handle
   */
  void onEvent(const Event &event) override {
    if (event.getType() == "click" && _onClick) {
      _onClick();
    }
  }
};
