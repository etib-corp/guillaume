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

#include "guillaume/entities/panel.hpp"

namespace guillaume::entities
{

	Panel::Panel::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								   ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Panel::Builder::~Builder(void)
	{
	}

	ecs::Entity::Identifier Panel::Builder::registerEntity(void)
	{
		ecs::Entity::Identifier identifier = ecs::Entity::InvalidIdentifier;

		_panel = std::make_unique<Panel>(this->getComponentRegistry(), _pose,
										 _color, _borderRadius, _children);
		identifier = _panel->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_panel));
		return identifier;
	}

	void Panel::Builder::reset(void)
	{
		_panel.reset();
		_pose		  = utility::graphic::PoseF();
		_color		  = { 255, 255, 255, 255 };
		_borderRadius = 16.0f;
	}

	Panel::Builder &
		Panel::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		return *this;
	}

	Panel::Builder &Panel::Builder::withChildren(
		const std::vector<ecs::Entity::Identifier> &children)
	{
		_children = children;
		return *this;
	}

	Panel::Builder &
		Panel::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Panel::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Panel::Director::~Director(void)
	{
	}

	ecs::Entity::Identifier Panel::Director::makeDefaultPanel(
		Builder &builder, const utility::graphic::PoseF &pose,
		const std::vector<ecs::Entity::Identifier> &children)
	{
		return builder.withPose(pose).withChildren(children).registerEntity();
	}

	ecs::Entity::Identifier Panel::Director::makeColorPanel(
		Builder &builder, const utility::graphic::PoseF &pose,
		const utility::graphic::Color32Bit &color,
		const std::vector<ecs::Entity::Identifier> &children)
	{
		return builder.withPose(pose)
			.withColor(color)
			.withChildren(children)
			.registerEntity();
	}

	Panel::Panel(ecs::ComponentRegistry &registry,
				 const utility::graphic::PoseF &pose,
				 const utility::graphic::Color32Bit &color, float borderRadius,
				 const std::vector<ecs::Entity::Identifier> &children)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Color, components::Borders>(registry)
	{
		setPose(pose);
		setColor(color);
		setBorderRadius(borderRadius);
		setChildren(children);
	}

	Panel::~Panel()
	{
	}

	Panel &Panel::setPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(pose);
		return *this;
	}

	Panel &Panel::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	Panel &Panel::setBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(borderRadius);
		return *this;
	}

	Panel &
		Panel::setChildren(const std::vector<ecs::Entity::Identifier> &children)
	{
		_children = children;

		return *this;
	}

}	 // namespace guillaume::entities