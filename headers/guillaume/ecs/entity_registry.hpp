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
	 * @brief Owns entities and gives the systems the list of entities to
	 * visit.
	 *
	 * Two kinds of object are an EntityRegistry: a Scene, which owns its root
	 * entities (through EntityRegistryContainer), and a ParentEntity, which
	 * owns its children. Because an owned entity can itself be a registry,
	 * registries nest and form the **ownership tree** of a scene. Each frame,
	 * System::routine asks a traveler for every entity of that tree, starting
	 * at the scene.
	 *
	 * @code
	 * // Add a child under a parent widget (a ParentEntity).
	 * child->setParent(parent);	   // parent link and components::Hierarchy
	 * parent->addEntity(child);	   // ownership
	 *
	 * // Remove it later, from any registry above it, with its own children
	 * // and all their components.
	 * scene.getEntityRegistry().removeEntity(child->getIdentifier());
	 * @endcode
	 *
	 * Things to watch out for:
	 * - Ownership (addEntity) and the parent link (Entity::setParent) are
	 *   set separately and can differ. A widget that builds its own
	 *   children owns them; an entity created through a widget Builder is
	 *   owned by the scene, whatever parent it was given. This is why
	 *   removeEntity also follows the parent links.
	 * - getEntities and getEntity look at the entities this registry owns
	 *   directly; getEntitiesBreadthFirst, getEntityWithSignature,
	 *   removeEntity and clearEntities go through the nested registries too.
	 * - Removing entities while iterating over getEntities() invalidates
	 *   the iteration.
	 *
	 * @see Entity::setParent
	 * @see ParentEntity
	 * @see EntityTreeTraveler
	 */
	class EntityRegistry
	{
		private:
		/**
		 * @brief Result of the last breadth-first walk of this registry and
		 * the registries nested in it.
		 *
		 * Reused as long as no entity was added to or removed from any
		 * registry of the program; rebuilt on the next walk otherwise.
		 */
		mutable std::vector<std::shared_ptr<Entity>> _bfsCache;

		/**
		 * @brief Value of the program-wide tree version when _bfsCache was
		 * computed. The version is incremented by every addition and removal.
		 */
		mutable std::size_t _cachedVersion { 0 };

		/**
		 * @brief Remove one entity, searched in this registry and the nested
		 * ones, together with everything it owns.
		 *
		 * Entities that are only parented to a removed entity (not owned by
		 * it) are not removed here: their identifiers are reported instead.
		 * @param identifier The identifier of the entity to remove.
		 * @param orphans Receives the identifiers of the entities that were
		 * parented to the removed entities. Identifiers are appended; some
		 * may belong to entities already removed.
		 * @return True when the entity was found and removed.
		 */
		bool removeOne(Entity::Identifier identifier,
					   std::vector<Entity::Identifier> &orphans);

		/**
		 * @brief Remove every entity this registry owns, and everything
		 * they own, releasing their components.
		 * @param orphans Receives the identifiers of the entities that were
		 * parented to the removed entities.
		 */
		void clearOwned(std::vector<Entity::Identifier> &orphans);

		protected:
		/**
		 * @brief Give access to the list of entities this registry owns
		 * directly. Implemented by the class that provides the storage.
		 * @return Mutable reference to the list, in insertion order.
		 */
		virtual std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) = 0;

		/**
		 * @brief Give read access to the list of entities this registry owns
		 * directly.
		 * @return Const reference to the list, in insertion order.
		 */
		virtual const std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) const = 0;

		public:
		/**
		 * @brief Create a registry.
		 */
		EntityRegistry(void) = default;

		/**
		 * @brief Virtual destructor.
		 *
		 * The entity objects are released; their components are not deleted
		 * from the ComponentRegistry. Call clearEntities() first for that.
		 */
		virtual ~EntityRegistry(void) = default;

		/**
		 * @brief Make this registry own an entity, so that it stays alive
		 * and systems visit it.
		 *
		 * The entity is appended after the ones already owned. Its parent
		 * link is not touched: call Entity::setParent as well when the
		 * entity is a child. Adding the same entity twice makes systems
		 * visit it twice.
		 * @param entity The entity to own. Must not be null.
		 */
		void addEntity(std::shared_ptr<Entity> entity);

		/**
		 * @brief Remove an entity, with its children and all their
		 * components.
		 *
		 * The entity is searched in this registry and in the registries
		 * nested in it. Everything the removed entity owns is removed with
		 * it, and so is every entity parented to it (recursively) that can
		 * be found from this registry; one registered out of reach is only
		 * unlinked (its parent identifier becomes
		 * Entity::InvalidIdentifier). Components are deleted and the entity
		 * leaves the child list of its parent's components::Hierarchy. Render
		 * systems drop its graphics on the next frame.
		 *
		 * `std::shared_ptr`s to a removed entity held elsewhere stay valid
		 * as objects, but the entity has no component left and must not be
		 * used.
		 * @param identifier The identifier of the entity to remove.
		 * @return True when the entity was found and removed; false when it
		 * is not below this registry or the identifier is
		 * Entity::InvalidIdentifier.
		 */
		bool removeEntity(Entity::Identifier identifier);

		/**
		 * @brief Remove every entity owned by this registry, with everything
		 * they own and all their components.
		 *
		 * A widget that rebuilds its children calls it before creating the
		 * new ones. Entities parented to a removed entity but owned
		 * elsewhere are unlinked, not removed.
		 */
		void clearEntities(void);

		/**
		 * @brief Get the entities this registry owns directly.
		 * @return Const reference to the list, in insertion order; the
		 * entities owned by nested registries are not included.
		 */
		const std::vector<std::shared_ptr<Entity>> &getEntities(void) const
		{
			return accessDirectEntities();
		}

		/**
		 * @brief List every entity of this registry and of the registries
		 * nested in it, level by level.
		 *
		 * The entities owned directly come first, in insertion order, then
		 * the entities owned by those, and so on: an owner always comes
		 * before what it owns. The result is cached until an entity is added
		 * or removed anywhere.
		 * @return A copy of the list; empty when the registry owns nothing.
		 */
		std::vector<std::shared_ptr<Entity>> getEntitiesBreadthFirst(void);

		/**
		 * @brief List every entity of this registry and of the registries
		 * nested in it, level by level (const overload).
		 * @return A copy of the list, in the same order as the non-const
		 * overload; empty when the registry owns nothing.
		 */
		std::vector<std::shared_ptr<Entity>>
			getEntitiesBreadthFirst(void) const;

		/**
		 * @brief Find an entity owned directly by this registry, by
		 * identifier.
		 *
		 * Nested registries are not searched.
		 * @tparam EntityType The type the entity is expected to have, for
		 * instance `entities::Button`, or `ecs::Entity` to accept any.
		 * @param identifier The identifier of the entity to find.
		 * @return The entity, or nullptr when this registry does not own an
		 * entity with that identifier or when the entity is not an
		 * `EntityType`.
		 */
		template<InheritFromEntity EntityType>
		std::shared_ptr<EntityType> getEntity(Entity::Identifier identifier)
		{
			if (identifier == Entity::InvalidIdentifier) {
				return nullptr;
			}
			for (auto &entity: accessDirectEntities()) {
				if (entity->getIdentifier() == identifier) {
					return std::dynamic_pointer_cast<EntityType>(entity);
				}
			}
			return nullptr;
		}

		/**
		 * @brief Find an entity owned directly by this registry, by
		 * identifier (const overload).
		 *
		 * Nested registries are not searched.
		 * @tparam EntityType The type the entity is expected to have.
		 * @param identifier The identifier of the entity to find.
		 * @return The entity, or nullptr when this registry does not own an
		 * entity with that identifier or when the entity is not an
		 * `EntityType`.
		 */
		template<InheritFromEntity EntityType> std::shared_ptr<const EntityType>
			getEntity(Entity::Identifier identifier) const
		{
			if (identifier == Entity::InvalidIdentifier) {
				return nullptr;
			}
			for (const auto &entity: accessDirectEntities()) {
				if (entity->getIdentifier() == identifier) {
					return std::dynamic_pointer_cast<const EntityType>(entity);
				}
			}
			return nullptr;
		}

		/**
		 * @brief List the entities that have every component type of a
		 * signature, from the root down.
		 *
		 * Same as the other overload with a LevelOrderTraveler.
		 * @param systemSignature The component types an entity must declare
		 * to be listed. An empty signature matches every entity.
		 * @return The identifiers of the matching entities of this registry
		 * and the nested ones, owners before what they own; empty when none
		 * matches.
		 */
		std::vector<Entity::Identifier>
			getEntityWithSignature(Entity::Signature systemSignature) const;

		/**
		 * @brief List the entities that have every component type of a
		 * signature, in the order chosen by a traveler.
		 *
		 * An entity matches when its own signature (Entity::getSignature)
		 * contains every bit of `systemSignature`; extra bits do not matter.
		 * This is the test System::routine applies.
		 * @param systemSignature The component types an entity must declare
		 * to be listed.
		 * @param traveler Decides the order in which the tree is walked.
		 * @return The identifiers of the matching entities, in traveler
		 * order; empty when none matches.
		 */
		std::vector<Entity::Identifier>
			getEntityWithSignature(Entity::Signature systemSignature,
								   const EntityTreeTraveler &traveler) const;
	};
}	 // namespace guillaume::ecs
