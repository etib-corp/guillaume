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
#include "guillaume/ecs/leaf_entity_builder.hpp"
#include "guillaume/ecs/leaf_entity_filler.hpp"

#include "guillaume/components/render.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities {

/**
 * @brief Text component for the button.
 */
class Text
    : public ecs::LeafEntityFiller<components::Transform, components::Text,
                                   components::Render> {

    class Builder : public ecs::LeafEntityBuilder {
      private:
        std::unique_ptr<Text>
            _text; ///< Unique pointer to the Text entity being built

      public:
        /**
         * @brief Construct a new Text Builder object.
         * @param componentRegistry The component registry to register
         * components to.
         */
        Builder(void);

        /**
         * @brief Default destructor for the Text Builder class.
         */
        ~Builder(void);

        /**
         * @brief Get the entity being built.
         * @return Unique pointer to the Text entity being built.
         */
        std::unique_ptr<ecs::Entity>
        getEntity(ecs::ComponentRegistry &componentRegistry) override {
            _text = std::make_unique<Text>(componentRegistry);
            return std::move(_text);
        }

        /**
         * @brief Reset the builder to its initial state for creating a new
         * Text entity.
         */
        void reset(void) override { _text.reset(); }
    };

  public:
    /**
     * @brief Default constructor for the Text component.
     * @param registry Reference to the component registry for initializing
     */
    Text(ecs::ComponentRegistry &registry);

    /**
     * @brief Default destructor for the Text component.
     */
    ~Text(void);
};

} // namespace guillaume::entities
