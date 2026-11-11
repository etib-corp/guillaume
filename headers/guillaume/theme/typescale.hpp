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

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace guillaume::theme
{

	/**
	 * @brief The named text styles of Material Design 3: what a piece of
	 * text is, not how big it is.
	 *
	 * A widget asks for a role (`TitleLarge` for a dialog title,
	 * `LabelLarge` for a button label, `BodyMedium` for running text) and
	 * the theme answers with a TypeToken. Five families (Display, Headline,
	 * Title, Label, Body), each in three sizes, plus `Default`. The values
	 * given are the defaults: font size / line height, in dp.
	 *
	 * The list mirrors react-native-paper's `MD3TypescaleKey` plus its
	 * `default` entry.
	 * @see Typescale
	 */
	enum class TypeRole : std::size_t {
		DisplayLarge,	   ///< 57 / 64, regular.
		DisplayMedium,	   ///< 45 / 52, regular.
		DisplaySmall,	   ///< 36 / 44, regular.
		HeadlineLarge,	   ///< 32 / 40, regular.
		HeadlineMedium,	   ///< 28 / 36, regular.
		HeadlineSmall,	   ///< 24 / 32, regular.
		TitleLarge,		   ///< 22 / 28, regular.
		TitleMedium,	   ///< 16 / 24, medium weight.
		TitleSmall,		   ///< 14 / 20, medium weight.
		LabelLarge,		   ///< 14 / 20, medium weight.
		LabelMedium,	   ///< 12 / 16, medium weight.
		LabelSmall,		   ///< 11 / 16, medium weight.
		BodyLarge,		   ///< 16 / 24, regular.
		BodyMedium,		   ///< 14 / 20, regular.
		BodySmall,		   ///< 12 / 16, regular.
		Default,		   ///< 14 / 20, regular: text with no specific role.
		Count			   ///< Number of roles; not a role.
	};

	/**
	 * @brief Font weights available to the typescale. The value is the
	 * usual numeric weight (400 regular, 500 medium).
	 */
	enum class FontWeight : std::uint16_t {
		Regular = 400,	  ///< Roboto Regular.
		Medium	= 500	  ///< Roboto Medium.
	};

	/**
	 * @brief One text style: everything needed to measure and draw a piece
	 * of text.
	 *
	 * The default-constructed token is the `Default` role: Roboto regular,
	 * 14 dp on a 20 dp line.
	 */
	struct TypeToken {
		std::string fontFamily { "Roboto" };			  ///< Font family name.
		FontWeight fontWeight { FontWeight::Regular };	  ///< Font weight.
		float fontSize { 14.0f };						  ///< Font size, in dp.
		float lineHeight {
			20.0f
		};	  ///< Distance between two baselines, in dp.
		float letterSpacing {
			0.0f
		};	  ///< Extra space between characters, in dp.
	};

	/**
	 * @brief Get the font file shipped with the library for a weight.
	 * @param weight The font weight.
	 * @return The path of the font file, relative to the resource root:
	 * `fonts/Roboto-Medium.ttf` for `Medium`, `fonts/Roboto-Regular.ttf`
	 * otherwise.
	 */
	const std::string &fontPath(FontWeight weight);

	/**
	 * @brief The text styles of a theme: one TypeToken per TypeRole.
	 *
	 * Read a style through guillaume::getTypeToken. To change one, edit a
	 * copy of the theme and activate it:
	 * @code
	 * guillaume::Theme custom = guillaume::getActiveTheme();
	 * auto title = custom.getTypescale().get(theme::TypeRole::TitleLarge);
	 * title.fontSize	 = 24.0f;
	 * title.lineHeight = 30.0f;
	 * custom.getTypescale().set(theme::TypeRole::TitleLarge, title);
	 * guillaume::setActiveTheme(custom);
	 * @endcode
	 *
	 * @see guillaume::Theme::getTypescale
	 */
	class Typescale
	{
		public:
		/**
		 * @brief Create the scale with the Material 3 baseline styles
		 * listed in TypeRole.
		 */
		Typescale(void);

		/**
		 * @brief Default destructor.
		 */
		~Typescale(void) = default;

		/**
		 * @brief Get the text style of a role.
		 * @param role The typescale role to look up.
		 * @return The token of that role, read-only.
		 * @throws std::out_of_range When `role` is `TypeRole::Count`.
		 */
		const TypeToken &get(TypeRole role) const;

		/**
		 * @brief Replace the text style of a role.
		 * @param role The typescale role to change.
		 * @param token The new style. Its font size and line height must be
		 * greater than 0.
		 * @throws std::out_of_range When `role` is `TypeRole::Count`.
		 * @throws std::invalid_argument When the font size or the line
		 * height is zero or negative. The role keeps its previous style.
		 */
		void set(TypeRole role, const TypeToken &token);

		private:
		std::array<TypeToken, static_cast<std::size_t>(TypeRole::Count)>
			_tokens;	///< Tokens indexed by TypeRole.
	};

}	 // namespace guillaume::theme
