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
	 * @brief Build an `Icon` child entity.
	 * @param registry The component registry.
	 * @param parent The parent entity (usually the owning surface).
	 * @param glyphName The glyph name to render.
	 * @param fontSize The icon font size.
	 * @param color The icon color.
	 * @param style The glyph style.
	 * @return The newly created icon entity, or nullptr when the name is empty.
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
		Icon::Director director;

		return director.makeIcon(builder, parent, glyphName, fontSize, color,
								 style);
	}

	/**
	 * @brief Build a `Text` child entity.
	 * @param registry The component registry.
	 * @param parent The parent entity (usually the owning surface).
	 * @param content The text content.
	 * @param fontSize The text font size.
	 * @param color The text color.
	 * @return The newly created text entity, or nullptr when content is empty.
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
		Text::Director director;

		return director.makeText(builder, parent, content, fontSize, color);
	}

}	 // namespace guillaume::entities
