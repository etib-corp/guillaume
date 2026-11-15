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
	 * @brief Look of a Material 3 icon button: color variant and size.
	 *
	 * It sits on the same entity as components::Button, which holds the
	 * glyph and the interaction state. entities::IconButton writes it;
	 * systems::IconButton reads both, each frame in the Layout phase and at
	 * once after every input change, and writes the button's Color, Borders
	 * and Bound and centers the icon child in the circular container.
	 *
	 * Both setters compare before writing and mark the component changed
	 * only when the value differs.
	 *
	 * @see components::Button, systems::IconButton
	 */
	class IconButton: public ecs::Component
	{
		public:
		/**
		 * @brief Which theme colors fill the container and the icon.
		 *
		 * Standard has no container. Filled and Tonal have a filled
		 * container. Outlined has an outline and no container until it is
		 * selected, where it is filled and loses the outline.
		 */
		enum class Variant { Standard, Filled, Tonal, Outlined };

		/**
		 * @brief Size step: a 32, 40 or 48 dp container around an 18, 24 or
		 * 30 dp icon.
		 */
		enum class Size { Small, Medium, Large };

		private:
		Variant _variant { Variant::Standard };	   ///< Color variant
		Size _size { Size::Medium };			   ///< Size step

		public:
		/**
		 * @brief Create the configuration of a medium standard icon button.
		 */
		IconButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~IconButton(void) = default;

		/**
		 * @brief Get the color variant.
		 * @return The variant (Standard by default).
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose which theme colors the button uses.
		 * @param variant The color variant.
		 * @return This component, for chaining.
		 */
		IconButton &setVariant(Variant variant);

		/**
		 * @brief Get the size step.
		 * @return The size (Medium by default).
		 */
		Size getSize(void) const noexcept;

		/**
		 * @brief Choose the size step.
		 * @param size The size; it sets the container extent and the icon
		 * font size.
		 * @return This component, for chaining.
		 */
		IconButton &setSize(Size size);
	};

}	 // namespace guillaume::components
