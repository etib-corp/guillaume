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

#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	Text::Text::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Text::Builder::~Builder(void)
	{
	}

	ecs::Entity::Identifier Text::Builder::registerEntity(void)
	{
		ecs::Entity::Identifier identifier = ecs::Entity::InvalidIdentifier;

		_text = std::make_unique<Text>(this->getComponentRegistry(), _content,
									   _fontSize, _color);

		identifier = _text->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_text));
		return identifier;
	}

	void Text::Builder::reset(void)
	{
		_text.reset();
		_content.clear();
		_fontSize = 24;
		_color	  = { 255, 255, 255, 255 };
	}

	Text::Builder &Text::Builder::withContent(const std::string &content)
	{
		_content = content;
		return *this;
	}

	Text::Builder &Text::Builder::withFontSize(const std::size_t &fontSize)
	{
		_fontSize = fontSize;
		return *this;
	}

	Text::Builder &
		Text::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Text::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Text::Director::~Director(void)
	{
	}

	ecs::Entity::Identifier
		Text::Director::makeDefaultText(Builder &builder,
										const std::string &content)
	{
		return builder.withContent(content).registerEntity();
	}

	Text::Text(ecs::ComponentRegistry &registry, const std::string &content,
			   const std::size_t &fontSize,
			   const utility::graphic::Color32Bit &color)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Text, components::Color>(registry)
	{
		setContent(content);
		setFontSize(fontSize);
		setColor(color);
	}

	Text::~Text()
	{
	}

	Text &Text::setContent(const std::string &content)
	{
		_content = content;
		getComponentRegistry()
			.getComponent<components::Text>(getIdentifier())
			.setContent(content);
		return *this;
	}

	Text &Text::setFontSize(std::size_t fontSize)
	{
		_fontSize = fontSize;
		getComponentRegistry()
			.getComponent<components::Text>(getIdentifier())
			.setFontSize(fontSize);
		return *this;
	}

	Text &Text::setColor(utility::graphic::Color32Bit color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}
}	 // namespace guillaume::entities