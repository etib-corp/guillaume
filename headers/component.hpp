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

#include <cstdint>
#include <map>
#include <memory>

#include <rectangle.hpp>

#include "property.hpp"
#include "renderer.hpp"

namespace guillaume {
class Container; /// Forward declaration of Container

/**
 * @brief Component base class.
 */
class Component {
  private:
    std::shared_ptr<Container> _parent; /**< Parent container. */
    std::vector<std::pair<PropertyPriority, std::unique_ptr<Property>>>
        _properties; ///< Properties with their priorities

  protected:
    /**
     * @brief Set the parent component.
     * @param parent The parent container as a shared_ptr<Container>.
     */
    void setParent(std::shared_ptr<Container> parent) { _parent = parent; }

    /**
     * @brief Get the parent container.
     * @return The parent container as a shared_ptr<Container>.
     */
    std::shared_ptr<Container> getParent(void) const { return _parent; }

  public:
    /**
     * @brief Default destructor
     */
    virtual ~Component(void) = default;

    /**
     * @brief Add a property to the component.
     * @param property The property to add.
     */
    void addProperty(std::unique_ptr<Property> property) {
        _properties.emplace_back(property->getPriority(), std::move(property));
    }

    /**
     * @brief Process and apply all properties to the component.
     */
    void processProperties(void);

    /**
     * @brief Render the component.
     * @param renderer The renderer to use for rendering.
     */
    virtual void render(Renderer &renderer) = 0;
};

} // namespace guillaume
