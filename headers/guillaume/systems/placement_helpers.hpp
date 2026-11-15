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

#include <cstdint>

#include <utility/graphic/pose.hpp>
#include <utility/graphic/position.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/layout_extent.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/layout.hpp"

/**
 * @file
 * @brief Placement helpers for the children systems::Layout does not
 * arrange.
 *
 * Some widgets position a child by hand instead of through a
 * components::Layout: the label beside a checkbox or a radio button, the
 * handle of a switch, the badge of a navigation bar destination, the
 * supporting text of a text field, the parts of a progress or a loading
 * indicator. Their systems call the inline functions of
 * guillaume::systems::placement from `apply`, in the Layout phase, once
 * systems::Layout has resolved the bound of the parent.
 * `guillaume/entities/placement_helpers.hpp` re-exports the same names in
 * guillaume::entities for the widget classes.
 *
 * Every function writes the components::Transform of the child and nothing
 * else, and does nothing when a component it needs is missing, so an
 * optional child can be passed as ecs::Entity::InvalidIdentifier. Offsets
 * are in dp, measured from the position of the parent's
 * components::Transform.
 */

/**
 * @namespace guillaume::systems::placement
 * @brief Helpers that position a child relative to its parent by writing
 * the child's components::Transform.
 * @see guillaume::systems::Layout, guillaume::systems::style
 */
namespace guillaume::systems::placement
{

	/**
	 * @brief Read the pose (position and orientation) of an entity.
	 *
	 * The other helpers use it to get the pose of the parent a child is
	 * placed against.
	 * @param registry The registry holding the components.
	 * @param identifier The entity whose components::Transform is read.
	 * @return The pose of the entity, or a default-constructed pose when the
	 * entity has no components::Transform.
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
	 * @brief Place a child at an offset from the position of its parent.
	 *
	 * Writes the pose of the child: the parent position plus the offset,
	 * with the parent orientation, moved toward the viewer by
	 * systems::Layout::contentLayerPose so the child never z-fights with
	 * the surface of the parent. Does nothing when the child has no
	 * components::Transform.
	 *
	 * @code
	 * // Put a label 8 dp right of and 4 dp below the parent position.
	 * placement::placeChild(registry, parent, label,
	 * 	utility::graphic::PositionF(8.0f, 4.0f, 0.0f),
	 * 	Layout::layerOf(registry, parent));
	 * @endcode
	 * @param registry The registry holding the components.
	 * @param parentIdentifier The entity the child is placed against. Without
	 * a components::Transform it counts as a default pose.
	 * @param childIdentifier The entity whose components::Transform is
	 * written.
	 * @param offset The offset from the parent position, in dp. Only x and y
	 * are used; the depth comes from `parentLayer`.
	 * @param parentLayer The tree depth of the parent, as returned by
	 * systems::Layout::layerOf.
	 */
	inline void placeChild(ecs::ComponentRegistry &registry,
						   const ecs::Entity::Identifier &parentIdentifier,
						   const ecs::Entity::Identifier &childIdentifier,
						   const utility::graphic::PositionF &offset,
						   std::uint32_t parentLayer)
	{
		if (!registry.hasComponent<components::Transform>(childIdentifier)) {
			return;
		}

		const auto origin = poseOf(registry, parentIdentifier);

		const utility::graphic::PositionF basePosition(
			origin.getPosition().getX() + offset.x,
			origin.getPosition().getY() - offset.y,
			origin.getPosition().getZ());

		const auto pose = systems::Layout::contentLayerPose(
			basePosition, origin.getOrientation(), parentLayer);

		registry.getComponent<components::Transform>(childIdentifier)
			.setPose(pose);
	}

	/**
	 * @brief Center a child inside the bound of its parent, on both axes.
	 *
	 * Does nothing when the parent or the child has no components::Bound.
	 * Both bounds must be up to date: call it after the child has been
	 * measured and the parent laid out.
	 * @param registry The registry holding the components.
	 * @param parentIdentifier The entity the child is centered in.
	 * @param childIdentifier The entity whose components::Transform is
	 * written.
	 * @param parentLayer The tree depth of the parent, as returned by
	 * systems::Layout::layerOf.
	 */
	inline void centerChild(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &parentIdentifier,
							const ecs::Entity::Identifier &childIdentifier,
							std::uint32_t parentLayer)
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
				0.0f),
			parentLayer);
	}

	/**
	 * @brief Place a child along the width of its parent from a 0 to 1
	 * value, vertically centered.
	 *
	 * 0 puts the child against the left inset, 1 against the right inset.
	 * systems::Switch places its handle this way (0 when off, 1 when on).
	 * Does nothing when the parent or the child has no components::Bound.
	 * @param registry The registry holding the components.
	 * @param parentIdentifier The entity the child travels in.
	 * @param childIdentifier The entity whose components::Transform is
	 * written.
	 * @param normalized The position along the available travel, 0 to 1.
	 * The value is not clamped.
	 * @param inset The space kept free at the left and right edges of the
	 * parent, in dp. When the child is wider than what remains, it stays
	 * against the left inset.
	 * @param parentLayer The tree depth of the parent, as returned by
	 * systems::Layout::layerOf.
	 */
	inline void placeNormalized(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &parentIdentifier,
								const ecs::Entity::Identifier &childIdentifier,
								float normalized, float inset,
								std::uint32_t parentLayer)
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
				0.0f),
			parentLayer);
	}

	/**
	 * @brief Reserve layout room after an entity's Bound.
	 *
	 * For a widget that draws a small shape and places a child beside it
	 * (a checkbox and its label): the parent layout then keeps that room
	 * free instead of putting the next widget over the child. Adds the
	 * components::LayoutExtent when needed.
	 * @param registry The registry holding the components.
	 * @param identifier The widget entity.
	 * @param width The room after the Bound, in dp; 0 or less releases it.
	 */
	inline void reserveTrailingWidth(ecs::ComponentRegistry &registry,
									 const ecs::Entity::Identifier &identifier,
									 float width)
	{
		if (!registry.hasComponent<components::LayoutExtent>(identifier)) {
			if (width <= 0.0f) {
				return;
			}
			registry.addComponent<components::LayoutExtent>(identifier);
		}
		registry.getComponent<components::LayoutExtent>(identifier)
			.setTrailingWidth(width);
	}

}	 // namespace guillaume::systems::placement
