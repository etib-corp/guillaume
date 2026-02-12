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

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include <utility>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/component_type_id.hpp"
#include "guillaume/ecs/system_registry.hpp"

namespace guillaume::ecs {

/**
 * @brief ECS (Entity-Component-System) main class.
 * @tparam ComponentRegistryType The type of the component registry.
 * @tparam SystemRegistryType The type of the system registry.
 */
template <InheritFromComponentRegistry ComponentRegistryType>
class ECS
    : protected utility::logging::Loggable<ECS<ComponentRegistryType>,
                                           utility::logging::StandardLogger> {
  protected:
    SystemRegistry _systemRegistry;            ///< System registry instance
    ComponentRegistryType _componentRegistry;  ///< Component registry instance
    std::vector<Entity::Identifier> _entities; ///< Managed entities

  public:
    /**
     * @brief Default constructor.
     */
    ECS(void) = default;

    /**
     * @brief Default destructor.
     */
    virtual ~ECS(void) = default;

    /**
     * @brief Get the System Registry.
     * @return Reference to the system registry.
     */
    SystemRegistry &getSystemRegistry(void) { return _systemRegistry; }

    /**
     * @brief Get the Component Registry.
     * @return Reference to the component registry.
     */
    ComponentRegistryType &getComponentRegistry(void) {
        return _componentRegistry;
    }

    /**
     * @brief Add an entity to all compatible systems and manage it.
     * @param entity The entity to add.
     */
    void addEntity(Entity &entity) {
        _systemRegistry.addEntityToSystems(entity);
        _entities.push_back(entity.getIdentifier());
    }

    /**
     * @brief Add or replace a component on an entity and update system
     * membership.
     * @tparam ComponentType The component type to add.
     * @param entity The entity to update.
     * @param args Arguments forwarded to the component constructor.
     * @return Reference to the stored component.
     */
    template <InheritFromComponent ComponentType, typename... Args>
    ComponentType &addComponent(Entity &entity, Args &&...args) {
        auto &component =
            _componentRegistry.template addComponent<ComponentType>(
                entity.getIdentifier(), std::forward<Args>(args)...);
        auto signature = entity.getSignature();
        signature.set(ComponentTypeId::get<ComponentType>());
        entity.setSignature(signature);
        _systemRegistry.onEntitySignatureChanged(entity.getIdentifier(),
                                                 signature);
        return component;
    }

    /**
     * @brief Remove a component from an entity and update system membership.
     * @tparam ComponentType The component type to remove.
     * @param entity The entity to update.
     */
    template <InheritFromComponent ComponentType>
    void removeComponent(Entity &entity) {
        _componentRegistry.template removeComponent<ComponentType>(
            entity.getIdentifier());
        auto signature = entity.getSignature();
        signature.reset(ComponentTypeId::get<ComponentType>());
        entity.setSignature(signature);
        _systemRegistry.onEntitySignatureChanged(entity.getIdentifier(),
                                                 signature);
    }

    /**
     * @brief Check whether an entity has a component.
     * @tparam ComponentType The component type to check.
     * @param entity The entity to inspect.
     * @return True if the entity has the component.
     */
    template <InheritFromComponent ComponentType>
    bool hasComponent(const Entity &entity) const {
        return _componentRegistry.template hasComponent<ComponentType>(
            entity.getIdentifier());
    }

    /**
     * @brief Get a component attached to an entity.
     * @tparam ComponentType The component type.
     * @param entity The entity to inspect.
     * @return Reference to the component.
     */
    template <InheritFromComponent ComponentType>
    ComponentType &getComponent(Entity &entity) {
        return _componentRegistry.template getComponent<ComponentType>(
            entity.getIdentifier());
    }

    /**
     * @brief Get a component attached to an entity (const).
     * @tparam ComponentType The component type.
     * @param entity The entity to inspect.
     * @return Reference to the component.
     */
    template <InheritFromComponent ComponentType>
    const ComponentType &getComponent(const Entity &entity) const {
        return _componentRegistry.template getComponent<ComponentType>(
            entity.getIdentifier());
    }

    /**
     * @brief Routine to update all systems.
     */
    void routine(void) {
        for (auto &[typeIndex, system] : _systemRegistry.getSystems()) {
            system->routine(_componentRegistry);
        }
    }
};

} // namespace guillaume::ecs
