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

#include "component.hpp"

#include <memory>
#include <vector>

namespace guillaume::component {
/**
 * @brief A container class.
 */
class Container : public Component,
                  public std::enable_shared_from_this<Container> {
  public:
    /**
     * @brief Destructor.
     */
    virtual ~Container(void) = default;

    /**
     * @brief Get the child components.
     * @return A vector of shared_ptr<Component> representing the child
     * components.
     */
    std::vector<std::shared_ptr<Component>> getChildren(void) const {
        return _children;
    }

    /**
     * @brief Add a child component to the container.
     * @param child The child component to add as a shared_ptr<Component>.
     * @return A reference to the current Container object.
     */
    Container &addChild(std::shared_ptr<Component> child);

    void render(Renderer &renderer) override;

  private:
    std::shared_ptr<Container> _parent;                /**< Parent container. */
    std::vector<std::shared_ptr<Component>> _children; /**< Child components. */
};
} // namespace guillaume::component