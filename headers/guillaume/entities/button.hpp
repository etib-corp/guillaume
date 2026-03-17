/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <memory>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/node_entity_builder.hpp"
#include "guillaume/ecs/node_entity_filler.hpp"

#include "guillaume/components/click.hpp"
#include "guillaume/components/hover.hpp"
#include "guillaume/components/render.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Button entity class representing a UI button with various components.
 */
class Button
    : public ecs::NodeEntityFiller<components::Click, components::Hover,
                                   components::Transform, components::Render> {
  public:
    /**
     * @brief State of a toggle button, which can be either Default or Selected.
     */
    enum class TogleState { Default, Selected };

    /**
     * @brief Color style of the button.
     */
    enum class ColorStyle { Elevated, Filled, Tonal, Outlined, Text };

    /**
     * @brief Shape of the button.
     */
    enum class Shape { Round, Square };

    /**
     * @brief Size of the button.
     */
    enum class Size { ExtraSmall, Small, Medium, Large, ExtraLarge };

    /**
     * @brief Morph state of the button, which can be Default, Pressed, or
     * Selected.
     */
    enum class MorphState { Default, Pressed, Selected };

    /**
     * @brief Builder used to configure and create `Button` entities.
     */
    class Builder : public ecs::NodeEntityBuilder {
      private:
        std::unique_ptr<Button>
            _button; ///< Unique pointer to the Button entity being built
        TogleState _toggleState; ///< Current toggle state
        ColorStyle _colorStyle;  ///< Button color style
        Shape _shape;            ///< Button shape
        Size _size;              ///< Button size
        MorphState _morphState;  ///< Current morph state

      public:
        /**
         * @brief Construct a new Button Builder object.
         */
        Builder(void);

        /**
         * @brief Default destructor for the Button Builder class.
         */
        ~Builder(void);

        /**
         * @brief Get the entity being built.
         * @param componentRegistry The component registry used to create and
         * initialize button components.
         * @return Unique pointer to the Button entity being built.
         */
        std::unique_ptr<ecs::Entity>
        getEntity(ecs::ComponentRegistry &componentRegistry) override {
            _button = std::make_unique<Button>(componentRegistry, _toggleState,
                                               _colorStyle, _shape, _size,
                                               _morphState);
            return std::move(_button);
        }

        /**
         * @brief Reset the builder to its initial state for creating a new
         * Button entity.
         */
        void reset(void) override {
            _button.reset();
            _toggleState = TogleState::Default;
            _colorStyle = ColorStyle::Filled;
            _shape = Shape::Round;
            _size = Size::Small;
            _morphState = MorphState::Default;
        }

        /**
         * @brief Set the toggle state of the button.
         * @param toggleState The new toggle state to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withToggleState(TogleState toggleState) {
            _toggleState = toggleState;
            return *this;
        }

        /**
         * @brief Set the color style of the button.
         * @param colorStyle The new color style to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withColorStyle(ColorStyle colorStyle) {
            _colorStyle = colorStyle;
            return *this;
        }

        /**
         * @brief Set the shape of the button.
         * @param shape The new shape to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withShape(Shape shape) {
            _shape = shape;
            return *this;
        }

        /**
         * @brief Set the size of the button.
         * @param size The new size to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withSize(Size size) {
            _size = size;
            return *this;
        }

        /**
         * @brief Set the morph state of the button.
         * @param morphState The new morph state to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withMorphState(MorphState morphState) {
            _morphState = morphState;
            return *this;
        }
    };

    /**
     * @brief Director that orchestrates `Button::Builder` to create
     * preconfigured button entities.
     */
    class Director : public ecs::EntityDirector {
      private:
        Builder
            _builder; ///< Builder instance for constructing the Button entity

      public:
        /**
         * @brief Construct a new Button Director object.
         * @param componentRegistry The component registry used for button
         * entity creation.
         */
        Director(ecs::ComponentRegistry &componentRegistry);

        /**
         * @brief Default destructor for the Button Director class.
         */
        ~Director(void);

        /**
         * @brief Create a default Button entity using the builder.
         * @param builder The builder instance used to configure and create the
         * default button.
         * @return Unique pointer to the created Button entity.
         */
        std::unique_ptr<ecs::Entity> makeDefaultButton(Builder &builder) {
            return builder.withToggleState(TogleState::Default)
                .withColorStyle(ColorStyle::Filled)
                .withShape(Shape::Round)
                .withSize(Size::Small)
                .withMorphState(MorphState::Default)
                .getEntity(getComponentRegistry());
        }
    };

  private:
  public:
    /**
     * @brief Default constructor for the Button entity.
     * @param registry Reference to the component registry for initializing
     * components.
     * @param toggleState Initial toggle state for the button.
     * @param colorStyle Initial color style for the button.
     * @param shape Initial shape for the button.
     * @param size Initial size for the button.
     * @param morphState Initial morph state for the button.
     */
    Button(ecs::ComponentRegistry &registry, TogleState toggleState,
           ColorStyle colorStyle, Shape shape, Size size,
           MorphState morphState);

    /**
     * @brief Default destructor for the Button entity.
     */
    ~Button(void);
};

} // namespace guillaume::entities