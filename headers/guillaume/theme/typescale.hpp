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
	 * @brief Material 3 typescale roles.
	 *
	 * Mirrors react-native-paper's `MD3TypescaleKey` plus its `default`
	 * entry (`src/styles/themes/v3/tokens.tsx`).
	 */
	enum class TypeRole : std::size_t {
		DisplayLarge,
		DisplayMedium,
		DisplaySmall,
		HeadlineLarge,
		HeadlineMedium,
		HeadlineSmall,
		TitleLarge,
		TitleMedium,
		TitleSmall,
		LabelLarge,
		LabelMedium,
		LabelSmall,
		BodyLarge,
		BodyMedium,
		BodySmall,
		Default,
		Count
	};

	/**
	 * @brief Font weights available to the typescale.
	 */
	enum class FontWeight : std::uint16_t {
		Regular = 400,	  ///< Roboto Regular.
		Medium	= 500	  ///< Roboto Medium.
	};

	/**
	 * @brief Resolved typography for one typescale role.
	 */
	struct TypeToken {
		std::string fontFamily { "Roboto" };	///< Font family name.
		FontWeight fontWeight { FontWeight::Regular };	  ///< Font weight.
		float fontSize { 14.0f };		  ///< Font size, in dp.
		float lineHeight { 20.0f };		  ///< Line height, in dp.
		float letterSpacing { 0.0f };	  ///< Letter spacing, in dp.
	};

	/**
	 * @brief Get the bundled font file rendering a weight.
	 * @param weight Font weight.
	 * @return Font path relative to the resource root.
	 */
	const std::string &fontPath(FontWeight weight);

	/**
	 * @brief Material 3 typescale: one TypeToken per TypeRole.
	 */
	class Typescale
	{
		public:
		/**
		 * @brief Construct the M3 baseline typescale.
		 */
		Typescale(void);

		/**
		 * @brief Default destructor.
		 */
		~Typescale(void) = default;

		/**
		 * @brief Get the token of a role.
		 * @param role Typescale role.
		 * @return Immutable token reference.
		 * @throws std::out_of_range If @p role is `TypeRole::Count`.
		 */
		const TypeToken &get(TypeRole role) const;

		/**
		 * @brief Replace the token of a role.
		 * @param role Typescale role.
		 * @param token New token.
		 * @throws std::out_of_range If @p role is `TypeRole::Count`.
		 * @throws std::invalid_argument If the size or line height is not
		 * positive.
		 */
		void set(TypeRole role, const TypeToken &token);

		private:
		std::array<TypeToken, static_cast<std::size_t>(TypeRole::Count)>
			_tokens;	///< Tokens indexed by TypeRole.
	};

}	 // namespace guillaume::theme
