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

#include <utility/graphic/text/text.hpp>

#include "guillaume/systems/glyph_render.hpp"

namespace guillaume::systems
{
	namespace
	{
		std::string codePointToUtf8(uint32_t codePoint)
		{
			std::string result;
			if (codePoint <= 0x7F) {
				result.push_back(static_cast<char>(codePoint));
			} else if (codePoint <= 0x7FF) {
				result.push_back(static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F)));
				result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
			} else if (codePoint <= 0xFFFF) {
				result.push_back(static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F)));
				result.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
			} else {
				result.push_back(static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07)));
				result.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
			}
			return result;
		}
	}

	GlyphRender::GlyphRender(Renderer &renderer)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Glyph, components::Color>(
			  ecs::System::Phase::Render)
		, _renderer(renderer)
		, _defaultFontPath(
			  "assets/fonts/Material_Symbols_Outlined/"
			  "MaterialSymbolsOutlined-VariableFont_FILL,GRAD,opsz,wght.ttf")
	{
		loadGlyphCodes(
			"assets/fonts/Material_Symbols_Outlined/"
			"MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].codepoints");
	}

	GlyphRender::~GlyphRender(void)
	{
	}

	void GlyphRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating GlyphRender system for entity "
						  + std::to_string(entityIdentifier));
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Glyph>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		const std::string glyphName = glyphComponent.getName();

		getLogger().debug("Rendering glyph '" + glyphName + "' for entity "
						  + std::to_string(entityIdentifier));
		getLogger().debug("Glyph code found for '" + glyphName
						  + "': " + std::to_string(glyphComponent.getCode()));

		const uint32_t glyphCode =
			_glyphCode.count(glyphName) > 0 ? _glyphCode[glyphName] : '?';
		utility::graphic::Text glyphText(
			_renderer.getRessourceManager(), _renderer.getAssetManager(),
			codePointToUtf8(glyphCode), boundComponent.getHeight(),
			_defaultFontPath);
		glyphText.setColor(colorComponent.getColor());

		_renderer.drawText(glyphText, transformComponent.getPose());
	}

	void GlyphRender::loadGlyphCodes(const std::string &filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open()) {
			getLogger().error("Failed to open glyph code file: " + filePath);
			return;
		}
		std::string line;
		while (std::getline(file, line)) {
			size_t spacePos = line.find(' ');
			if (spacePos != std::string::npos) {
				std::string name = line.substr(0, spacePos);
				uint32_t code =
					std::stoul(line.substr(spacePos + 1), &spacePos, 16);
				_glyphCode[name] = code;
			}
		}
		getLogger().info("Loaded " + std::to_string(_glyphCode.size())
						 + " glyph codes from " + filePath);
	}

}	 // namespace guillaume::systems