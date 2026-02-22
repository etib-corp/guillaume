#include <guillaume/ecs/entity_filler.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/shapes/triangle.hpp>

#include <utility/logging/standard_logger.hpp>

#include "application.hpp"

int main(int argc, char *argv[]) {
    simple_application::Application application(argc, argv);

    return application.run();
}
