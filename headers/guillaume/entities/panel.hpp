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
#include <string>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/relationship.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Entity representing a generic container panel.
 * @see components::Relationship
 */
class Panel : public ecs::EntityFiller<components::Transform, components::Bound,
                                       components::Relationship> {
  public:
    /**
     * @brief Builder for constructing `Panel` entities.
     */
    class Builder {
      private:
        ecs::ComponentRegistry &_componentRegistry;
        std::unique_ptr<Panel> _panel;

        ecs::Entity::Identifier getIdentifier(void) const {
            return _panel->getIdentifier();
        }

      public:
        /**
         * @brief Construct a new panel builder.
         * @param componentRegistry Registry used to create and configure
         * components.
         */
        Builder(ecs::ComponentRegistry &componentRegistry)
            : _componentRegistry(componentRegistry) {
            reset();
        }

        /**
         * @brief Default destructor.
         */
        ~Builder(void) = default;

        /**
         * @brief Reset the builder with a fresh `Panel` instance.
         */
        void reset(void) {
            _panel = std::make_unique<Panel>(_componentRegistry);
        }

        /**
         * @brief Set panel local position.
         * @param position New local position.
         */
        void setPosition(const components::Transform::Position &position) {
            _componentRegistry
                .getComponent<components::Transform>(getIdentifier())
                .setPosition(position);
        }

        /**
         * @brief Set panel local rotation.
         * @param rotation New local rotation.
         */
        void setRotation(const components::Transform::Rotation &rotation) {
            _componentRegistry
                .getComponent<components::Transform>(getIdentifier())
                .setRotation(rotation);
        }

        /**
         * @brief Set panel local scale.
         * @param scale New local scale.
         */
        void setScale(const components::Transform::Scale &scale) {
            _componentRegistry
                .getComponent<components::Transform>(getIdentifier())
                .setScale(scale);
        }

        /**
         * @brief Set panel bound size.
         * @param size New bound size.
         */
        void setSize(const components::Bound::Size &size) {
            _componentRegistry.getComponent<components::Bound>(getIdentifier())
                .setSize(size);
        }

        /**
         * @brief Placeholder for API parity with other builders.
         * @param content Unused text payload.
         */
        void setText(const std::string &content) { (void)content; }

        /**
         * @brief Set parent entity identifier.
         * @param parentIdentifier Identifier of the parent entity.
         */
        void
        setParentIdentifier(const ecs::Entity::Identifier &parentIdentifier) {
            _componentRegistry
                .getComponent<components::Relationship>(getIdentifier())
                .setParentIdentifier(parentIdentifier);
        }

        /**
         * @brief Finalize and return the built panel.
         * @return The constructed panel entity.
         */
        std::unique_ptr<Panel> getProduct(void) {
            auto product = std::move(_panel);
            reset();
            return product;
        }
    };

    /**
     * @brief Director orchestrating `Panel::Builder` construction steps.
     */
    class Director {
      public:
        /**
         * @brief Construct a panel with provided attributes.
         * @tparam BuilderType Concrete builder type.
         * @param builder Builder instance.
         * @param position Panel position.
         * @param size Panel size.
         */
        template <typename BuilderType>
        void constructPanel(BuilderType &builder,
                            const components::Transform::Position &position,
                            const components::Bound::Size &size) const {
            builder.reset();
            builder.setPosition(position);
            builder.setRotation({0.0f, 0.0f, 0.0f});
            builder.setScale({1.0f, 1.0f, 1.0f});
            builder.setSize(size);
            builder.setParentIdentifier(ecs::Entity::InvalidIdentifier);
        }
    };

    /**
     * @brief Construct a new Panel entity filler.
     * @param componentRegistry Registry used to create and store components.
     */
    Panel(ecs::ComponentRegistry &componentRegistry)
        : ecs::EntityFiller<components::Transform, components::Bound,
                            components::Relationship>(componentRegistry) {}

    /**
     * @brief Virtual destructor for Panel.
     */
    ~Panel(void) override = default;
};

} // namespace guillaume::entities
