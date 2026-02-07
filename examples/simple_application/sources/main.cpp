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
    simple_application::Application application(argc, argv);
    guillaume::ECS &ecs = application.getECS();
    MyEntity myEntity(ecs.getComponentRegistry());

    ecs.addEntity(myEntity);

    return application.run();
}
