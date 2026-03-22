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

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/leaf_entity_builder.hpp"
#include "guillaume/ecs/leaf_entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/render.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Text component
 */
class Text
    : public ecs::LeafEntityFiller<components::Transform, components::Text> {

  public:
    using Color = utility::math::Color<uint8_t>; ///< Color type (RGBA)

  public:
    /**
     * @brief Text type, which can be Normal, Condensed, Monospace, Serif, or
     * Slab.
     */
    enum class Type { Normal, Condensed, Monospace, Serif, Slab };

    /**
     * @brief Builder used to configure and create `Text` entities.
     */
    class Builder : public ecs::LeafEntityBuilder<Text> {
      public:
        using Color = utility::math::Color<uint8_t>; ///< Color type (RGBA)

      private:
        std::unique_ptr<Text>
            _text;            ///< Unique pointer to the Text entity being built
        std::string _content; ///< Text content to be set in the Text component
        std::size_t _fontSize; ///< Font size to be set in the Text component
        Color _color;          ///< Color to be set in the Text component (RGBA)

      public:
        /**
         * @brief Construct a new Text Builder object.
         * @param componentRegistry The component registry to register
         * components to.
         * @param entityRegistry The entity registry to register entities to.
         */
        Builder(ecs::ComponentRegistry &componentRegistry,
                ecs::EntityRegistry &entityRegistry);

        /**
         * @brief Default destructor for the Text Builder class.
         */
        ~Builder(void);

        /**
         * @brief Build and register the text entity.
         */
        void registerEntity(void) override;

        /**
         * @brief Reset the builder to its initial state for creating a new
         * Text entity.
         */
        void reset(void) override;

        /**
         * @brief Set the text content for the Text component.
         * @param content The text content to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withContent(const std::string &content);

        /**
         * @brief Set the font size for the Text component.
         * @param fontSize The font size to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withFontSize(const std::size_t &fontSize);
        /**
         * @brief Set the color for the Text component.
         * @param color The color to set (RGBA).
         * @return Reference to the builder for chaining.
         */
        Builder &withColor(const Color &color);
    };

    /**
     * @brief Director that orchestrates `Text::Builder` to create
     * preconfigured text entities.
     */
    class Director : public ecs::EntityDirector {
      public:
        /**
         * @brief Construct a new Text Director object.
         */
        Director(void);

        /**
         * @brief Default destructor for the Text Director class.
         */
        ~Director(void);

        /**
         * @brief Create a default Text entity using the builder.
         * @param builder The builder instance used to configure and create the
         * default text
         * @param content The text content for the default Text entity.
         * @param fontSize The font size for the default Text entity.
         * @param color The color for the default Text entity (RGBA).
         */
        void makeDefaultText(Builder &builder, const std::string &content,
                             const std::size_t &fontSize, const Color &color);
    };

  private:
    std::string _content; ///< Text content to be set in the Text component
    std::size_t _fontSize = 24; ///< Font size to be set in the Text component
    Color _color = {255, 255, 255,
                    255}; ///< Color to be set in the Text component

  public:
    /**
     * @brief Default constructor for the Text component.
     * @param registry Reference to the component registry for initializing
     * components.
     * @param content The text content to initialize the Text component with.
     * @param fontSize The font size to initialize the Text component with.
     * @param color The color to initialize the Text component with (RGBA).F
     */
    Text(ecs::ComponentRegistry &registry, const std::string &content,
         const std::size_t &fontSize, const Color &color);

    /**
     * @brief Default destructor for the Text component.
     */
    ~Text(void);
};

} // namespace guillaume::entities
