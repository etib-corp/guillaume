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

#include <vector>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System arranging the children of a layout container.
	 *
	 * The system visits every entity carrying a `components::Layout` and
	 * derives each child `Transform`/`Bound` from the parent layout slot. The
	 * children are discovered through the entity hierarchy (see
	 * `ecs::ParentEntity`), so containers only have to attach their children
	 * as usual.
	 *
	 * The geometry computation is exposed through the static `apply` helper
	 * so that entities and tests can reuse it without running the full system
	 * loop.
	 *
	 * @see components::Layout
	 */
	class Layout:
		public ecs::SystemFiller<components::Layout, components::Transform,
								 components::Bound>
	{
		public:
		/**
		 * @brief Distance pushed toward the camera per layer.
		 */
		static constexpr float LayerDepthStep = 1.0f;

		/**
		 * @brief Construct a layout system running in the Layout phase.
		 */
		Layout(void);

		/**
		 * @brief Default destructor.
		 */
		~Layout(void) override = default;

		/**
		 * @brief Update the layout of one container entity.
		 * @param entityIdentifier The container entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Arrange the given children inside a layout container.
		 *
		 * Reads the parent `components::Layout`, `components::Transform` and
		 * `components::Bound`, positions every child along the layout axis,
		 * applies the main/cross axis alignment and writes the resulting size
		 * back to the parent `components::Bound`.
		 *
		 * @param registry The component registry holding the entities.
		 * @param parentIdentifier The container entity identifier.
		 * @param childIdentifiers The direct children to arrange, in order.
		 * @param layer The container layer, used to lift children toward the
		 * camera and avoid z-fighting with the container surface.
		 */
		static void
			apply(ecs::ComponentRegistry &registry,
				  const ecs::Entity::Identifier &parentIdentifier,
				  const std::vector<ecs::Entity::Identifier> &childIdentifiers,
				  std::uint32_t layer = 0);

		/**
		 * @brief Apply the current layer depth offset to a position.
		 * @param position The base position.
		 * @param orientation The orientation.
		 * @param layer The layer of the parent container.
		 * @return The offset pose.
		 */
		static const utility::graphic::PoseF applyLayerToPosition(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation,
			const std::uint32_t &layer);
	};

}	 // namespace guillaume::systems
