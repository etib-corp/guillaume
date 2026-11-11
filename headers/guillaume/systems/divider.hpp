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
#include "guillaume/components/color.hpp"
#include "guillaume/components/divider.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Turns the configuration of a divider (a thin horizontal
	 * separator line) into the geometry that is drawn.
	 *
	 * Runs in the Measure phase on entities with Divider, Transform, Bound,
	 * Color and Line, so the size is known before the layout.
	 * - Reads: components::Divider (length, thickness, leading and trailing
	 *   insets, color, origin pose).
	 * - Writes: components::Transform (the origin moved right by the
	 *   leading inset), components::Bound (the length minus both insets, by
	 *   the thickness), the thickness of components::Line and
	 *   components::Color. systems::LineRender draws the result.
	 * - It also writes back into components::Divider the pose it applied,
	 *   to detect on the next call that something else moved the entity.
	 *
	 * This is the reference example of the widget shape used across the
	 * library: components::Divider holds the state, this system holds the
	 * rules, and entities::Divider only creates the entity. The system can
	 * be driven on a bare identifier:
	 * @code
	 * registry.getComponent<components::Divider>(divider).setLength(240.0f);
	 * systems::Divider::apply(registry, divider);
	 * @endcode
	 *
	 * Transform, Bound, the Line thickness and Color of a divider are
	 * overwritten every frame: change components::Divider, not them.
	 *
	 * @see components::Divider, entities::Divider
	 * @see systems::LineRender
	 */
	class Divider:
		public ecs::SystemFiller<components::Divider, components::Transform,
								 components::Bound, components::Color,
								 components::Line>
	{
		public:
		/**
		 * @brief Create the system in the Measure phase.
		 */
		Divider(void);

		/**
		 * @brief Destroy the system. It owns no resource.
		 */
		~Divider(void) override = default;

		/**
		 * @brief Write the geometry of one divider from its configuration.
		 *
		 * update() calls this every frame. Call it directly when the result
		 * is needed before the next frame, for example right after changing
		 * the configuration while building a scene.
		 *
		 * When the Transform differs from the pose this function applied
		 * last (a layout or a scene moved the entity), the divider stays
		 * where it was put: the origin stored in components::Divider is
		 * recomputed as that position minus the leading inset.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The divider entity. It must have Divider,
		 * Transform, Bound, Color and Line.
		 * @throws ecs::EntityComponentNotFoundException When one of the five
		 * components is missing.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Write the geometry of one divider for this frame.
		 * @param entityIdentifier The divider entity. It is skipped, with a
		 * log message, when one of the five components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
