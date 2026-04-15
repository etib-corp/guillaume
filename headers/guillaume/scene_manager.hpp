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

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include "guillaume/scene.hpp"

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/system_registry.hpp"

#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

namespace guillaume
{

	/**
	 * @brief Scene manager class responsible for managing scenes in the
	 * application.
	 */
	class SceneManager:
		public utility::logging::Loggable<
			SceneManager, utility::logging::StandardLogger>
	{
		private:
		std::map<std::type_index, std::unique_ptr<Scene>>
			_scenes;	///< Registered scenes in the manager
		std::type_index
			_activeSceneType;	 ///< Type index of the currently active scene
		LocalStorage _localStorage;	   ///< Local storage for persistent data
		SessionStorage
			_sessionStorage;	///< Session storage for temporary data

		/**
		 * @brief Get the active scene.
		 * @return Pointer to the active scene, or nullptr if no active scene
		 * is set.
		 */
		std::unique_ptr<Scene> &getActiveScene(void)
		{
			if (_scenes.empty()) {
				getLogger().error("Cannot activate scene: no scenes are "
									  "registered");
				throw std::runtime_error("No scenes registered in scene manager");
			}

			if (_activeSceneType == typeid(void)) {
				_activeSceneType = _scenes.begin()->first;
				getLogger().info("No active scene set. Defaulting to first "
									 "registered scene");
			}
			return _scenes[_activeSceneType];
		}

		protected:
		/**
		 * @brief Add a scene to the scene manager.
		 * @tparam SceneType The type of the scene to be added.
		 */
		template<InheritFromScene SceneType> void addScene(void)
		{
			std::type_index typeIndex(typeid(SceneType));
			_scenes[typeIndex] =
				std::make_unique<SceneType>(_localStorage, _sessionStorage);
			getLogger().info("Registered scene type: "
						 + std::string(typeid(SceneType).name()));
		}

		public:
		/**
		 * @brief Default constructor for the SceneManager class.
		 */
		SceneManager(void);

		/**
		 * @brief Default destructor for the SceneManager class.
		 */
		~SceneManager(void);

		/**
		 * @brief Switch to a different scene.
		 * @tparam SceneType The type of the scene to switch to.
		 * @throws std::runtime_error if the specified scene type is not found
		 * in the scene manager.
		 */
		template<InheritFromScene SceneType> void switchToScene(void)
		{
			std::type_index typeIndex(typeid(SceneType));
			if (_scenes.find(typeIndex) == _scenes.end()) {
				getLogger().error("Scene switch failed. Scene type is not "
									  "registered: "
									  + std::string(typeid(SceneType).name()));
				throw std::runtime_error("Scene not found in scene manager");
			}
			_activeSceneType = typeIndex;
			getLogger().info("Switched active scene to type: "
						 + std::string(typeid(SceneType).name()));
		}

		/**
		 * @brief Get the active entity registry.
		 * @return Reference to the active entity registry.
		 */
		ecs::EntityRegistry &getActiveEntityRegistry(void)
		{
			return getActiveScene()->getEntityRegistry();
		}

		/**
		 * @brief Get the active component registry.
		 * @return Reference to the active component registry.
		 */
		ecs::ComponentRegistry &getActiveComponentRegistry(void)
		{
			return getActiveScene()->getComponentRegistry();
		}
	};
}	 // namespace guillaume