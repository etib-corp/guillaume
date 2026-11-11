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
	 * @brief State of a drag gesture on an entity: whether it is in progress,
	 * how far the pointer moved since the previous motion event, and the
	 * callbacks for its start, its moves and its end.
	 *
	 * systems::Drag writes it in the Event phase, on the entities that have a
	 * DragInteraction, a Transform, a Bound, a MouseButtonInteraction and a
	 * HandButtonInteraction. A drag starts on the first mouse motion event
	 * received while the primary button (left mouse button or hand button A)
	 * is held on the entity, continues on every following motion event, and
	 * ends on the first motion event received after the button was released
	 * or the entity was disabled. systems::StyleState copies the dragging
	 * flag into StyleState::isDragged.
	 *
	 * @code
	 * registry.addComponent<components::DragInteraction>(identifier)
	 * 	.setOnDragHandler([](const utility::math::Vector2F &delta) {
	 * 		moveThumb(delta.x);
	 * 	});
	 * @endcode
	 *
	 * Positions and deltas are in the coordinates of the mouse motion event
	 * (view coordinates), not in dp. The delta is the movement between the
	 * two last motion events, not the distance since the start of the drag.
	 * Nothing happens in a frame without mouse motion event.
	 *
	 * @see systems::Drag, MouseButtonInteraction, StyleState
	 */
	class DragInteraction: public ecs::Component
	{
		public:
		using DragStartHandler =
			std::function<void(void)>;	  ///< Callback run when a drag starts.
		using DragHandler = std::function<void(
			const utility::math::Vector2F &)>;	  ///< Called on each move.
		using DragEndHandler =
			std::function<void(void)>;	  ///< Callback run when a drag ends.

		private:
		bool _dragging { false };	 ///< Whether a drag is in progress.
		utility::math::Vector2F _lastPosition {
			0.0f, 0.0f
		};	  ///< Pointer position at the last motion event of the drag.
		utility::math::Vector2F _delta {
			0.0f, 0.0f
		};	  ///< Movement between the two last motion events of the drag.
		DragStartHandler _onDragStart {};	 ///< Called when a drag starts.
		DragHandler _onDrag {};				 ///< Called on each move.
		DragEndHandler _onDragEnd {};		 ///< Called when a drag ends.

		public:
		/**
		 * @brief Construct a component with no drag in progress and no
		 * handler.
		 */
		DragInteraction(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~DragInteraction(void) = default;

		/**
		 * @brief Check whether a drag is in progress.
		 * @return True from the start of the drag until its end.
		 */
		bool isDragging(void) const;

		/**
		 * @brief Record whether a drag is in progress.
		 *
		 * Called by systems::Drag; it calls no handler. Marks the component
		 * changed only when the flag differs.
		 *
		 * @param dragging True when a drag is in progress.
		 * @return This component, to chain calls.
		 */
		DragInteraction &setDragging(bool dragging);

		/**
		 * @brief Get the pointer position at the last motion event of the
		 * drag.
		 * @return The position, in the coordinates of the mouse motion event;
		 * (0, 0) before the first drag.
		 */
		utility::math::Vector2F getLastPosition(void) const;

		/**
		 * @brief Record the pointer position the next delta is measured from.
		 *
		 * Called by systems::Drag. Does not mark the component changed.
		 *
		 * @param position The pointer position, in the coordinates of the
		 * mouse motion event.
		 * @return This component, to chain calls.
		 */
		DragInteraction &
			setLastPosition(const utility::math::Vector2F &position);

		/**
		 * @brief Get the movement of the pointer between the two last motion
		 * events of the drag.
		 * @return The delta, in the coordinates of the mouse motion event. It
		 * keeps its last value after the drag ended; (0, 0) before the first
		 * move.
		 */
		utility::math::Vector2F getDelta(void) const;

		/**
		 * @brief Record the movement of the pointer since the previous motion
		 * event.
		 *
		 * Called by systems::Drag, which then passes the same delta to the
		 * drag handler. Always marks the component changed.
		 *
		 * @param delta The movement, in the coordinates of the mouse motion
		 * event.
		 * @return This component, to chain calls.
		 */
		DragInteraction &setDelta(const utility::math::Vector2F &delta);

		/**
		 * @brief Store the callback run when a drag starts.
		 *
		 * Does not mark the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		DragInteraction &setOnDragStartHandler(const DragStartHandler &handler);

		/**
		 * @brief Get the callback run when a drag starts.
		 * @return A copy of the handler; empty when none was set.
		 */
		DragStartHandler getOnDragStartHandler(void) const;

		/**
		 * @brief Store the callback run on each move of a drag.
		 *
		 * It receives the movement since the previous motion event. It is not
		 * called for the motion event that starts the drag. Does not mark the
		 * component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		DragInteraction &setOnDragHandler(const DragHandler &handler);

		/**
		 * @brief Get the callback run on each move of a drag.
		 * @return A copy of the handler; empty when none was set.
		 */
		DragHandler getOnDragHandler(void) const;

		/**
		 * @brief Store the callback run when a drag ends.
		 *
		 * Does not mark the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		DragInteraction &setOnDragEndHandler(const DragEndHandler &handler);

		/**
		 * @brief Get the callback run when a drag ends.
		 * @return A copy of the handler; empty when none was set.
		 */
		DragEndHandler getOnDragEndHandler(void) const;
	};

}	 // namespace guillaume::components
