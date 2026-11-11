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

#include <unordered_map>
#include <utility>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief What ComponentRegistry can do with a component storage without
	 * knowing which component type it holds.
	 *
	 * The registry keeps one ComponentStorage per component type behind this
	 * interface, which lets it remove an entity or reset the changed flags
	 * across all types in one loop.
	 * @see ComponentStorage
	 */
	class IComponentStorage
	{
		public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~IComponentStorage(void) = default;

		/**
		 * @brief Delete the component stored for an entity. Does nothing when
		 * the entity has none.
		 * @param entityIdentifier The identifier of the entity.
		 */
		virtual void remove(const Entity::Identifier &entityIdentifier) = 0;

		/**
		 * @brief Tell whether a component is stored for an entity.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when the entity has a component in this storage.
		 */
		virtual bool has(const Entity::Identifier &entityIdentifier) const = 0;

		/**
		 * @brief Tell whether the component stored for an entity is marked as
		 * changed.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when the component exists and is marked as changed;
		 * false when it is unchanged or the entity has none.
		 */
		virtual bool
			hasChanged(const Entity::Identifier &entityIdentifier) const = 0;

		/**
		 * @brief Mark every component of this storage as unchanged.
		 */
		virtual void resetChangedFlags(void) = 0;
	};

	/**
	 * @brief Holds every component of one type, indexed by entity identifier.
	 *
	 * ComponentRegistry creates one storage per component type the first
	 * time that type is used. Application code goes through the registry
	 * and does not use this class directly.
	 *
	 * @note Pointers and references to a stored component stay valid until
	 * that component is removed or replaced; adding components for other
	 * entities does not move it.
	 * @tparam ComponentType The component type stored.
	 * @see ComponentRegistry
	 */
	template<InheritFromComponent ComponentType> class ComponentStorage:
		public IComponentStorage
	{
		public:
		/**
		 * @brief The container: one component per entity identifier.
		 */
		using Storage = std::unordered_map<Entity::Identifier, ComponentType>;

		private:
		Storage _components;	///< The stored components, by entity identifier

		public:
		/**
		 * @brief Create an empty storage.
		 */
		ComponentStorage(void) = default;

		/**
		 * @brief Destroy the storage and every component it holds.
		 */
		~ComponentStorage(void) override = default;

		/**
		 * @brief Construct a component for an entity, replacing the one it
		 * already has, if any.
		 * @tparam Args The types of the constructor arguments.
		 * @param entityIdentifier The identifier of the entity.
		 * @param args Arguments forwarded to the constructor of
		 * `ComponentType`; none builds a default component.
		 * @return Reference to the stored component.
		 * @note When the entity already has a component, a new one is
		 * constructed and assigned over it: the previous values are lost.
		 */
		template<typename... Args> ComponentType &
			emplace(const Entity::Identifier &entityIdentifier, Args &&...args);

		/**
		 * @brief Look up the component of an entity.
		 * @param entityIdentifier The identifier of the entity.
		 * @return Pointer to the component, or nullptr when the entity has
		 * none.
		 */
		ComponentType *find(const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Look up the component of an entity, read-only.
		 * @param entityIdentifier The identifier of the entity.
		 * @return Pointer to the component, or nullptr when the entity has
		 * none.
		 */
		const ComponentType *
			find(const Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Delete the component stored for an entity. Does nothing when
		 * the entity has none.
		 * @param entityIdentifier The identifier of the entity.
		 */
		void remove(const Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Tell whether a component is stored for an entity.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when the entity has a component in this storage.
		 */
		bool has(const Entity::Identifier &entityIdentifier) const override;

		/**
		 * @brief Tell whether the component stored for an entity is marked as
		 * changed.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when the component exists and is marked as changed;
		 * false when it is unchanged or the entity has none.
		 */
		bool hasChanged(
			const Entity::Identifier &entityIdentifier) const override;

		/**
		 * @brief Mark every component of this storage as unchanged.
		 */
		void resetChangedFlags(void) override;
	};

}	 // namespace guillaume::ecs

// Include the implementation of the ComponentStorage template class.
#include "guillaume/ecs/component_storage.tpp"