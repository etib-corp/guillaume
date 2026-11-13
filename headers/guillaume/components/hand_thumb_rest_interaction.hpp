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
#include "utility/event/hand_thumb_rest_event.hpp"

namespace guillaume::components
{
	/**
	 * @brief Marks an entity as a target of XR controller thumb rest input
	 * and stores the callback for it.
	 *
	 * systems::HandThumbRest selects the entities that have this component, a
	 * Transform and a Bound, and consumes one thumb rest event per frame in
	 * the Event phase. It has no per-entity rule: it neither writes this
	 * component nor calls the handler set with setOnEventHandler().
	 *
	 * @see systems::HandThumbRest, Interaction
	 */
	class HandThumbRestInteraction:
		public Interaction<utility::event::HandThumbRestEvent>
	{
		public:
		/**
		 * @brief Construct a component without handler.
		 */
		HandThumbRestInteraction(void);

		/**
		 * @brief Default destructor.
		 */
		~HandThumbRestInteraction(void) = default;
	};

}	 // namespace guillaume::components
