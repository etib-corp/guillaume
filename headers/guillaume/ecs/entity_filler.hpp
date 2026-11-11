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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief The base class of a widget without children: an Entity that
	 * creates its components when it is constructed.
	 *
	 * The constructor does three things for the new entity:
	 * 1. sets its signature to the listed component types, so the systems
	 *    that need them visit it;
	 * 2. creates one default component of each listed type in the registry;
	 * 3. gives it a components::Hierarchy (added to the signature too), so
	 *    Entity::setParent keeps the tree readable from components.
	 *
	 * @code
	 * class Label:
	 * 	public ecs::EntityFiller<components::Transform, components::Bound,
	 * 							 components::Text>
	 * {
	 * 	public:
	 * 	Label(ecs::ComponentRegistry &registry)
	 * 		: ecs::EntityFiller<components::Transform, components::Bound,
	 * 							components::Text>(registry)
	 * 	{
	 * 	}
	 * };
	 *
	 * auto label = std::make_shared<Label>(scene.getComponentRegistry());
	 * scene.getEntityRegistry().addEntity(label);
	 * @endcode
	 *
	 * Things to watch out for:
	 * - A component added afterwards with ComponentRegistry::addComponent is
	 *   not in the signature; add its bit with Entity::setSignature.
	 * - The entity keeps a reference to the registry, which must outlive it.
	 * - A widget that owns children derives from ParentEntityFiller instead.
	 *
	 * @tparam ComponentTypes The component types the entity starts with.
	 * @see Entity
	 * @see ParentEntityFiller
	 * @see ComponentRegistry
	 */
	template<InheritFromComponent... ComponentTypes> class EntityFiller:
		public Entity
	{
		private:
		ComponentRegistry &_componentRegistry;	  ///< Registry storing the
												  ///< entity's components

		protected:
		/**
		 * @brief Get the registry that stores this entity's components.
		 * @return Reference to the registry given to the constructor.
		 */
		ComponentRegistry &getComponentRegistry(void);

		/**
		 * @brief Get the registry that stores this entity's components,
		 * read-only.
		 * @return Const reference to the registry given to the constructor.
		 */
		const ComponentRegistry &getComponentRegistry(void) const;

		public:
		/**
		 * @brief Create the entity, its listed components and its
		 * components::Hierarchy.
		 * @param componentRegistry The registry in which the components are
		 * created, normally the scene's. It must outlive the entity.
		 * @throws ComponentTypeLimitExceededException When more than
		 * MaxComponentTypes component types are in use.
		 */
		EntityFiller(ComponentRegistry &componentRegistry);

		/**
		 * @brief Destroy the entity object; its components stay in the
		 * registry (see EntityRegistry::removeEntity).
		 */
		virtual ~EntityFiller(void) = default;
	};

}	 // namespace guillaume::ecs

// Include the implementation of the EntityFiller
// template classes
#include "guillaume/ecs/entity_filler.tpp"