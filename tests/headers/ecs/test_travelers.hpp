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

#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/ecs/parent_entity.hpp"
#include "guillaume/ecs/reverse_level_order_traveler.hpp"

namespace guillaume::ecs::tests
{
	/**
	 * @brief Fixture building a two-level entity tree:
	 * two roots, the first of which owns two children.
	 */
	class TestTravelers: public ::testing::Test
	{
		protected:
		/// Root registry holding its direct entities in a vector.
		class Registry final: public EntityRegistry
		{
			private:
			std::vector<std::shared_ptr<Entity>> _entities;

			protected:
			std::vector<std::shared_ptr<Entity>> &
				accessDirectEntities(void) override
			{
				return _entities;
			}

			const std::vector<std::shared_ptr<Entity>> &
				accessDirectEntities(void) const override
			{
				return _entities;
			}
		};

		Registry _registry;						  ///< Tree root.
		std::shared_ptr<ParentEntity> _parent;	  ///< Root with children.
		std::shared_ptr<Entity> _leafRoot;		  ///< Root without child.
		std::shared_ptr<Entity> _firstChild;	  ///< Child of _parent.
		std::shared_ptr<Entity> _secondChild;	  ///< Child of _parent.

		void SetUp(void) override
		{
			_parent		 = std::make_shared<ParentEntity>();
			_leafRoot	 = std::make_shared<Entity>();
			_firstChild	 = std::make_shared<Entity>();
			_secondChild = std::make_shared<Entity>();

			_registry.addEntity(_parent);
			_registry.addEntity(_leafRoot);
			_firstChild->setParent(_parent);
			_secondChild->setParent(_parent);
			_parent->addEntity(_firstChild);
			_parent->addEntity(_secondChild);
		}

		/**
		 * @brief Position of an entity in a traversal.
		 * @param order The traversal result.
		 * @param entity The entity to look for.
		 * @return Its index, or the traversal size when absent.
		 */
		static std::size_t
			indexOf(const std::vector<std::shared_ptr<Entity>> &order,
					const std::shared_ptr<Entity> &entity)
		{
			return static_cast<std::size_t>(
				std::find(order.begin(), order.end(), entity) - order.begin());
		}
	};

}	 // namespace guillaume::ecs::tests
