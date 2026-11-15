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

#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Holds one builder per builder type and hands them out by type.
	 *
	 * Each Scene has one, reached with Scene::getBuilderManager and filled
	 * with a builder for every widget of the library. This is how a scene
	 * creates its widgets:
	 * @code
	 * auto &builder =
	 * 	getBuilderManager().getBuilder<entities::Button::Builder>();
	 * auto button = builder.makeButton(nullptr, "Save", [] {},
	 * 	entities::Button::Color::Filled, entities::Button::Shape::Round,
	 * 	entities::Button::Size::Medium, false);
	 * @endcode
	 *
	 * @note A builder is shared by all its users: it keeps its options
	 * between calls until an entity is built, which resets it.
	 * @see EntityBuilder
	 * @see EntityBuilderManagerFiller
	 */
	class EntityBuilderManager
	{
		private:
		std::map<std::type_index, std::unique_ptr<EntityBuilder>>
			_builders;	  ///< The builders, by builder type

		public:
		/**
		 * @brief Create a builder of a given type and keep it.
		 * @tparam BuilderType The builder type to create, for instance
		 * `entities::Button::Builder`. It must be constructible from the two
		 * registries.
		 * @param componentRegistry The registry in which the builder creates
		 * components. It must outlive the manager.
		 * @param entityRegistry The registry in which the builder registers
		 * entities. It must outlive the manager.
		 * @throws std::runtime_error When the manager already holds a
		 * builder of that type.
		 */
		template<InheritFromEntityBuilder BuilderType>
		void addBuilder(ComponentRegistry &componentRegistry,
						EntityRegistry &entityRegistry);

		public:
		/**
		 * @brief Create a manager with no builder.
		 */
		EntityBuilderManager(void);

		/**
		 * @brief Destroy the manager and the builders it holds.
		 */
		virtual ~EntityBuilderManager(void);

		/**
		 * @brief Get the builder of a given type.
		 * @tparam BuilderType The builder type to get, for instance
		 * `entities::Button::Builder`.
		 * @return Reference to the builder, valid as long as the manager.
		 * @throws std::runtime_error When no builder of that type was added.
		 */
		template<InheritFromEntityBuilder BuilderType>
		BuilderType &getBuilder(void);
	};

}	 // namespace guillaume::ecs

// Include the implementation of the EntityBuilderManager template class
#include "guillaume/ecs/entity_builder_manager.tpp"