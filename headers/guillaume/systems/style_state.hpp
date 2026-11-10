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
	 * @brief Derive `components::StyleState` from the interaction components.
	 *
	 * Every frame, the hover flag mirrors `MouseHoverInteraction` /
	 * `HandHoverInteraction`, the press flag mirrors the primary mouse button,
	 * hand button A, pinch and poke, and the drag flag mirrors
	 * `DragInteraction`. The focus flag is owned by `systems::Focus`. A
	 * disabled state keeps every transient flag cleared.
	 *
	 * @see components::StyleState
	 */
	class StyleState: public ecs::SystemFiller<components::StyleState>
	{
		public:
		/**
		 * @brief Construct the style-state system in the event phase.
		 */
		StyleState(void);

		/**
		 * @brief Default destructor.
		 */
		~StyleState(void) override = default;

		/**
		 * @brief Nothing to prepare.
		 */
		void prepare(void) override;

		/**
		 * @brief Nothing to clean up.
		 */
		void cleanup(void) override;

		/**
		 * @brief Refresh the transient flags of one entity.
		 * @param entityIdentifier Entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
