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
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "ecs/component_registry.hpp"
#include "ecs/entity.hpp"

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
class Scene {
  private:
    ecs::ComponentRegistry
        _componentRegistry; ///< Component registry for the scene
    std::vector<ecs::Entity::Identifier>
        _entities; ///< Entity identifiers owned by the scene
    std::map<ecs::Entity::Identifier, ecs::Entity::Signature>
        _entitySignatures;          ///< Entity signatures tracked per scene
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

    /**
     * @brief Request a scene switch.
     * @tparam SceneType Target scene type.
     */
    template <typename SceneType> void setActiveScene(void) {
        getApplication().requestSceneChange(typeid(SceneType).name());
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
     * @brief Called once after the scene has been attached to the application.
     */
    virtual void onApplicationAttached(void) {}

    /**
     * @brief Get the scene component registry.
     * @return Reference to the component registry.
     */
    ecs::ComponentRegistry &getComponentRegistry(void) {
        return _componentRegistry;
    }

    /**
     * @brief Get the scene component registry (const).
     * @return Const reference to the component registry.
     */
    const ecs::ComponentRegistry &getComponentRegistry(void) const {
        return _componentRegistry;
    }

    /**
     * @brief Register an entity as part of the scene.
     * @param entity The entity to register.
     */
    void addEntity(ecs::Entity &entity);

    /**
     * @brief Update the stored signature for an entity in this scene.
     * @param identityIdentifier The entity identifier.
     * @param signature The latest entity signature.
     */
    void setEntitySignature(const ecs::Entity::Identifier &identityIdentifier,
                            const ecs::Entity::Signature &signature);

    /**
     * @brief Get the list of entity identifiers belonging to the scene.
     * @return Const reference to entity identifiers.
     */
    const std::vector<ecs::Entity::Identifier> &getEntities(void) const {
        return _entities;
    }

    /**
     * @brief Get tracked entity signatures for this scene.
     * @return Const reference to the signatures map.
     */
    const std::map<ecs::Entity::Identifier, ecs::Entity::Signature> &
    getEntitySignatures(void) const {
        return _entitySignatures;
    }
};

/**
 * @brief Concept to ensure a type inherits from Scene.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromScene = std::is_base_of_v<Scene, Type>;

} // namespace guillaume
