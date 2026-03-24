/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/node_entity_builder.hpp"
#include "guillaume/ecs/node_entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Panel entity class representing a UI panel with transform and bound components.
 */
class Panel : public ecs::NodeEntityFiller<components::Transform,
                                           components::Bound>,
              public utility::logging::Loggable<Panel,
                                        utility::logging::StandardLogger> {

    /**
     * @brief Builder class for constructing and registering Panel entities.
     */
    class Builder : public ecs::NodeEntityBuilder<Panel> {
        private:
            std::unique_ptr<Panel>
                _panel; ///< Unique pointer to the Panel entity being built
            std::string _name; ///< Name of the panel entity

        public:
            /**
             * @brief Construct a new Panel Builder object.
             * @param componentRegistry The component registry used to build entities.
             * @param entityRegistry The entity registry used to build entities.
             */
            Builder(ecs::ComponentRegistry &componentRegistry,
                ecs::EntityRegistry &entityRegistry);

            /**
             * @brief Default destructor for the Panel Builder class.
             */
            ~Builder(void);

            /**
             * @brief Build and register the panel entity.
             */
            void registerEntity(void) override;

            /**
             * @brief Reset the builder to its initial state for creating a new Panel entity.
             */
            void reset(void) override;

            /**
             * @brief Set the name for the panel entity.
             * @param name The name to set for the panel.
             * @return Reference to the builder for chaining.
             */
            Builder &withName(const std::string &name);
    };

    /**
     * @brief Director class for orchestrating the construction of Panel entities using the Builder.
     */
    class Director : public ecs::EntityDirector {
        public:
            /**
             * @brief Construct a new Panel Director object.
             */
            Director(void);

            /**
             * @brief Default destructor for the Panel Director class.
             */
            ~Director(void);

            /**
             * @brief Create a panel entity using the builder.
             * @param builder The builder instance used to configure and create the panel.
             * @param name The name of the panel entity.
             */
            void makePanel(Builder &builder, const std::string &name);
    };

    private:
        ecs::ComponentRegistry
            &_registry; ///< Reference to the component registry for initializing components
        ecs::EntityRegistry
            &_entityRegistry; ///< Reference to the entity registry for registering child entities
        std::string _name; ///< Name of the panel entity

    public:
    /**
     * @brief Default constructor for the Panel entity.
     * @param registry Reference to the component registry for initializing components.
     * @param entityRegistry Reference to the entity registry for registering child entities.
     * @param name The name of the panel entity.
     */
    Panel(ecs::ComponentRegistry &registry, ecs::EntityRegistry &entityRegistry, const std::string &name);

    /**
     * @brief Default destructor for the Panel entity.
     */
    ~Panel(void);

    /**
     * @brief Get the component registry for this panel.
     * @return Reference to the component registry.
     */
    ecs::ComponentRegistry &getComponentRegistry(void) { return _registry; }

    /**
     * @brief Get the entity registry for this panel.
     * @return Reference to the entity registry.
     */
    ecs::EntityRegistry &getEntityRegistry(void) { return _entityRegistry; }
};

/**
 * @brief Concept to ensure a type inherits from Panel.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromPanel = std::is_base_of_v<Panel, Type>;
} // namespace guillaume::entities

