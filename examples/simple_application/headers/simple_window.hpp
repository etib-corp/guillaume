#pragma once

#include <rectangle.hpp>
#include <standard_logger.hpp>
#include <string>
#include <vector.hpp>
#include <window.hpp>

#include "simple_renderer.hpp"

namespace guillaume::simple_application {

class SimpleWindow : public guillaume::Window<SimpleRenderer> {
private:
  enum class WindowState { Normal, Minimized, Maximized };

  utility::StandardLogger _logger;
  std::string _title;
  utility::Vector<float, 2> _position;
  utility::Vector<std::size_t, 2> _size;
  utility::Vector<std::size_t, 2> _minimumSize;
  utility::Vector<std::size_t, 2> _maximumSize;
  float _opacity;
  bool _visible;
  bool _resizable;
  float _minimumAspectRatio;
  float _maximumAspectRatio;
  WindowState _state;

public:
  SimpleWindow(void);
  ~SimpleWindow(void) override = default;

  std::unique_ptr<SimpleRenderer> &getRenderer(void) override;
  std::string getTitle(void) const override;
  void setTitle(const std::string &title) override;
  utility::Vector<float, 2> getPosition(void) const override;
  void setPosition(utility::Vector<float, 2> position) override;
  utility::Vector<std::size_t, 2> getSize() const override;
  void setSize(utility::Vector<std::size_t, 2> size) override;
  void setMinimumSize(utility::Vector<std::size_t, 2> size) override;
  utility::Vector<std::size_t, 2> getMinimumSize() const override;
  void setMaximumSize(utility::Vector<std::size_t, 2> size) override;
  utility::Vector<std::size_t, 2> getMaximumSize(void) const override;
  void show(void) override;
  void hide(void) override;
  bool isVisible(void) const override;
  void maximize(void) override;
  void minimize(void) override;
  void restore(void) override;
  void setOpacity(float opacity) override;
  float getOpacity(void) const override;
  void requestClose(void) override;
  void setAspectRatio(float minimumAspectRatio,
                      float maximumAspectRatio) override;
  void getAspectRatio(float &minimumAspectRatio,
                      float &maximumAspectRatio) const override;
  void setResizable(bool resizable) override;
  bool sync(void) override;
};

} // namespace guillaume::simple_application
