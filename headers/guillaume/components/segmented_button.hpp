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
#include <functional>
#include <string>
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a segmented button: a row of joined toggle buttons of
	 * which one (or several) can be selected.
	 *
	 * It holds the options of each segment, the selection mode, the press
	 * callback and the identifier of the toggle button child of each
	 * segment. The selection itself is not stored here: a segment is
	 * selected when its button child is (components::Button::isSelected).
	 *
	 * Role in the frame:
	 * - entities::SegmentedButton writes the configuration, creates one
	 *   button child per segment and records their identifiers here.
	 * - systems::SegmentedButton (Layout phase) reads it: it gives a
	 *   selected segment a check glyph, makes all segments as wide as the
	 *   widest, places them in a row and rounds only the outer corners of
	 *   the row.
	 *
	 * To select a segment from code, call
	 * systems::SegmentedButton::select() or `deselect()`; to read the
	 * selection, `isSelected()` or `selectedIndices()`.
	 *
	 * @see systems::SegmentedButton, components::Button,
	 * components::CommonButton
	 */
	class SegmentedButton: public ecs::Component
	{
		public:
		/**
		 * @brief How many segments can be selected at once.
		 *
		 * Single: selecting a segment deselects the others, and pressing
		 * the selected segment keeps it selected. Multi: each segment
		 * toggles on its own.
		 */
		enum class SelectionMode { Single, Multi };

		/**
		 * @brief What one segment shows and whether it can be pressed.
		 */
		struct Segment {
			std::string label {};	 ///< Visible label.
			/// Glyph name of the leading icon shown while not selected;
			/// empty for none.
			std::string icon {};
			bool disabled { false };	///< Whether the segment is disabled.
			/// Name announced by assistive technology; empty to keep the
			/// one of the button.
			std::string accessibilityLabel {};

			/**
			 * @brief Compare two segment configurations.
			 * @param other The configuration to compare with.
			 * @return True when every option is equal.
			 */
			bool operator==(const Segment &other) const = default;
		};

		/// Called with the index of the segment the user just pressed.
		using ChangeHandler = std::function<void(std::size_t)>;

		/// Minimum width of a segment, in dp.
		static constexpr float MinSegmentWidth = 76.0f;

		private:
		std::vector<Segment> _segments {};	  ///< Segment options, in order
		SelectionMode _selectionMode {
			SelectionMode::Single
		};	  ///< Selection mode

		std::vector<ecs::Entity::Identifier>
			_buttons {};			   ///< Toggle button child of each segment
		ChangeHandler _onChange {};	   ///< User press callback

		public:
		/**
		 * @brief Create the state of a single-selection segmented button
		 * with no segment.
		 */
		SegmentedButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~SegmentedButton(void) = default;

		/**
		 * @brief Get the options of every segment.
		 * @return The options, in the order the segments are laid out.
		 */
		const std::vector<Segment> &getSegments(void) const noexcept;

		/**
		 * @brief Replace the options of every segment.
		 *
		 * Marks the component changed when the list differs. The widget
		 * class creates one button child per entry.
		 * @param segments The new options, in layout order.
		 * @return This component, for chaining.
		 */
		SegmentedButton &setSegments(const std::vector<Segment> &segments);

		/**
		 * @brief Record whether one segment is disabled.
		 *
		 * It only updates the stored option, and marks the component
		 * changed when it differs. Use
		 * systems::SegmentedButton::setSegmentDisabled() to also disable
		 * and restyle the segment's button.
		 * @param index The segment index (ignored when out of range).
		 * @param disabled True when the segment cannot be pressed.
		 * @return This component, for chaining.
		 */
		SegmentedButton &setSegmentDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Get the selection mode.
		 * @return Single (the default) or Multi.
		 */
		SelectionMode getSelectionMode(void) const noexcept;

		/**
		 * @brief Choose how many segments can be selected at once.
		 *
		 * Marks the component changed when the mode differs. It does not
		 * touch the current selection.
		 * @param mode Single or Multi.
		 * @return This component, for chaining.
		 */
		SegmentedButton &setSelectionMode(SelectionMode mode);

		/**
		 * @brief Get the toggle button child of each segment.
		 * @return Their identifiers, in segment order; entry `i` is the
		 * button of `getSegments()[i]`.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getButtons(void) const noexcept;

		/**
		 * @brief Record the toggle button child of each segment.
		 *
		 * Marks the component changed when the list differs.
		 * @param buttons Their identifiers, in segment order.
		 * @return This component, for chaining.
		 */
		SegmentedButton &
			setButtons(const std::vector<ecs::Entity::Identifier> &buttons);

		/**
		 * @brief Get the callback run when the user presses a segment.
		 * @return The callback, empty when none is set.
		 */
		const ChangeHandler &getOnChange(void) const noexcept;

		/**
		 * @brief Set the callback run when the user presses a segment.
		 *
		 * It runs after the selection was updated, and not for a selection
		 * made from code. Does not mark the component changed.
		 * @param handler The callback; it receives the index of the pressed
		 * segment.
		 * @return This component, for chaining.
		 */
		SegmentedButton &setOnChange(ChangeHandler handler);
	};

}	 // namespace guillaume::components
