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
#include <typeinfo>
#include <utility>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include <utility/demangle.hpp>

#include "ecs/system_registry.hpp"
#include "metadata.hpp"
#include "renderer.hpp"
#include "scene.hpp"

#include "event/event_bus.hpp"
#include "event/event_handler.hpp"

#include "systems/click.hpp"
#include "systems/hover.hpp"
#include "systems/keyboard_control.hpp"
#include "systems/render.hpp"
#include "systems/text_input.hpp"

#include "local_storage.hpp"
#include "session_storage.hpp"

namespace guillaume {

/**
 * @brief Application base class.
 *
 * @tparam RendererType The type of the renderer used by the application.
 * @tparam EventHandlerType The type of the event handler used by the
 * application.
 *
 * @code
 * class MyRenderer : public Renderer { ... };
 * class MyEventHandler : public event::EventHandler { ... };
 * Application<MyRenderer, MyEventHandler> app;
 * return app.run();
 * @endcode
 *
 * @see ecs::SystemRegistry
 * @see event::EventBus
 * @see event::EventHandler
 * @see Renderer
 */
template <InheritFromRenderer RendererType,
          event::InheritFromEventHandler EventHandlerType,
          InheritFromScene... SceneTypes>
class Application
    : protected utility::logging::Loggable<
          Application<RendererType, EventHandlerType, SceneTypes...>,
          utility::logging::StandardLogger>,
      public SceneApplication {
  private:
    enum class SceneLifecycleState {
        Created,
        Started,
        Paused,
        Stopped,
    };

    RendererType _renderer;              ///< Main application renderer
    EventHandlerType _eventHandler;      ///< Application event handler
    event::EventBus _eventBus;           ///< Event bus dispatching to systems
    ecs::SystemRegistry _systemRegistry; ///< Shared system registry
    LocalStorage _localStorage;          ///< Local storage for persistent data
    SessionStorage _sessionStorage;      ///< Session storage for temporary data
    std::map<std::string, std::unique_ptr<Scene>>
        _scenes; ///< Registered scenes
    std::map<Scene *, SceneLifecycleState>
        _sceneStates;    ///< Runtime lifecycle state for each scene
    Scene *_activeScene; ///< Active scene used by ECS runtime

    void startOrResumeScene(Scene &scene) {
        auto &state = _sceneStates.at(&scene);
        switch (state) {
        case SceneLifecycleState::Created:
            scene.onStart();
            state = SceneLifecycleState::Started;
            break;
        case SceneLifecycleState::Paused:
            scene.onResume();
            state = SceneLifecycleState::Started;
            break;
        case SceneLifecycleState::Stopped:
            scene.onRestart();
            state = SceneLifecycleState::Started;
            break;
        case SceneLifecycleState::Started:
            break;
        }
    }

    void stopScene(Scene &scene) {
        auto &state = _sceneStates.at(&scene);
        if (state == SceneLifecycleState::Started) {
            scene.onPause();
            state = SceneLifecycleState::Paused;
        }
        if (state == SceneLifecycleState::Paused) {
            scene.onStop();
            state = SceneLifecycleState::Stopped;
        }
    }

    void activateScene(Scene &scene) {
        if (&scene == _activeScene) {
            return;
        }
        if (_activeScene != nullptr) {
            stopScene(*_activeScene);
        }
        startOrResumeScene(scene);
        _activeScene = &scene;
    }

    template <ecs::InheritFromSystem SystemType>
    void registerSystem(std::unique_ptr<SystemType> system) {
        _systemRegistry.registerNewSystem<SystemType>(std::move(system));
        this->getLogger().debug("Registered system: " +
                                utility::demangle<SystemType>());
    }

    void registerCoreSystems(void) {
        registerSystem<systems::Click>(
            std::make_unique<systems::Click>(_eventBus, _renderer));
        registerSystem<systems::Hover>(
            std::make_unique<systems::Hover>(_eventBus, _renderer));
        registerSystem<systems::Render>(
            std::make_unique<systems::Render>(_renderer));
        registerSystem<systems::KeyboardControl>(
            std::make_unique<systems::KeyboardControl>(_eventBus));
        registerSystem<systems::TextInput>(
            std::make_unique<systems::TextInput>(_eventBus));
    }

    template <InheritFromScene SceneType> void registerScene(void) {
        auto [iterator, inserted] = _scenes.emplace(
            typeid(SceneType).name(), std::make_unique<SceneType>());
        iterator->second->setApplication(*this);
        if (inserted) {
            auto &scene = *iterator->second;
            scene.onCreate();
            _sceneStates.emplace(&scene, SceneLifecycleState::Created);
        }
        if (inserted && _activeScene == nullptr) {
            activateScene(*iterator->second);
        }
        this->getLogger().debug("Registered scene: " +
                                utility::demangle<SceneType>());
    }

  public:
    /**
     * @brief Default constructor
     */
    Application(void) : _eventHandler(), _eventBus(), _activeScene(nullptr) {
        registerCoreSystems();
        _eventHandler.setEventCallback(
            [this](std::unique_ptr<utility::event::Event> &event) {
                this->_eventBus.publish(std::move(event));
            });
        (registerScene<SceneTypes>(), ...);
    }

    /**
     * @brief Default destructor
     */
    virtual ~Application(void) {
        for (auto &[sceneName, scene] : _scenes) {
            (void)sceneName;
            try {
                if (_sceneStates.contains(scene.get())) {
                    stopScene(*scene);
                }
                scene->onDestroy();
            } catch (const std::exception &exception) {
                this->getLogger().error(std::string("Scene shutdown error: ") +
                                        exception.what());
            } catch (...) {
                this->getLogger().error("Unknown scene shutdown error");
            }
        }
    }

    /**
     * @brief Get a registered scene by type.
     * @tparam SceneType Scene type to retrieve.
     * @return Reference to the requested scene.
     */
    template <InheritFromScene SceneType> SceneType &getScene(void) {
        const auto iterator = _scenes.find(typeid(SceneType).name());
        if (iterator == _scenes.end()) {
            throw std::out_of_range("Requested scene type is not registered");
        }
        return *static_cast<SceneType *>(iterator->second.get());
    }

    /**
     * @brief Set the active scene by type.
     * @tparam SceneType Scene type to activate.
     */
    template <InheritFromScene SceneType> void setActiveScene(void) {
        auto &scene = getScene<SceneType>();
        activateScene(scene);
    }

    /**
     * @brief Request a scene change by scene type name.
     * @param sceneTypeName Scene key generated from typeid(SceneType).name().
     */
    void requestSceneChange(const std::string &sceneTypeName) override {
        const auto iterator = _scenes.find(sceneTypeName);
        if (iterator == _scenes.end()) {
            throw std::out_of_range("Requested scene type is not registered");
        }
        activateScene(*iterator->second);
    }

    /**
     * @brief Get the current active scene.
     * @return Reference to the active scene.
     */
    Scene &getActiveScene(void) {
        if (_activeScene == nullptr) {
            throw std::runtime_error("No active scene is registered");
        }
        return *_activeScene;
    }

    /**
     * @brief Get the current active scene (const).
     * @return Const reference to the active scene.
     */
    const Scene &getActiveScene(void) const {
        if (_activeScene == nullptr) {
            throw std::runtime_error("No active scene is registered");
        }
        return *_activeScene;
    }

    /**
     * @brief Get the shared system registry.
     * @return Reference to system registry.
     */
    ecs::SystemRegistry &getSystemRegistry(void) { return _systemRegistry; }

    /**
     * @brief Run one system update pass for the active scene.
     */
    void routine(void) {
        auto &entityRegistry = getActiveScene().getEntityRegistry();
        auto &componentRegistry = getActiveScene().getComponentRegistry();
        for (const auto &[systemType, system] : _systemRegistry.getSystems()) {
            (void)systemType;
            system->routine(componentRegistry, entityRegistry);
        }
    }

    /**
     * @brief Get the local storage instance.
     * @return Reference to the LocalStorage instance.
     */
    LocalStorage &getLocalStorage(void) override { return _localStorage; }

    /**
     * @brief Get the session storage instance.
     * @return Reference to the SessionStorage instance.
     */
    SessionStorage &getSessionStorage(void) override { return _sessionStorage; }

    /**
     * @brief Run the application main loop.
     * @return Exit code.
     */
    int run(void) {
        this->getLogger().info("Entering main loop");
        while (!_eventHandler.shouldQuit()) {
            try {
                _eventHandler.pollEvents();
                if (!_eventHandler.gotNewEvents()) {
                    continue;
                }
                _renderer.clear();
                routine();
                _renderer.present();
                this->getLogger().debug("Processed a frame");
            } catch (const std::exception &exception) {
                this->getLogger().error(std::string("Application error: ") +
                                        exception.what());
                return EXIT_FAILURE;
            }
        }
        this->getLogger().info("Exiting main loop");
        return EXIT_SUCCESS;
    }
};

} // namespace guillaume