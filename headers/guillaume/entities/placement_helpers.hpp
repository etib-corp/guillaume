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
#include <utility/graphic/position.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Extract the origin pose of an entity.
	 *
	 * Component families that position children absolutely (sliders, switches,
	 * indicators, clock faces) need the parent pose without going through
	 * `systems::Layout`. This helper centralizes that lookup and returns a
	 * default pose when the transform is missing.
	 *
	 * @param registry The component registry.
	 * @param identifier The entity identifier.
	 * @return The entity pose, or a default pose when absent.
	 */
	inline utility::graphic::PoseF
		poseOf(ecs::ComponentRegistry &registry,
			   const ecs::Entity::Identifier &identifier)
	{
		if (!registry.hasComponent<components::Transform>(identifier)) {
			return utility::graphic::PoseF();
		}

		return registry.getComponent<components::Transform>(identifier)
			.getPose();
	}

	/**
	 * @brief Place a child entity at an absolute position offset from a parent.
	 *
	 * @param registry The component registry.
	 * @param parentIdentifier The parent entity identifier.
	 * @param childIdentifier The child entity identifier.
	 * @param offset The offset applied to the parent position.
	 */
	inline void placeChild(ecs::ComponentRegistry &registry,
						   const ecs::Entity::Identifier &parentIdentifier,
						   const ecs::Entity::Identifier &childIdentifier,
						   const utility::graphic::PositionF &offset)
	{
		if (!registry.hasComponent<components::Transform>(childIdentifier)) {
			return;
		}

		const auto origin = poseOf(registry, parentIdentifier);

		auto &transform =
			registry.getComponent<components::Transform>(childIdentifier);
		auto pose = transform.getPose();
		pose.setPosition(
			utility::graphic::PositionF(origin.getPosition().getX() + offset.x,
										origin.getPosition().getY() + offset.y,
										origin.getPosition().getZ()));
		transform.setPose(pose);
	}

	/**
	 * @brief Center a child entity inside its parent bound.
	 *
	 * @param registry The component registry.
	 * @param parentIdentifier The parent entity identifier.
	 * @param childIdentifier The child entity identifier.
	 */
	inline void centerChild(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &parentIdentifier,
							const ecs::Entity::Identifier &childIdentifier)
	{
		if (!registry.hasComponent<components::Bound>(parentIdentifier)
			|| !registry.hasComponent<components::Bound>(childIdentifier)) {
			return;
		}

		const auto &parentBound =
			registry.getComponent<components::Bound>(parentIdentifier);
		const auto &childBound =
			registry.getComponent<components::Bound>(childIdentifier);

		placeChild(
			registry, parentIdentifier, childIdentifier,
			utility::graphic::PositionF(
				(parentBound.getWidth() - childBound.getWidth()) / 2.0f,
				(parentBound.getHeight() - childBound.getHeight()) / 2.0f,
				0.0f));
	}

	/**
	 * @brief Place a child at a normalized position along the parent width.
	 *
	 * Used by sliders and switches to place a thumb or fill between the
	 * left/right edges of the parent based on a `[0, 1]` value.
	 *
	 * @param registry The component registry.
	 * @param parentIdentifier The parent entity identifier.
	 * @param childIdentifier The child entity identifier.
	 * @param normalized The normalized `[0, 1]` position along the width.
	 * @param inset The horizontal inset reserved on both edges.
	 */
	inline void placeNormalized(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &parentIdentifier,
								const ecs::Entity::Identifier &childIdentifier,
								float normalized, float inset)
	{
		if (!registry.hasComponent<components::Bound>(parentIdentifier)
			|| !registry.hasComponent<components::Bound>(childIdentifier)) {
			return;
		}

		const auto &parentBound =
			registry.getComponent<components::Bound>(parentIdentifier);
		const auto &childBound =
			registry.getComponent<components::Bound>(childIdentifier);

		const float travel =
			parentBound.getWidth() - childBound.getWidth() - 2.0f * inset;

		placeChild(
			registry, parentIdentifier, childIdentifier,
			utility::graphic::PositionF(
				inset + normalized * (travel > 0.0f ? travel : 0.0f),
				(parentBound.getHeight() - childBound.getHeight()) / 2.0f,
				0.0f));
	}

}	 // namespace guillaume::entities
