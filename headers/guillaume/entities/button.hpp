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

#include "guillaume/components/bound.hpp"
#include "guillaume/components/click.hpp"
#include "guillaume/components/hover.hpp"
#include "guillaume/components/render.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Button entity class representing a UI button with various components.
 */
class Button
    : public ecs::NodeEntityFiller<components::Transform, components::Bound,
                                   components::Hover, components::Click,
                                   components::Render> {
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
     * @brief Drawable of the button.
     */
    enum class Drawable { Round, Square };

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
            _button;        ///< Unique pointer to the Button entity being built
        std::string _label; ///< Label text for the button
        std::function<void(void)>
            _onClick;            ///< Click event handler for the button
        TogleState _toggleState; ///< Current toggle state
        ColorStyle _colorStyle;  ///< Button color style
        Drawable _shape;         ///< Button shape
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
        getEntity(ecs::ComponentRegistry &componentRegistry) override;

        /**
         * @brief Reset the builder to its initial state for creating a new
         * Button entity.
         */
        void reset(void) override;

        /**
         * @brief Set the label text for the button.
         * @param label The new label text to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withLabel(const std::string &label);

        /**
         * @brief Set the click event handler for the button.
         * @param onClick The new click event handler to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withOnClick(std::function<void(void)> onClick);

        /**
         * @brief Set the toggle state of the button.
         * @param toggleState The new toggle state to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withToggleState(TogleState toggleState);

        /**
         * @brief Set the color style of the button.
         * @param colorStyle The new color style to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withColorStyle(ColorStyle colorStyle);

        /**
         * @brief Set the shape of the button.
         * @param shape The new shape to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withShape(Drawable shape);

        /**
         * @brief Set the size of the button.
         * @param size The new size to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withSize(Size size);

        /**
         * @brief Set the morph state of the button.
         * @param morphState The new morph state to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withMorphState(MorphState morphState);
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
         * @param label The label text for the default button.
         * @param onClick The click event handler for the default button.
         * @return Unique pointer to the created Button entity.
         */
        std::unique_ptr<ecs::Entity>
        makeDefaultButton(Builder &builder, const std::string &label,
                          std::function<void(void)> onClick);
    };

  private:
    TogleState _toggleState;            ///< Current toggle state of the button
    ColorStyle _colorStyle;             ///< Color style of the button
    Drawable _shape;                    ///< Drawable of the button
    Size _size;                         ///< Size of the button
    MorphState _morphState;             ///< Current morph state of the button
    std::string _label;                 ///< Label text for the button
    std::function<void(void)> _onClick; ///< Click event handler for the button

  private:
    /**
     * @brief Hover event handler for the button.
     */
    void hoverHandler(void);

    /**
     * @brief Unhover event handler for the button.
     */
    void unHoverHandler(void);

    /**
     * @brief Left click event handler for the button.
     * @param mousePosition The position of the mouse when the left click event
     * occurred.
     */
    void leftClickHandler(
        utility::event::MouseMotionEvent::MousePosition mousePosition);

    /**
     * @brief Render handler for the normal state of the button.
     * @param renderer The renderer to use for drawing the button.
     */
    void normalRender(ecs::ComponentRegistry &registry,
                      const ecs::Entity::Identifier &id, Renderer &renderer);

    /**
     * @brief Render handler for the hovered state of the button.
     * @param renderer The renderer to use for drawing the button in hovered
     * state.
     */
    void hoveredRender(ecs::ComponentRegistry &registry,
                       const ecs::Entity::Identifier &id, Renderer &renderer);

    /**
     * @brief Render handler for the clicked state of the button.
     * @param renderer The renderer to use for drawing the button in clicked
     * state.
     */
    void clickedRender(ecs::ComponentRegistry &registry,
                       const ecs::Entity::Identifier &id, Renderer &renderer);

    /**
     * @brief Render handler for the active state of the button.
     * @param renderer The renderer to use for drawing the button in active
     * state.
     */
    void activeRender(ecs::ComponentRegistry &registry,
                      const ecs::Entity::Identifier &id, Renderer &renderer);

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
     * @param label Label text for the button.
     * @param onClick Click event handler for the button.
     */
    Button(ecs::ComponentRegistry &registry, TogleState toggleState,
           ColorStyle colorStyle, Drawable shape, Size size,
           MorphState morphState, std::string label,
           std::function<void(void)> onClick);

    /**
     * @brief Default destructor for the Button entity.
     */
    ~Button(void);
};

} // namespace guillaume::entities