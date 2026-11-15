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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/focus.hpp"
#include "guillaume/components/text.hpp"

#include "guillaume/event/event_manager.hpp"

#include <utility/event/event.hpp>
#include <utility/event/text_input_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief Adds typed text to editable text.
	 *
	 * Runs in the Event phase on entities that have a components::Text and
	 * a components::Focus. It reads the text of the current text input
	 * event (the characters committed by the keyboard or the input method)
	 * and appends it to the content of components::Text.
	 *
	 * @note The text is appended at the end of the content, whatever the
	 * cursor position.
	 * @note The components::Focus is only required to be present: the
	 * system does not test whether the entity is the focused one, so every
	 * matching entity receives the text.
	 *
	 * @see components::Text
	 * @see systems::KeyboardControl
	 */
	class TextInput:
		public ecs::SystemFiller<components::Text, components::Focus>,
		public event::EventManager<utility::event::TextInputEvent>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase, and subscribe it to
		 * text input events.
		 * @param eventBus The event bus delivering the text input events.
		 */
		TextInput(event::EventBus &eventBus);

		/**
		 * @brief Default destructor.
		 */
		~TextInput(void) = default;

		/**
		 * @brief Take the next pending text input event, once per frame.
		 *
		 * When none is pending, the previous event stays the current one.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing.
		 */
		void cleanup(void) override;

		/**
		 * @brief Append the text of the current text input event to the
		 * content of one entity.
		 * @param entityIdentifier The entity holding the components::Text.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
