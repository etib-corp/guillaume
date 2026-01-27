#include "simple_application.hpp"

#include <iostream>

namespace guillaume::simple_application {

SimpleApplication::SimpleApplication(int argc, char *argv[])
    : guillaume::Application<SimpleWindow, SimpleRenderer,
                             SimpleEventHandler>() {}

} // namespace guillaume::simple_application