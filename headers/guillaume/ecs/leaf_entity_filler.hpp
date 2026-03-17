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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/ecs/components/parent.hpp"

namespace guillaume::ecs {

/**
 * @brief A leaf entity that have a parent but no children.
 * @tparam ComponentTypes The types of components this entity will have.
 */
template <InheritFromComponent... ComponentTypes>
class LeafEntityFiller
    : public EntityFiller<ComponentTypes..., components::Parent> {
  public:
    /**
     * @brief Construct a new LeafEntityFiller object.
     * @param registry The component registry to register components to.
     */
    LeafEntityFiller(ecs::ComponentRegistry &registry)
        : EntityFiller<ComponentTypes..., components::Parent>(registry) {}

    /**
     * @brief Default destructor for the LeafEntityFiller class.
     */
    virtual ~LeafEntityFiller(void) = default;

    /**
     * @brief Get the parent entity identifier for this leaf entity.
     * @return The identifier of the parent entity.
     */
    Entity::Identifier getParentIdentifier(void) const {
        return this->getComponentRegistry()
            .template getComponent<components::Parent>(this->getIdentifier())
            .getParentIdentifier();
    };

    /**
     * @brief Set the parent entity identifier for this leaf entity.
     * @param parentIdentifier The identifier of the parent entity.
     */
    void setParentIdentifier(const Entity::Identifier &parentIdentifier) {
        this->getComponentRegistry()
            .template getComponent<components::Parent>(this->getIdentifier())
            .setParentIdentifier(parentIdentifier);
    };
};

} // namespace guillaume::ecs