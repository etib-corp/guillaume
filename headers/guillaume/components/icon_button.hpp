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
	 * @brief Configuration of a Material 3 icon button.
	 *
	 * systems::IconButton turns it, with components::Button, into the button's
	 * colors, size and the placement of its icon.
	 */
	class IconButton: public ecs::Component
	{
		public:
		/**
		 * @brief Color variant.
		 */
		enum class Variant { Standard, Filled, Tonal, Outlined };

		/**
		 * @brief Size.
		 */
		enum class Size { Small, Medium, Large };

		private:
		Variant _variant { Variant::Standard };	   ///< Color variant
		Size _size { Size::Medium };			   ///< Size

		public:
		/**
		 * @brief Default constructor.
		 */
		IconButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~IconButton(void) = default;

		/** @brief Get: color variant. @return The value. */
		Variant getVariant(void) const noexcept;

		/** @brief Set: color variant. @param variant The value. @return It. */
		IconButton &setVariant(Variant variant);

		/** @brief Get: size. @return The value. */
		Size getSize(void) const noexcept;

		/** @brief Set: size. @param size The value. @return It. */
		IconButton &setSize(Size size);
	};

}	 // namespace guillaume::components
