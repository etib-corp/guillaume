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
	 * @brief An icon: one glyph of the Material Symbols font, chosen by
	 * name, at a given size.
	 *
	 * entities::Icon writes it, and the widget systems resize the icons of
	 * their widgets. systems::MeasureGlyph (Measure phase) looks the name
	 * up in the font's code point table and writes the size of the glyph
	 * into the entity's components::Bound; systems::GlyphRender (Render
	 * phase) draws it at the entity's components::Transform in the
	 * entity's components::Color.
	 *
	 * @code
	 * registry.getComponent<components::Glyph>(id)
	 * 	.setName("settings")
	 * 	.setFontSize(24.0f);
	 * @endcode
	 *
	 * @note The name is what selects the drawn glyph. A name missing from
	 * the table is drawn as `?`.
	 * @note The systems of the library always draw with the Outlined
	 * font: the style is stored (and is part of the measure cache key)
	 * but does not change the drawing, and the code is not read to choose
	 * the glyph.
	 *
	 * @see systems::MeasureGlyph, systems::GlyphRender, entities::Icon
	 */
	class Glyph: public ecs::Component
	{
		public:
		/**
		 * @brief Family of the Material Symbols font the icon belongs to.
		 */
		enum class Style { Outlined, Rounded, Sharp };

		private:
		uint32_t _code {};		 ///< Unicode code point; informative
		std::string _name {};	 ///< Material Symbols name, e.g. "home"
		float _fontSize {};		 ///< Font size, in dp
		Style _style {};		 ///< Font family of the icon

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
		 * @brief Get the stored code point.
		 * @return The Unicode code point given to setCode(); 0 by default.
		 */
		uint32_t getCode(void) const;

		/**
		 * @brief Store a code point for the glyph.
		 *
		 * The render systems resolve the glyph from its name, not from
		 * this value. Marks the component changed only when the value
		 * differs.
		 *
		 * @param code The Unicode code point.
		 * @return This component, for chaining.
		 */
		Glyph &setCode(uint32_t code);

		/**
		 * @brief Get the name of the icon.
		 * @return The Material Symbols name; empty by default.
		 */
		const std::string &getName(void) const;

		/**
		 * @brief Choose the icon by name.
		 *
		 * Marks the component changed only when the name differs.
		 *
		 * @param name The Material Symbols name, such as `"home"` or
		 * `"arrow_back"`.
		 * @return This component, for chaining.
		 */
		Glyph &setName(const std::string &name);

		/**
		 * @brief Get the size of the icon.
		 * @return The font size, in dp; 0 by default.
		 */
		const float &getFontSize(void) const;

		/**
		 * @brief Set the size of the icon.
		 *
		 * Marks the component changed only when the size differs.
		 *
		 * @param fontSize The font size, in dp (24 for a standard icon).
		 * @return This component, for chaining.
		 */
		Glyph &setFontSize(const float &fontSize);

		/**
		 * @brief Get the font family of the icon.
		 * @return The style (Outlined by default).
		 */
		const Style &getStyle(void) const;

		/**
		 * @brief Set the font family of the icon.
		 *
		 * Marks the component changed only when the style differs.
		 *
		 * @param style The style; see the class note on what the systems
		 * do with it.
		 * @return This component, for chaining.
		 */
		Glyph &setStyle(const Style &style);
	};

}	 // namespace guillaume::components