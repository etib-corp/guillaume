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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief A straight stroke across the width of the entity: a divider,
	 * an indicator, the underline of a text field.
	 *
	 * The stroke runs from the left edge to the right edge of the entity's
	 * components::Bound. The placement says where it sits across the
	 * height (middle, top edge or bottom edge); it can be solid or dashed.
	 * The stroke follows the orientation of the entity, so a rotated
	 * entity gives a vertical line.
	 *
	 * Written by the widget systems that own a stroke (systems::Divider,
	 * systems::TextField, ...) or by the application. Read by
	 * systems::LineRender in the Render phase, with the entity's
	 * Transform, Bound and Color.
	 *
	 * @code
	 * registry.getComponent<components::Line>(id)
	 * 	.setThickness(1.0f)
	 * 	.setPlacement(components::Line::Placement::Bottom);
	 * @endcode
	 *
	 * @note The stroke is drawn in the entity's components::Color unless
	 * setColor() gives it a color of its own. The entity needs a
	 * components::Color in both cases, or systems::LineRender skips it.
	 * @note The length is the width of the Bound. The height of the Bound
	 * only matters for the placement: the thickness is not limited by it.
	 *
	 * @see systems::LineRender, components::Bound, components::Color
	 */
	class Line: public ecs::Component
	{
		public:
		/**
		 * @brief Where the stroke sits across the bound height.
		 */
		enum class Placement {
			Center,	   ///< Half-way down the bound (dividers, indicators).
			Top,	   ///< Inside the bound, against its top edge.
			Bottom	   ///< Inside the bound, against its bottom edge
					   ///< (text field underline).
		};

		private:
		float _thickness { 2.0f };	   ///< Stroke thickness, in dp.
		bool _dashed { false };		   ///< Whether the stroke is dashed.
		float _dashLength { 8.0f };	   ///< Length of one dash, in dp.
		float _gapLength { 8.0f };	   ///< Gap between two dashes, in dp.
		Placement _placement { Placement::Center };	   ///< Stroke placement.
		bool _hasColor { false };	 ///< Whether a stroke color overrides
									 ///< the entity fill color.
		utility::graphic::Color32Bit _color { 0, 0, 0,
											  0 };	  ///< Stroke color.

		public:
		/**
		 * @brief Construct a solid, centered stroke 2 dp thick, with 8 dp
		 * dashes and gaps ready for setDashed(), and no color of its own.
		 */
		explicit Line(void);

		/**
		 * @brief Default destructor for the Line component.
		 */
		~Line(void) = default;

		/**
		 * @brief Get the thickness of the stroke.
		 * @return The thickness, in dp (2 by default).
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the thickness of the stroke.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param thickness The thickness, in dp.
		 * @return This component, for chaining.
		 */
		Line &setThickness(float thickness);

		/**
		 * @brief Check whether the stroke is dashed.
		 * @return True for dashes, false (the default) for a solid stroke.
		 */
		bool isDashed(void) const;

		/**
		 * @brief Choose between a solid and a dashed stroke.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param dashed True to draw dashes of getDashLength() separated
		 * by gaps of getGapLength().
		 * @return This component, for chaining.
		 */
		Line &setDashed(bool dashed);

		/**
		 * @brief Get the length of one dash.
		 * @return The dash length, in dp (8 by default).
		 */
		float getDashLength(void) const;

		/**
		 * @brief Set the length of one dash. Used only when the stroke is
		 * dashed.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param dashLength The dash length, in dp; 0 or less draws a
		 * solid stroke.
		 * @return This component, for chaining.
		 */
		Line &setDashLength(float dashLength);

		/**
		 * @brief Get the gap between two dashes.
		 * @return The gap length, in dp (8 by default).
		 */
		float getGapLength(void) const;

		/**
		 * @brief Set the gap between two dashes. Used only when the stroke
		 * is dashed.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param gapLength The gap length, in dp; a negative value counts
		 * as 0.
		 * @return This component, for chaining.
		 */
		Line &setGapLength(float gapLength);

		/**
		 * @brief Get where the stroke sits across the height of the bound.
		 * @return The placement (Center by default).
		 */
		Placement getPlacement(void) const;

		/**
		 * @brief Set where the stroke sits across the height of the bound.
		 *
		 * A Top or Bottom stroke is also drawn slightly in front of the
		 * entity, so it shows over a fill drawn by the same entity. Marks
		 * the component changed only when the value differs.
		 *
		 * @param placement The placement of the stroke.
		 * @return This component, for chaining.
		 */
		Line &setPlacement(Placement placement);

		/**
		 * @brief Give the stroke a color of its own, used in place of the
		 * entity's components::Color.
		 *
		 * This lets one entity carry both a fill (components::Color) and a
		 * stroke of another color, such as a filled text field and its
		 * underline. Marks the component changed unless the same color was
		 * already set.
		 *
		 * @param color The stroke color (RGBA).
		 * @return This component, for chaining.
		 */
		Line &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Draw the stroke in the entity's components::Color again.
		 *
		 * Marks the component changed only when a color was set.
		 *
		 * @return This component, for chaining.
		 */
		Line &clearColor(void);

		/**
		 * @brief Check whether the stroke has a color of its own.
		 * @return True after setColor(), false after clearColor() and by
		 * default.
		 */
		bool hasColor(void) const;

		/**
		 * @brief Get the color given to setColor().
		 * @return The stroke color. Check hasColor() first: when it is
		 * false this value is not the color the stroke is drawn in.
		 */
		const utility::graphic::Color32Bit &getColor(void) const;
	};

}	 // namespace guillaume::components
