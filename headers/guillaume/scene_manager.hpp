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

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/ressource_provider.hpp>

#include <utility/engine.hpp>

#include "guillaume/scene.hpp"

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/system_registry.hpp"

#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

namespace guillaume
{

	/**
	 * @brief Owns the scenes of an application, knows which one is active,
	 * and performs the switches between them.
	 *
	 * Application creates one manager. The manager builds one instance of
	 * every scene type at construction and keeps them all until it is
	 * destroyed: switching scenes changes which one is active, it does not
	 * create or destroy scenes. Each frame, Application asks it for the
	 * registries of the active scene, runs the systems on them, then calls
	 * processSceneTransition() to honor a Scene::goToScene request.
	 *
	 * Every switch goes through the same steps: `onExit()` on the active
	 * scene, then `onEnter()` on the new one, then
	 * Scene::placeEntitiesInFrontOfView with the engine's view.
	 *
	 * The manager also owns the LocalStorage and the SessionStorage handed
	 * to every scene. The LocalStorage uses its default path, so
	 * constructing a manager opens (and creates if needed) the file given
	 * by LocalStorage::defaultStoragePath.
	 *
	 * @note Application code normally switches scenes with
	 * Scene::goToScene and does not use this class directly.
	 * @tparam DefaultSceneType The scene activated when the application
	 * starts.
	 * @tparam SceneTypes The other scenes of the application. Every scene
	 * type must inherit from Scene and appear only once.
	 * @see Scene
	 * @see Application
	 */
	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	class SceneManager:
		public utility::logging::Loggable<
			SceneManager<DefaultSceneType, SceneTypes...>,
			utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief Create one instance of every scene type and make
		 * `DefaultSceneType` the active one.
		 *
		 * No scene is entered yet: call enterActiveScene() once the manager
		 * is built.
		 * @param ressourceProvider The resource provider handed to every
		 * scene.
		 * @param engine The engine asked for the current view each time a
		 * scene is entered. It must outlive the manager.
		 * @throws std::runtime_error When the local storage cannot be
		 * opened at all (see LocalStorage).
		 */
		SceneManager(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Destroy the manager and every scene. `onExit()` is not
		 * called on the active scene.
		 */
		~SceneManager(void);

		/**
		 * @brief Get the active scene.
		 * @return Pointer to the active scene, never nullptr. It stays
		 * valid as long as the manager.
		 * @throws std::runtime_error When the manager holds no scene.
		 */
		Scene *getActiveScene(void);

		/**
		 * @brief Tell whether the manager holds at least one scene.
		 * @return True when one or more scenes exist, which is always the
		 * case after construction.
		 */
		bool hasScenes(void) const noexcept;

		/**
		 * @brief Tell whether an active scene is selected.
		 * @return True when scenes exist and one of them is the active one,
		 * which is always the case after construction.
		 */
		bool hasActiveScene(void) const noexcept;

		/**
		 * @brief Enter the active scene: call its `onEnter()`, then place
		 * its entities in front of the engine's current view
		 * (Scene::placeEntitiesInFrontOfView).
		 *
		 * `onEnter()` comes first because that is where the scene builds the
		 * entities to place. Application calls this once at start-up for
		 * the default scene; later entries happen through the switches.
		 * @throws std::runtime_error When the manager holds no scene.
		 */
		void enterActiveScene(void);

		/**
		 * @brief Carry out the switch requested by the active scene with
		 * Scene::goToScene, if any.
		 *
		 * Does nothing when no switch is pending. Otherwise the request is
		 * cleared, `onExit()` is called on the active scene, the requested
		 * scene becomes active and is entered (enterActiveScene()).
		 * Application calls it once per frame, after the systems ran.
		 * @throws std::runtime_error When the requested scene type is not
		 * one of this manager's scenes. The request is dropped and the
		 * active scene stays active, without `onExit()` being called.
		 */
		void processSceneTransition(void);

		/**
		 * @brief Switch to another scene immediately, without waiting for
		 * the end of the frame.
		 *
		 * Same steps as a switch requested by the scene itself: `onExit()`
		 * on the active scene, then the new scene becomes active and is
		 * entered. Switching to the scene that is already active exits and
		 * re-enters it.
		 * @tparam SceneType The scene type to switch to.
		 * @throws std::runtime_error When `SceneType` is not one of this
		 * manager's scenes; the active scene is left as it was.
		 */
		template<InheritFromScene SceneType> void switchToScene(void);

		/**
		 * @brief Get the entity registry of the active scene.
		 * @return Reference to the active scene, seen as the registry that
		 * owns its entities. It designates another scene after a switch.
		 * @throws std::runtime_error When the manager holds no scene.
		 */
		ecs::EntityRegistry &getActiveEntityRegistry(void);

		/**
		 * @brief Get the component registry of the active scene.
		 * @return Reference to the registry of the active scene. It
		 * designates another scene's registry after a switch.
		 * @throws std::runtime_error When the manager holds no scene.
		 */
		ecs::ComponentRegistry &getActiveComponentRegistry(void);

		/**
		 * @brief List the scene types this manager holds.
		 * @return One `std::type_index` per scene, in the ordering of
		 * `std::type_index` (not the order of the template arguments).
		 */
		std::vector<std::type_index> getRegisteredSceneTypes(void) const;

		protected:
		/**
		 * @brief Create the instance of one scene type and keep it.
		 *
		 * The scene receives the resource provider and the two storages. A
		 * scene of the same type already held is replaced.
		 * @tparam SceneType The scene type to instantiate. It must be
		 * constructible from the arguments of the Scene constructor.
		 */
		template<InheritFromScene SceneType> void addScene(void);

		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Resource provider handed to the scenes
		std::map<std::type_index, std::unique_ptr<Scene>>
			_scenes;	///< One instance per scene type, kept for the whole
						///< life of the manager
		std::type_index _activeSceneType;	 ///< Type of the active scene
		LocalStorage
			_localStorage;	  ///< Persistent storage shared by the scenes
		SessionStorage
			_sessionStorage;		 ///< Session storage shared by the scenes
		utility::Engine &_engine;	 ///< Engine asked for the view; not owned

		/**
		 * @brief Leave the active scene and enter another one.
		 *
		 * Calls `onExit()` on the active scene, clears its pending request,
		 * makes `nextType` active and calls enterActiveScene().
		 * @param nextType The type of the scene to activate.
		 * @throws std::runtime_error When no scene of that type exists; the
		 * active scene is left untouched.
		 */
		void transitionTo(std::type_index nextType);
	};
}	 // namespace guillaume

// Include the SceneManager template implementation
#include "guillaume/scene_manager.tpp"
