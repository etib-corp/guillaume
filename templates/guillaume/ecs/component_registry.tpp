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

#include "guillaume/ecs/component_registry.hpp"

namespace guillaume::ecs
{
	template<InheritFromComponent ComponentType>
	EntityComponentNotFoundException<ComponentType>::
		EntityComponentNotFoundException(
			const Entity::Identifier &entityIdentifier)
		: _entityIdentifier(entityIdentifier)
	{
		_message = "Component of type " + _componentTypeName
			+ " not found for entity " + std::to_string(_entityIdentifier);
	}

	/**
	 * @brief Get the exception message.
	 * @return The exception message.
	 */
	template<InheritFromComponent ComponentType>
	const char *EntityComponentNotFoundException<ComponentType>::what(
		void) const noexcept
	{
		return _message.c_str();
	}

	template<InheritFromComponent ComponentType>
	ComponentStorage<ComponentType> &ComponentRegistry::getOrCreateStorage(void)
	{
		const std::type_index typeIndex(typeid(ComponentType));
		auto iterator = _storages.find(typeIndex);
		if (iterator == _storages.end()) {
			auto storage = std::make_unique<ComponentStorage<ComponentType>>();
			auto [insertedIterator, inserted] =
				_storages.emplace(typeIndex, std::move(storage));
			iterator = insertedIterator;
		}
		return static_cast<ComponentStorage<ComponentType> &>(
			*iterator->second);
	}

	template<InheritFromComponent ComponentType>
	void ComponentRegistry::registerComponent(
		const Entity::Identifier &entityIdentifier)
	{
		auto &storage = getOrCreateStorage<ComponentType>();
		storage.emplace(entityIdentifier);
		getLogger().debug() << "Registered component of type " << utility::demangle<ComponentType>() << " for entity " << entityIdentifier;
	}

	template<InheritFromComponent ComponentType>
	void ComponentRegistry::registerNewComponentType(void)
	{
		getOrCreateStorage<ComponentType>();
	}

	template<InheritFromComponent... NeededComponentTypes>
	void ComponentRegistry::registerComponentsForEntity(
		const Entity::Identifier &entityIdentifier)
	{
		(registerComponent<NeededComponentTypes>(entityIdentifier), ...);
	}

	template<InheritFromComponent ComponentType, typename... Args>
	ComponentType &ComponentRegistry::addComponent(
		const Entity::Identifier &entityIdentifier, Args &&...args)
	{
		auto &storage = getOrCreateStorage<ComponentType>();
		return storage.emplace(entityIdentifier, std::forward<Args>(args)...);
	}

	template<InheritFromComponent ComponentType>
	bool ComponentRegistry::hasComponent(
		const Entity::Identifier &entityIdentifier) const
	{
		const std::type_index typeIndex(typeid(ComponentType));
		auto iterator = _storages.find(typeIndex);
		if (iterator == _storages.end()) {
			return false;
		}
		return iterator->second->has(entityIdentifier);
	}

	template<InheritFromComponent ComponentType>
	void ComponentRegistry::removeComponent(
		const Entity::Identifier &entityIdentifier)
	{
		auto &storage = getOrCreateStorage<ComponentType>();
		storage.remove(entityIdentifier);
	}

	template<InheritFromComponent ComponentType>
	ComponentType &ComponentRegistry::getComponent(
		const Entity::Identifier &entityIdentifier)
	{
		auto &storage  = getOrCreateStorage<ComponentType>();
		auto component = storage.find(entityIdentifier);
		if (!component) {
			throw EntityComponentNotFoundException<ComponentType>(
				entityIdentifier);
		}
		return *component;
	}

	template<InheritFromComponent ComponentType>
	const ComponentType &ComponentRegistry::getComponent(
		const Entity::Identifier &entityIdentifier) const
	{
		const std::type_index typeIndex(typeid(ComponentType));
		auto iterator = _storages.find(typeIndex);
		if (iterator == _storages.end()) {
			throw EntityComponentNotFoundException<ComponentType>(
				entityIdentifier);
		}
		const auto *storage =
			static_cast<const ComponentStorage<ComponentType> *>(
				iterator->second.get());
		const auto *component = storage->find(entityIdentifier);
		if (!component) {
			throw EntityComponentNotFoundException<ComponentType>(
				entityIdentifier);
		}
		return *component;
	}
}	 // namespace guillaume::ecs