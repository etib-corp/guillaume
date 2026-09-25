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

#include <memory>

#include <utility/graphic/text/text.hpp>

#include "guillaume/systems/glyph_render.hpp"

namespace guillaume::systems
{
	GlyphRender::GlyphRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Glyph, components::Color>(
			  ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath(
			  "fonts/Material_Symbols_Outlined/"
			  "MaterialSymbolsOutlined-VariableFont_FILL,GRAD,opsz,wght.ttf")
		, _glyphCodePath(
			  "fonts/Material_Symbols_Outlined/"
			  "MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].codepoints")
	{
		_codePoints = _ressourceProvider->loadCodePoints(_glyphCodePath);
		if (_codePoints) {
			getLogger().info() << "Loaded code points from " + _glyphCodePath;
		} else {
			getLogger().error()
				<< "Failed to load glyph code asset: " << _glyphCodePath;
		}
	}

	GlyphRender::~GlyphRender(void)
	{
		clear();
	}

	void GlyphRender::prepare(void)
	{
		markAllUnused();
	}

	void GlyphRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void GlyphRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating GlyphRender system for entity " << entityIdentifier;
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Glyph>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		getLogger().debug() << "Rendering glyph '" << glyphComponent.getName()
							<< "' for entity " << entityIdentifier;

		uint32_t glyphCode = _codePoints->getCode(glyphComponent.getName());

		if (glyphCode == 0) {
			glyphCode = '?';
		}

		auto renderable = std::make_shared<utility::graphic::Text>(
			_ressourceProvider, transformComponent.getPose(),
			colorComponent.getColor(),
			utility::graphic::CodePoints::toUtf8(glyphCode),
			glyphComponent.getFontSize(), _defaultFontPath);

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine->createObject(renderable);
			insert(entityIdentifier, RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	  = true;
		}
	}

}	 // namespace guillaume::systems
