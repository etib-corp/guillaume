/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "guillaume/ecs/component_storage.hpp"

namespace guillaume::ecs
{
    template<InheritFromComponent ComponentType>
    template<typename... Args> ComponentType &
        ComponentStorage<ComponentType>::emplace(const Entity::Identifier &entityIdentifier, Args &&...args)
    {
        auto [iterator, inserted] = _components.emplace(
            entityIdentifier, ComponentType(std::forward<Args>(args)...));
        if (!inserted) {
            iterator->second = ComponentType(std::forward<Args>(args)...);
        }
        return iterator->second;
    }

    template<InheritFromComponent ComponentType>
    ComponentType *ComponentStorage<ComponentType>::find(const Entity::Identifier &entityIdentifier)
    {
        auto iterator = _components.find(entityIdentifier);
        if (iterator == _components.end()) {
            return nullptr;
        }
        return &iterator->second;
    }

    template<InheritFromComponent ComponentType>
    const ComponentType *
        ComponentStorage<ComponentType>::find(const Entity::Identifier &entityIdentifier) const
    {
        auto iterator = _components.find(entityIdentifier);
        if (iterator == _components.end()) {
            return nullptr;
        }
        return &iterator->second;
    }

    template<InheritFromComponent ComponentType>
    void ComponentStorage<ComponentType>::remove(const Entity::Identifier &entityIdentifier)
    {
        _components.erase(entityIdentifier);
    }

    template<InheritFromComponent ComponentType>
    bool ComponentStorage<ComponentType>::has(const Entity::Identifier &entityIdentifier) const
    {
        return _components.find(entityIdentifier) != _components.end();
    }

    template<InheritFromComponent ComponentType>
    bool ComponentStorage<ComponentType>::hasChanged(
        const Entity::Identifier &entityIdentifier) const
    {
        for (const auto &[id, component]: _components) {
            if (id != entityIdentifier) {
                continue;
            }
            if (component.hasChanged()) {
                return true;
            }
        }
        return false;
    }

    template<InheritFromComponent ComponentType>
    void ComponentStorage<ComponentType>::resetChangedFlags(void)
    {
        for (auto &[entityIdentifier, component]: _components) {
            (void)entityIdentifier;
            component.setHasChanged(false);
        }
    }
} // namespace guillaume::ecs