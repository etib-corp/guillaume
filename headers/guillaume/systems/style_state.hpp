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

#include "guillaume/components/style_state.hpp"
#include "guillaume/ecs/system_filler.hpp"

namespace guillaume::systems
{

	/**
	 * @brief Summarizes the input state of an entity into its
	 * components::StyleState, the one component widget systems read to
	 * choose a look.
	 *
	 * Runs in the Event phase on entities with a components::StyleState.
	 * Each frame it writes three flags, from the interaction components the
	 * entity happens to have:
	 * - hovered: MouseHoverInteraction or HandHoverInteraction is hovered;
	 * - pressed: the left mouse button or hand button A is pressed, or a
	 *   pinch or a poke is in progress;
	 * - dragged: DragInteraction is dragging.
	 *
	 * The focused flag is written by systems::Focus, and the enabled flag
	 * by the widget. components::StyleState refuses to raise a flag while
	 * it is disabled, so a disabled entity never looks hovered or pressed.
	 *
	 * It is registered after the input systems it mirrors, so the flags
	 * describe the current frame.
	 *
	 * @see components::StyleState
	 * @see systems::interaction::stateLayerColor
	 */
	class StyleState: public ecs::SystemFiller<components::StyleState>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase.
		 */
		StyleState(void);

		/**
		 * @brief Default destructor.
		 */
		~StyleState(void) override = default;

		/**
		 * @brief Does nothing: the system keeps no per-frame state.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing: the system keeps no per-frame state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Recompute the hovered, pressed and dragged flags of one
		 * entity.
		 * @param entityIdentifier The entity holding the
		 * components::StyleState.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
