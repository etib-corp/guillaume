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
#include <vector>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Arranges the children of a container in a row or a column and
	 * sizes the container around them.
	 *
	 * Runs in the Layout phase on every entity that has Layout, Transform
	 * and Bound.
	 * - Reads: the container's components::Layout (axis, alignments,
	 *   spacing, padding, fixed size and the list of children to arrange),
	 *   its components::Transform, and the components::Bound of each child.
	 * - Writes: the position of each child's components::Transform, and the
	 *   container's components::Bound (the fixed size when one is set,
	 *   otherwise the size of the content plus the padding).
	 *
	 * The children arranged are the ones listed in
	 * components::Layout::getChildren, in that order. A child of the entity
	 * tree that is not in the list (a switch thumb, a label placed by its
	 * widget system) keeps the pose its owner gave it.
	 *
	 * The rules live in the static apply(), so a widget system can arrange a
	 * container at once instead of waiting for the frame:
	 * @code
	 * const auto &layout =
	 * 	registry.getComponent<components::Layout>(container);
	 * systems::Layout::apply(registry, container, layout.getChildren(),
	 * 	systems::Layout::layerOf(registry, container));
	 * @endcode
	 *
	 * Things to know:
	 * - Children are never resized. Their Bound comes from the Measure phase
	 *   (text, icons) or from their own layout.
	 * - The Layout phase visits parents before children. A container
	 *   therefore reads the Bound a nested container computed on the
	 *   previous frame, and a change of size deep in the tree reaches the
	 *   root one level per frame.
	 * - The container's Bound is overwritten every frame: set a fixed width
	 *   or height on components::Layout to impose a size.
	 * - Children are offset along the world X and Y axes from the
	 *   container's position; only the depth lift follows the container's
	 *   orientation.
	 * - The widget systems are registered after this one because they place
	 *   their own children from the bounds resolved here.
	 *
	 * @see components::Layout, components::Hierarchy
	 * @see systems::registerCoreSystems
	 */
	class Layout:
		public ecs::SystemFiller<components::Layout, components::Transform,
								 components::Bound>
	{
		public:
		/**
		 * @brief Depth of one layer: the distance a child is moved towards
		 * the viewer, against the forward axis of its container, for each
		 * layer.
		 */
		static constexpr float LayerDepthStep = 1.0f;

		/**
		 * @brief Number of layers added between a container and what is
		 * placed on it.
		 *
		 * Text, icons and other children sit in front of the surface of
		 * their container; without this gap both would be at the same depth
		 * and flicker (z-fighting).
		 */
		static constexpr std::uint32_t ContentLayerOffset = 1;

		/**
		 * @brief Create the system in the Layout phase.
		 */
		Layout(void);

		/**
		 * @brief Destroy the system. It owns no resource.
		 */
		~Layout(void) override = default;

		/**
		 * @brief Arrange the children listed in the components::Layout of
		 * one container, using the layer of its components::Hierarchy.
		 * @param entityIdentifier The container entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Arrange children inside a container and size the container.
		 *
		 * Children are placed one after the other along the axis of the
		 * container's components::Layout, separated by its spacing and
		 * inset by its padding, then aligned along both axes. Each child is
		 * lifted `layer + ContentLayerOffset` layers in front of the
		 * container. The container's components::Bound receives the fixed
		 * size when one is set, otherwise the content size plus twice the
		 * padding.
		 *
		 * @param registry The registry holding the components.
		 * @param parentIdentifier The container. It must have Layout,
		 * Transform and Bound.
		 * @param childIdentifiers The children to arrange, in order.
		 * ecs::Entity::InvalidIdentifier and children without Transform or
		 * Bound are skipped and take no room.
		 * @param layer The layer of the container, as returned by layerOf().
		 * @throws ecs::EntityComponentNotFoundException When the container
		 * lacks one of the three components.
		 */
		static void
			apply(ecs::ComponentRegistry &registry,
				  const ecs::Entity::Identifier &parentIdentifier,
				  const std::vector<ecs::Entity::Identifier> &childIdentifiers,
				  std::uint32_t layer = 0);

		/**
		 * @brief Get the layer of an entity: its depth in the entity tree.
		 * @param registry The registry holding the components.
		 * @param identifier The entity to look up.
		 * @return The layer stored in its components::Hierarchy, or 0 when
		 * the entity has no Hierarchy.
		 */
		static std::uint32_t layerOf(const ecs::ComponentRegistry &registry,
									 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Move a position in front of a container.
		 *
		 * The position is moved by `(layer + ContentLayerOffset) *
		 * LayerDepthStep` towards the viewer, against the forward axis of
		 * @p orientation.
		 * @param position The position to lift, usually computed in the
		 * plane of the container.
		 * @param orientation The orientation of the container. It is copied
		 * into the returned pose.
		 * @param layer The layer of the container, as returned by layerOf().
		 * @return The lifted position with @p orientation.
		 */
		static const utility::graphic::PoseF applyLayerToPosition(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation,
			const std::uint32_t &layer);

		/**
		 * @brief Compute the pose of content placed by hand on a container.
		 *
		 * Same result as applyLayerToPosition(). Widget systems that place a
		 * text or an icon themselves call it so that everything drawn on a
		 * container sits at the same distance in front of it.
		 *
		 * @param position The position of the content in the plane of the
		 * container.
		 * @param orientation The orientation of the container.
		 * @param containerLayer The layer of the container, as returned by
		 * layerOf().
		 * @return The pose to give to the content.
		 */
		static const utility::graphic::PoseF
			contentLayerPose(const utility::graphic::PositionF &position,
							 const utility::graphic::OrientationF &orientation,
							 const std::uint32_t &containerLayer);
	};

}	 // namespace guillaume::systems
