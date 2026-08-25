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

#include "guillaume/systems/keyboard_control.hpp"

namespace guillaume::systems
{

	KeyboardControl::KeyboardControl(event::EventBus &eventBus)
		: ecs::SystemFiller<components::Text, components::Focus>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::KeyboardEvent>(eventBus)
	{
	}

	void KeyboardControl::prepare(void)
	{
		consumeNextEvent();
	}

	void KeyboardControl::cleanup(void)
	{
	}

	void
		KeyboardControl::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto keyboardEvent = getLastEvent();
		if (!keyboardEvent)
			return;

		auto &text = getComponent<components::Text>(entityIdentifier);

		if (!keyboardEvent->getIsDownEvent()) {
			return;	   // Only handle key down events for control keys
		}

		const auto keycode = keyboardEvent->getKeycode();

		// Handle navigation keys
		switch (keycode) {
			case utility::event::KeyboardEvent::KeyCode::Left:
				text.moveCursorLeft();
				break;

			case utility::event::KeyboardEvent::KeyCode::Right:
				text.moveCursorRight();
				break;

			case utility::event::KeyboardEvent::KeyCode::Home:
				text.setCursorPosition(0);
				break;

			case utility::event::KeyboardEvent::KeyCode::End:
				text.setCursorPosition(text.getContent().size());
				break;

			case utility::event::KeyboardEvent::KeyCode::Delete:
				if (text.deleteCharacterAfter()) {
					getLogger().debug() << "Deleted character after cursor";
				}
				break;

			case utility::event::KeyboardEvent::KeyCode::Backspace:
				if (text.deleteCharacterBefore()) {
					getLogger().debug() << "Deleted character before cursor";
				}
				break;

			default:
				break;
		}

		// Handle character input (printable characters)
		// These are handled by TextInput system for committed text
		// KeyboardControl only handles special keys
	}

}	 // namespace guillaume::systems
