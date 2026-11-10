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
	 * @brief Check whether an entity accepts interaction.
	 *
	 * Entities without a `StyleState` are always interactive; entities with
	 * one follow its enabled flag.
	 *
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @return False when the entity is disabled.
	 */
	inline bool isInteractive(const ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		return !registry.hasComponent<components::StyleState>(entityIdentifier)
			|| registry.getComponent<components::StyleState>(entityIdentifier)
				   .isEnabled();
	}

	/**
	 * @brief Get the accessibility hit margin of an entity.
	 *
	 * The margin is read from `MouseHoverInteraction` (or, failing that,
	 * `HandHoverInteraction`) so that a small control can grow its hit area
	 * to the M3 48 dp touch target without growing visually.
	 *
	 * @param registry Component registry.
	 * @param entityIdentifier Entity identifier.
	 * @return The horizontal and vertical margin added on each side.
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
	 * @brief Intersect a ray with an entity bound grown by a margin.
	 * @param ray World ray.
	 * @param pose Entity pose (top-left corner).
	 * @param bound Entity bound.
	 * @param margin Margin added on each side, along right and up.
	 * @return True when the ray hits the grown rectangle.
	 */
	inline bool hitTest(const utility::graphic::RayF &ray,
						const utility::graphic::PoseF &pose,
						const components::Bound &bound,
						const utility::math::Vector2F &margin)
	{
		const auto size = utility::math::Vector2F(
			{ bound.getWidth() + 2.0f * margin[0],
			  bound.getHeight() + 2.0f * margin[1] });

		if (margin[0] == 0.0f && margin[1] == 0.0f) {
			return ray.intersectRectangle(pose, size);
		}

		const auto &orientation = pose.getOrientation();
		utility::graphic::PositionF position = pose.getPosition();
		position.translate(utility::graphic::PositionF(
			-(orientation.getRight() * margin[0])
			- (orientation.getUp() * margin[1])));

		return ray.intersectRectangle(
			utility::graphic::PoseF(position, orientation), size);
	}

}	 // namespace guillaume::systems
