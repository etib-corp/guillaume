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
#include <tuple>
#include <typeinfo>
#include <utility>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/event/quit_event.hpp>

#include <utility/ressource_provider.hpp>

#include <utility/system_io/default_system_io.hpp>

#include <utility/demangle.hpp>

#include <utility/graphic/view.hpp>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_phase.hpp"
#include "guillaume/ecs/system_registry.hpp"

#include "guillaume/metadata.hpp"
#include "guillaume/scene_manager.hpp"

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_subscriber.hpp"

#include "guillaume/systems/core_systems.hpp"

namespace guillaume
{
	/**
	 * @brief Application base class.
	 *
	 * Owns the engine, the scene manager, the event bus and the system
	 * registry, and drives the frame loop.
	 *
	 * @tparam DefaultSceneType The scene activated when the application
	 * starts.
	 * @tparam SceneTypes The other scenes of the application. Every scene type
	 * must inherit from Scene and appear only once.
	 *
	 * @code
	 * Application<HomeScene, SettingsScene> app(
	 *     resources, std::make_unique<MyEngine>());
	 * return app.run();
	 * @endcode
	 *
	 * @see ecs::SystemRegistry
	 * @see event::EventBus
	 * @see utility::Engine
	 */
	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	class Application:
		protected utility::logging::Loggable<
			Application<DefaultSceneType, SceneTypes...>,
			utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief Construct an application.
		 *
		 * Registers the core systems, creates the scenes and enters the
		 * default scene.
		 * @param ressourceProvider Shared pointer to the resource provider
		 * used by the systems and the scenes.
		 * @param engine The engine that renders and polls events. The
		 * application takes ownership.
		 * @throws std::invalid_argument If the engine is null.
		 */
		Application(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<utility::Engine> engine);

		/**
		 * @brief Default destructor
		 */
		virtual ~Application(void);

		/**
		 * @brief Run one frame: poll events, clear, update the engine, run the
		 * system phases, process a pending scene transition and present.
		 */
		void routine(void);

		/**
		 * @brief Check if the application should quit (using engine).
		 * @return True if the application should quit, false otherwise.
		 */
		bool shouldQuit(void);

		/**
		 * @brief Run the application main loop
		 * @return Exit code.
		 */
		int run(void);

		protected:
		/**
		 * @brief Get a shared pointer to the resource provider.
		 * @return Shared pointer to the resource provider.
		 */
		std::shared_ptr<utility::RessourceProvider> getRessourceProvider(void)
		{
			return _ressourceProvider;
		}

		private:
		/**
		 * @brief Run one system update pass for the active scene.
		 * @param phaseDefinition The system phase definition to run.
		 */
		template<typename PhaseType> void runPhase(PhaseType &phaseDefinition);

		/**
		 * @brief Reject a null engine before any member depends on it.
		 * @param engine The engine handed to the constructor.
		 * @return The same engine.
		 * @throws std::invalid_argument If the engine is null.
		 */
		static std::unique_ptr<utility::Engine>
			requireEngine(std::unique_ptr<utility::Engine> engine);

		std::unique_ptr<utility::Engine>
			_engine;	///< Unique pointer to the application engine
		std::unique_ptr<SceneManager<DefaultSceneType, SceneTypes...>>
			_sceneManager;			  ///< Manager for application scenes
		event::EventBus _eventBus;	  ///< Event bus dispatching to systems
		event::EventSubscriber<utility::event::QuitEvent>
			_quitEventSubscriber;				///< Subscriber for quit events
		ecs::SystemRegistry _systemRegistry;	///< Shared system registry
		ecs::SystemPhaseList _systemPhases;		///< Ordered list of phases and
												///< traversal strategies
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared ressource provider
	};

}	 // namespace guillaume

// Include the implementation of the Application template class
#include "guillaume/application.tpp"