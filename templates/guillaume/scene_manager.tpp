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

#include <utility/demangle.hpp>

#include "guillaume/scene_manager.hpp"
#include "guillaume/engine.hpp"

namespace guillaume
{
	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	SceneManager<DefaultSceneType, SceneTypes...>::SceneManager(std::shared_ptr<utility::RessourceProvider> ressourceProvider)
		: _ressourceProvider(ressourceProvider)
		, _scenes()
		, _activeSceneType(typeid(void))
		, _engine(nullptr)
	{
		if constexpr (sizeof...(SceneTypes) == 0) {
			throw std::invalid_argument(
				"SceneManager requires at least one scene type");
		}
		(addScene<SceneTypes>(), ...);
		_activeSceneType = std::type_index(typeid(DefaultSceneType));
		this->getLogger().info() << "SceneManager initialized";
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	SceneManager<DefaultSceneType, SceneTypes...>::~SceneManager(void)
	{
		this->getLogger().info() << "SceneManager destroyed with "
								 << _scenes.size() << " registered scene(s)";
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	std::unique_ptr<Scene> &
		SceneManager<DefaultSceneType, SceneTypes...>::getActiveScene(void)
	{
		if (_scenes.empty()) {
			this->getLogger().error() << "Cannot activate scene: no scenes are "
										 "registered";
			throw std::runtime_error(
				"SceneManager requires at least one registered scene before "
				"accessing the active scene");
		}

		if (_activeSceneType == typeid(void)) {
			_activeSceneType = _scenes.begin()->first;
			this->getLogger().info()
				<< "No active scene set. Defaulting to first "
				   "registered scene";
		}
		return _scenes[_activeSceneType];
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	bool SceneManager<DefaultSceneType, SceneTypes...>::hasScenes(
		void) const noexcept
	{
		return !_scenes.empty();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	bool SceneManager<DefaultSceneType, SceneTypes...>::hasActiveScene(
		void) const noexcept
	{
		return _activeSceneType != typeid(void) && !_scenes.empty();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	ecs::EntityRegistry &
		SceneManager<DefaultSceneType, SceneTypes...>::getActiveEntityRegistry(
			void)
	{
		return getActiveScene()->getEntityRegistry();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	ecs::ComponentRegistry &
		SceneManager<DefaultSceneType,
					 SceneTypes...>::getActiveComponentRegistry(void)
	{
		return getActiveScene()->getComponentRegistry();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	std::vector<std::type_index>
		SceneManager<DefaultSceneType, SceneTypes...>::getRegisteredSceneTypes(
			void) const
	{
		std::vector<std::type_index> sceneTypes;
		for (const auto &entry: _scenes) {
			sceneTypes.push_back(entry.first);
		}
		return sceneTypes;
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	template<InheritFromScene SceneType>
	void SceneManager<DefaultSceneType, SceneTypes...>::addScene(void)
	{
		std::type_index typeIndex(typeid(SceneType));
		_scenes[typeIndex] =
			std::make_unique<SceneType>(_ressourceProvider, _localStorage, _sessionStorage);
		this->getLogger().info()
			<< "Registered scene type: " << utility::demangle<SceneType>();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void
		SceneManager<DefaultSceneType, SceneTypes...>::setEngine(Engine *engine)
	{
		_engine = engine;
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void SceneManager<DefaultSceneType, SceneTypes...>::enterActiveScene(void)
	{
		std::unique_ptr<Scene> &activeScene = getActiveScene();
		if (_engine != nullptr) {
			activeScene->placeEntitiesInFrontOfView(_engine->getView());
		}
		activeScene->onEnter();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	void SceneManager<DefaultSceneType, SceneTypes...>::processSceneTransition(
		void)
	{
		std::unique_ptr<Scene> &activeScene = getActiveScene();

		if (!activeScene->wantsToSwitch()) {
			return;
		}

		std::type_index nextType = activeScene->getNextSceneType();

		if (_scenes.find(nextType) == _scenes.end()) {
			this->getLogger().error()
				<< "Scene transition failed. Target scene type is not "
				   "registered: "
				<< utility::demangle(nextType.name());
			activeScene->clearNextScene();
			throw std::runtime_error("Scene transition failed: target scene "
									 "not found in scene manager");
		}

		this->getLogger().info() << "Transitioning from active scene to type: "
								 << utility::demangle(nextType.name());

		activeScene->onExit();
		activeScene->clearNextScene();
		_activeSceneType = nextType;
		enterActiveScene();
	}

	template<InheritFromScene DefaultSceneType, InheritFromScene... SceneTypes>
		requires IsOneOf<DefaultSceneType, SceneTypes...>
	template<InheritFromScene SceneType>
	void SceneManager<DefaultSceneType, SceneTypes...>::switchToScene(void)
	{
		std::type_index typeIndex(typeid(SceneType));
		if (_scenes.find(typeIndex) == _scenes.end()) {
			this->getLogger().error()
				<< "Scene switch failed. Scene type is not "
				   "registered: "
				<< utility::demangle<SceneType>();
			throw std::runtime_error("Scene not found in scene manager");
		}
		_activeSceneType = typeIndex;
		this->getLogger().info() << "Switched active scene to type: "
								 << utility::demangle<SceneType>();
	}
}	 // namespace guillaume
