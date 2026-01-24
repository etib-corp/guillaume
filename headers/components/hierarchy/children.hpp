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
#include <cstddef>
#include <vector>

#include "ecs/component.hpp"

namespace guillaume::components::hierarchy {

/**
 * @brief Children component listing child entities.
 */
class Children : public ecs::Component {
  private:
    std::vector<std::size_t> _child_ids_{};

  public:
    /**
     * @brief Default constructor.
     */
    Children(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Children(void) override = default;

    /**
     * @brief Add a child entity ID.
     * @param child_id The child entity ID to add.
     */
    void addChild(std::size_t child_id) { _child_ids_.push_back(child_id); }

    /**
     * @brief Remove a child entity ID.
     * @param child_id The child entity ID to remove.
     */
    void removeChild(std::size_t child_id) {
        _child_ids_.erase(
            std::remove(_child_ids_.begin(), _child_ids_.end(), child_id),
            _child_ids_.end());
    }

    /**
     * @brief Get all child entity IDs.
     * @return The list of child entity IDs.
     */
    const std::vector<std::size_t> &getChildIds(void) const {
        return _child_ids_;
    }

    /**
     * @brief Clear all child entities.
     */
    void clearChildren(void) { _child_ids_.clear(); }

    /**
     * @brief Get the number of children.
     * @return The number of child entities.
     */
    std::size_t getChildCount(void) const { return _child_ids_.size(); }
};

} // namespace guillaume::components::hierarchy
