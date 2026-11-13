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
#include "guillaume/ecs/parent_entity.hpp"

namespace guillaume::ecs
{
	/**
	 * @brief The base class of a widget with children: an EntityFiller that
	 * is also a ParentEntity.
	 *
	 * Like EntityFiller, the constructor sets the signature, creates one
	 * default component of each listed type and attaches a
	 * components::Hierarchy. In addition the entity owns child entities
	 * (EntityRegistry::addEntity) and the systems reach them through it.
	 *
	 * @code
	 * class Row:
	 * 	public ecs::ParentEntityFiller<components::Transform,
	 * 								   components::Bound, components::Layout>,
	 * 	public std::enable_shared_from_this<Row>
	 * {
	 * 	public:
	 * 	using ParentEntityFiller::ParentEntityFiller;
	 *
	 * 	void add(std::shared_ptr<ecs::Entity> child)
	 * 	{
	 * 		child->setParent(shared_from_this());
	 * 		addEntity(child);
	 * 	}
	 * };
	 * @endcode
	 *
	 * @tparam ComponentTypes The component types the entity starts with.
	 * @see EntityFiller
	 * @see ParentEntity
	 */
	template<InheritFromComponent... ComponentTypes> class ParentEntityFiller:
		public ParentEntity
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
		 * components::Hierarchy. It owns no child yet.
		 * @param componentRegistry The registry in which the components are
		 * created, normally the scene's. It must outlive the entity.
		 * @throws ComponentTypeLimitExceededException When more than
		 * MaxComponentTypes component types are in use.
		 */
		ParentEntityFiller(ComponentRegistry &componentRegistry);

		/**
		 * @brief Destroy the entity object and release the children it
		 * owns; components stay in the registry (see
		 * EntityRegistry::removeEntity).
		 */
		virtual ~ParentEntityFiller(void) = default;
	};

}	 // namespace guillaume::ecs

// Include the implementation of the EntityFiller and ParentEntityFiller
// template classes
#include "guillaume/ecs/parent_entity_filler.tpp"