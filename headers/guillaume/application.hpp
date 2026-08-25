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

#include "guillaume/ecs/system_phase.hpp"
#include "guillaume/ecs/system_registry.hpp"

#include "guillaume/metadata.hpp"
#include "guillaume/engine.hpp"
#include "guillaume/scene_manager.hpp"

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_subscriber.hpp"

#include "guillaume/systems/hand_button.hpp"
#include "guillaume/systems/hand_motion.hpp"
#include "guillaume/systems/hand_pinch.hpp"
#include "guillaume/systems/hand_poke.hpp"
#include "guillaume/systems/hand_squeeze.hpp"
#include "guillaume/systems/hand_thumb_rest.hpp"
#include "guillaume/systems/hand_thumb_stick.hpp"
#include "guillaume/systems/hand_trigger.hpp"
#include "guillaume/systems/mouse_button.hpp"
#include "guillaume/systems/mouse_motion.hpp"
#include "guillaume/systems/keyboard_control.hpp"
#include "guillaume/systems/measure_glyph.hpp"
#include "guillaume/systems/measure_text.hpp"
#include "guillaume/systems/rectangle_render.hpp"
#include "guillaume/systems/text_input.hpp"
#include "guillaume/systems/text_render.hpp"
#include "guillaume/systems/glyph_render.hpp"
#include "guillaume/systems/focus.hpp"

namespace guillaume
{
	/**
	 * @brief Application base class.
	 *
	 * @tparam DefaultSceneType The default scene type to activate when the
	 * application starts. Must be one of the registered SceneTypes.
	 * @tparam SceneTypes Variadic template parameter pack for scene types used
	 * in the application. Each scene type must inherit from the Scene class.
	 *
	 * @code
	 * class MyEngine : public Engine { ... };
	 * Application<DefaultScene, Scene1, Scene2> app;
	 * return app.run();
	 * @endcode
	 *
	 * @see ecs::SystemRegistry
	 * @see event::EventBus
	 * @see Engine
	 */
	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	class Application:
		protected utility::logging::Loggable<
			Application<DefaultSceneType, SceneTypes...>,
			utility::logging::DefaultLogger>
	{
		private:
		std::unique_ptr<Engine>
			_engine;	///< Unique pointer to the application engine
		std::unique_ptr<SceneManager<DefaultSceneType, SceneTypes...>>
			_sceneManager;			  ///< Manager for application scenes
		event::EventBus _eventBus;	  ///< Event bus dispatching to systems
		event::EventSubscriber<utility::event::QuitEvent>
			_quitEventSubscriber;				///< Subscriber for quit events
		ecs::SystemRegistry _systemRegistry;	///< Shared system registry
		ecs::SystemPhaseList _systemPhases;		///< Ordered list of phases and
												///< traversal strategies
		///< OS-level interactions
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared
								   ///< ressource
								   ///< provider

		/**
		 * @brief Run one system update pass for the active scene.
		 * @param phaseDefinition The system phase definition to run.
		 */
		template<typename PhaseType> void runPhase(PhaseType &phaseDefinition);

		/**
		 * @brief Register core systems used by the application.
		 */
		void registerCoreSystems(void);

		protected:
		/**
		 * @brief Get a shared pointer to the resource provider.
		 * @return Shared pointer to the resource provider.
		 */
		std::shared_ptr<utility::RessourceProvider> getRessourceProvider(void)
		{
			return _ressourceProvider;
		}

		public:
		/**
		 * @brief Construct an application with a shared resource provider.
		 * @param ressourceProvider Shared pointer to the resource provider to
		 * be used by the application. This allows for sharing resources across
		 * different parts of the application, such as systems and scenes, while
		 * maintaining a single source of truth for resource management.
		 */
		Application(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider);

		/**
		 * @brief Default destructor
		 */
		virtual ~Application(void);

		/**
		 * @brief Set the engine for the application.
		 * @param engine The engine to be used by the application.
		 */
		void setEngine(std::unique_ptr<Engine> engine);

		/**
		 * @brief Run one system update pass for the active scene.
		 */
		void routine(void);

		/**
		 * @brief Poll events from the engine.
		 */
		void pollEvents(void);

		/**
		 * @brief Update the engine state.
		 */
		void update(void);

		/**
		 * @brief Clear the engine rendering target.
		 */
		void clear(void);

		/**
		 * @brief Present the rendered frame.
		 */
		void present(void);

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
	};

}	 // namespace guillaume

// Include the implementation of the Application template class
#include "guillaume/application.tpp"