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

#include <stdexcept>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include "ecs/component_registry.hpp"
#include "ecs/entity_registry.hpp"

namespace guillaume {

class LocalStorage;
class SessionStorage;

/**
 * @brief Application context exposed to scenes.
 */
class SceneApplication {
  public:
    virtual ~SceneApplication(void) = default;

    /**
     * @brief Request a scene switch by scene type name key.
     * @param sceneTypeName Scene key generated from typeid(SceneType).name().
     */
    virtual void requestSceneChange(const std::string &sceneTypeName) = 0;

    /**
     * @brief Get local storage from the owning application.
     * @return Reference to local storage.
     */
    virtual LocalStorage &getLocalStorage(void) = 0;

    /**
     * @brief Get session storage from the owning application.
     * @return Reference to session storage.
     */
    virtual SessionStorage &getSessionStorage(void) = 0;
};

/**
 * @brief Represents a scene in the application, which can contain entities and
 * systems. Scenes can be switched to change the active content.
 */
class Scene
    : public utility::logging::Loggable<Scene,
                                        utility::logging::StandardLogger> {
  private:
    ecs::ComponentRegistry
        _componentRegistry;              ///< Component registry for the scene
    ecs::EntityRegistry _entityRegistry; ///< Entity registry for the scene
    SceneApplication *_application; ///< Application context attached to scene

  protected:
    /**
     * @brief Get the attached application context.
     * @return Reference to application context.
     */
    SceneApplication &getApplication(void) {
        if (_application == nullptr) {
            throw std::runtime_error("Scene is not attached to an application");
        }
        return *_application;
    }

    /**
     * @brief Get local storage from the attached application.
     * @return Reference to local storage.
     */
    LocalStorage &getLocalStorage(void) {
        return getApplication().getLocalStorage();
    }

    /**
     * @brief Get session storage from the attached application.
     * @return Reference to session storage.
     */
    SessionStorage &getSessionStorage(void) {
        return getApplication().getSessionStorage();
    }

  public:
    /**
     * @brief Default constructor for Scene.
     */
    Scene(void);

    /**
     * @brief Default destructor for Scene.
     */
    virtual ~Scene(void);

    /**
     * @brief Attach the application context to this scene.
     * @param application Application context.
     */
    void setApplication(SceneApplication &application) {
        _application = &application;
    }

    /**
     * @brief Get the component registry for this scene.
     * @return Reference to the component registry.
     */
    ecs::ComponentRegistry &getComponentRegistry(void) {
        return _componentRegistry;
    }

    /**
     * @brief Get the entity registry for this scene.
     * @return Reference to the entity registry.
     */
    ecs::EntityRegistry &getEntityRegistry(void) { return _entityRegistry; }

    /**
     * @brief Callback invoked when the scene is created. Override to perform
     * initialization logic.
     */
    virtual void onCreate(void);

    /**
     * @brief Callback invoked when the scene starts. Override to perform logic
     * that should run when the scene becomes visible.
     */
    virtual void onStart(void);

    /**
     * @brief Callback invoked when the scene is paused. Override to perform
     * logic that should run when the scene loses focus but is still partially
     * visible. This may occur when another scene is overlaid on top of this
     * one.
     */
    virtual void onPause(void);

    /**
     * @brief Callback invoked when the scene resumes from a paused state.
     * Override to perform logic that should run when the scene regains focus
     * after being paused.
     */
    virtual void onResume(void);

    /**
     * @brief Callback invoked when the scene stops. Override to perform cleanup
     * logic that should run when the scene is no longer visible.
     */
    virtual void onStop(void);

    /**
     * @brief Callback invoked when the scene is restarted. Override to perform
     * logic that should run when the scene is restarted after being stopped.
     */
    virtual void onRestart(void);

    /**
     * @brief Callback invoked when the scene is destroyed. Override to perform
     * final cleanup logic before the scene is removed from memory.
     */
    virtual void onDestroy(void);
};

/**
 * @brief Concept to ensure a type inherits from Scene.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromScene = std::is_base_of_v<Scene, Type>;

} // namespace guillaume
