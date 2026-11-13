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
	 * @brief Configuration of a segmented button.
	 *
	 * This component is the whole state of a segmented button: the options of
	 * each segment, the selection mode and the toggle button child of each
	 * segment. Whether a segment is selected is the selection of its button
	 * child. systems::SegmentedButton turns it into the icons, widths, row
	 * placement and connected corners of those children.
	 */
	class SegmentedButton: public ecs::Component
	{
		public:
		/**
		 * @brief Selection mode of the segmented button.
		 */
		enum class SelectionMode { Single, Multi };

		/**
		 * @brief Per-segment configuration (Paper `buttons[]` entry).
		 */
		struct Segment {
			std::string label {};				  ///< Visible label.
			std::string icon {};				  ///< Optional leading icon.
			bool disabled { false };			  ///< Whether disabled.
			std::string accessibilityLabel {};	  ///< Accessible name.

			/**
			 * @brief Compare two segment configurations.
			 * @param other The configuration to compare with.
			 * @return True when every option is equal.
			 */
			bool operator==(const Segment &other) const = default;
		};

		/// Called with the segment index when the user presses a segment.
		using ChangeHandler = std::function<void(std::size_t)>;

		/// Minimum segment width (Paper SegmentedButtons: 76 dp).
		static constexpr float MinSegmentWidth = 76.0f;

		private:
		std::vector<Segment> _segments {};	  ///< Segment options
		SelectionMode _selectionMode {
			SelectionMode::Single
		};	  ///< Selection mode

		std::vector<ecs::Entity::Identifier>
			_buttons {};			   ///< Toggle button child of each segment
		ChangeHandler _onChange {};	   ///< User press callback

		public:
		/**
		 * @brief Default constructor.
		 */
		SegmentedButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~SegmentedButton(void) = default;

		/** @brief Get the segment options. @return The options. */
		const std::vector<Segment> &getSegments(void) const noexcept;

		/** @brief Set the segment options. @param segments They. @return It.
		 */
		SegmentedButton &setSegments(const std::vector<Segment> &segments);

		/**
		 * @brief Record whether one segment is disabled.
		 * @param index The segment index (ignored when out of range).
		 * @param disabled True when disabled.
		 * @return It.
		 */
		SegmentedButton &setSegmentDisabled(std::size_t index, bool disabled);

		/** @brief Get the selection mode. @return The mode. */
		SelectionMode getSelectionMode(void) const noexcept;

		/** @brief Set the selection mode. @param mode It. @return It. */
		SegmentedButton &setSelectionMode(SelectionMode mode);

		/** @brief Get the button children. @return Their identifiers, in
		 * segment order. */
		const std::vector<ecs::Entity::Identifier> &
			getButtons(void) const noexcept;

		/** @brief Set the button children. @param buttons Their identifiers,
		 * in segment order. @return It. */
		SegmentedButton &
			setButtons(const std::vector<ecs::Entity::Identifier> &buttons);

		/** @brief Get the press callback. @return The callback. */
		const ChangeHandler &getOnChange(void) const noexcept;

		/** @brief Set the press callback. @param handler It. @return It. */
		SegmentedButton &setOnChange(ChangeHandler handler);
	};

}	 // namespace guillaume::components
