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

#include "guillaume/scene.hpp"

#include "guillaume/entities/panel.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/entities/icon.hpp"

namespace guillaume
{
	Scene::Scene(LocalStorage &localStorage, SessionStorage &sessionStorage)
		: _localStorage(localStorage)
		, _sessionStorage(sessionStorage)
		, _componentRegistry()
		, _entityBuilderManager(
			  std::make_unique<ecs::EntityBuilderManagerFiller<
				  entities::Panel::Builder, entities::Text::Builder,
				  entities::Button::Builder, entities::Icon::Builder>>(
				  _componentRegistry, *this))
		, _entityDirectorManager(
			  std::make_unique<ecs::EntityDirectorManagerFiller<
				  entities::Panel::Director, entities::Text::Director,
				  entities::Button::Director, entities::Icon::Director>>())
	{
		getLogger().info("Scene initialized with default entity builders and "
						 "directors");
	}

	Scene::~Scene(void)
	{
		getLogger().info(
			"Scene destroyed with "
			+ std::to_string(getEntitiesBreadthFirst().size())
			+ " entity/entities in hierarchy");
	}

}	 // namespace guillaume