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

#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs
{
	class EntityTreeTraveler;

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
		virtual std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) = 0;

		/**
		 * @brief Access direct child entities owned by this registry.
		 * @return Const reference to the direct child entities storage.
		 */
		virtual const std::vector<std::shared_ptr<Entity>> &
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
		void addEntity(std::shared_ptr<Entity> entity);

		/**
		 * @brief Get all registered entities.
		 * @return Const reference to registered entities.
		 */
		const std::vector<std::shared_ptr<Entity>> &getEntities(void) const
		{
			return accessDirectEntities();
		}

		/**
		 * @brief Collect all entities in this registry hierarchy using BFS.
		 * @return Entity pointers in breadth-first traversal order.
		 */
		std::vector<std::shared_ptr<Entity>> getEntitiesBreadthFirst(void);

		/**
		 * @brief Collect all entities in this registry hierarchy using BFS.
		 * @return Const entity pointers in breadth-first traversal order.
		 */
		std::vector<std::shared_ptr<Entity>>
			getEntitiesBreadthFirst(void) const;

		/**
		 * @brief Find an entity in this registry hierarchy by identifier.
		 * @tparam EntityType The expected entity type to cast to.
		 * @param identifier The entity identifier to search for.
		 * @return Shared pointer to the entity.
		 * @throws std::runtime_error if the entity is not found.
		 */
		template<InheritFromEntity EntityType>
		std::shared_ptr<EntityType> getEntity(Entity::Identifier identifier)
		{
			for (auto &entity: accessDirectEntities()) {
				if (entity->getIdentifier() == identifier) {
					auto castEntity =
						std::dynamic_pointer_cast<EntityType>(entity);
					if (castEntity != nullptr) {
						return castEntity;
					} else {
						throw std::runtime_error(
							"Entity found for identifier: "
							+ std::to_string(identifier)
							+ ", but it is not of the expected type.");
					}
				}
			}
			throw std::runtime_error("Entity not found for identifier: "
									 + std::to_string(identifier));
		}

		/**
		 * @brief Find an entity in this registry hierarchy by identifier
		 * (const).
		 * @tparam EntityType The expected entity type to cast to.
		 * @param identifier The entity identifier to search for.
		 * @return Const shared pointer to the entity.
		 * @throws std::runtime_error if the entity is not found.
		 */
		template<InheritFromEntity EntityType> std::shared_ptr<const EntityType>
			getEntity(Entity::Identifier identifier) const
		{
			for (const auto &entity: accessDirectEntities()) {
				if (entity->getIdentifier() == identifier) {
					auto castEntity =
						std::dynamic_pointer_cast<const EntityType>(entity);
					if (castEntity != nullptr) {
						return castEntity;
					} else {
						throw std::runtime_error(
							"Entity found for identifier: "
							+ std::to_string(identifier)
							+ ", but it is not of the expected type.");
					}
				}
			}
			throw std::runtime_error("Entity not found for identifier: "
									 + std::to_string(identifier));
		}

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

		/**
		 * @brief Get all registered entity identifiers that match the
		 * specified signature using the provided traversal strategy.
		 * @param systemSignature The signature to match.
		 * @param traveler Traversal strategy defining hierarchy walk order.
		 * @return A vector containing matching entity identifiers.
		 */
		std::vector<Entity::Identifier>
			getEntityWithSignature(Entity::Signature systemSignature,
								   const EntityTreeTraveler &traveler) const;
	};
}	 // namespace guillaume::ecs
