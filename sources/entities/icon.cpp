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

#include "guillaume/entities/icon.hpp"

namespace guillaume::entities
{

	Icon::Icon::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Icon::Builder::~Builder(void)
	{
	}

	std::shared_ptr<Icon>
		Icon::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_icon = std::make_shared<Icon>(this->getComponentRegistry(), _glyphName,
									   _fontSize, _color, _style);
		_icon->setParent(parent);

		this->getEntityRegistry().addEntity(_icon);

		auto iconCopy = _icon;

		reset();

		return iconCopy;
	}

	void Icon::Builder::reset(void)
	{
		_icon.reset();
		_glyphName.clear();
		_fontSize = 24.0f;
		_color	  = { 255, 255, 255, 255 };
		_style	  = components::Glyph::Style::Outlined;
	}

	Icon::Builder &Icon::Builder::withGlyphName(const std::string &glyphName)
	{
		_glyphName = glyphName;
		return *this;
	}

	Icon::Builder &Icon::Builder::withFontSize(float fontSize)
	{
		_fontSize = fontSize;
		return *this;
	}

	Icon::Builder &
		Icon::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Icon::Builder &
		Icon::Builder::withStyle(const components::Glyph::Style &style)
	{
		_style = style;
		return *this;
	}

	Icon::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Icon::Director::~Director(void)
	{
	}

	std::shared_ptr<Icon> Icon::Director::makeIcon(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &glyphName, const float &fontSize,
		const utility::graphic::Color32Bit &color,
		const components::Glyph::Style &style)
	{
		return builder.withGlyphName(glyphName)
			.withFontSize(fontSize)
			.withColor(color)
			.withStyle(style)
			.registerEntity(parent);
	}

	Icon::Icon(ecs::ComponentRegistry &registry, const std::string &glyphName,
			   const float &fontSize, const utility::graphic::Color32Bit &color,
			   const components::Glyph::Style &style)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Glyph, components::Color>(registry)
		, _glyphName(glyphName)
		, _fontSize(fontSize)
		, _color(color)
		, _style(style)
	{
	}

	Icon::~Icon()
	{
	}

	Icon &Icon::setGlyphName(const std::string &glyphName)
	{
		_glyphName = glyphName;

		getComponentRegistry()
			.getComponent<components::Glyph>(getIdentifier())
			.setName(glyphName);

		return *this;
	}

	Icon &Icon::setFontSize(const float &fontSize)
	{
		_fontSize = fontSize;

		getComponentRegistry()
			.getComponent<components::Glyph>(getIdentifier())
			.setFontSize(fontSize);

		return *this;
	}

	Icon &Icon::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);

		return *this;
	}

	Icon &Icon::setStyle(const components::Glyph::Style &style)
	{
		_style = style;

		getComponentRegistry()
			.getComponent<components::Glyph>(getIdentifier())
			.setStyle(style);

		return *this;
	}

	void Icon::initialize(void)
	{
	}

	void Icon::update(void)
	{
		setGlyphName(_glyphName);
		setFontSize(_fontSize);
		setColor(_color);
		setStyle(_style);
	}

}	 // namespace guillaume::entities