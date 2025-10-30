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

#include "my_event_handler.hpp"
#include "component.hpp"
#include "component_tree.hpp"
#include "point.hpp"
#include "vector.hpp"
#include <iostream>

namespace Guillaume {

MyEventHandler::MyEventHandler()
    : EventHandler(), _window(nullptr), _shouldContinue(true) {}

void MyEventHandler::setWindow(SDL_Window *window) { _window = window; }

SDL_Window *MyEventHandler::getWindow() const { return _window; }

bool MyEventHandler::shouldContinue() const { return _shouldContinue; }

std::vector<Event> MyEventHandler::pollEvents() {
  std::vector<Event> events;

  if (!_window) {
    return events;
  }

  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    Event guillaumerEvent = convertSDLEvent(sdlEvent);
    if (!guillaumerEvent.getType().empty()) {
      events.push_back(guillaumerEvent);
    }
  }

  return events;
}

Event MyEventHandler::convertSDLEvent(const SDL_Event &sdlEvent) {
  std::string eventType = "";
  std::shared_ptr<Component> target = nullptr;
  std::any eventData;

  switch (sdlEvent.type) {
  case SDL_EVENT_QUIT:
    _shouldContinue = false;
    eventType = "window_close";
    break;

  case SDL_EVENT_MOUSE_BUTTON_DOWN: {
    eventType = "mouse_button_down";
    float x = sdlEvent.button.x;
    float y = sdlEvent.button.y;
    Component *foundComponent = findComponentAtPosition(x, y);
    if (foundComponent) {
      // Need to get shared_ptr - for now use nullptr, will need to refactor
      target = nullptr; // TODO: Get shared_ptr from component tree
    }
    break;
  }

  case SDL_EVENT_MOUSE_BUTTON_UP: {
    eventType = "mouse_button_up";
    float x = sdlEvent.button.x;
    float y = sdlEvent.button.y;
    Component *foundComponent = findComponentAtPosition(x, y);
    if (foundComponent) {
      target = nullptr; // TODO: Get shared_ptr from component tree
    }
    break;
  }

  case SDL_EVENT_MOUSE_MOTION: {
    eventType = "mouse_motion";
    float x = sdlEvent.motion.x;
    float y = sdlEvent.motion.y;
    Component *foundComponent = findComponentAtPosition(x, y);
    if (foundComponent) {
      target = nullptr; // TODO: Get shared_ptr from component tree
    }
    break;
  }

  case SDL_EVENT_KEY_DOWN: {
    eventType = "key_down";
    break;
  }

  case SDL_EVENT_KEY_UP: {
    eventType = "key_up";
    break;
  }

  case SDL_EVENT_WINDOW_RESIZED:
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
    eventType = "window_resize";
    int width = 0, height = 0;
    SDL_GetWindowSize(_window, &width, &height);
    break;
  }

  default:
    break;
  }

  return Event(eventType, target, eventData);
}

Component *MyEventHandler::findComponentAtPosition(float x, float y) {
  (void)x; // Unused for now
  (void)y; // Unused for now

  // Simplified implementation - just return nullptr for now
  // Full hit-testing would require accessing component bounds from Properties
  // which is more complex than needed for this example
  return nullptr;
}

} // namespace Guillaume
