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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component describing how a container arranges its children.
	 *
	 * The `Layout` component holds the axis (row or column), the main and
	 * cross axis alignment, the spacing between children, the padding around
	 * them and optional fixed dimensions. The actual arrangement is performed
	 * by `systems::Layout`, which reads this component from the parent entity
	 * and derives each child `Transform`/`Bound`.
	 *
	 * @see systems::Layout
	 */
	class Layout: public ecs::Component
	{
		public:
		/**
		 * @brief Direction along which children are arranged.
		 */
		enum class Axis {
			Horizontal,	   ///< Arrange children left to right (row).
			Vertical	   ///< Arrange children top to bottom (column).
		};

		/**
		 * @brief Alignment of the children along the main axis.
		 */
		enum class MainAxisAlignment {
			Start,			///< Children packed at the start.
			Center,			///< Children centered along the main axis.
			End,			///< Children packed at the end.
			SpaceBetween	///< First/last at the edges, equal gaps between.
		};

		/**
		 * @brief Alignment of the children along the cross axis.
		 */
		enum class CrossAxisAlignment {
			Start,	   ///< Children aligned at the cross axis start.
			Center,	   ///< Children centered along the cross axis.
			End		   ///< Children aligned at the cross axis end.
		};

		private:
		Axis _axis { Axis::Horizontal };	///< Layout axis.
		MainAxisAlignment _mainAxisAlignment {
			MainAxisAlignment::Start
		};	  ///< Alignment along the main axis.
		CrossAxisAlignment _crossAxisAlignment {
			CrossAxisAlignment::Center
		};	  ///< Alignment along the cross axis.
		float _spacing { 0.0f };		   ///< Space between children.
		float _padding { 0.0f };		   ///< Space around the children.
		bool _hasFixedWidth { false };	   ///< Whether the width is forced.
		bool _hasFixedHeight { false };	   ///< Whether the height is forced.
		float _fixedWidth { 0.0f };		   ///< Forced container width.
		float _fixedHeight { 0.0f };	   ///< Forced container height.

		public:
		/**
		 * @brief Default constructor for the Layout component.
		 */
		Layout(void) = default;

		/**
		 * @brief Default destructor for the Layout component.
		 */
		~Layout(void) = default;

		/**
		 * @brief Get the layout axis.
		 * @return The layout axis.
		 */
		Axis getAxis(void) const;

		/**
		 * @brief Set the layout axis.
		 * @param axis The new layout axis.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setAxis(Axis axis);

		/**
		 * @brief Get the main axis alignment.
		 * @return The main axis alignment.
		 */
		MainAxisAlignment getMainAxisAlignment(void) const;

		/**
		 * @brief Set the main axis alignment.
		 * @param alignment The new main axis alignment.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setMainAxisAlignment(MainAxisAlignment alignment);

		/**
		 * @brief Get the cross axis alignment.
		 * @return The cross axis alignment.
		 */
		CrossAxisAlignment getCrossAxisAlignment(void) const;

		/**
		 * @brief Set the cross axis alignment.
		 * @param alignment The new cross axis alignment.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setCrossAxisAlignment(CrossAxisAlignment alignment);

		/**
		 * @brief Get the spacing between children.
		 * @return The spacing between children.
		 */
		float getSpacing(void) const;

		/**
		 * @brief Set the spacing between children.
		 * @param spacing The new spacing between children.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setSpacing(float spacing);

		/**
		 * @brief Get the padding around the children.
		 * @return The padding around the children.
		 */
		float getPadding(void) const;

		/**
		 * @brief Set the padding around the children.
		 * @param padding The new padding around the children.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setPadding(float padding);

		/**
		 * @brief Check whether the container width is forced.
		 * @return True when a fixed width is set.
		 */
		bool hasFixedWidth(void) const;

		/**
		 * @brief Get the forced container width.
		 * @return The forced container width.
		 */
		float getFixedWidth(void) const;

		/**
		 * @brief Force the container width, disabling auto sizing.
		 * @param width The new fixed width.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setFixedWidth(float width);

		/**
		 * @brief Clear the forced width and restore auto sizing.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &clearFixedWidth(void);

		/**
		 * @brief Check whether the container height is forced.
		 * @return True when a fixed height is set.
		 */
		bool hasFixedHeight(void) const;

		/**
		 * @brief Get the forced container height.
		 * @return The forced container height.
		 */
		float getFixedHeight(void) const;

		/**
		 * @brief Force the container height, disabling auto sizing.
		 * @param height The new fixed height.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &setFixedHeight(float height);

		/**
		 * @brief Clear the forced height and restore auto sizing.
		 * @return Reference to this Layout component for chaining.
		 */
		Layout &clearFixedHeight(void);
	};

}	 // namespace guillaume::components
