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

#include "guillaume/systems/measure_text.hpp"

namespace guillaume::systems
{

	MeasureText::MeasureText(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Text, components::Bound,
							components::Transform, components::Color>(
			  ecs::Phase::Measure)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath("fonts/Roboto-Regular.ttf")
	{
	}

	MeasureText::~MeasureText(void)
	{
	}

	void MeasureText::prepare(void)
	{
	}

	void MeasureText::cleanup(void)
	{
	}

	void MeasureText::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating MeasureText system for entity " << entityIdentifier;
		if (!requireComponent<components::Text>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &textComponent =
			getComponent<components::Text>(entityIdentifier);
		auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		MeasureTextCacheKey cacheKey { textComponent.getContent(),
									   textComponent.getFontSize(),
									   colorComponent.getColor() };

		getLogger().debug() << "Measuring text '" << cacheKey.content
							<< "' for entity " << entityIdentifier;

		if (contains(cacheKey)) {
			getLogger().debug() << "Text '" << cacheKey.content
								<< "' already measured, skipping.";
			const auto &cacheValue = get(cacheKey);
			if (cacheValue.has_value()) {
				getComponent<components::Bound>(entityIdentifier)
					.setWidth(cacheValue->getWidth())
					.setHeight(cacheValue->getHeight());
			}
			return;
		}

		utility::graphic::Text text(_ressourceProvider,
									transformComponent.getPose(),
									colorComponent.getColor(), cacheKey.content,
									cacheKey.fontSize, _defaultFontPath);
		text.setColor(utility::graphic::Color32Bit());

		auto textSize = _engine->measureText(text);

		getComponent<components::Bound>(entityIdentifier)
			.setWidth(textSize.getWidth())
			.setHeight(textSize.getHeight());

		put(cacheKey, textSize);
	}

}	 // namespace guillaume::systems
