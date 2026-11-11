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

#include "guillaume/ecs/entity_tree_traveler.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Visits owned entities before their owners: the deepest
	 * entities first, the entities owned by the scene last.
	 *
	 * It is the exact reverse of LevelOrderTraveler. For a scene owning A
	 * and B, where A owns A1 and A2 and B owns B1, the order is B1, A2, A1,
	 * B, A. Used by the Measure phase, where a parent is sized from its
	 * children, and by the Event phase, where the input handlers of a child
	 * run before those of its parent.
	 *
	 * @see LevelOrderTraveler
	 * @see EventPhase
	 * @see MeasurePhase
	 */
	class ReverseLevelOrderTraveler final: public EntityTreeTraveler
	{
		public:
		/**
		 * @brief List every entity below a registry, children before
		 * parents.
		 * @param entityRegistry The registry to start from, usually the
		 * scene.
		 * @return The entities from the deepest level to the root level,
		 * each level in reverse insertion order; empty when the registry
		 * owns nothing.
		 */
		std::vector<std::shared_ptr<Entity>>
			travel(EntityRegistry &entityRegistry) const override;

		/**
		 * @brief List every entity below a registry, children before
		 * parents, read-only.
		 * @param entityRegistry The registry to start from, usually the
		 * scene.
		 * @return The entities from the deepest level to the root level,
		 * each level in reverse insertion order; empty when the registry
		 * owns nothing.
		 */
		std::vector<std::shared_ptr<const Entity>>
			travel(const EntityRegistry &entityRegistry) const override;
	};

}	 // namespace guillaume::ecs