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
	 * @brief Which typescale role a text uses (BodyMedium, TitleLarge, ...)
	 * and an optional explicit size.
	 *
	 * entities::Text writes it. systems::Typography reads it every frame
	 * in the Measure phase, resolves the role against the active theme and
	 * writes the font size, weight, line height and letter spacing into
	 * the entity's components::Text, so a theme change restyles existing
	 * texts.
	 *
	 * @code
	 * registry.getComponent<components::Typography>(id).setRole(
	 * 	theme::TypeRole::TitleLarge);
	 * @endcode
	 *
	 * @note On an entity that has this component, metrics set directly on
	 * components::Text are replaced on the next frame. Change the role or
	 * the size override here.
	 *
	 * @see systems::Typography, components::Text, theme::TypeRole
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

		/**
		 * @brief Get the typescale role.
		 * @return The role (BodyMedium by default).
		 */
		theme::TypeRole getRole(void) const;

		/**
		 * @brief Set the typescale role.
		 *
		 * Marks the component changed only when the role differs.
		 *
		 * @param role The role systems::Typography resolves against the
		 * active theme.
		 * @return This component, for chaining.
		 */
		Typography &setRole(theme::TypeRole role);

		/**
		 * @brief Get the explicit font size.
		 * @return The size, in dp, that replaces the size of the role;
		 * `std::nullopt` (the default) when the role's size is used.
		 */
		std::optional<float> getFontSize(void) const;

		/**
		 * @brief Override the font size of the role, or clear the override.
		 *
		 * Only the size is replaced: the weight, line height and letter
		 * spacing still come from the role. Marks the component changed
		 * only when the value differs.
		 *
		 * @param fontSize The size, in dp, or `std::nullopt` to use the
		 * size of the role.
		 * @return This component, for chaining.
		 */
		Typography &setFontSize(std::optional<float> fontSize);

		private:
		theme::TypeRole _role {
			theme::TypeRole::BodyMedium
		};	  ///< Typescale role.
		std::optional<float> _fontSize {};	  ///< Size override, in dp.
	};

}	 // namespace guillaume::components
