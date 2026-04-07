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

#include <utility/graphic/glyph.hpp>

#include "guillaume/systems/glyph_render.hpp"
#include "utility/graphic/pose.hpp"

namespace guillaume::systems
{

	GlyphRender::GlyphRender(Renderer &renderer)
		: ecs::SystemFiller<components::Transform, components::Glyph>(
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
		if (!requireComponent<components::Glyph>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);

		const std::string glyphName = glyphComponent.getName();
		getLogger().debug("Rendering glyph '" + glyphName + "' for entity "
						  + std::to_string(entityIdentifier));
		getLogger().debug("Glyph code found for '" + glyphName
						  + "': " + std::to_string(glyphComponent.getCode()));

		utility::graphic::Glyph glyph(
			glyphName,
			_glyphCode.count(glyphName) > 0 ? _glyphCode[glyphName] : '?',
			_defaultFontPath, glyphComponent.getFontSize(),
			glyphComponent.getColor());

		utility::graphic::PoseF pose = transformComponent.getPose();
		pose.setPosition(pose.getPosition());

		_renderer.drawGlyph(glyph, pose);
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