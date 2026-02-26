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

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/relationship.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

class Input : public ecs::EntityFiller<components::Transform, components::Bound,
                                       components::Text, components::Focus,
                                       components::Relationship> {
  public:
    class Builder {
      private:
        ecs::ComponentRegistry &_componentRegistry;
        std::unique_ptr<Input> _input;

        ecs::Entity::Identifier getIdentifier(void) const {
            return _input->getIdentifier();
        }

      public:
        Builder(ecs::ComponentRegistry &componentRegistry)
            : _componentRegistry(componentRegistry) {
            reset();
        }

        ~Builder(void) = default;

        void reset(void) {
            _input = std::make_unique<Input>(_componentRegistry);
        }

        void setPosition(const components::Transform::Position &position) {
            _componentRegistry
                .getComponent<components::Transform>(getIdentifier())
                .setPosition(position);
        }

        void setRotation(const components::Transform::Rotation &rotation) {
            _componentRegistry
                .getComponent<components::Transform>(getIdentifier())
                .setRotation(rotation);
        }

        void setScale(const components::Transform::Scale &scale) {
            _componentRegistry
                .getComponent<components::Transform>(getIdentifier())
                .setScale(scale);
        }

        void setSize(const components::Bound::Size &size) {
            _componentRegistry.getComponent<components::Bound>(getIdentifier())
                .setSize(size);
        }

        void setText(const std::string &content) {
            _componentRegistry.getComponent<components::Text>(getIdentifier())
                .setContent(content);
        }

        void
        setParentIdentifier(const ecs::Entity::Identifier &parentIdentifier) {
            _componentRegistry
                .getComponent<components::Relationship>(getIdentifier())
                .setParentIdentifier(parentIdentifier);
        }

        std::unique_ptr<Input> getProduct(void) {
            auto product = std::move(_input);
            reset();
            return product;
        }
    };

    class Director {
      public:
        template <typename BuilderType>
        void
        constructInput(BuilderType &builder,
                       const components::Transform::Position &position,
                       const components::Bound::Size &size,
                       const std::string &content,
                       const ecs::Entity::Identifier &parentIdentifier) const {
            builder.reset();
            builder.setPosition(position);
            builder.setRotation({0.0f, 0.0f, 0.0f});
            builder.setScale({1.0f, 1.0f, 1.0f});
            builder.setSize(size);
            builder.setText(content);
            builder.setParentIdentifier(parentIdentifier);
        }
    };

    /**
     * @brief Construct a new Input entity filler.
     */
    Input(ecs::ComponentRegistry &componentRegistry)
        : ecs::EntityFiller<components::Transform, components::Bound,
                            components::Text, components::Focus,
                            components::Relationship>(componentRegistry) {}

    /**
     * @brief Virtual destructor for Input.
     */
    ~Input(void) override = default;
};

} // namespace guillaume::entities
