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

#include <SDL3/SDL.h>

#include <guillaume/event/event_handler.hpp>

#include <utility/logging/loggable.hpp>

#include <utility/event/keyboard_event.hpp>
#include <utility/event/mouse_button_event.hpp>

class EventHandler : public guillaume::event::EventHandler,
                     public utility::logging::Loggable {
  private:
    utility::event::KeyboardEvent::ScanCode
    convertScanCode(SDL_Scancode sdlScanCode);

    utility::event::KeyboardEvent::KeyCode
    convertKeyCode(SDL_Keycode sdlKeyCode);

    utility::event::KeyboardEvent::KeyModifiers
    convertKeyModifiers(SDL_Keymod modifiers);

    utility::event::MouseButtonEvent::MouseButton
    convertMouseButton(std::uint8_t sdlButton);

  public:
    EventHandler(void);
    ~EventHandler(void) override;
    bool pollEvents(void) override;
};
