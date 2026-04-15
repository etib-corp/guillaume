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

namespace guillaume::ecs
{

	void EntityRegistry::addEntity(std::unique_ptr<Entity> entity)
	{
		accessDirectEntities().push_back(std::move(entity));
	}

	std::vector<Entity *> EntityRegistry::getEntitiesBreadthFirst(void)
	{
		std::vector<Entity *> entities;
		std::queue<EntityRegistry *> registries;

		registries.push(this);
		while (!registries.empty()) {
			EntityRegistry *registry = registries.front();
			registries.pop();

			for (auto &entity: registry->accessDirectEntities()) {
				Entity *rawEntity = entity.get();
				entities.push_back(rawEntity);

				auto *childRegistry = dynamic_cast<EntityRegistry *>(rawEntity);
				if (childRegistry != nullptr) {
					registries.push(childRegistry);
				}
			}
		}

		return entities;
	}

	std::vector<const Entity *>
		EntityRegistry::getEntitiesBreadthFirst(void) const
	{
		std::vector<const Entity *> entities;
		std::queue<const EntityRegistry *> registries;

		registries.push(this);
		while (!registries.empty()) {
			const EntityRegistry *registry = registries.front();
			registries.pop();

			for (const auto &entity: registry->accessDirectEntities()) {
				const Entity *rawEntity = entity.get();
				entities.push_back(rawEntity);

				auto *childRegistry =
					dynamic_cast<const EntityRegistry *>(rawEntity);
				if (childRegistry != nullptr) {
					registries.push(childRegistry);
				}
			}
		}

		return entities;
	}

	std::vector<Entity::Identifier> EntityRegistry::getEntityWithSignature(
		Entity::Signature systemSignature) const
	{
		std::vector<Entity::Identifier> matchingIdentifiers;
		for (const auto *entity: getEntitiesBreadthFirst()) {
			if ((entity->getSignature() & systemSignature) == systemSignature) {
				matchingIdentifiers.push_back(entity->getIdentifier());
			}
		}

		return matchingIdentifiers;
	}

}	 // namespace guillaume::ecs
