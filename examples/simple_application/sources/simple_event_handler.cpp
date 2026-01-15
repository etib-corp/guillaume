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

#include "simple_event_handler.hpp"

SimpleEventHandler::SimpleEventHandler()
    : _callback(nullptr), _shouldQuit(false) {}

bool SimpleEventHandler::pollEvents() {
  SDL_Event event;
  bool hasEvents = false;

  while (SDL_PollEvent(&event)) {
    hasEvents = true;

    if (!_callback) {
      continue;
    }

    switch (event.type) {
    case SDL_EVENT_QUIT: {
      _shouldQuit = true;
      guillaume::AppQuitEvent quitEvent;
      _callback(quitEvent);
      break;
    }

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
      _shouldQuit = true;
      guillaume::WindowCloseEvent closeEvent;
      _callback(closeEvent);
      break;
    }

    case SDL_EVENT_WINDOW_RESIZED: {
      guillaume::WindowResizeEvent resizeEvent(
          static_cast<std::size_t>(event.window.data1),
          static_cast<std::size_t>(event.window.data2));
      _callback(resizeEvent);
      break;
    }

    case SDL_EVENT_KEY_DOWN: {
      guillaume::KeyPressedEvent keyEvent(convertKeyCode(event.key.key),
                                          event.key.repeat != 0);
      _callback(keyEvent);
      break;
    }

    case SDL_EVENT_KEY_UP: {
      guillaume::KeyReleasedEvent keyEvent(convertKeyCode(event.key.key));
      _callback(keyEvent);
      break;
    }

    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      guillaume::MouseButtonPressedEvent mouseEvent(
          convertMouseButton(event.button.button));
      _callback(mouseEvent);
      break;
    }

    case SDL_EVENT_MOUSE_BUTTON_UP: {
      guillaume::MouseButtonReleasedEvent mouseEvent(
          convertMouseButton(event.button.button));
      _callback(mouseEvent);
      break;
    }

    case SDL_EVENT_MOUSE_MOTION: {
      guillaume::MouseMovedEvent mouseEvent(event.motion.x, event.motion.y);
      _callback(mouseEvent);
      break;
    }

    case SDL_EVENT_MOUSE_WHEEL: {
      guillaume::MouseScrolledEvent scrollEvent(event.wheel.x, event.wheel.y);
      _callback(scrollEvent);
      break;
    }

    default:
      break;
    }
  }

  return hasEvents;
}

void SimpleEventHandler::setEventCallback(
    const guillaume::EventCallbackFunction &callback) {
  _callback = callback;
}

bool SimpleEventHandler::shouldQuit() const { return _shouldQuit; }

guillaume::KeyCode SimpleEventHandler::convertKeyCode(SDL_Keycode sdlKey) {
  switch (sdlKey) {
  case SDLK_SPACE:
    return guillaume::KeyCode::Space;
  case SDLK_APOSTROPHE:
    return guillaume::KeyCode::Apostrophe;
  case SDLK_COMMA:
    return guillaume::KeyCode::Comma;
  case SDLK_MINUS:
    return guillaume::KeyCode::Minus;
  case SDLK_PERIOD:
    return guillaume::KeyCode::Period;
  case SDLK_SLASH:
    return guillaume::KeyCode::Slash;

  case SDLK_0:
    return guillaume::KeyCode::Num0;
  case SDLK_1:
    return guillaume::KeyCode::Num1;
  case SDLK_2:
    return guillaume::KeyCode::Num2;
  case SDLK_3:
    return guillaume::KeyCode::Num3;
  case SDLK_4:
    return guillaume::KeyCode::Num4;
  case SDLK_5:
    return guillaume::KeyCode::Num5;
  case SDLK_6:
    return guillaume::KeyCode::Num6;
  case SDLK_7:
    return guillaume::KeyCode::Num7;
  case SDLK_8:
    return guillaume::KeyCode::Num8;
  case SDLK_9:
    return guillaume::KeyCode::Num9;

  case SDLK_SEMICOLON:
    return guillaume::KeyCode::Semicolon;
  case SDLK_EQUALS:
    return guillaume::KeyCode::Equal;

  case SDLK_A:
    return guillaume::KeyCode::A;
  case SDLK_B:
    return guillaume::KeyCode::B;
  case SDLK_C:
    return guillaume::KeyCode::C;
  case SDLK_D:
    return guillaume::KeyCode::D;
  case SDLK_E:
    return guillaume::KeyCode::E;
  case SDLK_F:
    return guillaume::KeyCode::F;
  case SDLK_G:
    return guillaume::KeyCode::G;
  case SDLK_H:
    return guillaume::KeyCode::H;
  case SDLK_I:
    return guillaume::KeyCode::I;
  case SDLK_J:
    return guillaume::KeyCode::J;
  case SDLK_K:
    return guillaume::KeyCode::K;
  case SDLK_L:
    return guillaume::KeyCode::L;
  case SDLK_M:
    return guillaume::KeyCode::M;
  case SDLK_N:
    return guillaume::KeyCode::N;
  case SDLK_O:
    return guillaume::KeyCode::O;
  case SDLK_P:
    return guillaume::KeyCode::P;
  case SDLK_Q:
    return guillaume::KeyCode::Q;
  case SDLK_R:
    return guillaume::KeyCode::R;
  case SDLK_S:
    return guillaume::KeyCode::S;
  case SDLK_T:
    return guillaume::KeyCode::T;
  case SDLK_U:
    return guillaume::KeyCode::U;
  case SDLK_V:
    return guillaume::KeyCode::V;
  case SDLK_W:
    return guillaume::KeyCode::W;
  case SDLK_X:
    return guillaume::KeyCode::X;
  case SDLK_Y:
    return guillaume::KeyCode::Y;
  case SDLK_Z:
    return guillaume::KeyCode::Z;

  case SDLK_LEFTBRACKET:
    return guillaume::KeyCode::LeftBracket;
  case SDLK_BACKSLASH:
    return guillaume::KeyCode::Backslash;
  case SDLK_RIGHTBRACKET:
    return guillaume::KeyCode::RightBracket;
  case SDLK_GRAVE:
    return guillaume::KeyCode::GraveAccent;

  case SDLK_ESCAPE:
    return guillaume::KeyCode::Escape;
  case SDLK_RETURN:
    return guillaume::KeyCode::Enter;
  case SDLK_TAB:
    return guillaume::KeyCode::Tab;
  case SDLK_BACKSPACE:
    return guillaume::KeyCode::Backspace;
  case SDLK_INSERT:
    return guillaume::KeyCode::Insert;
  case SDLK_DELETE:
    return guillaume::KeyCode::Delete;
  case SDLK_RIGHT:
    return guillaume::KeyCode::Right;
  case SDLK_LEFT:
    return guillaume::KeyCode::Left;
  case SDLK_DOWN:
    return guillaume::KeyCode::Down;
  case SDLK_UP:
    return guillaume::KeyCode::Up;
  case SDLK_PAGEUP:
    return guillaume::KeyCode::PageUp;
  case SDLK_PAGEDOWN:
    return guillaume::KeyCode::PageDown;
  case SDLK_HOME:
    return guillaume::KeyCode::Home;
  case SDLK_END:
    return guillaume::KeyCode::End;

  case SDLK_CAPSLOCK:
    return guillaume::KeyCode::CapsLock;
  case SDLK_SCROLLLOCK:
    return guillaume::KeyCode::ScrollLock;
  case SDLK_NUMLOCKCLEAR:
    return guillaume::KeyCode::NumLock;
  case SDLK_PRINTSCREEN:
    return guillaume::KeyCode::PrintScreen;
  case SDLK_PAUSE:
    return guillaume::KeyCode::Pause;

  case SDLK_F1:
    return guillaume::KeyCode::F1;
  case SDLK_F2:
    return guillaume::KeyCode::F2;
  case SDLK_F3:
    return guillaume::KeyCode::F3;
  case SDLK_F4:
    return guillaume::KeyCode::F4;
  case SDLK_F5:
    return guillaume::KeyCode::F5;
  case SDLK_F6:
    return guillaume::KeyCode::F6;
  case SDLK_F7:
    return guillaume::KeyCode::F7;
  case SDLK_F8:
    return guillaume::KeyCode::F8;
  case SDLK_F9:
    return guillaume::KeyCode::F9;
  case SDLK_F10:
    return guillaume::KeyCode::F10;
  case SDLK_F11:
    return guillaume::KeyCode::F11;
  case SDLK_F12:
    return guillaume::KeyCode::F12;
  case SDLK_F13:
    return guillaume::KeyCode::F13;
  case SDLK_F14:
    return guillaume::KeyCode::F14;
  case SDLK_F15:
    return guillaume::KeyCode::F15;
  case SDLK_F16:
    return guillaume::KeyCode::F16;
  case SDLK_F17:
    return guillaume::KeyCode::F17;
  case SDLK_F18:
    return guillaume::KeyCode::F18;
  case SDLK_F19:
    return guillaume::KeyCode::F19;
  case SDLK_F20:
    return guillaume::KeyCode::F20;
  case SDLK_F21:
    return guillaume::KeyCode::F21;
  case SDLK_F22:
    return guillaume::KeyCode::F22;
  case SDLK_F23:
    return guillaume::KeyCode::F23;
  case SDLK_F24:
    return guillaume::KeyCode::F24;

  case SDLK_KP_0:
    return guillaume::KeyCode::KP0;
  case SDLK_KP_1:
    return guillaume::KeyCode::KP1;
  case SDLK_KP_2:
    return guillaume::KeyCode::KP2;
  case SDLK_KP_3:
    return guillaume::KeyCode::KP3;
  case SDLK_KP_4:
    return guillaume::KeyCode::KP4;
  case SDLK_KP_5:
    return guillaume::KeyCode::KP5;
  case SDLK_KP_6:
    return guillaume::KeyCode::KP6;
  case SDLK_KP_7:
    return guillaume::KeyCode::KP7;
  case SDLK_KP_8:
    return guillaume::KeyCode::KP8;
  case SDLK_KP_9:
    return guillaume::KeyCode::KP9;
  case SDLK_KP_DECIMAL:
    return guillaume::KeyCode::KPDecimal;
  case SDLK_KP_DIVIDE:
    return guillaume::KeyCode::KPDivide;
  case SDLK_KP_MULTIPLY:
    return guillaume::KeyCode::KPMultiply;
  case SDLK_KP_MINUS:
    return guillaume::KeyCode::KPSubtract;
  case SDLK_KP_PLUS:
    return guillaume::KeyCode::KPAdd;
  case SDLK_KP_ENTER:
    return guillaume::KeyCode::KPEnter;
  case SDLK_KP_EQUALS:
    return guillaume::KeyCode::KPEqual;

  case SDLK_LSHIFT:
    return guillaume::KeyCode::LeftShift;
  case SDLK_LCTRL:
    return guillaume::KeyCode::LeftControl;
  case SDLK_LALT:
    return guillaume::KeyCode::LeftAlt;
  case SDLK_LGUI:
    return guillaume::KeyCode::LeftSuper;
  case SDLK_RSHIFT:
    return guillaume::KeyCode::RightShift;
  case SDLK_RCTRL:
    return guillaume::KeyCode::RightControl;
  case SDLK_RALT:
    return guillaume::KeyCode::RightAlt;
  case SDLK_RGUI:
    return guillaume::KeyCode::RightSuper;
  case SDLK_MENU:
    return guillaume::KeyCode::Menu;

  default:
    return guillaume::KeyCode::Unknown;
  }
}

guillaume::MouseButton
SimpleEventHandler::convertMouseButton(uint8_t sdlButton) {
  switch (sdlButton) {
  case SDL_BUTTON_LEFT:
    return guillaume::MouseButton::Left;
  case SDL_BUTTON_MIDDLE:
    return guillaume::MouseButton::Middle;
  case SDL_BUTTON_RIGHT:
    return guillaume::MouseButton::Right;
  case SDL_BUTTON_X1:
    return guillaume::MouseButton::X1;
  case SDL_BUTTON_X2:
    return guillaume::MouseButton::X2;
  default:
    return guillaume::MouseButton::Left;
  }
}
