# Getting Started

This tutorial walks you from an empty project to a working Guillaume scene. It
assumes you have already [built the library](../README.md#building).

## 1. Create a utility::Engine implementation

Guillaume is engine-coupled: rendering and event polling are delegated to a
`utility::Engine` subclass. Implement the pure virtual methods for your
platform:

```cpp
#include <utility/engine.hpp>

class MyEngine : public utility::Engine {
    public:
    void clear(void) override { /* clear the back buffer */ }
    void present(void) override { /* swap buffers */ }
    size_t createObject(
        std::shared_ptr<utility::graphic::Renderable> object) override {
        return 0;
    }
    bool updateObject(std::shared_ptr<utility::graphic::Renderable> object,
                      size_t objectID) override { return true; }
    bool removeObject(std::shared_ptr<utility::graphic::Renderable> object,
                      size_t objectID) override { return true; }
    utility::graphic::SizeF
        measureText(const utility::graphic::Text &text) const override {
        return { 0.0f, 0.0f };
    }
    utility::graphic::ViewF getView(void) const override {
        return utility::graphic::ViewF();
    }
    void pollEvents(void) override {}
    void update(void) override {}
};
```

## 2. Define a scene and build entities

A `Scene` owns its entity and component registries. Build your UI in
`onEnter()`:

```cpp
#include <guillaume/scene.hpp>

class MyScene : public guillaume::Scene {
    public:
    using guillaume::Scene::Scene;

    void onEnter(void) override {
        // Register components and add entities to the entity registry.
    }
};
```

## 3. Run the main loop

Wire the application together and call `run()`:

```cpp
#include <guillaume/application.hpp>

int main() {
    auto resources = std::make_shared<utility::RessourceProvider>(...);
    guillaume::Application<MyScene> app(resources);
    app.setEngine(std::make_unique<MyEngine>());
    return app.run();
}
```

`Application::run()` drives the frame loop: it polls events, clears, updates,
runs the system phases, processes scene transitions, and presents.

## 4. Handle an input event and render

Subscribe to events through the `EventBus` (available on the application) and
let systems react. For example, a `KeyboardControl` system already handles
text editing for entities with `Text` and `Focus` components.

## Next steps

- See the [examples](../examples/) directory for a runnable sample.
- Read [How Guillaume Works](HOW_GUILLAUME_WORKS.md) for the frame lifecycle.
- Browse the [API reference](https://etib-corp.github.io/guillaume).
