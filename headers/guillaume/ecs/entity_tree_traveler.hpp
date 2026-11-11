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

#include <vector>

#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs
{

	class EntityRegistry;

	/**
	 * @brief Decides in which order the entities of a scene are visited.
	 *
	 * A traveler turns the ownership tree of an EntityRegistry (the scene,
	 * the entities it owns, the entities those own, ...) into a flat list.
	 * System::routine walks that list and calls System::update on each
	 * entity that matches the system. The order matters because a system
	 * often needs the result it computed for a parent or for the children:
	 * - LevelOrderTraveler: parents before children. Used by the Layout and
	 *   Render phases (a parent places its children, then they place
	 *   theirs).
	 * - ReverseLevelOrderTraveler: children before parents. Used by the
	 *   Event and Measure phases (a parent is sized from the sizes of its
	 *   children).
	 *
	 * "Parent" and "child" here follow ownership (who called
	 * EntityRegistry::addEntity), not Entity::setParent. An entity created
	 * through a widget Builder is owned by the scene even when it has a
	 * parent: it is ordered among the entities of the scene, by creation
	 * order.
	 *
	 * Each phase is tied to its traveler in system_phase.hpp. A traveler
	 * has no state and can be shared.
	 *
	 * @see SystemPhase
	 * @see EntityRegistry::getEntitiesBreadthFirst
	 */
	class EntityTreeTraveler
	{
		public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~EntityTreeTraveler(void) = default;

		/**
		 * @brief List every entity below a registry, in this traveler's
		 * order.
		 * @param entityRegistry The registry to start from, usually the
		 * scene. Entities owned by nested registries are included.
		 * @return The entities, in visiting order; empty when the registry
		 * owns nothing.
		 */
		virtual std::vector<std::shared_ptr<Entity>>
			travel(EntityRegistry &entityRegistry) const = 0;

		/**
		 * @brief List every entity below a registry, in this traveler's
		 * order, read-only.
		 * @param entityRegistry The registry to start from, usually the
		 * scene. Entities owned by nested registries are included.
		 * @return The entities, in visiting order; empty when the registry
		 * owns nothing.
		 */
		virtual std::vector<std::shared_ptr<const Entity>>
			travel(const EntityRegistry &entityRegistry) const = 0;
	};

	/**
	 * @brief Constrains a template parameter to a type derived from
	 * EntityTreeTraveler.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEntityTreeTraveler =
		std::is_base_of_v<EntityTreeTraveler, Type>;

}	 // namespace guillaume::ecs