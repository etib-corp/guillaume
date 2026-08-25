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

#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component representing a glyph element.
	 */
	class Glyph: public ecs::Component
	{
		public:
		/**
		 * @brief Style of the glyph, which can be Outlined, Rounded, or Sharp.
		 */
		enum class Style { Outlined, Rounded, Sharp };

		private:
		uint32_t _code {};		 ///< Glyph code (e.g., Unicode code point)
		std::string _name {};	 ///< Glyph name (for font lookup)
		float _fontSize {};		 ///< Font size of the glyph
		Style _style {};		 ///< Style of the glyph

		public:
		/**
		 * @brief Default constructor for the Glyph component.
		 */
		Glyph(void) = default;

		/**
		 * @brief Default destructor for the Glyph component.
		 */
		~Glyph(void) = default;

		/**
		 * @brief Get the glyph code.
		 * @return The glyph code.
		 */
		uint32_t getCode(void) const;

		/**
		 * @brief Set the glyph code.
		 * @param code The new glyph code.
		 * @return The Glyph component for chaining.
		 */
		Glyph &setCode(uint32_t code);

		/**
		 * @brief Get the glyph name.
		 * @return The glyph name.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Set the glyph name.
		 * @param name The new glyph name.
		 * @return The Glyph component for chaining.
		 */
		Glyph &setName(const std::string &name);

		/**
		 * @brief Get the font size of the glyph.
		 * @return The font size.
		 */
		const float &getFontSize(void) const;

		/**
		 * @brief Set the font size of the glyph.
		 * @param fontSize The new font size.
		 * @return The Glyph component for chaining.
		 */
		Glyph &setFontSize(const float &fontSize);

		/**
		 * @brief Get the style of the glyph.
		 * @return The glyph style.
		 */
		const Style &getStyle(void) const;

		/**
		 * @brief Set the style of the glyph.
		 * @param style The new glyph style.
		 * @return The Glyph component for chaining.
		 */
		Glyph &setStyle(const Style &style);
	};

}	 // namespace guillaume::components