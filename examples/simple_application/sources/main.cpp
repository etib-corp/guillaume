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
#include "rotation_system.hpp"
#include "text_render_system.hpp"

#define entityDecl(name, ...) \
    class name : public guillaume::ecs::EntityFiller<__VA_ARGS__> { \
      public: \
        name(guillaume::ecs::ComponentRegistry &componentRegistry) \
            : guillaume::ecs::EntityFiller<__VA_ARGS__>(componentRegistry) {} \
    }

entityDecl(MyRectangle, guillaume::components::Transform, guillaume::components::Bound, guillaume::components::Color, simple_application::RectangleComponent);
entityDecl(MyTriangle, guillaume::components::Transform, guillaume::components::Color, simple_application::TriangleComponent);
entityDecl(MyCircle, guillaume::components::Transform, guillaume::components::Color, simple_application::CircleComponent);
entityDecl(MyText, guillaume::components::Transform, guillaume::components::Color, guillaume::components::Text);

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

    auto rotationSystem = std::make_unique<simple_application::RotationSystem>();
    ecs.getSystemRegistry().registerNewSystem<simple_application::RotationSystem>(
        std::move(rotationSystem));

    auto textSystem = std::make_unique<simple_application::TextRenderSystem>(
        ecs.getRenderer());
    ecs.getSystemRegistry().registerNewSystem<simple_application::TextRenderSystem>(
        std::move(textSystem));

    MyRectangle rectangle(ecs.getComponentRegistry());
    ecs.addEntity(rectangle);

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        rectangle.getIdentifier())
        .setPosition({200.0f, 300.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        rectangle.getIdentifier())
        .setRotation({0.0f, 30.0f, 0.0f});

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

    MyTriangle triangle(ecs.getComponentRegistry());
    ecs.addEntity(triangle);

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        triangle.getIdentifier())
        .setPosition({600.0f, 600.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        triangle.getIdentifier())
        .setRotation({0.0f, 0.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        triangle.getIdentifier())
        .setScale({2.0f, 2.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Color>(
        triangle.getIdentifier())
        .setRGBA(255, 0, 0, 255);

    ecs.getComponentRegistry().getComponent<simple_application::TriangleComponent>(
        triangle.getIdentifier())
        .setIsTriangleShape(true);

    MyCircle circle(ecs.getComponentRegistry());
    ecs.addEntity(circle);

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        circle.getIdentifier())
        .setPosition({1000.0f, 900.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        circle.getIdentifier())
        .setRotation({30.0f, 30.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        circle.getIdentifier())
        .setScale({2.0f, 2.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Color>(
        circle.getIdentifier())
        .setRGBA(0, 255, 0, 255);

    ecs.getComponentRegistry().getComponent<simple_application::CircleComponent>(
        circle.getIdentifier())
        .setIsCircleShape(true);

    MyText text(ecs.getComponentRegistry());
    ecs.addEntity(text);

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        text.getIdentifier())
        .setPosition({400.0f, 200.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        text.getIdentifier())
        .setRotation({0.0f, 0.0f, 0.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Transform>(
        text.getIdentifier())
        .setScale({1.0f, 1.0f, 1.0f});

    ecs.getComponentRegistry().getComponent<guillaume::components::Color>(
        text.getIdentifier())
        .setRGBA(255, 255, 255, 255);

    ecs.getComponentRegistry().getComponent<guillaume::components::Text>(
        text.getIdentifier())
        .setContent("Hello, World!");

    return application.run();
}
