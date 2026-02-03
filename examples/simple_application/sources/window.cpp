#include "window.hpp"

namespace simple_application {

Window::Window(void) : guillaume::Window() {
    getLogger().info("Window created");
}

Window::~Window(void) { getLogger().info("Window destroyed"); }

utility::math::Vector<std::size_t, 2> Window::getSize(void) const {
    return {0, 0};
}

} // namespace simple_application
