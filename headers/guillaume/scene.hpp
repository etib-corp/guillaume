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
#include <typeindex>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/ressource_provider.hpp>

#include <utility/graphic/view.hpp>

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
										  utility::logging::DefaultLogger>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared pointer to the resource provider
		LocalStorage &
			_localStorage;	  ///< Reference to the local storage for this scene
		SessionStorage &_sessionStorage;	///< Reference to the session
											///< storage for this scene
		ecs::ComponentRegistry
			_componentRegistry;	   ///< Component registry for the scene
		std::unique_ptr<ecs::EntityBuilderManager>
			_entityBuilderManager;	  ///< Manager for entity builders
		std::unique_ptr<ecs::EntityDirectorManager>
			_entityDirectorManager;		   ///< Manager for entity directors
		std::type_index _nextSceneType;	   ///< Type of the next scene to switch
										   ///< to at end of frame
		std::map<std::string, std::shared_ptr<ecs::Entity>>
			_rootEntities;	  ///< Map of root entities in the scene

		protected:
		/**
		 * @brief Get a shared pointer to the resource provider.
		 * @return Shared pointer to the resource provider.
		 */
		std::shared_ptr<utility::RessourceProvider> getRessourceProvider(void);

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

		/**
		 * @brief Add a root entity to the scene.
		 * @param name The name of the entity.
		 * @param entity The shared pointer to the entity.
		 */
		void addRootEntity(const std::string &name,
						   std::shared_ptr<ecs::Entity> entity);

		/**
		 * @brief Get a root entity by name.
		 * @param name The name of the entity.
		 * @return Shared pointer to the entity, or nullptr if not found.
		 */
		template<ecs::InheritFromEntity EntityType>
		std::shared_ptr<EntityType> getRootEntity(const std::string &name);

		public:
		/**
		 * @brief Place all root entities in front of the current view with
		 * horizontal spread.
		 * @param view The view to use for positioning.
		 */
		void placeEntitiesInFrontOfView(const utility::graphic::ViewF &view);

		/**
		 * @brief Request a switch to another scene at the end of the current
		 * frame.
		 * @tparam NextScene The type of the scene to switch to. Must inherit
		 * from Scene.
		 */
		template<typename NextScene> void goToScene(void);
		/**
		 * @brief Default constructor for Scene.
		 * @param ressourceProvider Shared pointer to the resource provider for
		 * this
		 * @param localStorage Reference to the local storage for this scene.
		 * @param sessionStorage Reference to the session storage for this
		 * scene.
		 */
		Scene(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			  LocalStorage &localStorage, SessionStorage &sessionStorage);

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

		/**
		 * @brief Check if the scene wants to switch to another scene.
		 * @return True if a scene switch has been requested, false otherwise.
		 */
		bool wantsToSwitch(void) const noexcept;

		/**
		 * @brief Get the type of the next scene to switch to.
		 * @return std::type_index of the next scene, or typeid(void) if none.
		 */
		std::type_index getNextSceneType(void) const noexcept;

		/**
		 * @brief Clear the pending scene transition request.
		 */
		void clearNextScene(void) noexcept;

		/**
		 * @brief Called when the scene becomes the active scene.
		 * Override to perform initialization logic.
		 */
		virtual void onEnter(void);

		/**
		 * @brief Called when the scene is leaving the active state.
		 * Override to perform cleanup logic.
		 */
		virtual void onExit(void);
	};

	/**
	 * @brief Concept to ensure a type inherits from Scene.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromScene = std::is_base_of_v<Scene, Type>;

	/**
	 * @brief Concept to ensure a type is present in a parameter pack.
	 * @tparam Type The type to check.
	 * @tparam Types The parameter pack to check against.
	 */
	template<typename Type, typename... Types>
	concept IsOneOf = (std::is_same_v<Type, Types> || ...);

}	 // namespace guillaume

// Include the goToScene template implementation
#include "guillaume/scene.tpp"
