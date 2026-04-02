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
#include "guillaume/entities/button.hpp"

namespace guillaume::entities
{

	Panel::Panel::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								   ecs::EntityRegistry &entityRegistry)
		: ecs::NodeEntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Panel::Builder::~Builder(void)
	{
	}

	void Panel::Builder::registerEntity(void)
	{
		_panel = std::make_unique<Panel>(this->getComponentRegistry(),
										 this->getEntityRegistry(), _name);
		this->getEntityRegistry().addEntity(std::move(_panel));
	}

	void Panel::Builder::reset(void)
	{
		ecs::NodeEntityBuilder::reset();
		_panel.reset();
		_name.clear();
	}

	Panel::Builder &Panel::Builder::withName(const std::string &name)
	{
		_name = name;
		return *this;
	}

	Panel::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Panel::Director::~Director(void)
	{
	}

	void Panel::Director::makePanel(Builder &builder, const std::string &name)
	{
		builder.withName(name).registerEntity();
	}

	Panel::Panel(ecs::ComponentRegistry &registry,
				 ecs::EntityRegistry &entityRegistry, const std::string &name)
		: ecs::NodeEntityFiller<components::Transform, components::Bound>(
			  registry)
		, _registry(registry)
		, _entityRegistry(entityRegistry)
		, _name(name)
	{
		registerEntityFactory<Button::Builder, Button::Director>();

		// Initialize panel pose
		registry.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(0.0f, 0.0f, 0.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)));

		registry.getComponent<components::Bound>(getIdentifier())
			.setSize({ 400.0f, 300.0f });
	}

	Panel::~Panel(void)
	{
	}
}	 // namespace guillaume::entities