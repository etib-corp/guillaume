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

#include "guillaume/scene.hpp"

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/system_registry.hpp"

#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

namespace guillaume
{

	// Forward declaration
	class Engine;

	/**
	 * @brief Scene manager class responsible for managing scenes in the
	 * application.
	 * @tparam DefaultSceneType The default scene type to activate when the
	 * application starts. Must be one of the registered SceneTypes.
	 * @tparam SceneTypes Variadic template parameter pack for scene types used
	 * in the application. Each scene type must inherit from the Scene class.
	 */
	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	class SceneManager:
		public utility::logging::Loggable<
			SceneManager<DefaultSceneType, SceneTypes...>,
			utility::logging::DefaultLogger>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared pointer to the resource provider
		std::map<std::type_index, std::unique_ptr<Scene>>
			_scenes;	///< Registered scenes in the manager
		std::type_index
			_activeSceneType;	 ///< Type index of the currently active scene
		LocalStorage _localStorage;	   ///< Local storage for persistent data
		SessionStorage
			_sessionStorage;	///< Session storage for temporary data
		Engine *_engine;	///< Pointer to the application engine (non-owning)

		protected:
		/**
		 * @brief Add a scene to the scene manager.
		 * @tparam SceneType The type of the scene to be added.
		 */
		template<InheritFromScene SceneType> void addScene(void);

		public:
		/**
		 * @brief Default constructor for the SceneManager class.
		 * @param ressourceProvider Shared pointer to the resource provider for
		 * the scenes.
		 */
		SceneManager(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider);

		/**
		 * @brief Default destructor for the SceneManager class.
		 */
		~SceneManager(void);

		/**
		 * @brief Get the active scene.
		 * @return Pointer to the active scene, or nullptr if no active scene
		 * is set.
		 */
		std::unique_ptr<Scene> &getActiveScene(void);

		/**
		 * @brief Check whether at least one scene is registered.
		 * @return True when one or more scenes are registered.
		 */
		bool hasScenes(void) const noexcept;

		/**
		 * @brief Check whether an active scene has been selected.
		 * @return True when the active scene points to a registered scene.
		 */
		bool hasActiveScene(void) const noexcept;

		/**
		 * @brief Set the engine pointer used to query the current view.
		 * @param engine Pointer to the engine instance.
		 */
		void setEngine(Engine *engine);

		/**
		 * @brief Enter the currently active scene, setting its view from the
		 * engine and calling onEnter().
		 * @throws std::runtime_error if there is no active scene.
		 */
		void enterActiveScene(void);

		/**
		 * @brief Process a pending scene transition request from the active
		 * scene. Calls onExit() on the current scene, switches the active
		 * scene, and calls onEnter() on the new scene.
		 * @throws std::runtime_error if the requested next scene type is not
		 * registered.
		 */
		void processSceneTransition(void);

		/**
		 * @brief Switch to a different scene.
		 * @tparam SceneType The type of the scene to switch to.
		 * @throws std::runtime_error if the specified scene type is not found
		 * in the scene manager.
		 */
		template<InheritFromScene SceneType> void switchToScene(void);

		/**
		 * @brief Get the active entity registry.
		 * @return Reference to the active entity registry.
		 */
		ecs::EntityRegistry &getActiveEntityRegistry(void);

		/**
		 * @brief Get the active component registry.
		 * @return Reference to the active component registry.
		 */
		ecs::ComponentRegistry &getActiveComponentRegistry(void);

		/**
		 * @brief Get a list of all registered scene types.
		 * @return Vector of type indices for all registered scenes.
		 */
		std::vector<std::type_index> getRegisteredSceneTypes(void) const;
	};
}	 // namespace guillaume

// Include the SceneManager template implementation
#include "guillaume/scene_manager.tpp"
