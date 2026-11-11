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
	 * @brief Turns a components::Divider into geometry.
	 *
	 * For every entity carrying a Divider configuration it writes the
	 * Transform (origin shifted by the leading inset), the Bound (length
	 * minus the insets, by the thickness), the Line thickness and the Color.
	 * It runs in the Measure phase so the size is known before layout.
	 *
	 * All the behavior of a divider lives here; the entity is only an
	 * identifier with components.
	 */
	class Divider:
		public ecs::SystemFiller<components::Divider, components::Transform,
								 components::Bound, components::Color,
								 components::Line>
	{
		public:
		/**
		 * @brief Construct the system.
		 */
		Divider(void);

		/**
		 * @brief Default destructor.
		 */
		~Divider(void) override = default;

		/**
		 * @brief Apply a divider configuration to its entity right now.
		 *
		 * update() calls this every frame. Call it directly when the result
		 * is needed before the next frame, for example right after changing
		 * the configuration while building a scene. The entity must carry
		 * the Divider, Transform, Bound, Color and Line components.
		 *
		 * If the Transform was moved by something else since the last call
		 * (a layout or a scene placing the entity), the new position is
		 * adopted as the divider's origin.
		 * @param componentRegistry Registry holding the components.
		 * @param entityIdentifier Identifier of the divider entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Apply the configuration of one divider entity.
		 * @param entityIdentifier Identifier of the entity being updated.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
