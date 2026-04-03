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
		: ecs::NodeEntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Panel::Builder::~Builder(void)
	{
	}

	ecs::Entity::Identifier Panel::Builder::registerEntity(void)
	{
		ecs::Entity::Identifier identifier = ecs::Entity::InvalidIdentifier;

		_panel = std::make_unique<Panel>(this->getComponentRegistry(), _name);
		identifier = _panel->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_panel));
		return identifier;
	}

	void Panel::Builder::reset(void)
	{
		NodeEntityBuilder::reset();
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

	ecs::Entity::Identifier
		Panel::Director::makeDefaultPanel(Builder &builder,
										  const std::string &name)
	{
		return builder.withName(name).registerEntity();
	}

	Panel::Panel(ecs::ComponentRegistry &registry, const std::string &name)
		: ecs::NodeEntityFiller<components::Transform, components::Bound,
								components::Color, components::Borders>(
			  registry)
	{
		setName(name);
	}

	Panel::~Panel()
	{
	}

	Panel &Panel::setName(const std::string &name)
	{
		_name = name;
		return *this;
	}

}	 // namespace guillaume::entities