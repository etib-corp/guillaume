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

#include "ecs/component_registry.hpp"
#include "ecs/system_registry.hpp"

namespace guillaume::ecs {

/**
 * @brief ECS (Entity-Component-System) main class.
 * @tparam ComponentRegistryType The type of the component registry.
 * @tparam SystemRegistryType The type of the system registry.
 */
template <InheritFromComponentRegistry ComponentRegistryType> class ECS {
  protected:
    SystemRegistry _systemRegistry; ///< System registry instance

  public:
    /**
     * @brief Default constructor.
     */
    ECS(void) = default;

    /**
     * @brief Default destructor.
     */
    virtual ~ECS(void) = default;

    /**
     * @brief Routine to update all systems.
     */
    void routine(void) {
        for (auto &[typeIndex, system] : _systemRegistry.getSystems()) {
            system->routine();
        }
    }
};

} // namespace guillaume::ecs
