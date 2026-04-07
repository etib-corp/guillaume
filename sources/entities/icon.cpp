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
									   _color, _style);
		identifier = _icon->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_icon));
		return identifier;
	}

	void Icon::Builder::reset(void)
	{
		_icon.reset();
		_glyphName.clear();
		_color = { 255, 255, 255, 255 };
		_style = Style::Outlined;
	}

	Icon::Builder &Icon::Builder::withGlyphName(const std::string &glyphName)
	{
		_glyphName = glyphName;
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

	ecs::Entity::Identifier
		Icon::Director::makeDefaultIcon(Builder &builder,
										const std::string &glyphName)
	{
		return builder.withGlyphName(glyphName).registerEntity();
	}

	Icon::Icon(ecs::ComponentRegistry &registry, const std::string &glyphName,
			   const utility::graphic::Color32Bit &color, const Style &style)
		: ecs::EntityFiller<components::Transform, components::Glyph>(registry)
		, _glyphName(glyphName)
		, _color(color)
		, _style(style)
	{
		registry.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(400.0f, 400.0f, 0.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)));

		registry.getComponent<components::Glyph>(getIdentifier())
			.setName(glyphName)
			.setFontSize(96)
			.setColor({ 255, 255, 255, 255 });
	}

	Icon::~Icon()
	{
	}

	Icon &Icon::setGlyphName(const std::string &glyphName)
	{
		_glyphName = glyphName;
		return *this;
	}

	Icon &Icon::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Icon &Icon::setStyle(const Style &style)
	{
		_style = style;
		return *this;
	}

}	 // namespace guillaume::entities