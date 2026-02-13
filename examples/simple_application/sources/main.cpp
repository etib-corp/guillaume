#include <guillaume/ecs/entity_filler.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/shapes/triangle.hpp>

#include <utility/logging/standard_logger.hpp>

#include "application.hpp"
#include "triangle_render_system.hpp"
#include "rectangle_render_system.hpp"
#include "circle_render_system.hpp"

class MyEntity
    : public guillaume::ecs::EntityFiller<guillaume::components::Transform,
                                          guillaume::components::Bound,
                                          guillaume::components::Color,
                                          simple_application::TriangleComponent,
                                          simple_application::RectangleComponent,
                                          simple_application::CircleComponent> {
  public:
    MyEntity(guillaume::ecs::ComponentRegistry &componentRegistry)
        : guillaume::ecs::EntityFiller<guillaume::components::Transform,
                                       guillaume::components::Bound,
                                       guillaume::components::Color,
                                       simple_application::TriangleComponent,
                                       simple_application::RectangleComponent,
                                       simple_application::CircleComponent>(
              componentRegistry) {}
};

int main(int argc, char *argv[]) {
    simple_application::Application application(argc, argv);
    guillaume::ECS &ecs = application.getECS();

    auto triangleSystem = std::make_unique<simple_application::TriangleRenderSystem>(
        ecs.getRenderer());
    ecs.getSystemRegistry().registerNewSystem<simple_application::TriangleRenderSystem>(
        std::move(triangleSystem));

    auto rectangleSystem = std::make_unique<simple_application::RectangleRenderSystem>(
        ecs.getRenderer());
    ecs.getSystemRegistry().registerNewSystem<simple_application::RectangleRenderSystem>(
        std::move(rectangleSystem));

    auto circleSystem = std::make_unique<simple_application::CircleRenderSystem>(
        ecs.getRenderer());
    ecs.getSystemRegistry().registerNewSystem<simple_application::CircleRenderSystem>(
        std::move(circleSystem));

    MyEntity rectangle(ecs.getComponentRegistry());
    ecs.addEntity(rectangle);
    // ecs.addComponent<guillaume::components::Bound, guillaume::components::Bound::Size>(rectangle.getIdentifier(), {150.0f, 100.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        rectangle.getIdentifier())
        .setPosition({200.0f, 300.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        rectangle.getIdentifier())
        .setScale({2.0f, 2.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Bound>(
        rectangle.getIdentifier())
        .setSize({150.0f, 100.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Color>(
        rectangle.getIdentifier())
        .setRGBA(0, 0, 255, 255);

    ecs.getComponentRegistry().getComponent<simple_application::RectangleComponent>(
        rectangle.getIdentifier())
        .setIsRectangleShape(true);

    MyEntity triangle(ecs.getComponentRegistry());
    ecs.addEntity(triangle);

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        triangle.getIdentifier())
        .setPosition({300.0f, 500.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        triangle.getIdentifier())
        .setScale({2.0f, 2.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Color>(
        triangle.getIdentifier())
        .setRGBA(255, 0, 0, 255);

    ecs.getComponentRegistry().getComponent<simple_application::TriangleComponent>(
        triangle.getIdentifier())
        .setIsTriangleShape(true);

    MyEntity circle(ecs.getComponentRegistry());
    ecs.addEntity(circle);

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        circle.getIdentifier())
        .setPosition({400.0f, 600.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        circle.getIdentifier())
        .setScale({2.0f, 2.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Color>(
        circle.getIdentifier())
        .setRGBA(0, 255, 0, 255);

    ecs.getComponentRegistry().getComponent<simple_application::CircleComponent>(
        circle.getIdentifier())
        .setIsCircleShape(true);

    return application.run();
}
