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
#include <vector>

#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base entity type that can own child entities.
	 *
	 * This class combines the ECS entity identity/signature with the
	 * EntityRegistry ownership contract. Parent-capable entities can inherit
	 * from this class to expose a consistent child management API.
	 */
	class ParentEntity: public Entity, public EntityRegistry
	{
		private:
		std::vector<std::unique_ptr<Entity>>
			_children;	  ///< Direct child entities owned by this parent.

		protected:
		std::vector<std::unique_ptr<Entity>> &
			accessDirectEntities(void) override
		{
			return _children;
		}

		const std::vector<std::unique_ptr<Entity>> &
			accessDirectEntities(void) const override
		{
			return _children;
		}
	};

}	 // namespace guillaume::ecs
