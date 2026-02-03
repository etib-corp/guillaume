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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

namespace guillaume::systems {

/**
 * @brief System handling rendering of entities.
 */
class Render
    : public ecs::SystemFiller<components::Transform, components::Bound> {
  private:
    Renderer &_renderer; ///< Renderer instance

  public:
    /**
     * @brief Default constructor for the Render system.
     * @param renderer The renderer to use for rendering operations.
     */
    Render(Renderer &renderer) : _renderer(renderer) {}

    /**
     * @brief Default destructor for the Render system.
     */
    ~Render(void) = default;

    /**
     * @brief Update the Render system for the specified entity.
     * @param componentRegistry The component registry instance.
     * @param identityIdentifier The identifier of the entity to update.
     */
    void update(ecs::ComponentRegistry &componentRegistry,
                const ecs::Entity::Identifier &identityIdentifier) override;
};

} // namespace guillaume::systems