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

#include <utility/graphic/text.hpp>

#include "guillaume/systems/measure_text.hpp"

namespace guillaume::systems
{

	MeasureText::MeasureText(Renderer &renderer)
		: ecs::SystemFiller<components::Transform, components::Text,
							components::Bound>()
		, _renderer(renderer)
		, _defaultFontPath(
			  "assets/fonts/Roboto/Roboto-VariableFont_wdth,wght.ttf")
	{
	}

	MeasureText::~MeasureText(void)
	{
	}

	void MeasureText::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating MeasureText system for entity "
						  + std::to_string(entityIdentifier));
		if (!requireComponent<components::Text>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &textComponent =
			getComponent<components::Text>(entityIdentifier);
		auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);

		utility::graphic::Text text;
		text.setPosition(transformComponent.getPosition())
			.setOrientation(transformComponent.getOrientation())
			.setScale(transformComponent.getScale())
			.setColor(textComponent.getColor())
			.setContent(textComponent.getContent())
			.setFontSize(textComponent.getFontSize())
			.setFontPath(_defaultFontPath);

		boundComponent.setSize(_renderer.measureText(text));
	}

}	 // namespace guillaume::systems
