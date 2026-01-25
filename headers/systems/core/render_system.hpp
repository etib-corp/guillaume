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

#include "components/visual/border.hpp"
#include "components/visual/opacity.hpp"
#include "components/visual/rectangle.hpp"
#include "components/visual/sprite.hpp"
#include "components/visual/text.hpp"
#include "components/visual/transform.hpp"

namespace guillaume::systems::core {

/**
 * @brief RenderSystem draws all visible UI elements on screen.
 *
 * Processes entities with Transform, Sprite, Text, Opacity, and Border
 * components.
 */
class RenderSystem
    : public ecs::SystemFiller<
          components::visual::Transform, components::visual::Sprite,
          components::visual::Text, components::visual::Opacity,
          components::visual::Border, components::visual::Rectangle> {
  public:
    /**
     * @brief Default constructor.
     */
    RenderSystem(void) = default;

    /**
     * @brief Default destructor.
     */
    ~RenderSystem(void) override = default;

    /**
     * @brief Update the render system and draw all visible entities.
     * @param identityIdentifier The identifier of the entity to update.
     */
    void update(const ecs::Entity::Identifier &identityIdentifier) override;
};

} // namespace guillaume::systems::core
