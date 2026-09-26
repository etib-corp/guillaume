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
	 * @brief Component marking an entity as an ellipse shape.
	 *
	 * The component also carries the outline resolution used by
	 * systems::EllipseRender when building the triangle fan mesh.
	 */
	class Ellipse: public ecs::Component
	{
		public:
		using SegmentCount = int;	 ///< Type representing the outline
									 ///< segment count

		private:
		SegmentCount _segments { 48 };	  ///< Number of outline segments

		public:
		/**
		 * @brief Constructor for the Ellipse component.
		 */
		explicit Ellipse(void);

		/**
		 * @brief Default destructor for the Ellipse component.
		 */
		~Ellipse(void) = default;

		/**
		 * @brief Get the number of outline segments.
		 * @return The number of outline segments.
		 */
		SegmentCount getSegments(void) const;

		/**
		 * @brief Set the number of outline segments.
		 * @param segments The new number of outline segments.
		 * @return Reference to this Ellipse component for chaining.
		 */
		Ellipse &setSegments(SegmentCount segments);
	};

}	 // namespace guillaume::components
