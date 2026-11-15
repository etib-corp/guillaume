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
#include <chrono>
#include <functional>
#include <stdexcept>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>
#include <utility/demangle.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/component_type_id.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/system_phase.hpp"
#include "guillaume/ecs/entity_tree_traveler.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base class of every system: a rule applied each frame to every
	 * entity that has a given set of components.
	 *
	 * A system declares two things: a **signature** (the component types it
	 * needs) and a **phase** (when it runs in the frame). It keeps no
	 * per-entity state: everything it reads and writes is in components,
	 * reached by entity identifier.
	 *
	 * **What happens each frame.** For each phase in order,
	 * Application::routine calls routine() on every system of that phase.
	 * routine() does this:
	 * 1. binds the scene's registries, so getComponent() and
	 *    getEntityRegistry() work;
	 * 2. calls prepare() once;
	 * 3. asks the traveler of the phase for every entity of the scene, in
	 *    order, and calls update(identifier) on each entity whose signature
	 *    contains the system's signature;
	 * 4. calls cleanup() once;
	 * 5. unbinds the registries.
	 *
	 * Every matching entity is visited every frame, whether or not its
	 * components changed, so update() must be cheap when there is nothing
	 * to do and must not write a value that is already there (component
	 * setters compare before marking the component changed).
	 *
	 * **Writing a system.** Derive from SystemFiller, list the components,
	 * pick the phase, implement update(), and register an instance in a
	 * SystemRegistry:
	 * @code
	 * // Opacity is a component defined by the application.
	 * class Fade: public ecs::SystemFiller<components::Color, Opacity>
	 * {
	 * 	public:
	 * 	Fade(void)
	 * 		: ecs::SystemFiller<components::Color, Opacity>(
	 * 			  ecs::Phase::Layout)
	 * 	{
	 * 	}
	 *
	 * 	void update(const ecs::Entity::Identifier &identifier) override
	 * 	{
	 * 		auto &color	  = getComponent<components::Color>(identifier);
	 * 		auto &opacity = getComponent<Opacity>(identifier);
	 * 		// read opacity, write color
	 * 	}
	 * };
	 *
	 * systemRegistry.registerNewSystem(std::make_unique<Fade>());
	 * @endcode
	 *
	 * Things to watch out for:
	 * - The match uses Entity::getSignature, not the content of the
	 *   ComponentRegistry. An entity that received a component through
	 *   ComponentRegistry::addComponent is skipped until its signature has
	 *   the bit (Entity::setSignature).
	 * - The registries are bound only while routine() runs (or after
	 *   bindComponentRegistry()). Outside of that, getComponent() and the
	 *   registry getters throw `std::runtime_error`.
	 * - The list of entities is taken before the first update(): an entity
	 *   added during the pass is visited from the next system on; one
	 *   removed during the pass is still in the list, and getComponent() on
	 *   it throws because its components are gone.
	 * - One system instance serves every scene; do not cache references to
	 *   components between frames.
	 *
	 * @see SystemFiller
	 * @see SystemRegistry
	 * @see Phase
	 */
	class System:
		protected utility::logging::Loggable<System,
											 utility::logging::DefaultLogger>
	{
		private:
		Phase _phase;	 ///< Phase of the frame in which the system runs
		Entity::Signature
			_signature;	   ///< Component types an entity must declare
		ecs::ComponentRegistry *_activeComponentRegistry {
			nullptr
		};	  ///< Component registry bound for the pass in progress; nullptr
			  ///< outside of a pass
		ecs::EntityRegistry *_activeEntityRegistry {
			nullptr
		};	  ///< Entity registry bound for the pass in progress; nullptr
			  ///< outside of a pass

		protected:
		/**
		 * @brief Replace the signature by the one made of the given
		 * component types.
		 *
		 * SystemFiller calls it from its constructor. With no type, the
		 * signature is empty and the system visits every entity.
		 * @tparam ComponentTypes The component types an entity must declare
		 * for update() to be called on it.
		 */
		template<InheritFromComponent... ComponentTypes>
		void setSignature(void);

		/**
		 * @brief Get the entity registry of the scene being processed.
		 * @return Reference to the registry given to routine().
		 * @throws std::runtime_error When called outside routine().
		 * bindComponentRegistry() does not bind an entity registry.
		 */
		ecs::EntityRegistry &getEntityRegistry(void);

		/**
		 * @brief Get the entity registry of the scene being processed,
		 * read-only.
		 * @return Const reference to the registry given to routine().
		 * @throws std::runtime_error When called outside routine().
		 */
		const ecs::EntityRegistry &getEntityRegistry(void) const;

		/**
		 * @brief Get the component registry of the scene being processed.
		 * @return Reference to the registry given to routine() or to
		 * bindComponentRegistry().
		 * @throws std::runtime_error When no registry is bound.
		 */
		ecs::ComponentRegistry &getComponentRegistry(void);

		/**
		 * @brief Get the component registry of the scene being processed,
		 * read-only.
		 * @return Const reference to the registry given to routine() or to
		 * bindComponentRegistry().
		 * @throws std::runtime_error When no registry is bound.
		 */
		const ecs::ComponentRegistry &getComponentRegistry(void) const;

		/**
		 * @brief Tell whether an entity has a component, in the bound
		 * registry.
		 *
		 * Use it for the components that are optional for the system; the
		 * ones of its signature are expected to be there.
		 * @tparam ComponentType The component type to look for.
		 * @param entityIdentifier The identifier of the entity, which can be
		 * any entity of the scene, not only the one being updated.
		 * @return True when the component exists; false when it does not or
		 * when no registry is bound.
		 */
		template<InheritFromComponent ComponentType> bool
			hasComponent(const ecs::Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Get a component of an entity from the bound registry, to
		 * read or modify it.
		 * @tparam ComponentType The component type to get.
		 * @param entityIdentifier The identifier of the entity, which can be
		 * any entity of the scene (a child, the parent).
		 * @return Reference to the stored component.
		 * @throws std::runtime_error When no registry is bound.
		 * @throws EntityComponentNotFoundException<ComponentType> When the
		 * entity does not have the component.
		 */
		template<InheritFromComponent ComponentType> ComponentType &
			getComponent(const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get a component of an entity from the bound registry,
		 * read-only.
		 * @tparam ComponentType The component type to get.
		 * @param entityIdentifier The identifier of the entity.
		 * @return Const reference to the stored component.
		 * @throws std::runtime_error When no registry is bound.
		 * @throws EntityComponentNotFoundException<ComponentType> When the
		 * entity does not have the component.
		 */
		template<InheritFromComponent ComponentType> const ComponentType &
			getComponent(const ecs::Entity::Identifier &entityIdentifier) const;

		/**
		 * @brief Tell whether an entity has a component, and log a warning
		 * naming the entity and the component when it does not.
		 *
		 * @code
		 * if (!requireComponent<components::Bound>(identifier)) {
		 * 	return;
		 * }
		 * @endcode
		 * @tparam ComponentType The component type the system cannot work
		 * without.
		 * @param entityIdentifier The identifier of the entity.
		 * @return True when the component exists; false (after the warning)
		 * when it does not or when no registry is bound.
		 */
		template<InheritFromComponent ComponentType>
		bool requireComponent(const ecs::Entity::Identifier &entityIdentifier);

		public:
		/**
		 * @brief Create a system that runs in a given phase, with an empty
		 * signature.
		 *
		 * An empty signature matches every entity. Derive from SystemFiller
		 * to set the signature from a list of component types, or call
		 * setSignature() in the derived constructor.
		 * @param phase The phase of the frame in which the system runs. It
		 * decides in which list SystemRegistry stores the system.
		 */
		System(Phase phase);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~System(void) = default;

		/**
		 * @brief Get the phase of the frame in which the system runs.
		 * @return The phase given to the constructor.
		 */
		Phase getPhase(void) const noexcept;

		/**
		 * @brief Get the component types an entity must declare to be
		 * visited by this system.
		 * @return A copy of the signature.
		 */
		Entity::Signature getSignature(void) const noexcept;

		/**
		 * @brief Run the system once over a scene: prepare(), update() on
		 * each matching entity, cleanup().
		 *
		 * Called by Application::routine for every system, phase after
		 * phase. The registries are bound for the duration of the call and
		 * unbound when it returns normally. See the class description for
		 * the exact steps.
		 * @param componentRegistry The component registry of the scene; the
		 * one getComponent() reads during the call.
		 * @param entityRegistry The entity registry of the scene; its
		 * entities and the ones nested below are the candidates.
		 * @param traveler The order in which the entities are visited,
		 * given by the phase (SystemPhase::getTraveler).
		 */
		void routine(ecs::ComponentRegistry &componentRegistry,
					 ecs::EntityRegistry &entityRegistry,
					 const ecs::EntityTreeTraveler &traveler);

		/**
		 * @brief Bind a component registry so update() can be called
		 * directly, without routine().
		 *
		 * Meant for tests and for code that drives one entity by hand:
		 * @code
		 * system.bindComponentRegistry(registry);
		 * system.update(identifier);
		 * system.unbindComponentRegistry();
		 * @endcode
		 * No entity registry is bound, so getEntityRegistry() still throws.
		 * routine() replaces this binding and clears it when it returns.
		 * @param componentRegistry The registry getComponent() reads from.
		 * It must stay alive until unbindComponentRegistry().
		 */
		void bindComponentRegistry(ecs::ComponentRegistry &componentRegistry);

		/**
		 * @brief Drop the binding made by bindComponentRegistry().
		 */
		void unbindComponentRegistry(void);

		/**
		 * @brief Apply the system's rule to one entity. Implemented by each
		 * system.
		 *
		 * routine() calls it once per frame for every entity whose signature
		 * contains the system's signature, in traveler order.
		 * @param entityIdentifier The identifier of the entity to process.
		 * Its components are reached with getComponent().
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) = 0;

		/**
		 * @brief Called once per pass, before the first update().
		 *
		 * Does nothing by default. Override it for work that is done once
		 * per frame rather than once per entity, such as taking the next
		 * input event from a queue. The registries are already bound.
		 */
		virtual void prepare(void)
		{
		}

		/**
		 * @brief Called once per pass, after the last update().
		 *
		 * Does nothing by default. Override it to release what prepare() set
		 * up or to act on what the pass collected. The registries are still
		 * bound. It is called even when no entity matched.
		 */
		virtual void cleanup(void)
		{
		}
	};

	/**
	 * @brief Constrains a template parameter to a type derived from System.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromSystem = std::is_base_of_v<System, Type>;

}	 // namespace guillaume::ecs

// Include the System template implementation
#include "guillaume/ecs/system.tpp"