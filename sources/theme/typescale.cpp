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

#include "guillaume/theme/typescale.hpp"

#include <stdexcept>

namespace guillaume::theme
{
	namespace
	{
		std::size_t toIndex(TypeRole role)
		{
			const auto index = static_cast<std::size_t>(role);
			if (index >= static_cast<std::size_t>(TypeRole::Count)) {
				throw std::out_of_range("Invalid typescale role");
			}
			return index;
		}

		TypeToken token(FontWeight weight, float size, float lineHeight,
						float letterSpacing)
		{
			TypeToken result;
			result.fontWeight	 = weight;
			result.fontSize		 = size;
			result.lineHeight	 = lineHeight;
			result.letterSpacing = letterSpacing;
			return result;
		}
	}	 // namespace

	const std::string &fontPath(FontWeight weight)
	{
		static const std::string regular("fonts/Roboto-Regular.ttf");
		static const std::string medium("fonts/Roboto-Medium.ttf");
		return weight == FontWeight::Medium ? medium : regular;
	}

	Typescale::Typescale(void)
		: _tokens()
	{
		constexpr auto R = FontWeight::Regular;
		constexpr auto M = FontWeight::Medium;

		// M3 baseline, as published in react-native-paper v5 tokens.
		set(TypeRole::DisplayLarge, token(R, 57.0f, 64.0f, 0.0f));
		set(TypeRole::DisplayMedium, token(R, 45.0f, 52.0f, 0.0f));
		set(TypeRole::DisplaySmall, token(R, 36.0f, 44.0f, 0.0f));
		set(TypeRole::HeadlineLarge, token(R, 32.0f, 40.0f, 0.0f));
		set(TypeRole::HeadlineMedium, token(R, 28.0f, 36.0f, 0.0f));
		set(TypeRole::HeadlineSmall, token(R, 24.0f, 32.0f, 0.0f));
		set(TypeRole::TitleLarge, token(R, 22.0f, 28.0f, 0.0f));
		set(TypeRole::TitleMedium, token(M, 16.0f, 24.0f, 0.15f));
		set(TypeRole::TitleSmall, token(M, 14.0f, 20.0f, 0.1f));
		set(TypeRole::LabelLarge, token(M, 14.0f, 20.0f, 0.1f));
		set(TypeRole::LabelMedium, token(M, 12.0f, 16.0f, 0.5f));
		set(TypeRole::LabelSmall, token(M, 11.0f, 16.0f, 0.5f));
		set(TypeRole::BodyLarge, token(R, 16.0f, 24.0f, 0.15f));
		set(TypeRole::BodyMedium, token(R, 14.0f, 20.0f, 0.25f));
		set(TypeRole::BodySmall, token(R, 12.0f, 16.0f, 0.4f));
		set(TypeRole::Default, token(R, 14.0f, 20.0f, 0.0f));
	}

	const TypeToken &Typescale::get(TypeRole role) const
	{
		return _tokens[toIndex(role)];
	}

	void Typescale::set(TypeRole role, const TypeToken &token)
	{
		if (token.fontSize <= 0.0f || token.lineHeight <= 0.0f) {
			throw std::invalid_argument(
				"Typescale font size and line height must be positive");
		}
		_tokens[toIndex(role)] = token;
	}

}	 // namespace guillaume::theme
