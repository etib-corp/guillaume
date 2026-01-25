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

#include "components/functional/slider.hpp"
#include "components/interaction/draggable.hpp"
#include "components/visual/transform.hpp"

namespace guillaume::systems::interaction {

/**
 * @brief SliderSystem updates slider values and handles user drag input.
 *
 * Processes entities with Slider, Draggable, and Transform components.
 */
class SliderSystem
    : public ecs::SystemFiller<components::functional::Slider,
                               components::interaction::Draggable,
                               components::visual::Transform> {
  public:
    /**
     * @brief Default constructor.
     */
    SliderSystem(void) = default;

    /**
     * @brief Default destructor.
     */
    ~SliderSystem(void) override = default;

    /**
     * @brief Update the slider system and handle slider interactions.
     * @param identityIdentifier The identifier of the entity to update.
     */
    void update(const ecs::Entity::Identifier &identityIdentifier) override;
};

} // namespace guillaume::systems::interaction
