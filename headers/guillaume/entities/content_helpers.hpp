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

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry.hpp"

#include "guillaume/components/glyph.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Create an Icon as a child of a widget, or nothing when there is
	 * no glyph to show.
	 *
	 * Widgets call this from their `build()` for optional icons, so an empty
	 * glyph name simply produces no child.
	 * @param registry The registry that stores the icon's components.
	 * @param entityRegistry The entity registry that owns the icon (usually
	 * the parent widget itself).
	 * @param parent The entity the icon is attached to.
	 * @param glyphName The name of the glyph to draw, such as "check".
	 * @param fontSize The glyph size, in dp.
	 * @param color The glyph color.
	 * @param style The glyph style (outlined by default).
	 * @return The registered icon, or nullptr when @p glyphName is empty or
	 * @p parent is null.
	 * @see Icon::Builder::makeIcon
	 */
	inline std::shared_ptr<Icon> buildIcon(
		ecs::ComponentRegistry &registry, ecs::EntityRegistry &entityRegistry,
		std::shared_ptr<ecs::Entity> parent, const std::string &glyphName,
		float fontSize, const utility::graphic::Color32Bit &color,
		components::Glyph::Style style = components::Glyph::Style::Outlined)
	{
		if (glyphName.empty() || parent == nullptr) {
			return nullptr;
		}

		Icon::Builder builder(registry, entityRegistry);

		return builder.makeIcon(parent, glyphName, fontSize, color, style);
	}

	/**
	 * @brief Create a Text of a given font size as a child of a widget, or
	 * nothing when there is no content.
	 * @param registry The registry that stores the text's components.
	 * @param entityRegistry The entity registry that owns the text (usually
	 * the parent widget itself).
	 * @param parent The entity the text is attached to.
	 * @param content The string to display.
	 * @param fontSize The font size, in dp.
	 * @param color The text color.
	 * @return The registered text, or nullptr when @p content is empty or
	 * @p parent is null.
	 * @see Text::Builder::makeText
	 */
	inline std::shared_ptr<Text> buildText(
		ecs::ComponentRegistry &registry, ecs::EntityRegistry &entityRegistry,
		std::shared_ptr<ecs::Entity> parent, const std::string &content,
		float fontSize, const utility::graphic::Color32Bit &color)
	{
		if (content.empty() || parent == nullptr) {
			return nullptr;
		}

		Text::Builder builder(registry, entityRegistry);

		return builder.makeText(parent, content, fontSize, color);
	}

	/**
	 * @brief Create a Text styled by a type scale role as a child of a
	 * widget, or nothing when there is no content.
	 * @param registry The registry that stores the text's components.
	 * @param entityRegistry The entity registry that owns the text (usually
	 * the parent widget itself).
	 * @param parent The entity the text is attached to.
	 * @param content The string to display.
	 * @param role The type scale role that sets the font size and weight,
	 * such as theme::TypeRole::LabelLarge.
	 * @param color The text color.
	 * @return The registered text, or nullptr when @p content is empty or
	 * @p parent is null.
	 * @see Text::Builder::makeText
	 */
	inline std::shared_ptr<Text> buildText(
		ecs::ComponentRegistry &registry, ecs::EntityRegistry &entityRegistry,
		std::shared_ptr<ecs::Entity> parent, const std::string &content,
		theme::TypeRole role, const utility::graphic::Color32Bit &color)
	{
		if (content.empty() || parent == nullptr) {
			return nullptr;
		}

		Text::Builder builder(registry, entityRegistry);

		return builder.makeText(parent, content, role, color);
	}

}	 // namespace guillaume::entities
