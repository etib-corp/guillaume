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

#include <type_traits>

namespace guillaume::ecs {

/**
 * @brief Base class for all components in the ECS architecture.
 *
 * Components are data containers that hold specific attributes or properties
 * of an entity. They do not contain any behavior or logic; that is the role
 * of systems.
 */
class Component {
  public:
    /**
     * @brief Default constructor for the Component class.
     */
    Component(void) = default;

    /**
     * @brief Virtual destructor for the Component base class.
     */
    virtual ~Component(void) = default;
};

/**
 * @brief Concept to ensure a type inherits from Component.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromComponent = std::is_base_of_v<Component, Type>;

} // namespace guillaume::ecs