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

#include <optional>

#include "guillaume/ecs/component.hpp"
#include "guillaume/theme/typescale.hpp"

namespace guillaume::components
{
	/**
	 * @brief Which typescale role a text uses, and an optional explicit size.
	 *
	 * systems::Typography resolves the role against the active theme every
	 * frame and writes the result into the entity's components::Text, so a
	 * theme change restyles existing texts.
	 */
	class Typography: public ecs::Component
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Typography(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Typography(void) = default;

		/** @brief Get the typescale role. @return The role. */
		theme::TypeRole getRole(void) const;

		/**
		 * @brief Set the typescale role.
		 * @param role The role to resolve against the theme.
		 * @return This component.
		 */
		Typography &setRole(theme::TypeRole role);

		/**
		 * @brief Get the explicit font size.
		 * @return The size overriding the role size, if any.
		 */
		std::optional<float> getFontSize(void) const;

		/**
		 * @brief Override the font size of the role, or clear the override.
		 * @param fontSize The explicit size, or std::nullopt to use the role.
		 * @return This component.
		 */
		Typography &setFontSize(std::optional<float> fontSize);

		private:
		theme::TypeRole _role { theme::TypeRole::BodyMedium };	  ///< Role.
		std::optional<float> _fontSize {};	  ///< Explicit size override.
	};

}	 // namespace guillaume::components
