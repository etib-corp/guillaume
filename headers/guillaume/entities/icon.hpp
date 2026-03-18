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
#include "guillaume/components/render.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Icon component
 */
class Icon
    : public ecs::LeafEntityFiller<components::Transform, components::Icon,
                                   components::Render> {
  public:
    /**
     * @brief Builder used to configure and create `Icon` entities.
     */
    class Builder : public ecs::LeafEntityBuilder {
      private:
        std::unique_ptr<Icon>
            _icon; ///< Unique pointer to the Icon entity being built

      public:
        /**
         * @brief Construct a new Icon Builder object.
         * components to.
         */
        Builder(void);

        /**
         * @brief Default destructor for the Icon Builder class.
         */
        ~Builder(void);

        /**
         * @brief Get the entity being built.
         * @param componentRegistry The component registry used to create and
         * @return Unique pointer to the Icon entity being built.
         */
        std::unique_ptr<ecs::Entity>
        getEntity(ecs::ComponentRegistry &componentRegistry) override {
            _icon = std::make_unique<Icon>(componentRegistry);
            return std::move(_icon);
        }

        /**
         * @brief Reset the builder to its initial state for creating a new
         * Icon entity.
         */
        void reset(void) override { _icon.reset(); }
    };

    /**
     * @brief Director that orchestrates `Icon::Builder` to create
     * preconfigured icon entities.
     */
    class Director : public ecs::EntityDirector {
      private:
        Builder _builder; ///< Builder instance for constructing the Icon entity

      public:
        /**
         * @brief Construct a new Icon Director object.
         * @param componentRegistry The component registry used for icon
         * entity creation.
         */
        Director(ecs::ComponentRegistry &componentRegistry);

        /**
         * @brief Default destructor for the Icon Director class.
         */
        ~Director(void);

        /**
         * @brief Create a default Icon entity using the builder.
         * @param builder The builder instance used to configure and create the
         * default icon
         * @return Unique pointer to the created Icon entity.
         */
        std::unique_ptr<ecs::Entity> makeDefaultIcon(Builder &builder) {
            return builder.getEntity(getComponentRegistry());
        }
    };

  public:
    /**
     * @brief Default constructor for the Icon component.
     * @param registry Reference to the component registry for initializing
     * components.
     */
    Icon(ecs::ComponentRegistry &registry);

    /**
     * @brief Default destructor for the Icon component.
     */
    ~Icon(void);
};

} // namespace guillaume::entities
