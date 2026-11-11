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
	 * @brief Arranges a segmented button: a row of joined toggle buttons of
	 * which one (Single mode) or several (Multi mode) are selected.
	 *
	 * The segmented button entity is a horizontal layout container. Each
	 * segment is a common button child (components::Button plus
	 * components::CommonButton) built by entities::SegmentedButton and
	 * recorded by identifier in components::SegmentedButton. There is no
	 * separate selection state: a segment is selected when its button is.
	 *
	 * - **Reads**: components::SegmentedButton (segment options, selection
	 *   mode, button identifiers), the selection and components::Bound of
	 *   each button.
	 * - **Writes**: on each button, the icon glyph (SelectedGlyph when
	 *   selected, the segment icon otherwise), the minimum width and the
	 *   corner override of its components::CommonButton, and through
	 *   systems::Button its selection; on the container, the children of
	 *   its components::Layout, which it then arranges through
	 *   systems::Layout::apply.
	 * - **Phase**: Layout. It is registered after systems::Layout and after
	 *   systems::CommonButton, so the buttons have their size for this
	 *   frame.
	 *
	 * @code
	 * // Select the second segment from code; no callback is called.
	 * systems::SegmentedButton::select(registry, identifier, 1);
	 * const auto selected =
	 * 	systems::SegmentedButton::selectedIndices(registry, identifier);
	 * @endcode
	 *
	 * All segments get the width of the widest one, and at least
	 * components::SegmentedButton::MinSegmentWidth. The list of children in
	 * the container's components::Layout is overwritten each frame.
	 *
	 * @see guillaume::components::SegmentedButton,
	 * guillaume::systems::CommonButton, guillaume::systems::Button
	 */
	class SegmentedButton: public ecs::SystemFiller<components::SegmentedButton>
	{
		public:
		/// Name of the glyph a selected segment shows in place of its own
		/// icon.
		static constexpr const char *SelectedGlyph = "check";

		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have a components::SegmentedButton.
		 */
		SegmentedButton(void);

		/**
		 * @brief Default destructor.
		 */
		~SegmentedButton(void) override = default;

		/**
		 * @brief Make an entity behave as a segmented button.
		 *
		 * Configures the container's components::Layout as a horizontal row
		 * packed at the start, centered across, with no spacing and no
		 * padding. Then, for every button recorded in the component: makes
		 * it a toggle, makes its click call segmentChanged with its index,
		 * enables or disables it and sets its accessibility label from the
		 * segment options at the same index, and restyles it.
		 *
		 * Call it once the buttons exist and are recorded in the component,
		 * and again whenever the segments or their buttons change. A button
		 * identifier that is invalid or has no components::Button is
		 * skipped. It does not place anything: `apply` does.
		 * @param componentRegistry The registry holding the components. The
		 * click handlers keep its address, so it must outlive the entity.
		 * @param entityIdentifier The segmented button entity. It must have
		 * a components::SegmentedButton and a components::Layout.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Lay out the row from the current selection of the buttons.
		 *
		 * In order:
		 * 1. each button shows SelectedGlyph when selected and the icon of
		 *    its segment otherwise;
		 * 2. every button gets, as minimum width, the width of the widest
		 *    one (at least components::SegmentedButton::MinSegmentWidth);
		 * 3. the buttons become the layout children of the container and
		 *    systems::Layout::apply places them in a row;
		 * 4. the corners are joined: only the left corners of the first
		 *    button and the right corners of the last one are rounded, with
		 *    a radius of half the height of the container.
		 *
		 * A button is restyled each time one of these steps changes it, so
		 * the result is visible at once. The frame calls it through
		 * `update`; select, deselect, segmentChanged and setSegmentDisabled
		 * call it too. Call it yourself after changing the selection of a
		 * segment's button directly.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Select a segment from code and show it at once.
		 *
		 * In Single mode every other segment is deselected; in Multi mode
		 * the others keep their state. The change callback is not called
		 * and a disabled segment can be selected this way.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The zero-based index of the segment. The call does
		 * nothing when it is out of range.
		 */
		static void select(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Deselect a segment from code and show it at once.
		 *
		 * The change callback is not called. In Single mode this can leave
		 * no segment selected, which a user press cannot do.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The zero-based index of the segment. The call does
		 * nothing when it is out of range.
		 */
		static void deselect(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index);

		/**
		 * @brief React to the user clicking a segment.
		 *
		 * It is the click handler install gives each button, so it runs
		 * after systems::Button::release has already toggled the button. In
		 * Single mode it makes the clicked segment the only selected one;
		 * clicking the selected segment keeps it selected. In Multi mode
		 * the toggle stands as it is. It then calls `apply` and the change
		 * callback of the component with the index.
		 *
		 * Call it directly to simulate a click in a test; unlike select, it
		 * calls the callback.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The zero-based index of the clicked segment. The
		 * call does nothing, and calls nothing, when it is out of range or
		 * the segment has no button.
		 */
		static void
			segmentChanged(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index);

		/**
		 * @brief Enable or disable one segment and show it at once.
		 *
		 * Records the flag in the segment options of the component, writes
		 * it to the components::StyleState of the segment's button (a
		 * disabled button ignores clicks and is drawn disabled), restyles
		 * that button and lays the row out again. The selection of the
		 * segment is kept.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The zero-based index of the segment. Out of range,
		 * nothing is changed.
		 * @param disabled True to disable the segment, false to enable it.
		 */
		static void
			setSegmentDisabled(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index, bool disabled);

		/**
		 * @brief Whether a segment is selected.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @param index The zero-based index of the segment.
		 * @return True when the button of the segment is selected; false
		 * when it is not, when the index is out of range or when the
		 * segment has no button.
		 */
		static bool isSelected(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index);

		/**
		 * @brief List the selected segments.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The segmented button entity.
		 * @return The zero-based indices of the selected segments, in
		 * ascending order; empty when none is selected. In Single mode it
		 * holds at most one index after a user click.
		 */
		static std::vector<std::size_t>
			selectedIndices(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Frame entry point: call `apply` on one segmented button.
		 * @param entityIdentifier The segmented button entity. An entity
		 * without a components::SegmentedButton is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
