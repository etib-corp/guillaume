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
	 * @brief Marks an entity as a filled ellipse: a dot, a switch thumb, a
	 * disc.
	 *
	 * The ellipse is the one inscribed in the entity's components::Bound
	 * (a square bound gives a circle) and is filled with the entity's
	 * components::Color. The only value held here is how many segments
	 * approximate the outline.
	 *
	 * Added by the widget classes that need a round shape, or by the
	 * application. Read by systems::EllipseRender in the Render phase,
	 * with the entity's Transform, Bound and Color.
	 *
	 * @code
	 * registry.addComponent<components::Ellipse>(id);
	 * registry.getComponent<components::Bound>(id)
	 * 	.setWidth(16.0f)
	 * 	.setHeight(16.0f);
	 * @endcode
	 *
	 * @see systems::EllipseRender, components::Ring for an outline only
	 */
	class Ellipse: public ecs::Component
	{
		public:
		using SegmentCount = int;	 ///< A number of straight segments.

		private:
		SegmentCount _segments { 48 };	  ///< Segments around the outline

		public:
		/**
		 * @brief Construct an ellipse drawn with 48 segments.
		 */
		explicit Ellipse(void);

		/**
		 * @brief Default destructor for the Ellipse component.
		 */
		~Ellipse(void) = default;

		/**
		 * @brief Get how many straight segments approximate the outline.
		 * @return The segment count (48 by default, never below 1).
		 */
		SegmentCount getSegments(void) const;

		/**
		 * @brief Set how many straight segments approximate the outline.
		 *
		 * More segments give a smoother curve for more triangles. Always
		 * marks the component changed, even when the count is the same.
		 *
		 * @param segments The segment count; a value below 1 is stored as
		 * 1.
		 * @return This component, for chaining.
		 */
		Ellipse &setSegments(SegmentCount segments);
	};

}	 // namespace guillaume::components
