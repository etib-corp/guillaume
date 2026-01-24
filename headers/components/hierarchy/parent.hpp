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

#include <cstddef>

#include "ecs/component.hpp"

namespace guillaume::components::hierarchy {

/**
 * @brief Parent component referencing the parent entity.
 */
class Parent : public ecs::Component {
  private:
    std::size_t _parent_id_{0};

  public:
    /**
     * @brief Default constructor.
     */
    Parent(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Parent(void) override = default;

    /**
     * @brief Set the parent entity ID.
     * @param parent_id The parent entity ID.
     */
    void setParentId(std::size_t parent_id) { _parent_id_ = parent_id; }

    /**
     * @brief Get the parent entity ID.
     * @return The parent entity ID.
     */
    std::size_t getParentId(void) const { return _parent_id_; }
};

} // namespace guillaume::components::hierarchy
