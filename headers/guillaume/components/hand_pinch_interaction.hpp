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

#include "guillaume/components/interaction.hpp"
#include "utility/event/hand_pinch_event.hpp"

namespace guillaume::components
{
	/**
	 * @brief Whether an XR hand pinches while pointing at an entity, and what
	 * to call when it does.
	 *
	 * systems::HandPinch writes it in the Event phase. On each hand pinch
	 * event it casts a ray from the hand pose along its forward direction and
	 * tests it against the entity's Transform and Bound: the pinch flag
	 * becomes the result of that test, and the handler set with
	 * setOnEventHandler() is called when the ray hits. systems::Focus reads
	 * the flag to give the focus to the entity and systems::StyleState reads
	 * it as "pressed".
	 *
	 * The handler runs for every pinch event that hits the entity, not only
	 * for the first one. The flag only changes in a frame that received a
	 * pinch event.
	 *
	 * @see systems::HandPinch, Interaction, HandPokeInteraction
	 */
	class HandPinchInteraction:
		public Interaction<utility::event::HandPinchEvent>
	{
		private:
		bool _isPinching { false };	   ///< Last pinch event hit the entity

		public:
		/**
		 * @brief Construct a component that is not pinched and has no handler.
		 */
		HandPinchInteraction(void);

		/**
		 * @brief Default destructor.
		 */
		~HandPinchInteraction(void) = default;

		/**
		 * @brief Check whether a hand pinches the entity.
		 * @return True when the ray of the last pinch event hit the entity.
		 */
		bool isPinch(void) const;

		/**
		 * @brief Record whether a hand pinches the entity.
		 *
		 * Called by systems::HandPinch; it calls no handler. Always marks the
		 * component changed, even when the flag is the same.
		 *
		 * @param pinching True when the ray of the pinch event hits the
		 * entity.
		 */
		void setPinching(bool pinching);
	};

}	 // namespace guillaume::components
