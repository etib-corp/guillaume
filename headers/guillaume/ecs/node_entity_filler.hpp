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
#include "guillaume/ecs/leaf_entity_filler.hpp"

#include "guillaume/ecs/components/children.hpp"

namespace guillaume::ecs {

/**
 * @brief A node entity that have a parent and children.
 * @tparam ComponentTypes The types of components this entity will have.
 */
template <InheritFromComponent... ComponentTypes>
class NodeEntityFiller
    : public LeafEntityFiller<ComponentTypes..., components::Children> {
  public:
    /**
     * @brief Construct a new NodeEntityFiller object.
     * @param registry The component registry to register components to.
     */
    NodeEntityFiller(ecs::ComponentRegistry &registry)
        : LeafEntityFiller<ComponentTypes..., components::Children>(registry) {}

    /**
     * @brief Default destructor for the NodeEntityFiller class.
     */
    virtual ~NodeEntityFiller(void) = default;

    /**
     * @brief Get the child entity identifiers for this node entity.
     * @return The identifiers of the child entities.
     */
    std::vector<Entity::Identifier> getChildIdentifiers(void) const {
        return this->getComponentRegistry()
            .template getComponent<components::Children>(this->getIdentifier())
            .getChildIdentifiers();
    };

    /**
     * @brief Add a child entity identifier to this node entity.
     * @param childIdentifier The identifier of the child entity to add.
     */
    void addChildIdentifier(const Entity::Identifier &childIdentifier) {
        this->getComponentRegistry()
            .template getComponent<components::Children>(this->getIdentifier())
            .addChildIdentifier(childIdentifier);
    };

    /**
     * @brief Set the child entity identifiers for this node entity.
     * @param childIdentifiers The new child entity identifiers.
     */
    void setChildIdentifiers(
        const std::vector<Entity::Identifier> &childIdentifiers) {
        this->getComponentRegistry()
            .template getComponent<components::Children>(this->getIdentifier())
            .setChildIdentifiers(childIdentifiers);
    };
};

} // namespace guillaume::ecs