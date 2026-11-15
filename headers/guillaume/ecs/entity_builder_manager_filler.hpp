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

#pragma once

#include "guillaume/ecs/entity_builder_manager.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief An EntityBuilderManager that creates one builder of each listed
	 * type when it is constructed.
	 *
	 * Scene uses it to make the builder of every widget of the library
	 * available from Scene::getBuilderManager.
	 *
	 * @code
	 * ecs::EntityBuilderManagerFiller<entities::Text::Builder,
	 * 								entities::Button::Builder>
	 * 	builders(componentRegistry, entityRegistry);
	 * auto &text = builders.getBuilder<entities::Text::Builder>();
	 * @endcode
	 *
	 * @tparam BuilderTypes The builder types to create. Each must appear
	 * once and be constructible from the two registries.
	 * @see EntityBuilderManager
	 */
	template<InheritFromEntityBuilder... BuilderTypes>
	class EntityBuilderManagerFiller: public EntityBuilderManager
	{
		public:
		/**
		 * @brief Create the manager and one builder of each listed type,
		 * all working on the same pair of registries.
		 * @param componentRegistry The registry in which the builders create
		 * components. It must outlive the manager.
		 * @param entityRegistry The registry in which the builders register
		 * entities. It must outlive the manager.
		 * @throws std::runtime_error When a builder type is listed twice.
		 */
		EntityBuilderManagerFiller(ComponentRegistry &componentRegistry,
								   EntityRegistry &entityRegistry);

		/**
		 * @brief Destroy the manager and the builders it holds.
		 */
		virtual ~EntityBuilderManagerFiller(void) = default;
	};

}	 // namespace guillaume::ecs

// Include the implementation of the EntityBuilderManagerFiller template class
#include "guillaume/ecs/entity_builder_manager_filler.tpp"