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

#include "guillaume/systems/measure_model.hpp"
#include "guillaume/systems/model_render.hpp"

namespace guillaume
{

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
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
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::registerCoreSystems()
	{
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureText>(_ressourceProvider,
												   _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureGlyph>(_ressourceProvider,
													_engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseMotion>(_eventBus, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseButton>(_eventBus, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandMotion>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandButton>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandPinch>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandPoke>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandSqueeze>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandThumbRest>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandThumbStick>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandTrigger>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::TextRender>(_ressourceProvider, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::GlyphRender>(_ressourceProvider,
												   _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::KeyboardControl>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::TextInput>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::RectangleRender>(_engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::Focus>(_eventBus, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureModel>(_ressourceProvider,
													_engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::ModelRender>(_ressourceProvider,
												   _engine));
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	Application<DefaultSceneType, SceneTypes...>::Application(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider)
		: _ressourceProvider(std::move(ressourceProvider))
		, _engine(nullptr)
		, _sceneManager(nullptr)
		, _eventBus()
		, _quitEventSubscriber(_eventBus)
		, _systemRegistry()
		, _systemPhases()
	{
		registerCoreSystems();
		_sceneManager =
			std::make_unique<SceneManager<DefaultSceneType, SceneTypes...>>(_ressourceProvider);
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	Application<DefaultSceneType, SceneTypes...>::~Application(void)
	{
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::setEngine(
		std::unique_ptr<Engine> engine)
	{
		_engine = std::move(engine);
		_engine->setEventCallback(
			[this](std::shared_ptr<utility::event::Event> &event) {
				this->_eventBus.publish(std::move(event));
			});
		std::vector<std::type_index> sceneTypes =
			_sceneManager->getRegisteredSceneTypes();
		for (const auto &sceneType: sceneTypes) {
			_engine->addScene(sceneType.hash_code());
		}
		_sceneManager->setEngine(_engine.get());
		_sceneManager->enterActiveScene();
	}

	static bool hasViewChanged(utility::graphic::ViewF &firstView,
							   utility::graphic::ViewF &currentView)
	{
		auto firstViewForward =
			firstView.getPose().getOrientation().getForward();
		auto currentViewForward =
			currentView.getPose().getOrientation().getForward();

		if (utility::math::dot(utility::math::normalize(firstViewForward),
							   utility::math::normalize(currentViewForward))
			< 0.0f) {
			return true;
		}
		return false;
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::routine(void)
	{
		static auto firstView = _engine->getView();

		_sceneManager->getActiveScene()->placeEntitiesInFrontOfView(firstView);
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
		_sceneManager->processSceneTransition();
		_engine->present();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::pollEvents(void)
	{
		_engine->pollEvents();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::update(void)
	{
		_engine->update();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::clear(void)
	{
		_engine->clear();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void Application<DefaultSceneType, SceneTypes...>::present(void)
	{
		_engine->present();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	bool Application<DefaultSceneType, SceneTypes...>::shouldQuit(void)
	{
		if (_quitEventSubscriber.hasPendingEvents()) {
			_quitEventSubscriber.getNextEvent();
			return true;
		}
		return false;
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
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
