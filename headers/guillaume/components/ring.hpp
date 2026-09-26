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
	 * @brief Component marking an entity as a ring (annulus) shape.
	 *
	 * The ring outer boundary is the entity `components::Bound`; the inner
	 * boundary is the outer boundary shrunk by `thickness`. The outline
	 * resolution is used by `systems::RingRender`.
	 *
	 * @see systems::RingRender
	 */
	class Ring: public ecs::Component
	{
		public:
		using SegmentCount = int;	 ///< Number of outline segments.

		private:
		float _thickness { 8.0f };		  ///< Radial thickness of the ring.
		SegmentCount _segments { 48 };	  ///< Number of outline segments.

		public:
		/**
		 * @brief Constructor for the Ring component.
		 */
		explicit Ring(void);

		/**
		 * @brief Default destructor for the Ring component.
		 */
		~Ring(void) = default;

		/**
		 * @brief Get the radial thickness of the ring.
		 * @return The radial thickness.
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the radial thickness of the ring.
		 * @param thickness The new radial thickness.
		 * @return Reference to this Ring component for chaining.
		 */
		Ring &setThickness(float thickness);

		/**
		 * @brief Get the number of outline segments.
		 * @return The number of outline segments.
		 */
		SegmentCount getSegments(void) const;

		/**
		 * @brief Set the number of outline segments.
		 * @param segments The new number of outline segments.
		 * @return Reference to this Ring component for chaining.
		 */
		Ring &setSegments(SegmentCount segments);
	};

}	 // namespace guillaume::components
