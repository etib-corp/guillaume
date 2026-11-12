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
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a Material 3 common button (elevated, filled,
	 * tonal, outlined, text).
	 *
	 * systems::CommonButton turns it, with components::Button, into the
	 * button's colors, shape, size and the placement of its icon and label.
	 */
	class CommonButton: public ecs::Component
	{
		public:
		/**
		 * @brief Color style.
		 */
		enum class Color { Elevated, Filled, Tonal, Outlined, Text, Segmented };

		/**
		 * @brief Corner shape.
		 */
		enum class Shape { Round, Square };

		/**
		 * @brief Size.
		 */
		enum class Size { ExtraSmall, Small, Medium, Large, ExtraLarge };

		/// Minimum width of the container, in dp.
		static constexpr float MinWidth = 64.0f;

		/// Minimum height of the container, in dp.
		static constexpr float MinHeight = 40.0f;

		private:
		Color _colorStyle { Color::Text };	  ///< Color style
		Shape _shape { Shape::Round };		  ///< Corner shape
		Size _size { Size::Small };			  ///< Size
		bool _compact {
			false
		};	  ///< Whether the horizontal padding is compact
		float _minWidth { 0.0f };	  ///< Extra minimum width
		bool _uppercase { false };	  ///< Whether the label is upper-cased

		public:
		/**
		 * @brief Default constructor.
		 */
		CommonButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~CommonButton(void) = default;

		/** @brief Get: color style. @return The value. */
		Color getColorStyle(void) const noexcept;

		/** @brief Set: color style. @param style The value. @return It. */
		CommonButton &setColorStyle(Color style);

		/** @brief Get: corner shape. @return The value. */
		Shape getShape(void) const noexcept;

		/** @brief Set: corner shape. @param shape The value. @return It. */
		CommonButton &setShape(Shape shape);

		/** @brief Get: size. @return The value. */
		Size getSize(void) const noexcept;

		/** @brief Set: size. @param size The value. @return It. */
		CommonButton &setSize(Size size);

		/** @brief Get: whether the horizontal padding is compact. @return The
		 * value. */
		bool isCompact(void) const noexcept;

		/** @brief Set: whether the horizontal padding is compact. @param
		 * compact The value. @return It. */
		CommonButton &setCompact(bool compact);

		/** @brief Get: extra minimum width. @return The value. */
		float getMinWidth(void) const noexcept;

		/** @brief Set: extra minimum width. @param width The value. @return It.
		 */
		CommonButton &setMinWidth(float width);

		/** @brief Get: whether the label is upper-cased. @return The value. */
		bool isUppercase(void) const noexcept;

		/** @brief Set: whether the label is upper-cased. @param uppercase The
		 * value. @return It. */
		CommonButton &setUppercase(bool uppercase);
	};

}	 // namespace guillaume::components
