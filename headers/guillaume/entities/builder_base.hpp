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
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Reusable base for entity builders.
	 *
	 * `EntityBuilderBase` implements the shared `registerEntity` logic once:
	 * build the entity through the `buildEntity` hook, attach the parent,
	 * register it, reset the builder, and return it. Concrete builders only
	 * declare their configuration fields and implement `buildEntity`.
	 *
	 * @tparam EntityType The concrete entity type produced by the builder.
	 */
	template<typename EntityType> class EntityBuilderBase:
		public ecs::EntityBuilder
	{
		public:
		/**
		 * @brief Construct a builder.
		 * @param componentRegistry The component registry.
		 * @param entityRegistry The entity registry.
		 */
		EntityBuilderBase(ecs::ComponentRegistry &componentRegistry,
						  ecs::EntityRegistry &entityRegistry);

		/**
		 * @brief Default destructor.
		 */
		~EntityBuilderBase(void) override = default;

		/**
		 * @brief Build and register the entity, then reset the builder.
		 * @param parent The parent entity to attach the new entity to.
		 * @return The newly created entity.
		 */
		std::shared_ptr<EntityType>
			registerEntity(std::shared_ptr<ecs::Entity> parent);

		protected:
		/**
		 * @brief Build the entity from the current configuration.
		 *
		 * Implemented by concrete builders. Must not attach the parent or
		 * register the entity; `registerEntity` handles both.
		 *
		 * @return The newly constructed entity.
		 */
		virtual std::shared_ptr<EntityType> buildEntity(void) = 0;
	};

	/**
	 * @brief Reusable base for entity directors.
	 *
	 * Directors orchestrate a matching builder with a preset configuration.
	 * The base only provides a shared destructor so concrete directors do not
	 * repeat the trivial constructor/destructor pair.
	 */
	class EntityDirectorBase: public ecs::EntityDirector
	{
		public:
		/**
		 * @brief Construct a director.
		 */
		EntityDirectorBase(void);

		/**
		 * @brief Default destructor.
		 */
		~EntityDirectorBase(void) override = default;
	};

}	 // namespace guillaume::entities

#include "guillaume/entities/builder_base.tpp"
