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

#include "guillaume/ecs/system.hpp"

namespace guillaume::ecs
{
	System::System(Phase phase)
		: _phase(phase)
		, _signature()
		, _activeComponentRegistry(nullptr)
	{
	}

	ecs::ComponentRegistry &System::getComponentRegistry(void)
	{
		if (_activeComponentRegistry == nullptr) {
			throw std::runtime_error(
				"No active component registry bound to system");
		}
		return *_activeComponentRegistry;
	}

	const ecs::ComponentRegistry &System::getComponentRegistry(void) const
	{
		if (_activeComponentRegistry == nullptr) {
			throw std::runtime_error(
				"No active component registry bound to system");
		}
		return *_activeComponentRegistry;
	}

	Phase System::getPhase(void) const
	{
		return _phase;
	}

	Entity::Signature System::getSignature(void) const
	{
		return _signature;
	}

	void System::routine(ecs::ComponentRegistry &componentRegistry,
						 ecs::EntityRegistry &entityRegistry,
						 const ecs::EntityTreeTraveler &traveler)
	{
		_activeComponentRegistry = &componentRegistry;
		getLogger().debug() << "System routine started";

		prepare();

		auto traversedEntities = traveler.travel(entityRegistry);

		for (const auto &entity: traversedEntities) {
			if ((entity->getSignature() & getSignature()) != getSignature()) {
				continue;
			}

			const bool entityWasChanged =
				componentRegistry.hasChanged(entity->getIdentifier());
			if (entityWasChanged) {
				entity->update();
			}

			update(entity->getIdentifier());

			if ((entityWasChanged
				 || componentRegistry.hasChanged(entity->getIdentifier()))
				&& entity->getParent() != nullptr) {
				entity->getParent()->update();
			}
		}

		cleanup();

		getLogger().debug() << "System routine finished. Visited entities: "
							<< traversedEntities.size();

		_activeComponentRegistry = nullptr;
	}

	void
		System::bindComponentRegistry(ecs::ComponentRegistry &componentRegistry)
	{
		_activeComponentRegistry = &componentRegistry;
	}

	void System::unbindComponentRegistry(void)
	{
		_activeComponentRegistry = nullptr;
	}
}	 // namespace guillaume::ecs