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

	std::shared_ptr<Text>
		Text::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_text = std::make_shared<Text>(this->getComponentRegistry(), _content,
									   _fontSize, _color);

		_text->setParent(parent);

		this->getEntityRegistry().addEntity(_text);

		auto textCopy =
			_text;	  // Create a copy of the shared pointer to return

		reset();

		return textCopy;
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

	Text::Builder &Text::Builder::withFontSize(const float &fontSize)
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

	std::shared_ptr<Text> Text::Director::makeText(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &content, const float &fontSize,
		const utility::graphic::Color32Bit &color)
	{
		return builder.withContent(content)
			.withFontSize(fontSize)
			.withColor(color)
			.registerEntity(parent);
	}

	Text::Text(ecs::ComponentRegistry &registry, const std::string &content,
			   const float &fontSize, const utility::graphic::Color32Bit &color)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Text, components::Color>(registry)
		, _content(content)
		, _fontSize(fontSize)
		, _color(color)
	{
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

	Text &Text::setFontSize(const float &fontSize)
	{
		_fontSize = fontSize;
		getComponentRegistry()
			.getComponent<components::Text>(getIdentifier())
			.setFontSize(fontSize);
		return *this;
	}

	Text &Text::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	void Text::initialize(void)
	{
	}

	void Text::update(void)
	{
		setContent(_content);
		setFontSize(_fontSize);
		setColor(_color);
	}
}	 // namespace guillaume::entities