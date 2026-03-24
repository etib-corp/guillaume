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

namespace
{

	void removeLastUtf8CodePoint(std::string &content)
	{
		if (content.empty()) {
			return;
		}

		std::size_t codePointStart = content.size() - 1;
		while (codePointStart > 0
			   && (static_cast<unsigned char>(content[codePointStart]) & 0xC0)
				   == 0x80) {
			--codePointStart;
		}

		content.erase(codePointStart);
	}

}	 // namespace

namespace guillaume::systems
{

	KeyboardControl::KeyboardControl(event::EventBus &eventBus)
		: _keyboardSubscriber(eventBus)
	{
	}

	void
		KeyboardControl::update(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating KeyboardControl system for entity "
						  + std::to_string(entityIdentifier));
		if (!_keyboardSubscriber.hasPendingEvents()) {
			return;
		}

		auto &text =
			componentRegistry.getComponent<components::Text>(entityIdentifier);
		std::string content = text.getContent();

		while (_keyboardSubscriber.hasPendingEvents()) {
			const auto keyboardEvent = _keyboardSubscriber.getNextEvent();
			if (!keyboardEvent || !keyboardEvent->getIsDownEvent()) {
				continue;
			}

			if (keyboardEvent->getKeycode()
				== utility::event::KeyboardEvent::KeyCode::Backspace) {
				removeLastUtf8CodePoint(content);
			}
		}

		text.setContent(content);
	}

}	 // namespace guillaume::systems
