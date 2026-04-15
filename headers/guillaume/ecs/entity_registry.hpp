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
#include <queue>
#include <vector>

#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Abstract base class for containers that own ECS entities.
	 *
	 * This contract unifies entity ownership for scene-level registries and
	 * hierarchy-capable entities. Traversal helpers process the hierarchy in
	 * breadth-first order.
	 */
	class EntityRegistry
	{
		protected:
		/**
		 * @brief Access mutable direct child entities owned by this registry.
		 * @return Mutable reference to the direct child entities storage.
		 */
		virtual std::vector<std::unique_ptr<Entity>> &
			accessDirectEntities(void) = 0;

		/**
		 * @brief Access direct child entities owned by this registry.
		 * @return Const reference to the direct child entities storage.
		 */
		virtual const std::vector<std::unique_ptr<Entity>> &
			accessDirectEntities(void) const = 0;

		public:
		/**
		 * @brief Default constructor.
		 */
		EntityRegistry(void) = default;

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~EntityRegistry(void) = default;

		/**
		 * @brief Register an entity in the registry.
		 * @param entity The entity to register.
		 */
		void addEntity(std::unique_ptr<Entity> entity);

		/**
		 * @brief Get all registered entities.
		 * @return Const reference to registered entities.
		 */
		const std::vector<std::unique_ptr<Entity>> &getEntities(void) const
		{
			return accessDirectEntities();
		}

		/**
		 * @brief Collect all entities in this registry hierarchy using BFS.
		 * @return Entity pointers in breadth-first traversal order.
		 */
		std::vector<Entity *> getEntitiesBreadthFirst(void);

		/**
		 * @brief Collect all entities in this registry hierarchy using BFS.
		 * @return Const entity pointers in breadth-first traversal order.
		 */
		std::vector<const Entity *> getEntitiesBreadthFirst(void) const;

		/**
		 * @brief Get all registered entity identifier that match the specified
		 * signature.
		 * @param systemSignature The signature to match against registered
		 * entities.
		 * @return A vector containing matching entity identifiers.
		 * @note This is used for systems to query entities that match their
		 * required component signature.
		 */
		std::vector<Entity::Identifier>
			getEntityWithSignature(Entity::Signature systemSignature) const;
	};
}	 // namespace guillaume::ecs
