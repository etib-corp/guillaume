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

#include <vector>

#include "guillaume/ecs/leaf_entity_builder.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base builder for entities that can have a parent and child
	 * relationships.
	 */
	class NodeEntityBuilder: public LeafEntityBuilder
	{
		private:
		std::vector<Entity::Identifier>
			_childIdentifiers;	  ///< Child entity identifiers for the node
								  ///< being built

		public:
		/**
		 * @brief Constructor for the NodeEntityBuilder class.
		 * @param componentRegistry The component registry used to build
		 * entities.
		 * @param entityRegistry The entity registry used to build entities.
		 */
		NodeEntityBuilder(ecs::ComponentRegistry &componentRegistry,
						  ecs::EntityRegistry &entityRegistry)
			: LeafEntityBuilder(componentRegistry, entityRegistry)
		{
		}

		/**
		 * @brief Default destructor for the NodeEntityBuilder class.
		 */
		virtual ~NodeEntityBuilder(void) = default;

		/**
		 * @brief Build and register the entity in the entity registry.
		 * @return The entity identifier of the newly created entity.
		 */
		virtual ecs::Entity::Identifier registerEntity(void) = 0;

		/**
		 * @brief Reset the builder to its initial state for creating a new
		 * entity.
		 */
		void reset(void) override
		{
			LeafEntityBuilder::reset();
			_childIdentifiers.clear();
		}

		/**
		 * @brief Add a child entity identifier to the node being built.
		 * @param childId The identifier of the child entity to add.
		 * @return Reference to the builder for chaining.
		 */
		NodeEntityBuilder &withChild(const Entity::Identifier &childId)
		{
			_childIdentifiers.push_back(childId);
			return *this;
		}
	};

}	 // namespace guillaume::ecs