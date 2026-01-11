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

#include <algorithm>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

#include <color.hpp>
#include <logger.hpp>
#include <rectangle.hpp>
#include <standard_logger.hpp>

#include "property.hpp"
#include "renderer.hpp"

namespace guillaume {

/**
 * @brief Component base class.
 */
class Component : public std::enable_shared_from_this<Component> {
private:
  std::weak_ptr<Component>
      _parent; ///< Parent component (weak_ptr avoids circular reference)
  std::vector<std::shared_ptr<Component>> _children; ///< Child components
  utility::Rectangle<std::size_t> _boundingBox;      ///< Component bounding box
  std::vector<std::pair<PropertyPriority, std::unique_ptr<Property>>>
      _properties; ///< Properties with their priorities

  // Style attributes
  float _opacity;                           ///< Component opacity (0.0 to 1.0)
  utility::Color<uint8_t> _backgroundColor; ///< Component background color
  bool _visible;                            ///< Component visibility
  int32_t _zIndex;                          ///< Component z-index for layering

  /**
   * @brief Set the parent component.
   * @param parent The parent component.
   */
  void setParent(std::shared_ptr<Component> parent);

public:
  /**
   * @brief Default constructor initializing all attributes.
   */
  Component(void)
      : _opacity(1.0f), _backgroundColor(0, 0, 0), _visible(true), _zIndex(0) {}

  /**
   * @brief Default destructor
   */
  virtual ~Component(void) = default;

  /**
   * @brief Set the opacity of the component.
   * @param opacity The opacity value (0.0 to 1.0).
   */
  void setOpacity(float opacity) {
    _opacity = (opacity < 0.0f) ? 0.0f : (opacity > 1.0f) ? 1.0f : opacity;
  }

  /**
   * @brief Get the opacity of the component.
   * @return The opacity value (0.0 to 1.0).
   */
  float getOpacity(void) const { return _opacity; }

  /**
   * @brief Set the background color of the component.
   * @param color The background color.
   */
  void setBackgroundColor(const utility::Color<uint8_t> &color) {
    _backgroundColor = color;
  }

  /**
   * @brief Get the background color of the component.
   * @return The background color.
   */
  utility::Color<uint8_t> getBackgroundColor(void) const {
    return _backgroundColor;
  }

  /**
   * @brief Set the visibility of the component.
   * @param visible True to show, false to hide.
   */
  void setVisible(bool visible) { _visible = visible; }

  /**
   * @brief Get the visibility of the component.
   * @return True if visible, false otherwise.
   */
  bool isVisible(void) const { return _visible; }

  /**
   * @brief Set the z-index of the component for layering.
   * @param zIndex The z-index value (higher values are drawn on top).
   */
  void setZIndex(int32_t zIndex) { _zIndex = zIndex; }

  /**
   * @brief Get the z-index of the component.
   * @return The z-index value.
   */
  int32_t getZIndex(void) const { return _zIndex; }

  /**
   * @brief Add a child component.
   * @param child The child component to add.
   */
  void addChild(std::shared_ptr<Component> child) {
    // Only set parent if this component is managed by a shared_ptr
    try {
      child->setParent(this->shared_from_this());
    } catch (const std::bad_weak_ptr &) {
      // Parent is not managed by shared_ptr, so we skip setting it
      // This can happen when the parent is managed by unique_ptr
    }
    _children.push_back(std::move(child));
  }

  /**
   * @brief Set the bounding box of the component.
   * @param boundingBox The bounding box as a Rectangle<std::size_t>.
   */
  void setBoundingBox(const utility::Rectangle<std::size_t> &boundingBox) {
    _boundingBox = boundingBox;
  }

  /**
   * @brief Get the bounding box of the component.
   * @return The bounding box as a Rectangle<std::size_t>.
   */
  utility::Rectangle<std::size_t> getBoundingBox(void) const {
    return _boundingBox;
  }

  /**
   * @brief Get the parent component.
   * @return Shared pointer to the parent component, or nullptr if no parent.
   */
  std::shared_ptr<Component> getParent(void) const { return _parent.lock(); }

  /**
   * @brief Add a property to the component.
   * @param property The property to add.
   * @return Reference to this component for method chaining.
   */
  Component &addProperty(std::unique_ptr<Property> property) {
    PropertyPriority priority = property->getPriority();
    _properties.emplace_back(priority, std::move(property));
    return *this;
  }

  /**
   * @brief Process and apply all properties to the component.
   */
  void processProperties(void);

  /**
   * @brief Render the component.
   * @param renderer The renderer to use for rendering.
   */
  virtual void render(Renderer &renderer);
};

} // namespace guillaume
