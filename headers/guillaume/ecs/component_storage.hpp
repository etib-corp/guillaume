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

#include <unordered_map>
#include <utility>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs {

/**
 * @brief Interface for type-erased component storage.
 */
class IComponentStorage {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IComponentStorage(void) = default;

    /**
     * @brief Remove a component for an entity.
     * @param identityIdentifier The entity identifier.
     */
    virtual void remove(const Entity::Identifier &identityIdentifier) = 0;

    /**
     * @brief Check if a component exists for an entity.
     * @param identityIdentifier The entity identifier.
     * @return True if a component exists.
     */
    virtual bool has(const Entity::Identifier &identityIdentifier) const = 0;
};

/**
 * @brief Storage for a single component type.
 * @tparam ComponentType The component type stored.
 */
template <InheritFromComponent ComponentType>
class ComponentStorage : public IComponentStorage {
  public:
    /**
     * @brief Container type for components.
     */
    using Storage = std::unordered_map<Entity::Identifier, ComponentType>;

  private:
    Storage _components;

  public:
    /**
     * @brief Default constructor.
     */
    ComponentStorage(void) = default;

    /**
     * @brief Default destructor.
     */
    ~ComponentStorage(void) override = default;

    /**
     * @brief Add or replace a component for an entity.
     * @param identityIdentifier The entity identifier.
     * @param args Arguments forwarded to the component constructor.
     * @return Reference to the stored component.
     */
    template <typename... Args>
    ComponentType &emplace(const Entity::Identifier &identityIdentifier,
                           Args &&...args) {
        auto [iterator, inserted] = _components.emplace(
            identityIdentifier, ComponentType(std::forward<Args>(args)...));
        if (!inserted) {
            iterator->second = ComponentType(std::forward<Args>(args)...);
        }
        return iterator->second;
    }

    /**
     * @brief Find a component for an entity.
     * @param identityIdentifier The entity identifier.
     * @return Pointer to the component or nullptr.
     */
    ComponentType *find(const Entity::Identifier &identityIdentifier) {
        auto iterator = _components.find(identityIdentifier);
        if (iterator == _components.end()) {
            return nullptr;
        }
        return &iterator->second;
    }

    /**
     * @brief Find a component for an entity (const).
     * @param identityIdentifier The entity identifier.
     * @return Pointer to the component or nullptr.
     */
    const ComponentType *
    find(const Entity::Identifier &identityIdentifier) const {
        auto iterator = _components.find(identityIdentifier);
        if (iterator == _components.end()) {
            return nullptr;
        }
        return &iterator->second;
    }

    /**
     * @brief Remove a component for an entity.
     * @param identityIdentifier The entity identifier.
     */
    void remove(const Entity::Identifier &identityIdentifier) override {
        _components.erase(identityIdentifier);
    }

    /**
     * @brief Check if a component exists for an entity.
     * @param identityIdentifier The entity identifier.
     * @return True if a component exists.
     */
    bool has(const Entity::Identifier &identityIdentifier) const override {
        return _components.find(identityIdentifier) != _components.end();
    }
};

} // namespace guillaume::ecs
