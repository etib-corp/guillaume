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

#include <event_handler.hpp>
#include <SDL3/SDL.h>

/**
 * @brief SDL3 implementation of the EventHandler interface.
 */
class SimpleEventHandler : public guillaume::EventHandler {
public:
  SimpleEventHandler();
  ~SimpleEventHandler() override = default;
  
  /**
   * @brief Poll for SDL3 events and convert them to Guillaume events.
   * @return True if events were processed.
   */
  bool pollEvents() override;
  
  /**
   * @brief Set the event callback function.
   * @param callback Function to call when an event is received.
   */
  void setEventCallback(const guillaume::EventCallbackFunction& callback) override;
  
  /**
   * @brief Check if a quit event has been received.
   * @return True if a quit event was received.
   */
  bool shouldQuit() const override;

private:
  /**
   * @brief Convert SDL key code to Guillaume key code.
   * @param sdlKey SDL key code.
   * @return Guillaume key code.
   */
  guillaume::KeyCode convertKeyCode(SDL_Keycode sdlKey);
  
  /**
   * @brief Convert SDL mouse button to Guillaume mouse button.
   * @param sdlButton SDL mouse button.
   * @return Guillaume mouse button.
   */
  guillaume::MouseButton convertMouseButton(uint8_t sdlButton);
  
  guillaume::EventCallbackFunction _callback;
  bool _shouldQuit = false;
};
