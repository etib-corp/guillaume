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

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <utility>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/demangle.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_storage.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Exception thrown when a requested component is not found for an
	 * entity.
	 * @tparam ComponentType The type of the missing component.
	 */
	template<InheritFromComponent ComponentType>
	class EntityComponentNotFoundException: public std::exception
	{
		private:
		const Entity::Identifier
			_entityIdentifier;	  ///< Identifier of the entity

		std::string _componentTypeName {
			utility::demangle<ComponentType>()
		};	  ///< Name of the missing component
			  ///< type
		mutable std::string _message;	 ///< Cached exception message

		public:
		/**
		 * @brief Construct a new Component Not Found Exception object.
		 * @param entityIdentifier The identifier of the entity.
		 */
		EntityComponentNotFoundException(
			const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the exception message.
		 * @return The exception message.
		 */
		const char *what(void) const noexcept override;
	};

	/**
	 * @brief Registry for component types.
	 *
	 * Manages the registration and retrieval of component types within the ECS
	 * architecture.
	 * @see ComponentStorage
	 */
	class ComponentRegistry:
		public utility::logging::Loggable<ComponentRegistry,
										  utility::logging::DefaultLogger>
	{
		private:
		std::map<std::type_index, std::unique_ptr<IComponentStorage>>
			_storages;	  ///< Registered component storages

		/**
		 * @brief Get or create a storage for a component type.
		 * @tparam ComponentType The type of component to store.
		 * @return Reference to the storage.
		 */
		template<InheritFromComponent ComponentType>
		ComponentStorage<ComponentType> &getOrCreateStorage(void);

		/**
		 * @brief Register a component for an entity.
		 * @tparam ComponentType The type of the component to register.
		 * @param entityIdentifier The entity identifier to which the
		 * component will be associated.
		 */
		template<InheritFromComponent ComponentType>
		void registerComponent(const Entity::Identifier &entityIdentifier);

		protected:
		/**
		 * @brief Register a new component type in the registry.
		 * @tparam ComponentType The type of the component to register.
		 */
		template<InheritFromComponent ComponentType>
		void registerNewComponentType(void);

		public:
		/**
		 * @brief Default constructor.
		 */
		ComponentRegistry(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~ComponentRegistry(void) = default;

		/**
		 * @brief Register a component for an entity.
		 * @tparam NeededComponentTypes The types of the components to register.
		 * @param entityIdentifier The entity identifier to which the
		 * component will be associated.
		 */
		template<InheritFromComponent... NeededComponentTypes>
		void registerComponentsForEntity(
			const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Add or replace a component for an entity.
		 * @tparam ComponentType The type of the component to add.
		 * @param entityIdentifier The entity identifier.
		 * @param args Arguments forwarded to the component constructor.
		 * @return Reference to the stored component.
		 * @note If a component already exists for the entity, it is replaced.
		 */
		template<InheritFromComponent ComponentType, typename... Args>
		ComponentType &addComponent(const Entity::Identifier &entityIdentifier,
									Args &&...args);

		/**
		 * @brief Check whether an entity has a component.
		 * @tparam ComponentType The component type to check.
		 * @param entityIdentifier The entity identifier.
		 * @return True if the component is present.
		 */
		template<InheritFromComponent ComponentType>
		bool hasComponent(const Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Remove a component from an entity.
		 * @tparam ComponentType The component type to remove.
		 * @param entityIdentifier The entity identifier.
		 */
		template<InheritFromComponent ComponentType>
		void removeComponent(const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Check whether any component stored for an entity has changed.
		 * @param entityIdentifier The entity identifier.
		 * @return True if at least one stored component is marked as changed.
		 */
		bool hasChanged(const Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Clear the changed flags for all stored components.
		 */
		void resetChangedFlags(void);

		/**
		 * @brief Get a component for an entity.
		 * @tparam ComponentType The type of the component to retrieve.
		 * @param entityIdentifier The entity identifier to which the
		 * component belongs.
		 * @return Reference to the requested component.
		 * @throws EntityComponentNotFoundException<ComponentType> If the entity
		 * does not have the component.
		 */
		template<InheritFromComponent ComponentType>
		ComponentType &getComponent(const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get a component for an entity (const).
		 * @tparam ComponentType The type of the component to retrieve.
		 * @param entityIdentifier The entity identifier to which the
		 * component belongs.
		 * @return Reference to the requested component.
		 * @throws EntityComponentNotFoundException<ComponentType> If the entity
		 * does not have the component.
		 */
		template<InheritFromComponent ComponentType> const ComponentType &
			getComponent(const Entity::Identifier &entityIdentifier) const;
	};

	/**
	 * @brief Concept to ensure a type inherits from ComponentRegistry.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromComponentRegistry =
		std::is_base_of_v<ComponentRegistry, Type>;

}	 // namespace guillaume::ecs

// Iclude the implementation of the ComponentRegistry template class.
#include "guillaume/ecs/component_registry.tpp"