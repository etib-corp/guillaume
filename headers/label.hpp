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
#include "primitives/text.hpp"
#include <string>

/**
 * @class Label
 * @brief Simple display component for text.
 *
 * The Label class represents a text display component that typically does not
 * handle events.
 */
class Label : public Component {
private:
  std::string _text; ///< The label's text content

public:
  /**
   * @brief Constructs a Label object.
   *
   * @param text The label's text content
   */
  Label(const std::string &text) : Component(), _text(text) {
    // Store text in state
    _state.set<std::string>("text", text);
  }

  /**
   * @brief Constructs a Label object with properties.
   *
   * @param text The label's text content
   * @param properties The properties for the label
   */
  Label(const std::string &text, const Properties &properties)
      : Component(properties), _text(text) {
    // Store text in state
    _state.set<std::string>("text", text);
  }

  /**
   * @brief Destroys the Label object.
   */
  ~Label(void) override = default;

  /**
   * @brief Gets the label's text.
   *
   * @return const std::string& The label's text content
   */
  const std::string &getText(void) const { return _text; }

  /**
   * @brief Sets the label's text.
   *
   * @param text The new text content
   */
  void setText(const std::string &text) {
    _text = text;
    _state.set<std::string>("text", text);
  }

  /**
   * @brief Renders the label.
   *
   * @return std::shared_ptr<Component> Pointer to the rendered component
   */
  std::shared_ptr<Component> render(void) override {
    // Update text from state if it has changed
    if (_state.getData().find("text") != _state.getData().end()) {
      _text = _state.get<std::string>("text");
    }

    // Clear existing primitives and regenerate
    _primitives.clear();

    // Create a text primitive for the label content
    auto textPrimitive = std::make_shared<Text>(_text, Point(0, 0, 0));
    _primitives.push_back(textPrimitive);

    return shared_from_this();
  }

  /**
   * @brief Handles events for the label.
   *
   * Labels typically do not handle events, so this is a no-op.
   *
   * @param event The event to handle
   */
  void onEvent(const Event &event) override {
    // Labels typically don't handle events
    (void)event;
  }
};
