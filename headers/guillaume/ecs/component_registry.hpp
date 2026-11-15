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
	 * @brief Thrown by ComponentRegistry::getComponent when the entity has no
	 * component of the requested type.
	 *
	 * The message names the component type and the entity identifier. Call
	 * ComponentRegistry::hasComponent first when the component is optional.
	 * @tparam ComponentType The type of the missing component.
	 */
	template<InheritFromComponent ComponentType>
	class EntityComponentNotFoundException: public std::exception
	{
		private:
		const Entity::Identifier
			_entityIdentifier;	  ///< Identifier of the entity that was queried

		std::string _componentTypeName {
			utility::demangle<ComponentType>()
		};	  ///< Readable name of the missing component
			  ///< type
		mutable std::string _message;	 ///< Text returned by what()

		public:
		/**
		 * @brief Build the exception and its message.
		 * @param entityIdentifier The identifier of the entity that lacks
		 * the component.
		 */
		EntityComponentNotFoundException(
			const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the exception message.
		 * @return A null-terminated string owned by the exception, of the
		 * form "Component of type X not found for entity N".
		 */
		const char *what(void) const noexcept override;
	};

	/**
	 * @brief Stores every component of a scene, per component type and per
	 * entity identifier, and knows which ones changed this frame.
	 *
	 * This is where all the state of the UI lives. Each Scene owns one
	 * registry. Widgets write to it when they are built and when their
	 * setters are called; systems read and write it every frame through
	 * System::getComponent. Everything is addressed by entity identifier, so
	 * no entity object is needed to use it.
	 *
	 * Storage for a component type is created the first time that type is
	 * used: there is no list of component types to maintain.
	 *
	 * @code
	 * ecs::ComponentRegistry registry;
	 * const ecs::Entity::Identifier id = entity->getIdentifier();
	 *
	 * registry.addComponent<components::Bound>(id);
	 * if (registry.hasComponent<components::Bound>(id)) {
	 * 	registry.getComponent<components::Bound>(id).setWidth(120.0f);
	 * }
	 * registry.hasChanged(id);	   // true: Bound was modified
	 * registry.resetChangedFlags();
	 * registry.hasChanged(id);	   // false until the next modification
	 * @endcode
	 *
	 * **The changed flags.** Every component carries a flag
	 * (Component::hasChanged) that its setters raise when a value really
	 * changes. hasChanged(identifier) tells whether any component of an
	 * entity has its flag raised; resetChangedFlags() lowers all of them and
	 * is called by Application::routine once the four phases have run. So a
	 * flag raised during a frame, or between two frames by application code,
	 * stays visible until the end of the frame in progress. A frame in which
	 * no flag is raised means the UI has settled. The flags are information
	 * only: System::routine visits every matching entity every frame,
	 * whether it changed or not.
	 *
	 * Things to watch out for:
	 * - Adding a component here does not update the signature of the entity
	 *   (Entity::setSignature). A system only visits an entity whose
	 *   signature contains the system's components.
	 * - addComponent on an entity that already has the component replaces
	 *   it with a newly constructed one.
	 * - A reference returned by getComponent or addComponent stays valid
	 *   until that component is removed or replaced.
	 *
	 * @see Component
	 * @see Entity
	 * @see System
	 */
	class ComponentRegistry:
		public utility::logging::Loggable<ComponentRegistry,
										  utility::logging::DefaultLogger>
	{
		private:
		std::map<std::type_index, std::unique_ptr<IComponentStorage>>
			_storages;	  ///< One storage per component type used so far

		/**
		 * @brief Get the storage of a component type, creating it empty on
		 * first use.
		 * @tparam ComponentType The component type the storage holds.
		 * @return Reference to the storage of that type.
		 */
		template<InheritFromComponent ComponentType>
		ComponentStorage<ComponentType> &getOrCreateStorage(void);

		/**
		 * @brief Give an entity a default-constructed component, replacing
		 * the one it already has, if any.
		 * @tparam ComponentType The component type to create.
		 * @param entityIdentifier The identifier of the entity that receives
		 * the component.
		 */
		template<InheritFromComponent ComponentType>
		void registerComponent(const Entity::Identifier &entityIdentifier);

		protected:
		/**
		 * @brief Create the (empty) storage of a component type ahead of its
		 * first use.
		 *
		 * Optional: storages are created on demand anyway.
		 * ComponentRegistryFiller uses it to prepare a fixed list of types.
		 * @tparam ComponentType The component type to prepare storage for.
		 */
		template<InheritFromComponent ComponentType>
		void registerNewComponentType(void);

		public:
		/**
		 * @brief Create a registry that holds no component.
		 */
		ComponentRegistry(void) = default;

		/**
		 * @brief Destroy the registry and every component it stores.
		 */
		~ComponentRegistry(void) = default;

		/**
		 * @brief Give an entity one default-constructed component of each
		 * listed type.
		 *
		 * EntityFiller calls it from its constructor with the component list
		 * of the widget. A listed component the entity already has is
		 * replaced by a default one.
		 * @tparam NeededComponentTypes The component types to create.
		 * @param entityIdentifier The identifier of the entity that receives
		 * the components.
		 */
		template<InheritFromComponent... NeededComponentTypes>
		void registerComponentsForEntity(
			const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Construct a component for an entity, replacing the one it
		 * already has, if any.
		 *
		 * The new component is not marked as changed, and the signature of
		 * the entity is left as it is.
		 * @tparam ComponentType The component type to add.
		 * @tparam Args The types of the constructor arguments.
		 * @param entityIdentifier The identifier of the entity that receives
		 * the component.
		 * @param args Arguments forwarded to the constructor of
		 * `ComponentType`; none builds a default component.
		 * @return Reference to the stored component, so setters can be
		 * chained on it.
		 */
		template<InheritFromComponent ComponentType, typename... Args>
		ComponentType &addComponent(const Entity::Identifier &entityIdentifier,
									Args &&...args);

		/**
		 * @brief Tell whether an entity has a component of a given type.
		 * @tparam ComponentType The component type to look for.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when the component exists. False for an unknown
		 * identifier, including Entity::InvalidIdentifier.
		 */
		template<InheritFromComponent ComponentType>
		bool hasComponent(const Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Delete one component of an entity. Does nothing when the
		 * entity does not have it.
		 * @tparam ComponentType The component type to remove.
		 * @param entityIdentifier The identifier of the entity.
		 */
		template<InheritFromComponent ComponentType>
		void removeComponent(const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Delete every component of an entity, of every type.
		 *
		 * Only the components are touched: the entity object and its place
		 * in the tree are not. To remove an entity completely, call
		 * EntityRegistry::removeEntity, which ends up here.
		 * @param entityIdentifier The identifier of the entity.
		 */
		void removeEntity(const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Tell whether at least one component of an entity was
		 * modified since the flags were last reset.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when at least one of its components is marked as
		 * changed; false when none is, or when the entity has no component.
		 * @see Component::hasChanged
		 */
		bool hasChanged(const Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Mark every component of every entity as unchanged.
		 *
		 * Application::routine calls it once per frame, after the Render
		 * phase. Code that drives systems by hand (a test, for instance)
		 * calls it itself between two frames.
		 */
		void resetChangedFlags(void);

		/**
		 * @brief Get a component of an entity, to read or modify it.
		 * @tparam ComponentType The component type to get.
		 * @param entityIdentifier The identifier of the entity.
		 * @return Reference to the stored component. It stays valid until
		 * the component is removed or replaced.
		 * @throws EntityComponentNotFoundException<ComponentType> When the
		 * entity does not have the component.
		 */
		template<InheritFromComponent ComponentType>
		ComponentType &getComponent(const Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get a component of an entity, read-only.
		 * @tparam ComponentType The component type to get.
		 * @param entityIdentifier The identifier of the entity.
		 * @return Const reference to the stored component. It stays valid
		 * until the component is removed or replaced.
		 * @throws EntityComponentNotFoundException<ComponentType> When the
		 * entity does not have the component.
		 */
		template<InheritFromComponent ComponentType> const ComponentType &
			getComponent(const Entity::Identifier &entityIdentifier) const;
	};

	/**
	 * @brief Constrains a template parameter to a type derived from
	 * ComponentRegistry (or ComponentRegistry itself).
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromComponentRegistry =
		std::is_base_of_v<ComponentRegistry, Type>;

}	 // namespace guillaume::ecs

// Include the implementation of the ComponentRegistry template class.
#include "guillaume/ecs/component_registry.tpp"