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
#include "guillaume/components/click.hpp"
#include "guillaume/components/hover.hpp"
#include "guillaume/components/relationship.hpp"
#include "guillaume/components/render.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

class Button
    : public ecs::EntityFiller<components::Bound, components::Click, components::Hover, components::Relationship,
                               components::Render, components::Text, components::Transform> {
  public:
    class Builder {
      private:
        ecs::ComponentRegistry &_componentRegistry;
        std::unique_ptr<Button> _button;

        ecs::Entity::Identifier getIdentifier(void) const {
            return _button->getIdentifier();
        }

      public:
        Builder(ecs::ComponentRegistry &componentRegistry)
            : _componentRegistry(componentRegistry) {
            reset();
        }

        ~Builder(void) = default;

        void reset(void) {
            _button = std::make_unique<Button>(_componentRegistry);
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

        std::unique_ptr<Button> getProduct(void) {
            auto product = std::move(_button);
            reset();
            return product;
        }
    };

    class Director {
      public:
        template <typename BuilderType>
        void
        constructButton(BuilderType &builder,
                        const components::Transform::Position &position,
                        const components::Transform::Rotation &rotation,
                        const components::Bound::Size &size,
                        const std::string &content,
                        const ecs::Entity::Identifier &parentIdentifier) const {
            builder.reset();
            builder.setPosition(position);
            builder.setRotation(rotation);
            builder.setScale({1.0f, 1.0f, 1.0f});
            builder.setSize(size);
            builder.setText(content);
            builder.setParentIdentifier(parentIdentifier);
        }
    };

    /**
     * @brief Construct a new Button entity filler.
     */
    Button(ecs::ComponentRegistry &componentRegistry)
        : ecs::EntityFiller<components::Bound, components::Click, components::Hover, components::Relationship,
                               components::Render, components::Text, components::Transform>(
              componentRegistry) {}

    /**
     * @brief Virtual destructor for Button.
     */
    ~Button(void) override = default;
};

} // namespace guillaume::entities