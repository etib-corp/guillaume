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
	 * @brief What a render system remembers about the engine object it
	 * created for one entity.
	 *
	 * It pairs the object id returned by utility::Engine::createObject with
	 * the renderable that was uploaded last, because
	 * utility::Engine::updateObject and utility::Engine::removeObject need
	 * both. A render system sets `used` each frame it draws the entity.
	 * @see RenderHandleMap
	 */
	struct RenderHandle {
		size_t objectId { 0 };	  ///< Identifier given by the engine
		std::shared_ptr<utility::graphic::Renderable>
			renderable;		   ///< Renderable uploaded last for this object
		bool used { true };	   ///< Whether the entity was drawn this frame
	};

	/**
	 * @brief The engine objects one render system owns, keyed by entity
	 * identifier.
	 *
	 * Every render system (RectangleRender, TextRender, ...) inherits from
	 * this class. An engine object is created the first time an entity is
	 * drawn and updated in place afterwards. An entity that is not drawn
	 * during a frame (it was destroyed or lost a component) has its engine
	 * object removed at the end of that frame.
	 *
	 * The sequence in a render system is:
	 * @code
	 * void MyRender::prepare(void) { markAllUnused(); }
	 *
	 * void MyRender::update(const ecs::Entity::Identifier &identifier)
	 * {
	 * 	auto renderable = buildRenderable(identifier);
	 * 	RenderHandle *handle = find(identifier);
	 * 	if (handle == nullptr) {
	 * 		insert(identifier, RenderHandle {
	 * 			_engine.createObject(renderable), renderable, true });
	 * 	} else {
	 * 		_engine.updateObject(renderable, handle->objectId);
	 * 		handle->renderable = renderable;
	 * 		handle->used	   = true;
	 * 	}
	 * }
	 *
	 * void MyRender::cleanup(void) { removeUnused(_engine); }
	 * @endcode
	 *
	 * find() does not set `used`: a system that finds a handle and forgets to
	 * set the flag loses the engine object at cleanup.
	 * @see RenderHandle
	 */
	class RenderHandleMap
	{
		private:
		std::map<ecs::Entity::Identifier, RenderHandle>
			_handles;	 ///< One handle per entity drawn by the owning system

		public:
		/**
		 * @brief Create an empty map.
		 */
		RenderHandleMap(void) = default;

		/**
		 * @brief Destroy the map without removing the engine objects it
		 * refers to.
		 */
		~RenderHandleMap(void) = default;

		/**
		 * @brief Clear the `used` flag of every handle. Call it from the
		 * system's prepare(), before any entity is drawn.
		 */
		void markAllUnused(void)
		{
			for (auto &[identifier, handle]: _handles) {
				handle.used = false;
			}
		}

		/**
		 * @brief Remove from the engine, then from the map, every handle
		 * whose `used` flag is still false. Call it from the system's
		 * cleanup().
		 * @param engine The engine the objects were created in.
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
		 * @brief Look up the handle of an entity.
		 *
		 * The `used` flag is left as it is: the caller sets it once the
		 * entity has been drawn.
		 * @param identifier The entity to look up.
		 * @return The handle, or `nullptr` when the entity has no engine
		 * object in this map. The pointer is valid until the handle is
		 * removed.
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
		 * @brief Store the handle of an entity, replacing the one it had.
		 *
		 * A replaced handle is dropped without calling
		 * utility::Engine::removeObject: remove the engine object first when
		 * the entity already has one.
		 * @param identifier The entity the engine object belongs to.
		 * @param handle The handle to store, with `used` set to true if the
		 * entity was drawn this frame.
		 */
		void insert(const ecs::Entity::Identifier &identifier,
					RenderHandle handle)
		{
			_handles.insert_or_assign(identifier, std::move(handle));
		}

		/**
		 * @brief Count the entities that have an engine object in this map.
		 * @return The number of handles stored.
		 */
		std::size_t size(void) const
		{
			return _handles.size();
		}

		/**
		 * @brief Forget every handle without calling
		 * utility::Engine::removeObject. Use it only when the engine objects
		 * are gone already or are destroyed with the engine.
		 */
		void clear(void)
		{
			_handles.clear();
		}
	};

}	 // namespace guillaume::systems
