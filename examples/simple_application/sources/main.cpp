#include <guillaume/ecs/entity_filler.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/transform.hpp>

#include <utility/logging/standard_logger.hpp>

#include "application.hpp"

class MyEntity
    : public guillaume::ecs::EntityFiller<guillaume::components::Transform,
                                          guillaume::components::Bound> {
  public:
    MyEntity(guillaume::ecs::ComponentRegistry &componentRegistry)
        : guillaume::ecs::EntityFiller<guillaume::components::Transform,
                                       guillaume::components::Bound>(
              componentRegistry) {}
};

int main(int argc, char *argv[]) {
    std::string loggerName = "main";
    utility::logging::StandardLogger logger(loggerName);

    logger.info("Starting simple_application");
    logger.debug("Creating application with argc=" + std::to_string(argc));

    simple_application::Application application(argc, argv);
    guillaume::ECS &ecs = application.getECS();

    // Set clear color to dark blue so we can confirm rendering is working
    logger.info("Setting clear color to dark blue (0, 0, 100)");
    // Note: Direct access to renderer not available through application interface yet
    // This would need to be added to see the color change
    
    logger.info("Creating test entity");
    MyEntity entity(ecs.getComponentRegistry());
    logger.debug("Entity created with ID: " +
                 std::to_string(entity.getIdentifier()));

    // Set up the entity with visible position and size
    auto &transform = ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(entity.getIdentifier());
    transform->setPosition({0.0f, 0.0f, -5.0f}); // Position in front of camera (camera is at z=3 looking at z=0)
    
    auto &bound = ecs.getComponentRegistry().getComponent<guillaume::components::Bound>(entity.getIdentifier());
    utility::math::Rectangle<float> rect;
    rect.setSize({2.0f, 2.0f}); // 2x2 unit rectangle
    rect.setPosition({-1.0f, -1.0f}); // Centered
    bound->setRectangle(rect);
    
    logger.debug("Entity configured with position (0, 0, -5) and size (2, 2)");

    ecs.addEntity(entity);
    logger.debug("Entity added to ECS");

    logger.info("Starting application main loop");
    int result = application.run();

    logger.info("Application exited with code: " + std::to_string(result));
    return result;
}
