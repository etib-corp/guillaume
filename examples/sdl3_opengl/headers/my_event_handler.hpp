/*
 Copyright (c) 2025 ETIB Corporation

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

#include "event_handler.hpp"
#include <SDL3/SDL.h>

namespace Guillaume {

/**
 * @brief SDL3-specific event handler implementation.
 *
 * This class implements the EventHandler interface for SDL3,
 * handling mouse, keyboard, and window events.
 */
class MyEventHandler : public EventHandler {
public:
  /**
   * @brief Construct a new SDL3 Event Handler object
   */
  MyEventHandler();

  /**
   * @brief Destroy the SDL3 Event Handler object
   */
  ~MyEventHandler() override = default;

  /**
   * @brief Set the SDL window for this event handler.
   *
   * @param window The SDL window to use for event handling
   */
  void setWindow(SDL_Window *window);

  /**
   * @brief Get the SDL window used by this event handler.
   *
   * @return SDL_Window* The SDL window
   */
  SDL_Window *getWindow() const;

  /**
   * @brief Check if the application should continue running.
   *
   * @return true if the application should continue
   * @return false if the user requested to quit
   */
  bool shouldContinue() const override;

  /**
   * @brief Poll for SDL events and convert them to Guillaume events.
   *
   * @return std::vector<Event> Vector of Guillaume events
   */
  std::vector<Event> pollEvents() override;

private:
  SDL_Window *_window;
  bool _shouldContinue;

  /**
   * @brief Convert an SDL event to a Guillaume event.
   *
   * @param sdlEvent The SDL event to convert
   * @return Event The converted Guillaume event
   */
  Event convertSDLEvent(const SDL_Event &sdlEvent);

  /**
   * @brief Find the component at a specific position.
   *
   * @param x The x coordinate
   * @param y The y coordinate
   * @return Component* The component at the position, or nullptr
   */
  Component *findComponentAtPosition(float x, float y);
};

} // namespace Guillaume
