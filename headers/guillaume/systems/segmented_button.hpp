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

#include <cstddef>
#include <vector>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/segmented_button.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Arranges the toggle buttons of a segmented button.
	 *
	 * Reads components::SegmentedButton and the selection of its button
	 * children, and writes their icon (a check when selected), their common
	 * width, the row the Layout system arranges and the connected corner
	 * radii. Runs in the Layout phase, after the buttons have been styled.
	 */
	class SegmentedButton: public ecs::SystemFiller<components::SegmentedButton>
	{
		public:
		/// Glyph shown on a selected segment.
		static constexpr const char *SelectedGlyph = "check";

		/**
		 * @brief Default constructor.
		 */
		SegmentedButton(void);

		/**
		 * @brief Default destructor.
		 */
		~SegmentedButton(void) override = default;

		/**
		 * @brief Make an entity behave as a segmented button.
		 *
		 * Configures the row layout, then makes every button recorded in the
		 * component a toggle, applies its segment options (disabled,
		 * accessibility label) and wires its click to this system. Call it
		 * again after the button children change. The entity must have a
		 * components::SegmentedButton and a components::Layout.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Set the segment icons, equalize the widths, place the
		 * segments in a row and connect their corners.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select a segment programmatically (no callback). In single
		 * selection mode the other segments are deselected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The segment index (ignored when out of range).
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Deselect a segment programmatically (no callback).
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The segment index (ignored when out of range).
		 */
		static void deselect(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index);

		/**
		 * @brief React to the user pressing a segment, once its button has
		 * toggled: enforce the selection mode, restyle and call the change
		 * callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The segment index (ignored when out of range).
		 */
		static void
			segmentChanged(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Enable or disable one segment.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The segment index.
		 * @param disabled True to disable.
		 */
		static void
			setSegmentDisabled(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index, bool disabled);

		/**
		 * @brief Whether a segment is selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The segment index.
		 * @return True when its button is selected, false when it is not or
		 * the index is out of range.
		 */
		static bool isSelected(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index);

		/**
		 * @brief Get the indices of the selected segments.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @return The indices, in ascending order.
		 */
		static std::vector<std::size_t>
			selectedIndices(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Arrange the segmented button with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
