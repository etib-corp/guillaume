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

#include "guillaume/components/icon.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Icon component
 */
class Icon
    : public ecs::LeafEntityFiller<components::Transform, components::Icon> {
  public:
    /**
     * @brief Style of the icon, which can be Outlined, Rounded, or Sharp.
     */
    enum class Style { Outlined, Rounded, Sharp };

    /**
     * @brief Builder used to configure and create `Icon` entities.
     */
    class Builder : public ecs::LeafEntityBuilder<Icon> {
      private:
        std::unique_ptr<Icon>
            _icon; ///< Unique pointer to the Icon entity being built
        std::string _iconName; ///< Name of the icon to be used
        Style _style;          ///< Style of the icon

      public:
        /**
         * @brief Construct a new Icon Builder object.
         * @param componentRegistry The component registry to register
         * components to.
         * @param entityRegistry The entity registry to register entities to.
         */
        Builder(ecs::ComponentRegistry &componentRegistry,
                ecs::EntityRegistry &entityRegistry);

        /**
         * @brief Default destructor for the Icon Builder class.
         */
        ~Builder(void);

        /**
         * @brief Build and register the icon entity.
         */
        void registerEntity(void) override;

        /**
         * @brief Reset the builder to its initial state for creating a new
         * Icon entity.
         */
        void reset(void) override;

        /**
         * @brief Set the name of the icon to be used for the Icon entity.
         * @param iconName The name of the icon to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withIconName(const std::string &iconName);

        /**
         * @brief Set the style of the icon to be used for the Icon entity.
         * @param style The style of the icon to set.
         * @return Reference to the builder for chaining.
         */
        Builder &withStyle(const Style &style);
    };

    /**
     * @brief Director that orchestrates `Icon::Builder` to create
     * preconfigured icon entities.
     */
    class Director : public ecs::EntityDirector {
      public:
        /**
         * @brief Construct a new Icon Director object.
         */
        Director(void);

        /**
         * @brief Default destructor for the Icon Director class.
         */
        ~Director(void);

        /**
         * @brief Create a default Icon entity using the builder.
         * @param builder The builder instance used to configure and create the
         * default icon
         * @param iconName The icon name to assign to the created entity.
         */
        void makeDefaultIcon(Builder &builder, const std::string &iconName);
    };

  private:
    std::string _iconName; ///< Name of the icon to be used for this Icon entity
    Style _style; ///< Style of the icon to be used for this Icon entity

  public:
    /**
     * @brief Default constructor for the Icon component.
     * @param registry Reference to the component registry for initializing
     * components.
     * @param iconName The name of the icon to be used for this Icon entity.
     * @param style The style of the icon to be used for this Icon entity.
     */
    Icon(ecs::ComponentRegistry &registry, const std::string &iconName,
         const Style &style);

    /**
     * @brief Default destructor for the Icon component.
     */
    ~Icon(void);
};

} // namespace guillaume::entities
