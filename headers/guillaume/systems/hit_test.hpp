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

#include <utility/graphic/pose.hpp>
#include <utility/graphic/ray.hpp>
#include <utility/math/vector.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::systems
{

	/**
	 * @file
	 * @brief Free functions the input systems share to decide whether a
	 * pointer or hand ray hits an entity.
	 *
	 * Used by systems::MouseMotion, systems::MouseButton, the hand systems
	 * built on systems::HandEventSystem, systems::Drag and systems::Focus.
	 * Everything here is inline and reads components only.
	 */

	/**
	 * @brief Whether an entity accepts input.
	 *
	 * An entity without a components::StyleState always does; an entity
	 * with one follows its enabled flag. The input systems call this before
	 * hit-testing, so a disabled widget is never hovered, pressed, dragged
	 * or focused.
	 *
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to test.
	 * @return False only when the entity has a StyleState that is disabled.
	 */
	inline bool isInteractive(const ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		return !registry.hasComponent<components::StyleState>(entityIdentifier)
			|| registry.getComponent<components::StyleState>(entityIdentifier)
				   .isEnabled();
	}

	/**
	 * @brief Extra hit area of an entity around its Bound, in dp.
	 *
	 * A small control (an 18 dp checkbox box) sets an accessibility margin
	 * so it can be hit on a larger touch target without being drawn larger.
	 * The margin is read from components::MouseHoverInteraction when the
	 * entity has one, otherwise from components::HandHoverInteraction.
	 *
	 * @param registry The registry holding the components.
	 * @param entityIdentifier The entity to read the margin of.
	 * @return The margin added on each side: x on the left and on the
	 * right, y above and below. (0, 0) when the entity has neither hover
	 * component.
	 */
	inline utility::math::Vector2F
		hitMargin(const ecs::ComponentRegistry &registry,
				  const ecs::Entity::Identifier &entityIdentifier)
	{
		if (registry.hasComponent<components::MouseHoverInteraction>(
				entityIdentifier)) {
			const auto margin =
				registry
					.getComponent<components::MouseHoverInteraction>(
						entityIdentifier)
					.getAccessibilityMargin();
			return utility::math::Vector2F(
				{ static_cast<float>(margin.getWidth()),
				  static_cast<float>(margin.getHeight()) });
		}
		if (registry.hasComponent<components::HandHoverInteraction>(
				entityIdentifier)) {
			const auto margin =
				registry
					.getComponent<components::HandHoverInteraction>(
						entityIdentifier)
					.getAccessibilityMargin();
			return utility::math::Vector2F(
				{ static_cast<float>(margin.getWidth()),
				  static_cast<float>(margin.getHeight()) });
		}
		return utility::math::Vector2F({ 0.0f, 0.0f });
	}

	/**
	 * @brief Whether a ray crosses the rectangle of an entity, grown by a
	 * margin.
	 *
	 * The rectangle starts at the pose position, has the size of the bound
	 * and lies in the plane given by the pose orientation. With a margin,
	 * its origin is moved back by the margin along the orientation's right
	 * and up axes and its size grows by twice the margin.
	 *
	 * @code
	 * const auto pose = registry.getComponent<components::Transform>(id)
	 * 	.getPose();
	 * if (hitTest(ray, pose, registry.getComponent<components::Bound>(id),
	 * 		hitMargin(registry, id))) {
	 * 	// the pointer is over the entity
	 * }
	 * @endcode
	 *
	 * @param ray The pointer or hand ray, in world space.
	 * @param pose The world pose of the entity: the origin corner of its
	 * rectangle and its orientation.
	 * @param bound The size of the entity, in dp.
	 * @param margin The margin added on each side, in dp, as returned by
	 * hitMargin().
	 * @return True when the ray hits the grown rectangle.
	 */
	inline bool hitTest(const utility::graphic::RayF &ray,
						const utility::graphic::PoseF &pose,
						const components::Bound &bound,
						const utility::math::Vector2F &margin)
	{
		const auto size =
			utility::math::Vector2F({ bound.getWidth() + 2.0f * margin[0],
									  bound.getHeight() + 2.0f * margin[1] });

		if (margin[0] == 0.0f && margin[1] == 0.0f) {
			return ray.intersectRectangle(pose, size);
		}

		const auto &orientation				 = pose.getOrientation();
		utility::graphic::PositionF position = pose.getPosition();
		position.translate(
			utility::graphic::PositionF(-(orientation.getRight() * margin[0])
										- (orientation.getUp() * margin[1])));

		return ray.intersectRectangle(
			utility::graphic::PoseF(position, orientation), size);
	}

}	 // namespace guillaume::systems
