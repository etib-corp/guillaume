#include "event_handler.hpp"

#include <utility/event/mouse_motion_event.hpp>
#include <utility/event/quit_event.hpp>

namespace simple_application
{

	EventHandler::EventHandler(void)
		: guillaume::event::EventHandler()
	{
		getLogger().info("SDL3 Event Handler initialized");
	}

	EventHandler::~EventHandler(void)
	{
		getLogger().info("SDL3 Event Handler destroyed");
	}

	void EventHandler::pollEvents(void)
	{
		SDL_Event sdlEvent;
		setGotNewEvents(false);

		while (SDL_PollEvent(&sdlEvent)) {
			setGotNewEvents(true);
			getLogger().debug("Get new SDL event of type: "
							  + std::to_string(sdlEvent.type));

			std::unique_ptr<utility::event::Event> event = nullptr;

			switch (sdlEvent.type) {
				case SDL_EVENT_QUIT: {
					getLogger().info("Quit event received");
					auto quitEvent =
						std::make_unique<utility::event::QuitEvent>();
					setShouldQuit(true);
					event = std::move(quitEvent);
					break;
				}

				case SDL_EVENT_KEY_DOWN:
				case SDL_EVENT_KEY_UP: {
					getLogger().debug(
						"Keyboard event: "
						+ std::string(sdlEvent.type == SDL_EVENT_KEY_DOWN
										  ? "KEY_DOWN"
										  : "KEY_UP"));
					auto keyEvent =
						std::make_unique<utility::event::KeyboardEvent>();
					keyEvent->setKeycode(convertKeyCode(sdlEvent.key.key));
					keyEvent->setModifiers(
						convertKeyModifiers(sdlEvent.key.mod));
					keyEvent->setIsDownEvent(sdlEvent.type
											 == SDL_EVENT_KEY_DOWN);
					keyEvent->setIsRepeatEvent(sdlEvent.key.repeat != 0);
					event = std::move(keyEvent);
					break;
				}

				case SDL_EVENT_TEXT_INPUT: {
					getLogger().debug("Text input event received");
					auto textInputEvent =
						std::make_unique<utility::event::TextInputEvent>();
					textInputEvent->setText(std::string(sdlEvent.text.text));
					event = std::move(textInputEvent);
					break;
				}

				case SDL_EVENT_MOUSE_BUTTON_UP: {
					getLogger().debug("Mouse button released event at ("
									  + std::to_string(sdlEvent.button.x) + ", "
									  + std::to_string(sdlEvent.button.y)
									  + ")");
					auto mouseEvent =
						std::make_unique<utility::event::MouseButtonEvent>();
					mouseEvent->setPosition(
						{ static_cast<float>(sdlEvent.button.x),
						  static_cast<float>(sdlEvent.button.y) });
					mouseEvent->setButtonState(
						convertMouseButton(sdlEvent.button.button), false);
					event = std::move(mouseEvent);
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN: {
					getLogger().debug("Mouse button clicked event at ("
									  + std::to_string(sdlEvent.button.x) + ", "
									  + std::to_string(sdlEvent.button.y)
									  + ")");
					auto mouseEvent =
						std::make_unique<utility::event::MouseButtonEvent>();
					mouseEvent->setPosition(
						{ static_cast<float>(sdlEvent.button.x),
						  static_cast<float>(sdlEvent.button.y) });
					mouseEvent->setButtonState(
						convertMouseButton(sdlEvent.button.button), true);
					event = std::move(mouseEvent);
					break;
				}

				case SDL_EVENT_MOUSE_MOTION: {
					auto motionEvent =
						std::make_unique<utility::event::MouseMotionEvent>();
					motionEvent->setPosition(
						{ static_cast<float>(sdlEvent.motion.x),
						  static_cast<float>(sdlEvent.motion.y) });
					event = std::move(motionEvent);
					break;
				}

				default: {
					// Ignore other events
					break;
				}
			}

			if (event && getEventCallback()) {
				getEventCallback()(event);
			}
		}
	}

	utility::event::KeyboardEvent::ScanCode
		EventHandler::convertScanCode(SDL_Scancode sdlScanCode)
	{
		// SDL3 scancodes map directly to the utility ScanCode enum
		// as they follow the USB HID specification
		return static_cast<utility::event::KeyboardEvent::ScanCode>(
			sdlScanCode);
	}

	utility::event::KeyboardEvent::KeyCode
		EventHandler::convertKeyCode(SDL_Keycode sdlKeyCode)
	{
		// SDL3 keycodes map directly to the utility KeyCode enum
		return static_cast<utility::event::KeyboardEvent::KeyCode>(sdlKeyCode);
	}

	utility::event::KeyboardEvent::KeyModifiers
		EventHandler::convertKeyModifiers(SDL_Keymod modifiers)
	{
		std::uint16_t result = 0;

		if (modifiers & SDL_KMOD_LSHIFT)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::LeftShift);
		if (modifiers & SDL_KMOD_RSHIFT)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::RightShift);
		if (modifiers & SDL_KMOD_LCTRL)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::LeftCtrl);
		if (modifiers & SDL_KMOD_RCTRL)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::RightCtrl);
		if (modifiers & SDL_KMOD_LALT)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::LeftAlt);
		if (modifiers & SDL_KMOD_RALT)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::RightAlt);
		if (modifiers & SDL_KMOD_LGUI)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::LeftGUI);
		if (modifiers & SDL_KMOD_RGUI)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::RightGUI);
		if (modifiers & SDL_KMOD_NUM)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::NumLock);
		if (modifiers & SDL_KMOD_CAPS)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::CapsLock);
		if (modifiers & SDL_KMOD_MODE)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::AltGr);
		if (modifiers & SDL_KMOD_SCROLL)
			result |= static_cast<std::uint16_t>(
				utility::event::KeyboardEvent::KeyModifiers::ScrollLock);

		return static_cast<utility::event::KeyboardEvent::KeyModifiers>(result);
	}

	utility::event::MouseButtonEvent::Button
		EventHandler::convertMouseButton(std::uint8_t sdlButton)
	{
		// SDL3 mouse button values map directly to the utility Button enum
		return static_cast<utility::event::MouseButtonEvent::Button>(
			sdlButton);
	}

}	 // namespace simple_application
