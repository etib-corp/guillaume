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

#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a bounding parallelepiped.
 * @see systems::Render
 */
class Bound : public ecs::Component {
  public:
    using Size = utility::math::Vector<float, 3>; ///< Size type

  private:
    Size _size; ///< Size of the bounding parallelepiped

  public:
    /**
     * @brief Default constructor for the Bound component.
     */
    Bound(void) = default;

    /**
     * @brief Default destructor for the Bound component.
     */
    ~Bound(void) = default;

    /**
     * @brief Set the bounding parallelepiped.
     * @param size The size of the bounding parallelepiped.
     */
    void setSize(const Size &size) { _size = size; };

    /**
     * @brief Get the bounding parallelepiped.
     * @return The bounding parallelepiped.
     */
    Size getSize(void) const { return _size; }
};

} // namespace guillaume::components