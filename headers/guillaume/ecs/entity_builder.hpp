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

#include <memory>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base class for building entities with specific components.
	 *
	 * This class provides an interface for creating entities and managing their
	 * components. It is intended to be inherited by specific entity builders
	 * that define the components and structure of the entities they create.
	 */
	class EntityBuilder
	{
		private:
		ComponentRegistry
			&_componentRegistry;	///< Reference to the component
									///< registry for entity creation
		EntityRegistry
			&_entityRegistry;	 ///< Reference to the entity registry for
								 ///< entity registration

		protected:
		/**
		 * @brief Get the component registry reference.
		 * @return Reference to the component registry.
		 */
		ComponentRegistry &getComponentRegistry(void);

		/**
		 * @brief Get the entity registry reference.
		 * @return Reference to the entity registry.
		 */
		EntityRegistry &getEntityRegistry(void);

		public:
		/**
		 * @brief Default constructor for the EntityBuilder class.
		 * @param componentRegistry The component registry to use for entity
		 * construction.
		 * @param entityRegistry The entity registry to use for entity
		 * construction.
		 */
		EntityBuilder(ecs::ComponentRegistry &componentRegistry,
					  ecs::EntityRegistry &entityRegistry);

		/**
		 * @brief Default destructor for the EntityBuilder class.
		 */
		virtual ~EntityBuilder(void) = default;

		/**
		 * @brief Build and register the entity in the entity registry.
		 * @return The entity identifier of the newly created entity.
		 */
		virtual ecs::Entity::Identifier registerEntity(void) = 0;

		/**
		 * @brief Reset the builder to its initial state for creating a new
		 * entity.
		 */
		virtual void reset(void) = 0;
	};

	/**
	 * @brief Concept to ensure a type inherits from EntityBuilder.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEntityBuilder = std::is_base_of_v<EntityBuilder, Type>;

}	 // namespace guillaume::ecs