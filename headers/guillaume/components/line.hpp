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
	 * @brief Component marking an entity as a horizontal stroke.
	 *
	 * The stroke runs from the left edge to the right edge of the entity
	 * `components::Bound`, vertically centered, with the given `thickness`.
	 * It can be drawn dashed, which is useful for dividers and tab
	 * indicators. It is rendered by `systems::LineRender`.
	 *
	 * @see systems::LineRender
	 */
	class Line: public ecs::Component
	{
		private:
		float _thickness { 2.0f };	   ///< Stroke thickness.
		bool _dashed { false };		   ///< Whether the stroke is dashed.
		float _dashLength { 8.0f };	   ///< Length of one dash.
		float _gapLength { 8.0f };	   ///< Length of the gap between dashes.

		public:
		/**
		 * @brief Constructor for the Line component.
		 */
		explicit Line(void);

		/**
		 * @brief Default destructor for the Line component.
		 */
		~Line(void) = default;

		/**
		 * @brief Get the stroke thickness.
		 * @return The stroke thickness.
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the stroke thickness.
		 * @param thickness The new stroke thickness.
		 * @return Reference to this Line component for chaining.
		 */
		Line &setThickness(float thickness);

		/**
		 * @brief Check whether the stroke is dashed.
		 * @return True when the stroke is dashed.
		 */
		bool isDashed(void) const;

		/**
		 * @brief Set whether the stroke is dashed.
		 * @param dashed Whether the stroke should be dashed.
		 * @return Reference to this Line component for chaining.
		 */
		Line &setDashed(bool dashed);

		/**
		 * @brief Get the length of one dash.
		 * @return The dash length.
		 */
		float getDashLength(void) const;

		/**
		 * @brief Set the length of one dash.
		 * @param dashLength The new dash length.
		 * @return Reference to this Line component for chaining.
		 */
		Line &setDashLength(float dashLength);

		/**
		 * @brief Get the length of the gap between dashes.
		 * @return The gap length.
		 */
		float getGapLength(void) const;

		/**
		 * @brief Set the length of the gap between dashes.
		 * @param gapLength The new gap length.
		 * @return Reference to this Line component for chaining.
		 */
		Line &setGapLength(float gapLength);
	};

}	 // namespace guillaume::components
