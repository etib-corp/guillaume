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

	void System::routine(ecs::ComponentRegistry &componentRegistry,
						 ecs::EntityRegistry &entityRegistry)
	{
		_activeComponentRegistry = &componentRegistry;
		getLogger().debug("System routine started");

		bool hasPendingChanges = false;
		std::size_t visitedEntities = 0;
		for (auto *entity: entityRegistry.getEntitiesBreadthFirst()) {
			++visitedEntities;
			if (!componentRegistry.hasChanged(entity->getIdentifier())) {
				continue;
			}
			hasPendingChanges = true;
			entity->update();
		}

		if (hasPendingChanges) {
			componentRegistry.resetChangedFlags();
			getLogger().debug("System routine applied pending component "
							  "changes");
		}

		std::size_t matchingEntities = 0;
		for (const auto &entityIdentifier:
			 entityRegistry.getEntityWithSignature(getSignature())) {
			++matchingEntities;
			update(entityIdentifier);
		}

		getLogger().debug("System routine finished. Visited entities: "
						  + std::to_string(visitedEntities)
						  + ", matching entities: "
						  + std::to_string(matchingEntities));

		_activeComponentRegistry = nullptr;
	}

}	 // namespace guillaume::ecs