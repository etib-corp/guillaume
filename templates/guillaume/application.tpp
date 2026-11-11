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

#include <functional>


namespace guillaume
{

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	template<typename PhaseType>
	void Application<DefaultSceneType, SceneTypes...>::runPhase(
		PhaseType &phaseDefinition)
	{
		const ecs::Phase phase					= phaseDefinition.getPhase();
		const ecs::EntityTreeTraveler &traveler = phaseDefinition.getTraveler();

		this->getLogger().debug()
			<< "Running systems for phase: " << static_cast<int>(phase);
		for (const auto &system: _systemRegistry.getSystemsByPhase(phase)) {
			system->routine(_sceneManager->getActiveComponentRegistry(),
							_sceneManager->getActiveEntityRegistry(), traveler);
		}
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	std::unique_ptr<utility::Engine>
		Application<DefaultSceneType, SceneTypes...>::requireEngine(
			std::unique_ptr<utility::Engine> engine)
	{
		if (engine == nullptr) {
			throw std::invalid_argument(
				"Application requires a non-null engine");
		}
		return engine;
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	Application<DefaultSceneType, SceneTypes...>::Application(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<utility::Engine> engine)
		: _engine(requireEngine(std::move(engine)))
		, _sceneManager(nullptr)
		, _eventBus()
		, _quitEventSubscriber(_eventBus)
		, _systemRegistry()
		, _systemPhases()
		, _ressourceProvider(std::move(ressourceProvider))
	{
		systems::registerCoreSystems(_systemRegistry, _eventBus, *_engine,
									 _ressourceProvider);
		_engine->setEventCallback(
			[this](std::shared_ptr<utility::event::Event> &event) {
				this->_eventBus.publish(std::move(event));
			});
		_sceneManager =
			std::make_unique<SceneManager<DefaultSceneType, SceneTypes...>>(
				_ressourceProvider, *_engine);
		_sceneManager->enterActiveScene();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	Application<DefaultSceneType, SceneTypes...>::~Application(void)
	{
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::routine(void)
	{
		// Entities are placed once per scene entry (see
		// SceneManager::enterActiveScene) and are deliberately left untouched
		// afterwards: re-placing them on view changes would move and rotate
		// them, defeating the render caches keyed on entity state and making
		// UI jump when the view turns.
		this->getLogger().debug() << "Current view: " << _engine->getView();
		this->getLogger().debug()
			<< "Forward vector: "
			<< _engine->getView().getPose().getOrientation().getForward();

		_engine->pollEvents();
		_engine->clear();
		_engine->update();
		std::apply(
			[this](auto &...phaseDefinition) {
				(this->runPhase(phaseDefinition), ...);
			},
			_systemPhases);
		_sceneManager->getActiveComponentRegistry().resetChangedFlags();
		_sceneManager->processSceneTransition();
		_engine->present();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	bool Application<DefaultSceneType, SceneTypes...>::shouldQuit(void)
	{
		if (_quitEventSubscriber.hasPendingEvents()) {
			_quitEventSubscriber.getNextEvent();
			return true;
		}
		return false;
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires AllDistinct<DefaultSceneType, SceneTypes...>
	int Application<DefaultSceneType, SceneTypes...>::run(void)
	{
		this->getLogger().info() << "Entering main loop";
		while (!shouldQuit()) {
			try {
				routine();
			} catch (const std::exception &exception) {
				this->getLogger().error()
					<< std::string("Application error: ") << exception.what();
				return EXIT_FAILURE;
			}
		}
		this->getLogger().info() << "Exiting main loop";
		return EXIT_SUCCESS;
	}
}	 // namespace guillaume
