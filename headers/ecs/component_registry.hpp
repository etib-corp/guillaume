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

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>

#include <singleton.hpp>

#include "ecs/component.hpp"
#include "ecs/entity.hpp"
#include "ecs/system.hpp"

namespace guillaume::ecs {

/**
 * @brief Exception thrown when a requested component is not found for an
 * entity.
 * @tparam ComponentType The type of the missing component.
 */
template <InheritFromComponent ComponentType>
class EntityComponentNotFoundException : public std::exception {
  private:
    const Entity::Identifier _identityIdentifier; ///< Identifier of the entity
    std::type_index _componentTypeIndex{
        typeid(ComponentType)}; ///< Type index of the missing component
    std::string _componentTypeName{
        _componentTypeIndex.name()}; ///< Name of the missing component type

  public:
    /**
     * @brief Construct a new Component Not Found Exception object.
     * @param identityIdentifier The identifier of the entity.
     */
    EntityComponentNotFoundException(
        const Entity::Identifier &identityIdentifier)
        : _identityIdentifier(identityIdentifier) {}

    /**
     * @brief Get the exception message.
     * @return The exception message.
     */
    const char *what(void) const noexcept override {
        return ("Component of type " + _componentTypeName +
                " not found for entity " + std::to_string(_identityIdentifier))
            .c_str();
    }

    /**
     * @brief Get the type index of the missing component.
     * @return The type index.
     */
    std::type_index getComponentTypeIndex(void) const {
        return _componentTypeIndex;
    }
};

/**
 * @brief Registry for component types.
 *
 * Manages the registration and retrieval of component types within the ECS
 * architecture.
 */
class ComponentRegistry {
  private:
    std::map<std::type_index,
             std::map<Entity::Identifier, std::unique_ptr<Component>>>
        _components; ///< Registered components

    /**
     * @brief Register a component for an entity.
     * @tparam ComponentType The type of the component to register.
     * @param identityIdentifier The entity identifier to which the
     * component will be associated.
     */
    template <InheritFromComponent ComponentType>
    void registerComponent(const Entity::Identifier &identityIdentifier) {
        auto &entityComponents =
            _components[std::type_index(typeid(ComponentType))];
        entityComponents[identityIdentifier] =
            std::make_unique<ComponentType>();
    }

  public:
    /**
     * @brief Default constructor.
     */
    ComponentRegistry(void) = default;

    /**
     * @brief Default destructor.
     */
    ~ComponentRegistry(void) = default;

    /**
     * @brief Register a component for an entity.
     * @tparam ComponentType The type of the component to register.
     * @param identityIdentifier The entity identifier to which the
     * component will be associated.
     */
    template <InheritFromComponent... NeededComponentTypes>
    void
    registerComponentsForEntity(const Entity::Identifier &identityIdentifier) {
        (registerComponent<NeededComponentTypes>(identityIdentifier), ...);
    }

    /**
     * @brief Get a component for an entity.
     * @tparam ComponentType The type of the component to retrieve.
     * @param identityIdentifier The entity identifier to which the
     * component belongs.
     * @return Unique pointer to the requested component.
     */
    template <InheritFromComponent ComponentType>
    std::unique_ptr<ComponentType> &
    getComponent(const Entity::Identifier &identityIdentifier) {
        auto &entityComponents =
            _components[std::type_index(typeid(ComponentType))];
        auto iterator = entityComponents.find(identityIdentifier);
        if (iterator == entityComponents.end()) {
            throw ComponentNotFoundException<ComponentType>(identityIdentifier);
        }
        return static_cast<std::unique_ptr<ComponentType> &>(iterator->second);
    }
};

/**
 * @brief Concept to ensure a type inherits from ComponentRegistry.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromComponentRegistry =
    std::is_base_of_v<ComponentRegistry, Type>;

} // namespace guillaume::ecs
