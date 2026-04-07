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
#include <functional>
#include <stdexcept>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>
#include <utility/demangle.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/component_type_id.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base class for all systems in the ECS architecture.
	 *
	 * This base class provides common functionality that doesn't depend on
	 * component types.
	 * @see SystemRegistry
	 * @see SystemFiller
	 */
	class System:
		protected utility::logging::Loggable<System,
											 utility::logging::StandardLogger>
	{
		public:
		/**
		 * @brief Enumeration of system update phases.
		 *
		 * Defines the different phases during which systems can be updated,
		 * allowing for organized and efficient processing of entities based on
		 * their components.
		 */
		enum class Phase {
			Event,		///< Main update phase for application logic
			Measure,	///< Measurement phase for calculating layout and text
						///< sizes
			Layout,		///< Layout phase for arranging entities based on
						///< measurements
			Render		///< Render phase for drawing entities
		};

		private:
		Phase _phase;					 ///< Update phase of the system
		Entity::Signature _signature;	 ///< System signature
		ecs::ComponentRegistry *_activeComponentRegistry {
			nullptr
		};	  ///< Active component registry for the current update scope

		protected:
		/**
		 * @brief Set the system's signature based on the specified component
		 * types.
		 * @tparam ComponentTypes The component types that define the system's
		 * signature.
		 */
		template<InheritFromComponent... ComponentTypes> void setSignature(void)
		{
			_signature.reset();
			(_signature.set(ComponentTypeId::get<ComponentTypes>()), ...);
		}

		/**
		 * @brief Get the active component registry for the current update
		 * scope.
		 * @return Mutable reference to the active component registry.
		 */
		ecs::ComponentRegistry &getComponentRegistry(void)
		{
			if (_activeComponentRegistry == nullptr) {
				throw std::runtime_error(
					"No active component registry bound to system");
			}
			return *_activeComponentRegistry;
		}

		/**
		 * @brief Get the active component registry for the current update
		 * scope.
		 * @return Const reference to the active component registry.
		 */
		const ecs::ComponentRegistry &getComponentRegistry(void) const
		{
			if (_activeComponentRegistry == nullptr) {
				throw std::runtime_error(
					"No active component registry bound to system");
			}
			return *_activeComponentRegistry;
		}

		/**
		 * @brief Check whether the current entity has the specified component.
		 * @tparam ComponentType The component type to query.
		 * @param entityIdentifier The target entity.
		 * @return True if the component exists, false otherwise.
		 */
		template<InheritFromComponent ComponentType>
		bool hasComponent(const ecs::Entity::Identifier &entityIdentifier) const
		{
			if (_activeComponentRegistry == nullptr) {
				return false;
			}
			return _activeComponentRegistry->hasComponent<ComponentType>(
				entityIdentifier);
		}

		/**
		 * @brief Get a mutable component from the active component registry.
		 * @tparam ComponentType The component type to retrieve.
		 * @param entityIdentifier The target entity.
		 * @return Mutable reference to the requested component.
		 */
		template<InheritFromComponent ComponentType> ComponentType &
			getComponent(const ecs::Entity::Identifier &entityIdentifier)
		{
			if (_activeComponentRegistry == nullptr) {
				throw std::runtime_error(
					"No active component registry bound to system");
			}
			return _activeComponentRegistry->getComponent<ComponentType>(
				entityIdentifier);
		}

		/**
		 * @brief Get a const component from the active component registry.
		 * @tparam ComponentType The component type to retrieve.
		 * @param entityIdentifier The target entity.
		 * @return Const reference to the requested component.
		 */
		template<InheritFromComponent ComponentType> const ComponentType &
			getComponent(const ecs::Entity::Identifier &entityIdentifier) const
		{
			if (_activeComponentRegistry == nullptr) {
				throw std::runtime_error(
					"No active component registry bound to system");
			}
			return _activeComponentRegistry->getComponent<ComponentType>(
				entityIdentifier);
		}

		/**
		 * @brief Ensure a component exists for an entity and log if missing.
		 * @tparam ComponentType The required component type.
		 * @param entityIdentifier The target entity.
		 * @return True if present, false otherwise.
		 */
		template<InheritFromComponent ComponentType>
		bool requireComponent(const ecs::Entity::Identifier &entityIdentifier)
		{
			if (hasComponent<ComponentType>(entityIdentifier)) {
				return true;
			}

			getLogger().warning(
				"Entity " + std::to_string(entityIdentifier) + " does not have "
				+ utility::demangle<ComponentType>() + " component");
			return false;
		}

		public:
		/**
		 * @brief Default constructor for the System class.
		 * @param phase The update phase during which this system should be
		 * updated.
		 * @note The phase is used by the SystemRegistry to determine when to
		 * call the system's routine method during the application update loop.
		 * The constructor does not set the system's signature, so it must be
		 * configured separately using setSignature() after construction.
		 */
		System(Phase phase)
			: _phase(phase)
			, _signature()
			, _activeComponentRegistry(nullptr)

		{
		}

		/**
		 * @brief Default destructor for the System class.
		 */
		virtual ~System(void) = default;

		/**
		 * @brief Get the system's update phase.
		 * @return The system's update phase.
		 */
		Phase getPhase(void) const
		{
			return _phase;
		}

		/**
		 * @brief Get the system's signature.
		 * @return The system's signature.
		 */
		Entity::Signature getSignature(void) const
		{
			return _signature;
		}

		/**
		 * @brief Routine to update all managed entities.
		 * @param componentRegistry The component registry instance.
		 * @param entityRegistry The entity registry used to query matching
		 * entities.
		 */
		void routine(ecs::ComponentRegistry &componentRegistry,
					 ecs::EntityRegistry &entityRegistry);

		/**
		 * @brief Update the system, processing relevant entities.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) = 0;
	};

	/**
	 * @brief Concept to ensure a type inherits from System.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromSystem = std::is_base_of_v<System, Type>;

}	 // namespace guillaume::ecs