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

#include "guillaume/ecs/system.hpp"

namespace guillaume::ecs
{
	template<InheritFromComponent... ComponentTypes>
	void System::setSignature(void)
	{
		_signature.reset();
		(_signature.set(ComponentTypeId::get<ComponentTypes>()), ...);
	}

	template<InheritFromComponent ComponentType> bool System::hasComponent(
		const ecs::Entity::Identifier &entityIdentifier) const
	{
		if (_activeComponentRegistry == nullptr) {
			return false;
		}
		return _activeComponentRegistry->hasComponent<ComponentType>(
			entityIdentifier);
	}

	template<InheritFromComponent ComponentType> ComponentType &
		System::getComponent(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (_activeComponentRegistry == nullptr) {
			throw std::runtime_error(
				"No active component registry bound to system");
		}
		return _activeComponentRegistry->getComponent<ComponentType>(
			entityIdentifier);
	}

	template<InheritFromComponent ComponentType>
	const ComponentType &System::getComponent(
		const ecs::Entity::Identifier &entityIdentifier) const
	{
		if (_activeComponentRegistry == nullptr) {
			throw std::runtime_error(
				"No active component registry bound to system");
		}
		return _activeComponentRegistry->getComponent<ComponentType>(
			entityIdentifier);
	}

	template<InheritFromComponent ComponentType> bool System::requireComponent(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (hasComponent<ComponentType>(entityIdentifier)) {
			return true;
		}

		getLogger().warning() << "Entity " << entityIdentifier << " does not have " << utility::demangle<ComponentType>() << " component";
		return false;
	}
}	 // namespace guillaume::ecs