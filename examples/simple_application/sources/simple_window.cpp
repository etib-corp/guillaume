#include "simple_window.hpp"

#include <algorithm>
#include <sstream>
#include <string>

namespace {

template <typename Type>
std::string formatVector(const utility::Vector<Type, 2> &vector) {
  std::ostringstream stream;
  stream << "(" << vector[0] << ", " << vector[1] << ")";
  return stream.str();
}

} // namespace

namespace guillaume::simple_application {

SimpleWindow::SimpleWindow()
    : _logger(), _title("Guillaume Simple Window"), _position({0.0F, 0.0F}),
      _size({800, 600}), _minimumSize({100, 100}), _maximumSize({3840, 2160}),
      _opacity(1.0F), _visible(false), _resizable(true),
      _minimumAspectRatio(0.0F), _maximumAspectRatio(0.0F),
      _state(WindowState::Normal) {
  _logger.info("SimpleWindow created with title '" + _title + "'");
}

std::unique_ptr<SimpleRenderer> &SimpleWindow::getRenderer(void) {
  static std::unique_ptr<SimpleRenderer> renderer =
      std::make_unique<SimpleRenderer>();
  return renderer;
}

std::string SimpleWindow::getTitle(void) const { return _title; }

void SimpleWindow::setTitle(const std::string &title) {
  _title = title;
  _logger.info("Window title set to '" + _title + "'");
}

utility::Vector<float, 2> SimpleWindow::getPosition(void) const {
  return _position;
}

void SimpleWindow::setPosition(utility::Vector<float, 2> position) {
  _position = position;
  _logger.debug("Window moved to " + formatVector(position));
}

utility::Vector<std::size_t, 2> SimpleWindow::getSize() const { return _size; }

void SimpleWindow::setSize(utility::Vector<std::size_t, 2> size) {
  _size = size;
  _logger.debug("Window size set to " + formatVector(size));
}

void SimpleWindow::setMinimumSize(utility::Vector<std::size_t, 2> size) {
  _minimumSize = size;
  _logger.debug("Window minimum size set to " + formatVector(size));
}

utility::Vector<std::size_t, 2> SimpleWindow::getMinimumSize() const {
  return _minimumSize;
}

void SimpleWindow::setMaximumSize(utility::Vector<std::size_t, 2> size) {
  _maximumSize = size;
  _logger.debug("Window maximum size set to " + formatVector(size));
}

utility::Vector<std::size_t, 2> SimpleWindow::getMaximumSize(void) const {
  return _maximumSize;
}

void SimpleWindow::show(void) {
  _visible = true;
  _logger.info("Window shown");
}

void SimpleWindow::hide(void) {
  _visible = false;
  _logger.info("Window hidden");
}

bool SimpleWindow::isVisible(void) const { return _visible; }

void SimpleWindow::maximize(void) {
  _state = WindowState::Maximized;
  _logger.info("Window maximized");
}

void SimpleWindow::minimize(void) {
  _state = WindowState::Minimized;
  _logger.info("Window minimized");
}

void SimpleWindow::restore(void) {
  _state = WindowState::Normal;
  _logger.info("Window restored");
}

void SimpleWindow::setOpacity(float opacity) {
  _opacity = std::clamp(opacity, 0.0F, 1.0F);
  _logger.debug("Window opacity set to " + std::to_string(_opacity));
}

float SimpleWindow::getOpacity(void) const { return _opacity; }

void SimpleWindow::requestClose(void) {
  _visible = false;
  _logger.info("Window close requested");
}

void SimpleWindow::setAspectRatio(float minimumAspectRatio,
                                  float maximumAspectRatio) {
  _minimumAspectRatio = minimumAspectRatio;
  _maximumAspectRatio = maximumAspectRatio;
  _logger.debug(
      "Window aspect ratio set min=" + std::to_string(_minimumAspectRatio) +
      " max=" + std::to_string(_maximumAspectRatio));
}

void SimpleWindow::getAspectRatio(float &minimumAspectRatio,
                                  float &maximumAspectRatio) const {
  minimumAspectRatio = _minimumAspectRatio;
  maximumAspectRatio = _maximumAspectRatio;
}

void SimpleWindow::setResizable(bool resizable) {
  _resizable = resizable;
  _logger.info(std::string("Window resizable set to ") +
               (_resizable ? "true" : "false"));
}

bool SimpleWindow::sync(void) {
  _logger.debug("Window sync complete");
  return true;
}

} // namespace guillaume::simple_application
