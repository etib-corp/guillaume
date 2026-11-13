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
#include "guillaume/ecs/entity_registry_container.hpp"

#include "guillaume/local_storage.hpp"
#include "guillaume/session_storage.hpp"

namespace guillaume
{
	/**
	 * @brief One screen of the application: it owns the entities shown and
	 * the components that hold their state.
	 *
	 * A program derives one class per screen and overrides onEnter() to
	 * build its widgets. Application creates one instance of each scene
	 * type at start-up and keeps it until the program ends; exactly one
	 * scene is active at a time, and only that one is processed by the
	 * systems each frame.
	 *
	 * A scene is three things:
	 * - an ecs::EntityRegistry (it derives from
	 *   ecs::EntityRegistryContainer): it owns the root entities, and
	 *   through them every entity of the screen;
	 * - an ecs::ComponentRegistry (getComponentRegistry()): the components
	 *   of those entities;
	 * - an ecs::EntityBuilderManager (getBuilderManager()): one builder per
	 *   widget of the library, already wired to the two registries.
	 *
	 * @code
	 * class HomeScene: public guillaume::Scene
	 * {
	 * 	public:
	 * 	using guillaume::Scene::Scene;
	 *
	 * 	void onEnter(void) override
	 * 	{
	 * 		auto &builder = getBuilderManager()
	 * 			.getBuilder<guillaume::entities::Button::Builder>();
	 * 		builder.makeButton(
	 * 			nullptr, "Settings",
	 * 			[this] { goToScene<SettingsScene>(); },
	 * 			guillaume::entities::Button::Color::Filled,
	 * 			guillaume::entities::Button::Shape::Round,
	 * 			guillaume::entities::Button::Size::Medium, false);
	 * 	}
	 *
	 * 	void onExit(void) override
	 * 	{
	 * 		clearEntities();
	 * 	}
	 * };
	 *
	 * guillaume::Application<HomeScene, SettingsScene> app(
	 * 	resources, std::move(engine));
	 * @endcode
	 *
	 * Things to watch out for:
	 * - onEnter() runs every time the scene becomes active, and the scene
	 *   keeps its entities while it is inactive. A scene that can be
	 *   entered twice either removes its entities in onExit(), as above, or
	 *   builds them only once.
	 * - Right after onEnter(), the entities the scene owns directly that
	 *   have a Transform and a Bound are placed side by side in front of
	 *   the engine's view (placeEntitiesInFrontOfView()).
	 * - goToScene() only records a request; the switch happens at the end
	 *   of the frame.
	 * - A builder registers what it creates in the scene.
	 *   addRootEntity() only gives an entity a name to find it again with
	 *   getRootEntity(); it does not register it. A name cannot be taken
	 *   back or reused, so a scene that names entities in onEnter() must
	 *   build them only once.
	 *
	 * @see Application
	 * @see SceneManager
	 * @see ecs::EntityBuilderManager
	 */
	class Scene:
		public ecs::EntityRegistryContainer,
		public utility::logging::Loggable<Scene,
										  utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief Line up the scene's entities in front of a view.
		 *
		 * Concerns the entities the scene owns directly that have both a
		 * components::Transform and a components::Bound. They are put in a
		 * row, in registration order, 300 units in front of the view,
		 * centered on its axis, 50 units apart, each one facing the same
		 * way as the view. The Transform position is the top-left corner of
		 * the entity. The other entities are not touched.
		 *
		 * SceneManager calls it once after each onEnter(), with the
		 * engine's current view. The entities are not moved again when the
		 * view changes.
		 * @param view The view to place the entities in front of.
		 */
		void placeEntitiesInFrontOfView(const utility::graphic::ViewF &view);

		/**
		 * @brief Ask to leave this scene for another one.
		 *
		 * The request is recorded and carried out at the end of the frame
		 * by SceneManager::processSceneTransition: onExit() is called on
		 * this scene, then onEnter() on the other. A second call before
		 * that replaces the first request.
		 * @tparam NextScene The scene type to switch to. It must be one of
		 * the scene types of the Application; otherwise the switch throws
		 * `std::runtime_error` at the end of the frame.
		 */
		template<typename NextScene> void goToScene(void);
		/**
		 * @brief Create an empty scene with its registries and its builders.
		 *
		 * Called by SceneManager. A derived scene inherits this constructor
		 * (`using guillaume::Scene::Scene;`) or forwards the three
		 * arguments, and builds its widgets in onEnter(), not here.
		 * @param ressourceProvider Gives access to the files the scene
		 * loads, see getRessourceProvider().
		 * @param localStorage The application's persistent storage. The
		 * scene keeps a reference; it must outlive the scene.
		 * @param sessionStorage The application's session storage. The
		 * scene keeps a reference; it must outlive the scene.
		 */
		Scene(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			  LocalStorage &localStorage, SessionStorage &sessionStorage);

		/**
		 * @brief Destroy the scene with its entities and components.
		 */
		virtual ~Scene(void);

		/**
		 * @brief Get the registry holding the components of every entity of
		 * this scene.
		 * @return Reference to the registry, valid as long as the scene.
		 * Pass it to a widget's `create()` or constructor.
		 */
		ecs::ComponentRegistry &getComponentRegistry(void);

		/**
		 * @brief Get the scene as the registry that owns its entities.
		 * @return Reference to the scene itself, seen as an
		 * ecs::EntityRegistry. Use it to add, find or remove entities.
		 */
		ecs::EntityRegistry &getEntityRegistry(void);

		/**
		 * @brief Tell whether goToScene() was called and the switch has not
		 * happened yet.
		 * @return True when a scene switch is pending.
		 */
		bool wantsToSwitch(void) const noexcept;

		/**
		 * @brief Get the scene type asked for by goToScene().
		 * @return The type of the scene to switch to, or `typeid(void)`
		 * when no switch is pending.
		 */
		std::type_index getNextSceneType(void) const noexcept;

		/**
		 * @brief Forget the pending switch request, if any.
		 *
		 * SceneManager calls it when it carries out the switch.
		 */
		void clearNextScene(void) noexcept;

		/**
		 * @brief Called each time the scene becomes the active one: at
		 * start-up for the default scene, then after each switch to it.
		 *
		 * Override it to build the widgets of the screen. The default
		 * implementation only logs.
		 */
		virtual void onEnter(void);

		/**
		 * @brief Called each time the scene stops being the active one,
		 * before onEnter() of the next scene.
		 *
		 * Override it to remove what onEnter() built (clearEntities()) or
		 * to save state. The default implementation only logs: the
		 * entities are kept. It is not called when the application ends.
		 */
		virtual void onExit(void);

		protected:
		/**
		 * @brief Get the resource provider given to the constructor.
		 * @return The shared resource provider, which loads the files
		 * (fonts, images, meshes) the scene needs.
		 */
		std::shared_ptr<utility::RessourceProvider> getRessourceProvider(void);

		/**
		 * @brief Get the builders of this scene, one per widget of the
		 * library.
		 *
		 * @code
		 * auto &builder =
		 * 	getBuilderManager().getBuilder<entities::Text::Builder>();
		 * @endcode
		 * @return Reference to the manager. Every builder it returns
		 * creates components in getComponentRegistry() and registers
		 * entities in this scene.
		 */
		ecs::EntityBuilderManager &getBuilderManager(void);

		/**
		 * @brief Give an entity a name, to find it again with
		 * getRootEntity().
		 *
		 * The name table also keeps the entity alive. The entity is not
		 * registered in the scene by this call: an entity that does not
		 * come from a builder must also be passed to addEntity() to be
		 * processed by the systems. Removing the entity from the scene does
		 * not remove its name.
		 * @param name The name to file the entity under, unique in the
		 * scene.
		 * @param entity The entity to name.
		 * @throws std::runtime_error When the name is already used.
		 */
		void addRootEntity(const std::string &name,
						   std::shared_ptr<ecs::Entity> entity);

		/**
		 * @brief Find an entity by the name given to addRootEntity().
		 * @tparam EntityType The type the entity is expected to have, for
		 * instance `entities::Button`.
		 * @param name The name the entity was filed under.
		 * @return The entity, or nullptr when the name is unknown or when
		 * the entity is not an `EntityType`.
		 */
		template<ecs::InheritFromEntity EntityType>
		std::shared_ptr<EntityType> getRootEntity(const std::string &name);

		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Access to fonts, images and meshes
		LocalStorage
			&_localStorage;	   ///< The application's persistent storage
		SessionStorage
			&_sessionStorage;	 ///< The application's session storage
		ecs::ComponentRegistry
			_componentRegistry;	   ///< Components of the scene's entities
		std::unique_ptr<ecs::EntityBuilderManager>
			_entityBuilderManager;		   ///< One builder per widget type
		std::type_index _nextSceneType;	   ///< Scene asked for by goToScene();
										   ///< typeid(void) when none
		std::map<std::string, std::shared_ptr<ecs::Entity>>
			_rootEntities;	  ///< Entities named with addRootEntity()
	};

	/**
	 * @brief Constrains a template parameter to a type derived from Scene.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromScene = std::is_base_of_v<Scene, Type>;

	/**
	 * @brief True when no type appears twice in a parameter pack.
	 *
	 * This primary definition covers the empty pack; the specialization
	 * below handles one type or more.
	 * @tparam Types The parameter pack to check.
	 */
	template<typename... Types> inline constexpr bool areDistinct = true;

	/**
	 * @brief Recursive case: the first type differs from every other one and
	 * the remaining types are distinct.
	 * @tparam First The type compared with the rest of the pack.
	 * @tparam Rest The remaining types.
	 */
	template<typename First, typename... Rest>
	inline constexpr bool areDistinct<First, Rest...> =
		(!std::is_same_v<First, Rest> && ...) && areDistinct<Rest...>;

	/**
	 * @brief Constrains a parameter pack to types that are all different.
	 *
	 * Application and SceneManager use it to reject a scene type listed
	 * twice.
	 * @tparam Types The parameter pack to check.
	 */
	template<typename... Types>
	concept AllDistinct = areDistinct<Types...>;

}	 // namespace guillaume

// Include the goToScene template implementation
#include "guillaume/scene.tpp"
