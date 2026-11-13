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

#include <memory>
#include <queue>
#include <vector>

#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief An EntityRegistry that is not itself an entity: the top of an
	 * ownership tree.
	 *
	 * Scene derives from it to own its root entities. Use it directly to
	 * hold entities outside a scene, in a test for instance. An entity that
	 * owns children derives from ParentEntity instead.
	 *
	 * @code
	 * ecs::EntityRegistryContainer entities;
	 * entities.addEntity(button);
	 * @endcode
	 *
	 * @see EntityRegistry
	 * @see ParentEntity
	 */
	class EntityRegistryContainer: public EntityRegistry
	{
		private:
		std::vector<std::shared_ptr<Entity>>
			_entities;	  ///< Entities owned directly, in insertion order

		protected:
		/**
		 * @brief Give access to the list of entities this registry owns
		 * directly.
		 * @return Mutable reference to the list, in insertion order.
		 */
		std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) override;

		/**
		 * @brief Give read access to the list of entities this registry owns
		 * directly.
		 * @return Const reference to the list, in insertion order.
		 */
		const std::vector<std::shared_ptr<Entity>> &
			accessDirectEntities(void) const override;
	};

}	 // namespace guillaume::ecs