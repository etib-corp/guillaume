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

#include "ecs/system_filler.hpp"

#include "components/interaction/hoverable.hpp"
#include "components/state/tooltip.hpp"
#include "components/visual/transform.hpp"

namespace guillaume::systems::interaction {

/**
 * @brief TooltipSystem shows/hides tooltips based on hover or focus.
 *
 * Processes entities with Tooltip, Hoverable, and Transform components.
 */
class TooltipSystem
    : public ecs::SystemFiller<components::state::Tooltip,
                               components::interaction::Hoverable,
                               components::visual::Transform> {
  public:
    /**
     * @brief Default constructor.
     */
    TooltipSystem(void) = default;

    /**
     * @brief Default destructor.
     */
    ~TooltipSystem(void) override = default;

    /**
     * @brief Update the tooltip system and manage tooltip visibility.
     * @param identityIdentifier The identifier of the entity to update.
     */
    void update(const ecs::Entity::Identifier &identityIdentifier) override;
};

} // namespace guillaume::systems::interaction
