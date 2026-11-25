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

A `Scene` owns the entities of one screen and the `ecs::ComponentRegistry`
that holds their state. Build your UI in `onEnter()` with the builders the
scene provides through `getBuilderManager()`: each builder creates the
widget's components and registers the entity in the scene.

```cpp
#include <guillaume/entities/button.hpp>
#include <guillaume/entities/text.hpp>
#include <guillaume/scene.hpp>
#include <guillaume/theme.hpp>

class MyScene : public guillaume::Scene {
    public:
    using guillaume::Scene::Scene;

    void onEnter(void) override {
        auto &textBuilder = getBuilderManager()
            .getBuilder<guillaume::entities::Text::Builder>();
        textBuilder.makeText(nullptr, "Hello, Guillaume",
            guillaume::theme::TypeRole::TitleLarge,
            guillaume::getActiveColor(
                guillaume::SchemeColorRole::OnSurface));

        auto &buttonBuilder = getBuilderManager()
            .getBuilder<guillaume::entities::Button::Builder>();
        buttonBuilder.makeButton(nullptr, "Save", [] { /* on click */ },
            guillaume::entities::Button::Color::Filled,
            guillaume::entities::Button::Shape::Round,
            guillaume::entities::Button::Size::Medium, false);
    }
};
```

Entities are thin facades: they hold no state and the framework never calls
them back. Each frame, the systems read and write the components of the
active scene. To change a widget after it is built, keep the
`std::shared_ptr` the builder returns and call its setters (for example
`Text::setContent()`), which write its components.

`onEnter()` runs every time the scene becomes active, and the scene keeps its
entities while it is inactive. A scene that can be entered twice removes what
it built in `onExit()` (`getEntityRegistry().clearEntities()`) or builds it
only once.

## 3. Run the main loop

Wire the application together and call `run()`:

```cpp
#include <guillaume/application.hpp>

#include <utility/ressource_provider.hpp>
#include <utility/system_io/default_system_io.hpp>

int main(void) {
    utility::DefaultSystemIO systemIo;
    auto resources = std::make_shared<utility::RessourceProvider>(systemIo);
    guillaume::Application<MyScene> app(resources,
                                        std::make_unique<MyEngine>());
    return app.run();
}
```

The first template argument is the scene shown at start-up; list any other
scenes after it (`Application<HomeScene, SettingsScene>`). The application
takes ownership of the engine and enters the first scene in its constructor.

`Application::run()` repeats `routine()`, one frame, until the engine reports
a quit event. Each frame polls events, clears, updates, runs the system phases
(Event, Measure, Layout, Render) on the active scene, resets the components'
changed flags, processes a pending scene transition, and presents.

## 4. Handle an input event and render

The engine reports input to the application, which publishes each event on
its `event::EventBus`; the systems subscribed to that event type queue it and
consume it in the Event phase, and the render systems draw the result in the
Render phase. The event bus belongs to the application and is not exposed to
scenes.

Your code reacts to input through the widgets: pass a callback when you build
them, such as the `onClick` argument of `makeButton()` above. To switch
screens from a callback, call `goToScene<OtherScene>()`; the switch happens at
the end of the frame. Text editing is already handled by systems:
`KeyboardControl` applies editing keys (arrows, Home, End, Delete, Backspace)
and `TextInput` inserts typed characters, both for entities with `Text` and
`Focus` components.

## Next steps

- See the [examples](../examples/) directory for a runnable sample.
- Read [How Guillaume Works](HOW_GUILLAUME_WORKS.md) for the frame lifecycle.
- Browse the [API reference](https://etib-corp.github.io/guillaume).
