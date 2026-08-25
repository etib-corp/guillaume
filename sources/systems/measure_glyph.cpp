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
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <utility/graphic/text/text.hpp>
#include <utility/ressource_provider.hpp>

#include "guillaume/systems/measure_glyph.hpp"

namespace guillaume::systems
{

	MeasureGlyph::MeasureGlyph(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Glyph, components::Bound,
							components::Transform, components::Color>(
			  ecs::Phase::Measure)
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

	MeasureGlyph::~MeasureGlyph(void)
	{
	}

	void MeasureGlyph::prepare(void)
	{
	}

	void MeasureGlyph::cleanup(void)
	{
	}

	void MeasureGlyph::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating MeasureGlyph system for entity " << entityIdentifier;
		if (!requireComponent<components::Glyph>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);
		auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		MeasureGlyphCacheKey cacheKey { glyphComponent.getName(),
										glyphComponent.getFontSize(),
										glyphComponent.getStyle(),
										colorComponent.getColor() };

		getLogger().debug() << "Measuring glyph '" << cacheKey.glyphName
							<< "' for entity " << entityIdentifier;
		getLogger().debug() << "Glyph code found for '" << cacheKey.glyphName
							<< "': " << glyphComponent.getCode();

		if (contains(cacheKey)) {
			getLogger().debug() << "Glyph '" << cacheKey.glyphName
								<< "' already measured, skipping.";
			const auto &cacheValue = get(cacheKey);
			if (cacheValue.has_value()) {
				getComponent<components::Bound>(entityIdentifier)
					.setWidth(cacheValue->getWidth())
					.setHeight(cacheValue->getHeight());
			}
			return;
		}

		uint32_t glyphCode = _codePoints->getCode(cacheKey.glyphName);
		if (glyphCode == 0) {
			glyphCode = '?';
		}

		utility::graphic::Text text(
			_ressourceProvider, transformComponent.getPose(),
			colorComponent.getColor(),
			utility::graphic::CodePoints::toUtf8(glyphCode), cacheKey.fontSize,
			_defaultFontPath);
		text.setColor(utility::graphic::Color32Bit());

		auto textSize = _engine->measureText(text);

		getComponent<components::Bound>(entityIdentifier)
			.setWidth(textSize.getWidth())
			.setHeight(textSize.getHeight());

		put(cacheKey, textSize);
	}

}	 // namespace guillaume::systems
