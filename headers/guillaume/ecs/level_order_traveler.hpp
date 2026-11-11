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
	 * @brief Visits owners before what they own: the entities owned by the
	 * scene first, then the entities those own, and so on.
	 *
	 * For a scene owning A and B, where A owns A1 and A2 and B owns B1, the
	 * order is A, B, A1, A2, B1. Used by the Layout and Render phases, where
	 * a parent must be placed before its children.
	 *
	 * @see ReverseLevelOrderTraveler
	 * @see LayoutPhase
	 * @see RenderPhase
	 */
	class LevelOrderTraveler final: public EntityTreeTraveler
	{
		public:
		/**
		 * @brief List every entity below a registry, parents before
		 * children.
		 * @param entityRegistry The registry to start from, usually the
		 * scene.
		 * @return The entities level by level, each level in insertion
		 * order; empty when the registry owns nothing.
		 */
		std::vector<std::shared_ptr<Entity>>
			travel(EntityRegistry &entityRegistry) const override;

		/**
		 * @brief List every entity below a registry, parents before
		 * children, read-only.
		 * @param entityRegistry The registry to start from, usually the
		 * scene.
		 * @return The entities level by level, each level in insertion
		 * order; empty when the registry owns nothing.
		 */
		std::vector<std::shared_ptr<const Entity>>
			travel(const EntityRegistry &entityRegistry) const override;
	};

}	 // namespace guillaume::ecs