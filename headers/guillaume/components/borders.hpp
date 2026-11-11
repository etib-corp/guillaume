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

#include <cmath>
#include <ostream>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Corner radii and outline stroke of a rectangular entity.
	 *
	 * systems::RectangleRender (Render phase) reads it, with Transform,
	 * Bound and Color, to draw the entity: the bound filled with the
	 * entity's components::Color, its corners rounded, and, when
	 * hasStroke() is true, an outline of the stroke width and color drawn
	 * inside the bound. An entity needs this component, even left at its
	 * defaults, to be drawn as a rectangle. systems::FocusRingRender reads
	 * the radii too, so the focus ring follows the corners.
	 *
	 * Written by the application for a plain surface and by the widget
	 * systems for their widgets, which usually take the radius from a
	 * theme shape token.
	 *
	 * @code
	 * registry.getComponent<components::Borders>(id)
	 * 	.setBorderRadius(12.0f)
	 * 	.setWidth(1.0f)
	 * 	.setColor(utility::graphic::Color32Bit(121, 116, 126, 255));
	 * @endcode
	 *
	 * @note The four radii are stored separately, but
	 * systems::RectangleRender draws every corner with their average,
	 * clamped to half the smaller side of the bound. Use setBorderRadius()
	 * unless another reader needs distinct values.
	 * @note Defaults: square corners, transparent color, width 0, so no
	 * stroke.
	 *
	 * @see systems::RectangleRender, components::Color, components::Bound
	 */
	class Borders: public ecs::Component
	{
		public:
		using BorderRadius = float;	   ///< A corner radius, in dp.

		private:
		BorderRadius _topLeftRadius;		///< Top-left corner radius, in dp
		BorderRadius _topRightRadius;		///< Top-right corner radius, in dp
		BorderRadius _bottomRightRadius;	///< Bottom-right corner
											///< radius, in dp
		BorderRadius _bottomLeftRadius;	   ///< Bottom-left corner radius, in dp
		utility::graphic::Color32Bit _color;	///< Stroke color (RGBA)
		float _width;	 ///< Stroke width, in dp; 0 paints no stroke

		public:
		/**
		 * @brief Construct square corners with no stroke: every radius 0,
		 * a transparent color and a width of 0.
		 */
		explicit Borders(void);

		/**
		 * @brief Default destructor for the Borders component.
		 */
		~Borders(void) = default;

		/**
		 * @brief Get the radius of the top-left corner.
		 * @return The radius, in dp (0 for a square corner).
		 */
		BorderRadius getTopLeftRadius(void) const;

		/**
		 * @brief Set the radius of the top-left corner.
		 *
		 * Marks the component changed only when the radius differs.
		 *
		 * @param radius The radius, in dp (0 for a square corner).
		 * @return This component, for chaining.
		 */
		Borders &setTopLeftRadius(BorderRadius radius);

		/**
		 * @brief Get the radius of the top-right corner.
		 * @return The radius, in dp (0 for a square corner).
		 */
		BorderRadius getTopRightRadius(void) const;

		/**
		 * @brief Set the radius of the top-right corner.
		 *
		 * Marks the component changed only when the radius differs.
		 *
		 * @param radius The radius, in dp (0 for a square corner).
		 * @return This component, for chaining.
		 */
		Borders &setTopRightRadius(BorderRadius radius);

		/**
		 * @brief Get the radius of the bottom-right corner.
		 * @return The radius, in dp (0 for a square corner).
		 */
		BorderRadius getBottomRightRadius(void) const;

		/**
		 * @brief Set the radius of the bottom-right corner.
		 *
		 * Marks the component changed only when the radius differs.
		 *
		 * @param radius The radius, in dp (0 for a square corner).
		 * @return This component, for chaining.
		 */
		Borders &setBottomRightRadius(BorderRadius radius);

		/**
		 * @brief Get the radius of the bottom-left corner.
		 * @return The radius, in dp (0 for a square corner).
		 */
		BorderRadius getBottomLeftRadius(void) const;

		/**
		 * @brief Set the radius of the bottom-left corner.
		 *
		 * Marks the component changed only when the radius differs.
		 *
		 * @param radius The radius, in dp (0 for a square corner).
		 * @return This component, for chaining.
		 */
		Borders &setBottomLeftRadius(BorderRadius radius);

		/**
		 * @brief Set the same radius on the four corners.
		 *
		 * Marks the component changed when at least one corner differs. A
		 * radius of half the smaller side of the bound, or more, gives a
		 * pill or a circle.
		 *
		 * @param radius The radius, in dp (0 for square corners).
		 * @return This component, for chaining.
		 */
		Borders &setBorderRadius(BorderRadius radius);

		/**
		 * @brief Set the color of the stroke.
		 *
		 * Marks the component changed only when the color differs. The
		 * stroke is visible only when the width is also above 0.
		 *
		 * @param color The stroke color (RGBA); an alpha of 0 hides the
		 * stroke.
		 * @return This component, for chaining.
		 */
		Borders &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Get the color of the stroke.
		 * @return A copy of the stroke color (RGBA).
		 */
		utility::graphic::Color32Bit getColor(void) const;

		/**
		 * @brief Set the width of the stroke painted by
		 * systems::RectangleRender.
		 *
		 * The stroke is drawn inside the bound, following the corner
		 * radius. A width of 0 (the default) disables the stroke. Marks the
		 * component changed only when the width differs.
		 *
		 * @param width The stroke width, in dp; must not be negative.
		 * @return This component, for chaining.
		 * @throws std::invalid_argument If @p width is negative.
		 */
		Borders &setWidth(float width);

		/**
		 * @brief Get the width of the stroke.
		 * @return The stroke width, in dp (0 when no stroke is painted).
		 */
		float getWidth(void) const;

		/**
		 * @brief Check whether a visible stroke must be painted.
		 * @return True when the width is above 0 and the color alpha is not
		 * 0.
		 */
		bool hasStroke(void) const;

		/**
		 * @brief Compare the four radii, the stroke color and the stroke
		 * width. The changed flag is not compared.
		 * @param other The borders to compare with.
		 * @return True when every value is equal.
		 */
		bool operator==(const Borders &other) const;

		/**
		 * @brief Opposite of operator==.
		 * @param other The borders to compare with.
		 * @return True when a radius, the stroke color or the stroke width
		 * differs.
		 */
		bool operator!=(const Borders &other) const;

		/**
		 * @brief Order borders so they can be used as a key in an ordered
		 * container.
		 *
		 * Compares, in this order, the top-left, top-right, bottom-right
		 * and bottom-left radii, then the stroke color. The stroke width is
		 * not compared: two borders that differ only by their width are
		 * unequal for operator== but neither is less than the other.
		 *
		 * @param other The borders to compare with.
		 * @return True when this one sorts before @p other.
		 */
		bool operator<(const Borders &other) const;
	};

	/**
	 * @brief Print the four corner radii, for logs and test failures.
	 *
	 * The output is `Borders(topLeft, topRight, bottomRight, bottomLeft)`;
	 * the stroke color and width are not printed.
	 *
	 * @param stream The stream to write to.
	 * @param borders The borders to print.
	 * @return @p stream, for chaining.
	 */
	std::ostream &operator<<(std::ostream &stream, const Borders &borders);

}	 // namespace guillaume::components