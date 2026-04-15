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

#include <memory>
#include <stdexcept>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/entity_builder_manager_filler.hpp"
#include "guillaume/ecs/entity_director_manager_filler.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"

#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

namespace guillaume
{
	/**
	 * @brief Represents a scene in the application, which can contain entities
	 * and systems. Scenes can be switched to change the active content.
	 */
	class Scene:
		public ecs::EntityRegistryContainer,
		public utility::logging::Loggable<Scene,
										  utility::logging::StandardLogger>
	{
		private:
		LocalStorage &
			_localStorage;	  ///< Reference to the local storage for this scene
		SessionStorage &_sessionStorage;	///< Reference to the session
											///< storage for this scene
		ecs::ComponentRegistry
			_componentRegistry;	   ///< Component registry for the scene
		std::unique_ptr<ecs::EntityBuilderManager>
			_entityBuilderManager;	  ///< Manager for entity builders
		std::unique_ptr<ecs::EntityDirectorManager>
			_entityDirectorManager;	   ///< Manager for entity directors

		protected:
		/**
		 * @brief Get the entity builder manager for this scene.
		 * @return Reference to the entity builder manager.
		 */
		ecs::EntityBuilderManager &getBuilderManager(void);

		/**
		 * @brief Get the entity director manager for this scene.
		 * @return Reference to the entity director manager.
		 */
		ecs::EntityDirectorManager &getDirectorManager(void);

		public:
		/**
		 * @brief Default constructor for Scene.
		 * @param localStorage Reference to the local storage for this scene.
		 * @param sessionStorage Reference to the session storage for this
		 * scene.
		 */
		Scene(LocalStorage &localStorage, SessionStorage &sessionStorage);

		/**
		 * @brief Default destructor for Scene.
		 */
		virtual ~Scene(void);

		/**
		 * @brief Get the component registry for this scene.
		 * @return Reference to the component registry.
		 */
		ecs::ComponentRegistry &getComponentRegistry(void);

		/**
		 * @brief Get the entity registry for this scene.
		 * @return Reference to the entity registry.
		 */
		ecs::EntityRegistry &getEntityRegistry(void);
	};

	/**
	 * @brief Concept to ensure a type inherits from Scene.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromScene = std::is_base_of_v<Scene, Type>;

}	 // namespace guillaume
