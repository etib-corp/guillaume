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

#include "component_registry.hpp"
#include "ecs/component_type_id.hpp"
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
class Application : protected utility::logging::Loggable<
                        Application<RendererType, EventHandlerType>,
                        utility::logging::StandardLogger>,
                    public SceneApplication {
  private:
    RendererType _renderer;              ///< Main application renderer
    EventHandlerType _eventHandler;      ///< Application event handler
    event::EventBus _eventBus;           ///< Event bus dispatching to systems
    ecs::SystemRegistry _systemRegistry; ///< Shared system registry
    LocalStorage _localStorage;          ///< Local storage for persistent data
    SessionStorage _sessionStorage;      ///< Session storage for temporary data
    std::map<std::string, std::unique_ptr<Scene>>
        _scenes;         ///< Registered scenes
    Scene *_activeScene; ///< Active scene used by ECS runtime

    template <ecs::InheritFromSystem SystemType>
    void registerSystem(std::unique_ptr<SystemType> system) {
        _systemRegistry.registerNewSystem<SystemType>(std::move(system));
        this->getLogger().debug("Registered system: " +
                                std::string(typeid(SystemType).name()));
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
        iterator->second->onApplicationAttached();
        if (inserted && _activeScene == nullptr) {
            _activeScene = iterator->second.get();
            syncSystemsWithActiveScene();
        }
        this->getLogger().debug("Registered scene: " +
                                std::string(typeid(SceneType).name()));
    }

    void syncSystemsWithActiveScene(void) {
        const ecs::Entity::Signature emptySignature;

        for (const auto &[sceneName, scene] : _scenes) {
            (void)sceneName;
            const bool isActive = scene.get() == _activeScene;
            for (const auto &[identityIdentifier, signature] :
                 scene->getEntitySignatures()) {
                _systemRegistry.onEntitySignatureChanged(
                    identityIdentifier, isActive ? signature : emptySignature);
            }
        }
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
    virtual ~Application(void) = default;

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
        _activeScene = &scene;
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
        _activeScene = iterator->second.get();
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
     * @brief Get the active scene component registry.
     * @return Reference to active scene component registry.
     */
    ecs::ComponentRegistry &getComponentRegistry(void) {
        return getActiveScene().getComponentRegistry();
    }

    /**
     * @brief Get the shared system registry.
     * @return Reference to system registry.
     */
    ecs::SystemRegistry &getSystemRegistry(void) { return _systemRegistry; }

    /**
     * @brief Add or replace a component for an entity in the active scene.
     * @tparam ComponentType The component type.
     * @param entity Entity to update.
     * @param args Constructor arguments for the component.
     * @return Reference to the stored component.
     */
    template <ecs::InheritFromComponent ComponentType, typename... Args>
    ComponentType &addComponent(ecs::Entity &entity, Args &&...args) {
        auto &component =
            getComponentRegistry().template addComponent<ComponentType>(
                entity.getIdentifier(), std::forward<Args>(args)...);
        auto signature = entity.getSignature();
        signature.set(ecs::ComponentTypeId::get<ComponentType>());
        entity.setSignature(signature);
        getActiveScene().setEntitySignature(entity.getIdentifier(), signature);
        _systemRegistry.onEntitySignatureChanged(entity.getIdentifier(),
                                                 signature);
        return component;
    }

    /**
     * @brief Remove a component from an entity in the active scene.
     * @tparam ComponentType The component type.
     * @param entity Entity to update.
     */
    template <ecs::InheritFromComponent ComponentType>
    void removeComponent(ecs::Entity &entity) {
        getComponentRegistry().template removeComponent<ComponentType>(
            entity.getIdentifier());
        auto signature = entity.getSignature();
        signature.reset(ecs::ComponentTypeId::get<ComponentType>());
        entity.setSignature(signature);
        getActiveScene().setEntitySignature(entity.getIdentifier(), signature);
        _systemRegistry.onEntitySignatureChanged(entity.getIdentifier(),
                                                 signature);
    }

    /**
     * @brief Check whether an entity has a component in the active scene.
     * @tparam ComponentType Component type to check.
     * @param entity Entity to inspect.
     * @return True when the component exists for the entity.
     */
    template <ecs::InheritFromComponent ComponentType>
    bool hasComponent(const ecs::Entity &entity) const {
        return getActiveScene()
            .getComponentRegistry()
            .template hasComponent<ComponentType>(entity.getIdentifier());
    }

    /**
     * @brief Get a component of an entity from the active scene.
     * @tparam ComponentType Component type.
     * @param entity Entity to inspect.
     * @return Reference to the component.
     */
    template <ecs::InheritFromComponent ComponentType>
    ComponentType &getComponent(ecs::Entity &entity) {
        return getComponentRegistry().template getComponent<ComponentType>(
            entity.getIdentifier());
    }

    /**
     * @brief Get a component of an entity from the active scene (const).
     * @tparam ComponentType Component type.
     * @param entity Entity to inspect.
     * @return Const reference to the component.
     */
    template <ecs::InheritFromComponent ComponentType>
    const ComponentType &getComponent(const ecs::Entity &entity) const {
        return getActiveScene()
            .getComponentRegistry()
            .template getComponent<ComponentType>(entity.getIdentifier());
    }

    /**
     * @brief Run one system update pass for the active scene.
     */
    void routine(void) {
        syncSystemsWithActiveScene();
        auto &componentRegistry = getComponentRegistry();
        for (const auto &[systemType, system] : _systemRegistry.getSystems()) {
            (void)systemType;
            system->routine(componentRegistry);
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
    SessionStorage &getSessionStorage(void) override {
        return _sessionStorage;
    }

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

                // Clear the screen before rendering
                _renderer.clear();

                // Update all ECS systems
                routine();

                // Present the rendered frame
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