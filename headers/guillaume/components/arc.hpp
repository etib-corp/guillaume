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
	 * @brief Component marking an entity as an arc (stroke segment) shape.
	 *
	 * The arc outer boundary is the entity `components::Bound`; it spans
	 * `sweepAngle` radians starting at `startAngle`, with the given stroke
	 * `thickness`. It is rendered by `systems::ArcRender`.
	 *
	 * @see systems::ArcRender
	 */
	class Arc: public ecs::Component
	{
		public:
		using SegmentCount = int;	 ///< Number of outline segments.

		private:
		float _thickness { 8.0f };	   ///< Stroke thickness of the arc.
		float _startAngle { 0.0f };	   ///< Start angle, in radians.
		float _sweepAngle { 3.14159265358979323846f };	  ///< Angular span,
														  ///< in radians.
		SegmentCount _segments { 48 };	  ///< Number of outline segments.

		public:
		/**
		 * @brief Constructor for the Arc component.
		 */
		explicit Arc(void);

		/**
		 * @brief Default destructor for the Arc component.
		 */
		~Arc(void) = default;

		/**
		 * @brief Get the stroke thickness of the arc.
		 * @return The stroke thickness.
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the stroke thickness of the arc.
		 * @param thickness The new stroke thickness.
		 * @return Reference to this Arc component for chaining.
		 */
		Arc &setThickness(float thickness);

		/**
		 * @brief Get the start angle.
		 * @return The start angle, in radians.
		 */
		float getStartAngle(void) const;

		/**
		 * @brief Set the start angle.
		 * @param startAngle The new start angle, in radians.
		 * @return Reference to this Arc component for chaining.
		 */
		Arc &setStartAngle(float startAngle);

		/**
		 * @brief Get the angular span.
		 * @return The angular span, in radians.
		 */
		float getSweepAngle(void) const;

		/**
		 * @brief Set the angular span.
		 * @param sweepAngle The new angular span, in radians.
		 * @return Reference to this Arc component for chaining.
		 */
		Arc &setSweepAngle(float sweepAngle);

		/**
		 * @brief Get the number of outline segments.
		 * @return The number of outline segments.
		 */
		SegmentCount getSegments(void) const;

		/**
		 * @brief Set the number of outline segments.
		 * @param segments The new number of outline segments.
		 * @return Reference to this Arc component for chaining.
		 */
		Arc &setSegments(SegmentCount segments);
	};

}	 // namespace guillaume::components
