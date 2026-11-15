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

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_type_id.hpp"

namespace guillaume::ecs
{
	class ComponentRegistry;

	/**
	 * @brief One thing in the UI: an identifier, a signature and a place in
	 * the tree. It holds no state of its own.
	 *
	 * - The **identifier** is the key under which the entity's components
	 *   are stored in a ComponentRegistry. It is unique in the program and
	 *   never reused.
	 * - The **signature** is a bitmask with one bit per component type the
	 *   entity declares. A system visits an entity when the signature
	 *   contains every bit of the system's own signature.
	 * - The **place in the tree** is the parent given to setParent. It is
	 *   mirrored into the entity's components::Hierarchy (parent identifier,
	 *   child identifiers, layer), which is what systems read to walk the
	 *   tree from identifiers alone.
	 *
	 * Widgets (entities::Button, ...) derive from EntityFiller or
	 * ParentEntityFiller, which create the components, set the signature and
	 * attach the Hierarchy. A bare Entity has none of that.
	 *
	 * Two different links make up the tree, and a child needs both:
	 * @code
	 * child->setParent(parent);	   // parent link: layout, depth, Hierarchy
	 * parent->addEntity(child);	   // ownership: lifetime, visited by
	 * systems
	 * @endcode
	 * setParent alone does not keep the child alive (the entity only holds a
	 * weak reference to its parent, and the parent none to the child), and
	 * systems only visit entities owned, directly or not, by the scene's
	 * EntityRegistry. The owner is not always the parent: the widget
	 * Builders register what they create in the scene itself.
	 *
	 * @note Entities are handled through `std::shared_ptr`.
	 * @see EntityFiller
	 * @see ComponentRegistry
	 * @see EntityRegistry
	 */
	class Entity:
		protected utility::logging::Loggable<Entity,
											 utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief The key under which an entity's components are stored.
		 * Systems and components refer to entities by this value.
		 */
		using Identifier = std::size_t;
		/**
		 * @brief A set of component types, one bit per type (bit position
		 * given by ComponentTypeId, MaxComponentTypes bits).
		 */
		using Signature = std::bitset<128>;

		constexpr static Identifier InvalidIdentifier =
			0;	  ///< "No entity": never the identifier of a real entity

		private:
		/**
		 * @brief Hand out the identifier of a new entity.
		 * @return A value never returned before, starting at 1. Safe to call
		 * from several threads.
		 */
		static Identifier getNextIdentifier(void);

		public:
		/**
		 * @brief Build the signature that has exactly the bits of the given
		 * component types.
		 *
		 * @code
		 * entity->setSignature(entity->getSignature()
		 * 	| Entity::getSignatureFromTypes<components::Focus>());
		 * @endcode
		 * @tparam ComponentTypes The component types to include.
		 * @return The signature; empty when no type is given.
		 * @throws ComponentTypeLimitExceededException When more than
		 * MaxComponentTypes component types are in use.
		 */
		template<InheritFromComponent... ComponentTypes>
		static Signature getSignatureFromTypes(void);

		private:
		const Identifier _identifier;	 ///< Unique identifier, set once
		Signature _signature;	 ///< Component types the entity declares
		std::int32_t _layer {
			0
		};	  ///< Depth in the tree, used when there is no Hierarchy
		std::weak_ptr<Entity> _parent;	  ///< Parent entity; expired if none
		ComponentRegistry *_hierarchyRegistry {
			nullptr
		};	  ///< Registry holding this entity's Hierarchy; nullptr if none

		protected:
		/**
		 * @brief Give the entity a components::Hierarchy in a registry and
		 * add that component to its signature.
		 *
		 * From then on setParent mirrors the parent, the children and the
		 * layer into that component, and getLayer reads it. An existing
		 * Hierarchy of the entity is kept. Called by EntityFiller and
		 * ParentEntityFiller.
		 * @param componentRegistry The registry that stores the component.
		 * The entity keeps a pointer to it, so it must stay alive as long as
		 * the entity is used.
		 */
		void attachHierarchy(ComponentRegistry &componentRegistry);

		/**
		 * @brief Replace the signature by the one made of the given
		 * component types.
		 *
		 * Bits set before the call are lost, including the one of
		 * components::Hierarchy.
		 * @tparam ComponentTypes The component types the entity declares.
		 */
		template<InheritFromComponent... ComponentTypes>
		void setSignature(void);

		public:
		/**
		 * @brief Create an entity with a fresh identifier, an empty
		 * signature, no parent and no component.
		 */
		Entity(void);

		/**
		 * @brief Destroy the entity object.
		 *
		 * Its components stay in the registry. Remove an entity with
		 * EntityRegistry::removeEntity to delete them too.
		 */
		virtual ~Entity(void) = default;

		/**
		 * @brief Get the identifier of the entity.
		 * @return The identifier, never InvalidIdentifier.
		 */
		Entity::Identifier getIdentifier(void) const noexcept;

		/**
		 * @brief Get the set of component types the entity declares.
		 * @return A copy of the signature.
		 */
		Entity::Signature getSignature(void) const noexcept;

		/**
		 * @brief Replace the signature of the entity.
		 *
		 * The signature is not derived from the registry: after adding a
		 * component with ComponentRegistry::addComponent, add its bit here,
		 * otherwise the systems that need that component skip the entity.
		 * @param signature The new signature. Start from getSignature() to
		 * keep the bits already set.
		 */
		void setSignature(const Signature &signature);

		/**
		 * @brief Get the depth of the entity in the tree.
		 *
		 * Read from the entity's components::Hierarchy when it has one.
		 * systems::Layout uses the layer to offset an entity along the
		 * forward axis of its parent, so a child sits in front of it.
		 * @return 0 for an entity without parent, the parent's layer plus one
		 * otherwise.
		 */
		std::int32_t getLayer(void) const noexcept;

		/**
		 * @brief Attach the entity under another one, or detach it.
		 *
		 * The layer becomes the parent's layer plus one (0 when detached).
		 * When the entity has a components::Hierarchy, the tree is mirrored
		 * into the components: the entity leaves the child list of its
		 * previous parent, joins the one of the new parent, stores the
		 * parent's identifier, and the layer of every descendant is
		 * recomputed. Those writes mark the Hierarchy components changed
		 * when a value differs.
		 *
		 * The parent does not take ownership: also register the entity with
		 * EntityRegistry::addEntity (usually on the parent) so that it stays
		 * alive and systems visit it.
		 * @param parent The new parent, or nullptr to make the entity a
		 * root. Only a weak reference is kept.
		 * @return Reference to this entity, for chaining.
		 * @throws std::invalid_argument When the parent is the entity itself
		 * or, for an entity with a Hierarchy, one of its descendants. The
		 * tree is left as it was.
		 */
		Entity &setParent(std::shared_ptr<Entity> parent);

		/**
		 * @brief Drop the entity's place in the tree and all its components.
		 *
		 * Removes the entity from its parent's components::Hierarchy, resets
		 * the parent identifier of the entities parented to it to
		 * InvalidIdentifier, and deletes every component stored for it in
		 * the registry given to attachHierarchy. Called by
		 * EntityRegistry::removeEntity; the entity must not be used
		 * afterwards. An entity without Hierarchy only forgets its parent.
		 * @return The identifiers of the entities that were parented to it;
		 * empty when it had no child or no Hierarchy.
		 */
		std::vector<Identifier> releaseComponents(void);

		/**
		 * @brief Get the parent entity.
		 * @return The parent, or nullptr when the entity has none or when the
		 * parent object was destroyed.
		 */
		std::shared_ptr<Entity> getParent(void) const noexcept;
	};

	/**
	 * @brief Constrains a template parameter to a type derived from Entity
	 * (or Entity itself).
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEntity = std::is_base_of_v<Entity, Type>;

}	 // namespace guillaume::ecs

// Include the implementation of the Entity template methods
#include "guillaume/ecs/entity.tpp"