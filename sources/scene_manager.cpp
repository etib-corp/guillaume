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

#include "guillaume/scene_manager.hpp"

namespace guillaume
{
	SceneManager::SceneManager(void)
		: _scenes()
		, _activeSceneType(typeid(void))
	{
		getLogger().info("SceneManager initialized");
	}

	SceneManager::~SceneManager(void)
	{
		getLogger().info("SceneManager destroyed with "
						 + std::to_string(_scenes.size())
						 + " registered scene(s)");
	}

	std::unique_ptr<Scene> &SceneManager::getActiveScene(void)
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

	ecs::EntityRegistry &SceneManager::getActiveEntityRegistry(void)
	{
		return getActiveScene()->getEntityRegistry();
	}

	ecs::ComponentRegistry &SceneManager::getActiveComponentRegistry(void)
	{
		return getActiveScene()->getComponentRegistry();
	}

}	 // namespace guillaume