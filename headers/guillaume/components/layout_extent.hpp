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
#include "guillaume/components/hierarchy.hpp"
#pragma once

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Space a widget takes in its parent's layout beyond its Bound.
	 *
	 * The Bound is what the shape renderers draw. Some widgets draw a small
	 * shape and place a child beside it: a checkbox draws an 18 dp box and
	 * its label to the right. Without this component the parent layout
	 * would reserve only the box and the label would overlap the next
	 * widget.
	 *
	 * Written by the widget system that places the outside child
	 * (systems::Checkbox, RadioButton, Switch); read by systems::Layout,
	 * which adds it to the child's width when arranging a parent.
	 */
	class LayoutExtent: public ecs::Component
	{
		private:
		float _trailingWidth { 0.0f };	  ///< Extra width after the Bound, dp.

		public:
		/**
		 * @brief Default constructor: no extra space.
		 */
		LayoutExtent(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~LayoutExtent(void) = default;

		/**
		 * @brief Get the extra width reserved after the Bound.
		 * @return The width, in dp.
		 */
		float getTrailingWidth(void) const noexcept;

		/**
		 * @brief Set the extra width reserved after the Bound.
		 *
		 * Marks the component changed only when the value differs.
		 * @param width The width, in dp; negative values are treated as 0.
		 * @return Reference to this component for chaining.
		 */
		LayoutExtent &setTrailingWidth(float width);
	};

}	 // namespace guillaume::components
