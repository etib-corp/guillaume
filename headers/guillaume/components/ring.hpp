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
	 * @brief A full ring: the outline of a radio button, the track of a
	 * circular progress indicator.
	 *
	 * The outer edge of the ring is the ellipse inscribed in the entity's
	 * components::Bound; the inner edge is `thickness` closer to the
	 * center. A square bound gives a circle.
	 *
	 * Written by the widget systems that draw a ring, or by the
	 * application. Read by systems::RingRender in the Render phase, with
	 * the entity's Transform, Bound and Color (the color of the ring).
	 *
	 * @code
	 * registry.getComponent<components::Ring>(id).setThickness(2.0f);
	 * @endcode
	 *
	 * @note A thickness of half the bound or more fills the ring into a
	 * disc.
	 *
	 * @see systems::RingRender, components::Arc for a portion of a ring,
	 * components::Ellipse for a filled disc
	 */
	class Ring: public ecs::Component
	{
		public:
		using SegmentCount = int;	 ///< A number of straight segments.

		private:
		float _thickness { 8.0f };		  ///< Radial thickness, in dp.
		SegmentCount _segments { 48 };	  ///< Segments around the ring.

		public:
		/**
		 * @brief Construct a ring 8 dp thick drawn with 48 segments.
		 */
		explicit Ring(void);

		/**
		 * @brief Default destructor for the Ring component.
		 */
		~Ring(void) = default;

		/**
		 * @brief Get the thickness of the ring.
		 * @return The distance between its outer and inner edges, in dp (8
		 * by default).
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the thickness of the ring.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param thickness The distance between the outer and inner edges,
		 * in dp, measured towards the center of the bound.
		 * @return This component, for chaining.
		 */
		Ring &setThickness(float thickness);

		/**
		 * @brief Get how many straight segments approximate the ring.
		 * @return The segment count around the ring (48 by default).
		 */
		SegmentCount getSegments(void) const;

		/**
		 * @brief Set how many straight segments approximate the ring.
		 *
		 * More segments give a smoother curve for more triangles. Marks the
		 * component changed only when the value differs.
		 *
		 * @param segments The segment count around the ring;
		 * systems::RingRender draws at least 1.
		 * @return This component, for chaining.
		 */
		Ring &setSegments(SegmentCount segments);
	};

}	 // namespace guillaume::components
