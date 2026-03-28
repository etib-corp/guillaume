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

#include "guillaume/ecs/entity_builder.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base builder for entities that can be attached to a parent entity.
	 */
	class LeafEntityBuilder: public EntityBuilder
	{
		private:
		Entity::Identifier _parentIdentifier {
			Entity::InvalidIdentifier
		};	  ///< Parent entity identifier

		public:
		/**
		 * @brief Constructor for the LeafEntityBuilder class.
		 * @param componentRegistry The component registry used to build
		 * entities.
		 * @param entityRegistry The entity registry used to build entities.
		 */
		LeafEntityBuilder(ecs::ComponentRegistry &componentRegistry,
						  ecs::EntityRegistry &entityRegistry)
			: EntityBuilder(componentRegistry, entityRegistry)
		{
		}

		/**
		 * @brief Default destructor for the LeafEntityBuilder class.
		 */
		virtual ~LeafEntityBuilder(void) = default;

		/**
		 * @brief Build and register the entity in the entity registry.
		 */
		virtual void registerEntity(void) = 0;

		/**
		 * @brief Reset the builder to its initial state for creating a new
		 * entity.
		 */
		void reset(void)
		{
			_parentIdentifier = Entity::InvalidIdentifier;
		}

		/**
		 * @brief Set the parent entity identifier for the entity being built.
		 * @param parentId The identifier of the parent entity.
		 */
		void withParent(const Entity::Identifier &parentId)
		{
			_parentIdentifier = parentId;
		}
	};

}	 // namespace guillaume::ecs