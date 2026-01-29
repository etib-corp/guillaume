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

#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a relationship between entities.
 */
class Relationship : public ecs::Component {
  private:
    ecs::Entity::Identifier parentIdentifier{
        ecs::Entity::InvalidIdentifier}; ///< Parent entity identifier
    std::vector<ecs::Entity::Identifier>
        childrenIdentifiers; ///< Children entity identifiers

  public:
    /**
     * @brief Default constructor for the Relationship component.
     */
    Relationship(void) = default;

    /**
     * @brief Default destructor for the Relationship component.
     */
    ~Relationship(void) = default;

    /**
     * @brief Get the parent entity identifier.
     * @param identifier The new parent entity identifier.
     */
    void setParentIdentifier(ecs::Entity::Identifier identifier) {
        parentIdentifier = identifier;
    }

    /**
     * @brief Get the parent entity identifier.
     * @return The parent entity identifier.
     */
    ecs::Entity::Identifier getParentIdentifier(void) const {
        return parentIdentifier;
    }

    /**
     * @brief Add a child entity identifier.
     * @param identifier The child entity identifier to add.
     */
    void addChildIdentifier(ecs::Entity::Identifier identifier) {
        childrenIdentifiers.push_back(identifier);
    }

    /**
     * @brief Get the children entity identifiers.
     * @return The children entity identifiers.
     */
    const std::vector<ecs::Entity::Identifier> &
    getChildrenIdentifiers(void) const {
        return childrenIdentifiers;
    }
};

} // namespace guillaume::components