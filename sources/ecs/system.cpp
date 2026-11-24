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

#include <optional>
#include <unordered_set>

namespace guillaume::ecs
{
	namespace
	{
		/**
		 * @brief Binds the registries of a pass to a system and unbinds
		 * them when the pass ends, including when it ends by an exception.
		 */
		class RegistryBinding
		{
			public:
			RegistryBinding(ComponentRegistry *&componentSlot,
							EntityRegistry *&entitySlot,
							ComponentRegistry &componentRegistry,
							EntityRegistry &entityRegistry) noexcept
				: _componentSlot(componentSlot)
				, _entitySlot(entitySlot)
			{
				_componentSlot = &componentRegistry;
				_entitySlot	   = &entityRegistry;
			}

			~RegistryBinding(void)
			{
				_componentSlot = nullptr;
				_entitySlot	   = nullptr;
			}

			RegistryBinding(const RegistryBinding &)			= delete;
			RegistryBinding &operator=(const RegistryBinding &) = delete;

			private:
			ComponentRegistry *&_componentSlot;	   ///< Pointer of the system
			EntityRegistry *&_entitySlot;		   ///< Pointer of the system
		};
	}	 // namespace

	System::System(Phase phase)
		: _phase(phase)
		, _signature()
		, _activeComponentRegistry(nullptr)
		, _activeEntityRegistry(nullptr)
	{
	}

	ecs::EntityRegistry &System::getEntityRegistry(void)
	{
		if (_activeEntityRegistry == nullptr) {
			throw std::runtime_error(
				"No active entity registry bound to system");
		}
		return *_activeEntityRegistry;
	}

	const ecs::EntityRegistry &System::getEntityRegistry(void) const
	{
		if (_activeEntityRegistry == nullptr) {
			throw std::runtime_error(
				"No active entity registry bound to system");
		}
		return *_activeEntityRegistry;
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

	Phase System::getPhase(void) const noexcept
	{
		return _phase;
	}

	Entity::Signature System::getSignature(void) const noexcept
	{
		return _signature;
	}

	void System::routine(ecs::ComponentRegistry &componentRegistry,
						 ecs::EntityRegistry &entityRegistry,
						 const ecs::EntityTreeTraveler &traveler)
	{
		const RegistryBinding binding(_activeComponentRegistry,
									  _activeEntityRegistry, componentRegistry,
									  entityRegistry);
		getLogger().debug() << "System routine started";

		prepare();

		auto traversedEntities	= traveler.travel(entityRegistry);
		std::size_t treeVersion = EntityRegistry::currentTreeVersion();
		// Entities still in the tree; filled only once the tree has changed
		// during the pass, so a pass that removes nothing pays nothing.
		std::optional<std::unordered_set<const Entity *>> liveEntities;

		for (const auto &entity: traversedEntities) {
			if ((entity->getSignature() & getSignature()) != getSignature()) {
				continue;
			}

			// An update() may have removed entities that are still in the
			// list taken above: their components are gone, skip them.
			if (EntityRegistry::currentTreeVersion() != treeVersion) {
				const EntityRegistry &registry = entityRegistry;

				liveEntities.emplace();
				for (const auto &live: registry.getEntitiesBreadthFirst()) {
					liveEntities->insert(live.get());
				}
				treeVersion = EntityRegistry::currentTreeVersion();
			}
			if (liveEntities.has_value()
				&& !liveEntities->contains(entity.get())) {
				continue;
			}

			// Systems are the only place behavior lives: entities are not
			// called back, whatever changed.
			update(entity->getIdentifier());
		}

		cleanup();

		getLogger().debug() << "System routine finished. Visited entities: "
							<< traversedEntities.size();
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