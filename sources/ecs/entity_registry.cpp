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

#include "guillaume/ecs/entity_registry.hpp"

#include <algorithm>

#include "guillaume/ecs/level_order_traveler.hpp"

namespace guillaume::ecs
{

	namespace
	{
		/**
		 * @brief Global tree version, incremented whenever an entity is added
		 * to any registry. Used to invalidate cached breadth-first traversals.
		 */
		std::size_t &getTreeVersion(void)
		{
			static std::size_t version = 0;
			return version;
		}
	}	 // namespace

	void EntityRegistry::addEntity(std::shared_ptr<Entity> entity)
	{
		accessDirectEntities().push_back(entity);
		++getTreeVersion();
	}

	bool EntityRegistry::removeOne(Entity::Identifier identifier,
								   std::vector<Entity::Identifier> &orphans)
	{
		if (identifier == Entity::InvalidIdentifier) {
			return false;
		}

		auto &entities = accessDirectEntities();
		const auto iterator =
			std::find_if(entities.begin(), entities.end(),
						 [identifier](const std::shared_ptr<Entity> &entity) {
							 return entity->getIdentifier() == identifier;
						 });
		if (iterator == entities.end()) {
			for (auto &entity: entities) {
				auto *nested = dynamic_cast<EntityRegistry *>(entity.get());
				if (nested != nullptr
					&& nested->removeOne(identifier, orphans)) {
					return true;
				}
			}
			return false;
		}

		// Keep the entity alive until it is fully released.
		const std::shared_ptr<Entity> entity = *iterator;
		if (auto *nested = dynamic_cast<EntityRegistry *>(entity.get())) {
			nested->clearOwned(orphans);
		}
		const auto children = entity->releaseComponents();
		orphans.insert(orphans.end(), children.begin(), children.end());
		entities.erase(iterator);
		++getTreeVersion();
		return true;
	}

	void EntityRegistry::clearOwned(std::vector<Entity::Identifier> &orphans)
	{
		auto &entities = accessDirectEntities();

		for (auto &entity: entities) {
			if (auto *nested = dynamic_cast<EntityRegistry *>(entity.get())) {
				nested->clearOwned(orphans);
			}
			const auto children = entity->releaseComponents();
			orphans.insert(orphans.end(), children.begin(), children.end());
		}
		entities.clear();
		++getTreeVersion();
	}

	bool EntityRegistry::removeEntity(Entity::Identifier identifier)
	{
		std::vector<Entity::Identifier> orphans;

		if (!removeOne(identifier, orphans)) {
			return false;
		}
		// Entities parented to a removed one go with it, wherever below this
		// registry they are registered. Most were owned and are already gone.
		while (!orphans.empty()) {
			const Entity::Identifier orphan = orphans.back();
			orphans.pop_back();
			removeOne(orphan, orphans);
		}
		return true;
	}

	void EntityRegistry::clearEntities(void)
	{
		std::vector<Entity::Identifier> orphans;

		clearOwned(orphans);
	}

	std::vector<std::shared_ptr<Entity>>
		EntityRegistry::getEntitiesBreadthFirst(void)
	{
		if (_cachedVersion == getTreeVersion()) {
			return _bfsCache;
		}

		_bfsCache.clear();
		std::queue<EntityRegistry *> registries;

		registries.push(this);
		while (!registries.empty()) {
			EntityRegistry *registry = registries.front();
			registries.pop();

			for (auto &entity: registry->accessDirectEntities()) {
				_bfsCache.push_back(entity);

				auto *childRegistry =
					dynamic_cast<EntityRegistry *>(entity.get());
				if (childRegistry != nullptr) {
					registries.push(childRegistry);
				}
			}
		}

		_cachedVersion = getTreeVersion();
		return _bfsCache;
	}

	std::vector<std::shared_ptr<Entity>>
		EntityRegistry::getEntitiesBreadthFirst(void) const
	{
		if (_cachedVersion == getTreeVersion()) {
			return _bfsCache;
		}

		_bfsCache.clear();
		std::queue<const EntityRegistry *> registries;

		registries.push(this);
		while (!registries.empty()) {
			const EntityRegistry *registry = registries.front();
			registries.pop();

			for (const auto &entity: registry->accessDirectEntities()) {
				_bfsCache.push_back(entity);

				auto *childRegistry =
					dynamic_cast<const EntityRegistry *>(entity.get());
				if (childRegistry != nullptr) {
					registries.push(childRegistry);
				}
			}
		}

		_cachedVersion = getTreeVersion();
		return _bfsCache;
	}

	std::vector<Entity::Identifier> EntityRegistry::getEntityWithSignature(
		Entity::Signature systemSignature) const
	{
		LevelOrderTraveler traveler;
		return getEntityWithSignature(systemSignature, traveler);
	}

	std::vector<Entity::Identifier> EntityRegistry::getEntityWithSignature(
		Entity::Signature systemSignature,
		const EntityTreeTraveler &traveler) const
	{
		std::vector<Entity::Identifier> matchingIdentifiers;
		for (const auto &entity: traveler.travel(*this)) {
			if ((entity->getSignature() & systemSignature) == systemSignature) {
				matchingIdentifiers.push_back(entity->getIdentifier());
			}
		}

		return matchingIdentifiers;
	}

}	 // namespace guillaume::ecs
