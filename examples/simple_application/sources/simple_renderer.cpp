#include "simple_renderer.hpp"

#include <sstream>
#include <string>

namespace {

std::string formatColor(const utility::Color<uint8_t> &color) {
  std::ostringstream stream;
  stream << "rgba(" << static_cast<int>(color.red()) << ", "
         << static_cast<int>(color.green()) << ", "
         << static_cast<int>(color.blue()) << ", "
         << static_cast<int>(color.alpha()) << ")";
  return stream.str();
}

template <typename Type>
std::string formatVector(const utility::Vector<Type, 2> &vector) {
  std::ostringstream stream;
  stream << "(" << vector[0] << ", " << vector[1] << ")";
  return stream.str();
}

} // namespace

namespace guillaume::simple_application {

SimpleRenderer::SimpleRenderer()
    : _logger(), _drawColor(0, 0, 0, 255), _scale({1.0F, 1.0F}) {
  _logger.info("SimpleRenderer initialised");
}

void SimpleRenderer::clear(void) { _logger.debug("Renderer clear"); }

void SimpleRenderer::present(void) { _logger.debug("Renderer present"); }

void SimpleRenderer::setDrawColor(utility::Color<uint8_t> color) {
  _drawColor = color;
  _logger.debug("Renderer draw color set to " + formatColor(color));
}

utility::Color<uint8_t> SimpleRenderer::getDrawColor(void) const {
  return _drawColor;
}

void SimpleRenderer::drawPoint(utility::Vector<std::size_t, 2> point) {
  _logger.debug("Renderer draw point at " + formatVector(point));
}

void SimpleRenderer::drawLine(utility::Vector<std::size_t, 2> start,
                              utility::Vector<std::size_t, 2> end) {
  _logger.debug("Renderer draw line from " + formatVector(start) + " to " +
                formatVector(end));
}

void SimpleRenderer::drawRect(utility::Rectangle<std::size_t> rectangle) {
  _logger.debug("Renderer draw rectangle (x=" + std::to_string(rectangle.x()) +
                ", y=" + std::to_string(rectangle.y()) +
                ", w=" + std::to_string(rectangle.width()) +
                ", h=" + std::to_string(rectangle.height()) + ")");
}

void SimpleRenderer::fillRect(utility::Rectangle<std::size_t> rectangle) {
  _logger.debug("Renderer fill rectangle (x=" + std::to_string(rectangle.x()) +
                ", y=" + std::to_string(rectangle.y()) +
                ", w=" + std::to_string(rectangle.width()) +
                ", h=" + std::to_string(rectangle.height()) + ")");
}

void SimpleRenderer::setScale(utility::Vector<float, 2> scale) {
  _scale = scale;
  _logger.debug("Renderer scale set to " + formatVector(scale));
}

utility::Vector<float, 2> SimpleRenderer::getScale(void) const {
  return _scale;
}

bool SimpleRenderer::flush(void) {
  _logger.debug("Renderer flush");
  return true;
}

} // namespace guillaume::simple_application
