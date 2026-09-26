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

#include <map>
#include <memory>

#include <utility/graphic/renderable.hpp>

#include <utility/engine.hpp>

#include "guillaume/ecs/entity.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Per-entity render handle linking an entity to an engine object.
	 *
	 * Keeps the stable engine object id returned by
	 * utility::Engine::createObject and the renderable that was last uploaded,
	 * so updateObject/removeObject can be called with the matching renderable.
	 */
	struct RenderHandle {
		size_t objectId { 0 };	 ///< utility::Engine object identifier
		std::shared_ptr<utility::graphic::Renderable>
			renderable;	   ///< Last uploaded renderable
		bool used { true };	   ///< Whether the entity was seen this frame
	};

	/**
	 * @brief Map of per-entity render handles with frame-scoped garbage
	 * collection.
	 *
	 * Replaces the former pose+content cache: handles are keyed by entity
	 * identifier, so an object is created once per entity and updated in place
	 * afterwards. Entries not touched during a frame are removed on cleanup.
	 */
	class RenderHandleMap
	{
		private:
		std::map<ecs::Entity::Identifier, RenderHandle> _handles;

		public:
		/**
		 * @brief Default constructor.
		 */
		RenderHandleMap(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~RenderHandleMap(void) = default;

		/**
		 * @brief Mark every handle as unused at the start of a frame.
		 */
		void markAllUnused(void)
		{
			for (auto &[identifier, handle]: _handles) {
				handle.used = false;
			}
		}

		/**
		 * @brief Remove unused handles, calling utility::Engine::removeObject.
		 * @param engine utility::Engine used to remove stale objects.
		 */
		void removeUnused(utility::Engine &engine)
		{
			for (auto it = _handles.begin(); it != _handles.end();) {
				if (!it->second.used) {
					engine.removeObject(it->second.renderable,
										it->second.objectId);
					it = _handles.erase(it);
				} else {
					++it;
				}
			}
		}

		/**
		 * @brief Get the handle for an entity, or nullptr.
		 * @param identifier The target entity identifier.
		 * @return Mutable pointer to the handle, or nullptr.
		 */
		RenderHandle *find(const ecs::Entity::Identifier &identifier)
		{
			auto it = _handles.find(identifier);
			if (it == _handles.end()) {
				return nullptr;
			}
			return &it->second;
		}

		/**
		 * @brief Insert a handle for an entity.
		 * @param identifier The target entity identifier.
		 * @param handle The handle to store.
		 */
		void insert(const ecs::Entity::Identifier &identifier,
					RenderHandle handle)
		{
			_handles.insert_or_assign(identifier, std::move(handle));
		}

		/**
		 * @brief Get the number of tracked handles.
		 * @return Number of entity handles currently tracked.
		 */
		std::size_t size(void) const
		{
			return _handles.size();
		}

		/**
		 * @brief Clear all handles without removing engine objects.
		 */
		void clear(void)
		{
			_handles.clear();
		}
	};

}	 // namespace guillaume::systems
