#include "simple_application.hpp"

#include <properties/layout/margin.hpp>
#include <properties/layout/padding.hpp>
#include <properties/style/background_color.hpp>
#include <properties/style/opacity.hpp>

#include <iostream>

int main(void) {
    guillaume::simple_application::SimpleApplication application;
    application.addWindow("main");

    auto &window = application.getWindow("main");
    window.setTitle("Guillaume SDL3 Demo Window");

    while (application.hasOpenWindows() && !application.shouldQuit()) {
        application.routine();
    }

    return 0;
}
