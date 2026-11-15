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
	 * @brief A portion of a ring: the moving part of a circular progress
	 * or loading indicator.
	 *
	 * The outer edge of the arc is the ellipse inscribed in the entity's
	 * components::Bound; the arc is `thickness` wide towards the center,
	 * starts at `startAngle` and covers `sweepAngle`. Its ends are flat.
	 *
	 * Written by systems::ProgressIndicator and systems::LoadingIndicator,
	 * every frame while they animate, or by the application. Read by
	 * systems::ArcRender in the Render phase, with the entity's Transform,
	 * Bound and Color (the color of the arc).
	 *
	 * @code
	 * // The first quarter of a circle, starting at 12 o'clock.
	 * const float pi = 3.14159265f;
	 * registry.getComponent<components::Arc>(id)
	 * 	.setThickness(4.0f)
	 * 	.setStartAngle(-pi / 2.0f)
	 * 	.setSweepAngle(pi / 2.0f);
	 * @endcode
	 *
	 * @note Angles are in radians. Angle 0 points to the right (3
	 * o'clock) and, as Y grows downwards, a positive angle turns
	 * clockwise on screen: `-pi / 2` is 12 o'clock.
	 *
	 * @see systems::ArcRender, components::Ring for a full ring
	 */
	class Arc: public ecs::Component
	{
		public:
		using SegmentCount = int;	 ///< A number of straight segments.

		private:
		float _thickness { 8.0f };	   ///< Radial thickness, in dp.
		float _startAngle { 0.0f };	   ///< Start angle, in radians.
		float _sweepAngle { 3.14159265358979323846f };	  ///< Angular span,
														  ///< in radians.
		SegmentCount _segments { 48 };	  ///< Segments along the arc.

		public:
		/**
		 * @brief Construct a half circle: 8 dp thick, starting at angle 0,
		 * covering pi radians, drawn with 48 segments.
		 */
		explicit Arc(void);

		/**
		 * @brief Default destructor for the Arc component.
		 */
		~Arc(void) = default;

		/**
		 * @brief Get the thickness of the arc.
		 * @return The distance between its outer and inner edges, in dp (8
		 * by default).
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the thickness of the arc.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param thickness The distance between the outer and inner edges,
		 * in dp, measured towards the center of the bound.
		 * @return This component, for chaining.
		 */
		Arc &setThickness(float thickness);

		/**
		 * @brief Get the angle the arc starts at.
		 * @return The start angle, in radians (0, pointing right, by
		 * default).
		 */
		float getStartAngle(void) const;

		/**
		 * @brief Set the angle the arc starts at.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param startAngle The start angle, in radians; 0 points right
		 * and `-pi / 2` points up.
		 * @return This component, for chaining.
		 */
		Arc &setStartAngle(float startAngle);

		/**
		 * @brief Get the angle the arc covers.
		 * @return The sweep, in radians (pi, a half circle, by default).
		 */
		float getSweepAngle(void) const;

		/**
		 * @brief Set the angle the arc covers from its start angle.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param sweepAngle The sweep, in radians: `2 * pi` is a full
		 * turn, and a positive value turns clockwise on screen.
		 * @return This component, for chaining.
		 */
		Arc &setSweepAngle(float sweepAngle);

		/**
		 * @brief Get how many straight segments approximate the arc.
		 * @return The segment count over the whole sweep (48 by default).
		 */
		SegmentCount getSegments(void) const;

		/**
		 * @brief Set how many straight segments approximate the arc.
		 *
		 * More segments give a smoother curve for more triangles. Marks the
		 * component changed only when the value differs.
		 *
		 * @param segments The segment count over the whole sweep;
		 * systems::ArcRender draws at least 1.
		 * @return This component, for chaining.
		 */
		Arc &setSegments(SegmentCount segments);
	};

}	 // namespace guillaume::components
