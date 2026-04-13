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

	ecs::Entity::Identifier Icon::Builder::registerEntity(void)
	{
		ecs::Entity::Identifier identifier = ecs::Entity::InvalidIdentifier;

		_icon = std::make_unique<Icon>(this->getComponentRegistry(), _glyphName,
									   _size, _color, _style);
		identifier = _icon->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_icon));
		return identifier;
	}

	void Icon::Builder::reset(void)
	{
		_icon.reset();
		_glyphName.clear();
		_size  = 24.0f;
		_color = { 255, 255, 255, 255 };
		_style = Style::Outlined;
	}

	Icon::Builder &Icon::Builder::withGlyphName(const std::string &glyphName)
	{
		_glyphName = glyphName;
		return *this;
	}

	Icon::Builder &Icon::Builder::withSize(float size)
	{
		_size = size;
		return *this;
	}

	Icon::Builder &
		Icon::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Icon::Builder &Icon::Builder::withStyle(const Style &style)
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

	ecs::Entity::Identifier Icon::Director::makeDefaultIcon(
		Builder &builder, const std::string &glyphName, const float size)
	{
		return builder.withGlyphName(glyphName).withSize(size).registerEntity();
	}

	Icon::Icon(ecs::ComponentRegistry &registry, const std::string &glyphName,
			   const float size, const utility::graphic::Color32Bit &color,
			   const Style &style)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Glyph, components::Color>(registry)
		, _glyphName(glyphName)
		, _size(size)
		, _color(color)
		, _style(style)
	{
		update();
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

	Icon &Icon::setSize(const float size)
	{
		_size = size;
		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(size)
			.setHeight(size);
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

	Icon &Icon::setStyle(const Style &style)
	{
		_style = style;
		return *this;
	}

	void Icon::update(void)
	{
		setGlyphName(_glyphName);
		setSize(_size);
		setColor(_color);
		setStyle(_style);
	}

}	 // namespace guillaume::entities