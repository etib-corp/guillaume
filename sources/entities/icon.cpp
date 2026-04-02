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
		: ecs::LeafEntityBuilder(componentRegistry, entityRegistry)
	{
	}

	Icon::Builder::~Builder(void)
	{
	}

	void Icon::Builder::registerEntity(void)
	{
		_icon =
			std::make_unique<Icon>(getComponentRegistry(), _iconName, _style);
		getEntityRegistry().addEntity(std::move(_icon));
	}

	void Icon::Builder::reset(void)
	{
		_icon.reset();
		_iconName.clear();
		_style = Style::Outlined;
	}

	Icon::Builder &Icon::Builder::withIconName(const std::string &iconName)
	{
		_iconName = iconName;
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

	void Icon::Director::makeDefaultIcon(Builder &builder,
										 const std::string &iconName)
	{
		builder.withIconName(iconName).registerEntity();
	}

	Icon::Icon(ecs::ComponentRegistry &registry, const std::string &iconName,
			   const Style &style)
		: ecs::LeafEntityFiller<components::Transform, components::Icon>(
			  registry)
		, _iconName(iconName)
		, _style(style)
	{
		registry.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, 0.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)));

		registry.getComponent<components::Icon>(getIdentifier());
	}

	Icon::~Icon()
	{
	}

}	 // namespace guillaume::entities