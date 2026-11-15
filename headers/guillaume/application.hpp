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
	 * @brief The entry point of a program: owns the engine, the scenes and
	 * the systems, and runs the frame loop.
	 *
	 * An application is declared with the list of its scene types. It
	 * creates one instance of each, enters the first one, and then repeats
	 * routine() until the engine reports a quit event. The engine
	 * (`utility::Engine`) is the platform layer the program provides: it
	 * opens the window or the XR session, draws, and reports input.
	 *
	 * A minimal program:
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
	 * 		builder.makeButton(nullptr, "Save", [] {},
	 * 			guillaume::entities::Button::Color::Filled,
	 * 			guillaume::entities::Button::Shape::Round,
	 * 			guillaume::entities::Button::Size::Medium, false);
	 * 	}
	 * };
	 *
	 * int main(void)
	 * {
	 * 	utility::DefaultSystemIO systemIo;
	 * 	auto resources =
	 * 		std::make_shared<utility::RessourceProvider>(systemIo);
	 *
	 * 	// MyEngine is the program's implementation of utility::Engine.
	 * 	guillaume::Application<HomeScene> app(
	 * 		resources, std::make_unique<MyEngine>());
	 * 	return app.run();
	 * }
	 * @endcode
	 *
	 * **One frame** (routine()), in order:
	 * 1. the engine polls input; each event is published on the event bus
	 *    and queued by the systems that subscribed to its type;
	 * 2. the engine clears the frame and updates;
	 * 3. the four phases run on the active scene: Event, Measure, Layout,
	 *    Render. In each phase, every system of the phase runs once over
	 *    the whole scene (ecs::System::routine);
	 * 4. the changed flags of the active scene's components are reset
	 *    (ecs::ComponentRegistry::resetChangedFlags);
	 * 5. a scene switch requested with Scene::goToScene is carried out;
	 * 6. the engine presents the frame.
	 *
	 * Things to watch out for:
	 * - Systems belong to the application and are shared by all scenes;
	 *   components and entities belong to each scene.
	 * - Only the active scene is processed.
	 * - The constructor already enters the default scene, so
	 *   `DefaultSceneType::onEnter` runs before run() is called.
	 *
	 * @tparam DefaultSceneType The scene activated when the application
	 * starts.
	 * @tparam SceneTypes The other scenes of the application. Every scene
	 * type must inherit from Scene, appear only once, and be constructible
	 * from the arguments of the Scene constructor.
	 *
	 * @see Scene
	 * @see ecs::System::routine
	 * @see systems::registerCoreSystems
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
		 * @brief Build the application and enter the default scene.
		 *
		 * In order: registers the core systems
		 * (systems::registerCoreSystems), routes the engine's events to the
		 * event bus, creates one instance of every scene type, then calls
		 * `onEnter()` on the default scene and places its root entities in
		 * front of the engine's view.
		 * @param ressourceProvider Gives access to the files the systems and
		 * the scenes load (fonts, images, meshes). Shared with them.
		 * @param engine The engine that renders and polls events. The
		 * application takes ownership.
		 * @throws std::invalid_argument When the engine is null.
		 */
		Application(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<utility::Engine> engine);

		/**
		 * @brief Destroy the application with its scenes, systems and
		 * engine.
		 */
		virtual ~Application(void);

		/**
		 * @brief Run one frame.
		 *
		 * Polls the engine's events, clears and updates the engine, runs the
		 * Event, Measure, Layout and Render phases on the active scene,
		 * resets the changed flags of its components, carries out a pending
		 * scene switch and presents the frame. See the class description.
		 *
		 * run() calls it in a loop; call it directly to drive the loop from
		 * outside. Exceptions thrown by a system, a handler or a scene
		 * switch to an unknown scene propagate to the caller.
		 */
		void routine(void);

		/**
		 * @brief Tell whether a quit event was received since the last call.
		 *
		 * The pending quit event is consumed: a second call returns false
		 * until the engine reports another one.
		 * @return True when the engine reported a quit event (window closed,
		 * for instance).
		 */
		bool shouldQuit(void);

		/**
		 * @brief Run frames until a quit event is received.
		 * @return `EXIT_SUCCESS` after a quit event; `EXIT_FAILURE` when a
		 * frame threw a `std::exception`, which is logged and ends the loop.
		 */
		int run(void);

		protected:
		/**
		 * @brief Get the resource provider given to the constructor.
		 * @return The shared resource provider.
		 */
		std::shared_ptr<utility::RessourceProvider> getRessourceProvider(void)
		{
			return _ressourceProvider;
		}

		private:
		/**
		 * @brief Run every system of one phase on the active scene, in
		 * registration order, with the traveler of the phase.
		 * @tparam PhaseType One of the ecs::SystemPhase aliases
		 * (ecs::EventPhase, ...).
		 * @param phaseDefinition The phase to run and its traveler.
		 */
		template<typename PhaseType> void runPhase(PhaseType &phaseDefinition);

		/**
		 * @brief Reject a null engine before any member depends on it.
		 * @param engine The engine handed to the constructor.
		 * @return The same engine.
		 * @throws std::invalid_argument When the engine is null.
		 */
		static std::unique_ptr<utility::Engine>
			requireEngine(std::unique_ptr<utility::Engine> engine);

		std::unique_ptr<utility::Engine>
			_engine;	///< Platform layer: rendering and input
		std::unique_ptr<SceneManager<DefaultSceneType, SceneTypes...>>
			_sceneManager;	  ///< Owns the scenes and knows the active one
		event::EventBus
			_eventBus;	  ///< Delivers engine events to the subscribed systems
		event::EventSubscriber<utility::event::QuitEvent>
			_quitEventSubscriber;	 ///< Queue of quit events, read by
									 ///< shouldQuit()
		ecs::SystemRegistry
			_systemRegistry;	///< The systems, shared by every scene
		ecs::SystemPhaseList _systemPhases;	   ///< The phases in running
											   ///< order, each with its
											   ///< traveler
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Access to fonts, images and meshes
	};

}	 // namespace guillaume

// Include the implementation of the Application template class
#include "guillaume/application.tpp"