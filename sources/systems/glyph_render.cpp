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
		apply([this](const GlyphRenderCacheKey &key,
					 GlyphRenderCacheEntry &entry) {
			entry.used = false;
		});
	}

	void GlyphRender::cleanup(void)
	{
		erase_if([this](const GlyphRenderCacheKey &key,
						const GlyphRenderCacheEntry &entry) {
			if (!entry.used) {
				_engine->removeObject(entry.value);
				return true;
			}
			return false;
		});
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
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		GlyphRenderCacheKey cacheKey { transformComponent.getPose(),
									   glyphComponent.getName(),
									   glyphComponent.getFontSize(),
									   glyphComponent.getStyle(),
									   colorComponent.getColor() };

		getLogger().debug() << "Rendering glyph '" << cacheKey.glyphName
							<< "' for entity " << entityIdentifier;
		getLogger().debug() << "Glyph code found for '" << cacheKey.glyphName
							<< "': " << glyphComponent.getCode();

		if (const auto &entry = get(cacheKey); entry.has_value()) {
			GlyphRenderCacheEntry newEntry { .used	= true,
											 .value = entry->value };
			put(cacheKey, std::move(newEntry));
			getLogger().debug() << "Cache hit for entity " << entityIdentifier;
			return;
		}

		uint32_t glyphCode = _codePoints->getCode(cacheKey.glyphName);

		if (glyphCode == 0) {
			glyphCode = '?';
		}

		utility::graphic::Text glyphText(
			_ressourceProvider, cacheKey.pose, cacheKey.color,
			utility::graphic::CodePoints::toUtf8(glyphCode), cacheKey.fontSize,
			_defaultFontPath);

		auto identifier = _engine->addText(std::move(glyphText));

		GlyphRenderCacheEntry cacheEntry { .used = true, .value = identifier };
		put(cacheKey, std::move(cacheEntry));
	}

}	 // namespace guillaume::systems
