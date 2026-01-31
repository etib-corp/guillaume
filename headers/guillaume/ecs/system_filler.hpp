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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/system.hpp"

namespace guillaume::ecs {

/**
 * @brief Templated System class that automatically sets its signature based
 * on the specified component types.
 * @tparam ComponentTypes The component types that define the system's
 * signature.
 */
template <InheritFromComponent... ComponentTypes>
class SystemFiller : public System {
  public:
    /**
     * @brief Construct a new System Filler object and set its signature.
     */
    SystemFiller(void) { setSignature<ComponentTypes...>(); }

    /**
     * @brief Default destructor for the SystemFiller class.
     */
    virtual ~SystemFiller(void) = default;
};

} // namespace guillaume::ecs