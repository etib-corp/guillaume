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

#include <functional>

#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component handling continuous pointer/hand drag interactions.
	 *
	 * The drag state and the accumulated position delta are updated by
	 * `systems::Drag`. Consumers can react to the start, the continuous drag
	 * (with the delta payload) and the end of the gesture.
	 *
	 * @see systems::Drag
	 */
	class DragInteraction: public ecs::Component
	{
		public:
		using DragStartHandler =
			std::function<void(void)>;	  ///< Drag start handler.
		using DragHandler = std::function<void(
			const utility::math::Vector2F &)>;	  ///< Drag delta handler.
		using DragEndHandler =
			std::function<void(void)>;	  ///< Drag end handler.

		private:
		bool _dragging { false };	 ///< Whether a drag is in progress.
		utility::math::Vector2F _lastPosition { 0.0f,
												0.0f };	   ///< Last position.
		utility::math::Vector2F _delta { 0.0f,
										 0.0f };	///< Accumulated delta.
		DragStartHandler _onDragStart {};			///< Drag start handler.
		DragHandler _onDrag {};						///< Drag delta handler.
		DragEndHandler _onDragEnd {};				///< Drag end handler.

		public:
		/**
		 * @brief Default constructor for the DragInteraction component.
		 */
		DragInteraction(void) = default;

		/**
		 * @brief Default destructor for the DragInteraction component.
		 */
		~DragInteraction(void) = default;

		/**
		 * @brief Check whether a drag is in progress.
		 * @return True when dragging.
		 */
		bool isDragging(void) const;

		/**
		 * @brief Set whether a drag is in progress.
		 * @param dragging The new dragging state.
		 * @return Reference to this DragInteraction for chaining.
		 */
		DragInteraction &setDragging(bool dragging);

		/**
		 * @brief Get the last pointer position.
		 * @return The last position.
		 */
		utility::math::Vector2F getLastPosition(void) const;

		/**
		 * @brief Set the last pointer position.
		 * @param position The new last position.
		 * @return Reference to this DragInteraction for chaining.
		 */
		DragInteraction &
			setLastPosition(const utility::math::Vector2F &position);

		/**
		 * @brief Get the last drag delta.
		 * @return The last delta.
		 */
		utility::math::Vector2F getDelta(void) const;

		/**
		 * @brief Set the last drag delta.
		 * @param delta The new delta.
		 * @return Reference to this DragInteraction for chaining.
		 */
		DragInteraction &setDelta(const utility::math::Vector2F &delta);

		/**
		 * @brief Set the drag start handler.
		 * @param handler The handler to call when a drag starts.
		 * @return Reference to this DragInteraction for chaining.
		 */
		DragInteraction &setOnDragStartHandler(const DragStartHandler &handler);

		/**
		 * @brief Get the drag start handler.
		 * @return The drag start handler.
		 */
		DragStartHandler getOnDragStartHandler(void) const;

		/**
		 * @brief Set the drag handler.
		 * @param handler The handler to call on each drag update.
		 * @return Reference to this DragInteraction for chaining.
		 */
		DragInteraction &setOnDragHandler(const DragHandler &handler);

		/**
		 * @brief Get the drag handler.
		 * @return The drag handler.
		 */
		DragHandler getOnDragHandler(void) const;

		/**
		 * @brief Set the drag end handler.
		 * @param handler The handler to call when a drag ends.
		 * @return Reference to this DragInteraction for chaining.
		 */
		DragInteraction &setOnDragEndHandler(const DragEndHandler &handler);

		/**
		 * @brief Get the drag end handler.
		 * @return The drag end handler.
		 */
		DragEndHandler getOnDragEndHandler(void) const;
	};

}	 // namespace guillaume::components
