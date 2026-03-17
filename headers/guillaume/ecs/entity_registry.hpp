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

#include <map>
#include <vector>

#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs {

/**
 * @brief Registry that tracks scene-owned entities and their signatures.
 */
class EntityRegistry {
  private:
    std::vector<Entity::Identifier>
        _entities; ///< Entity identifiers registered in the scene
    std::map<Entity::Identifier, Entity::Signature>
        _entitySignatures; ///< Entity signatures tracked by identifier

  public:
    /**
     * @brief Default constructor.
     */
    EntityRegistry(void) = default;

    /**
     * @brief Default destructor.
     */
    ~EntityRegistry(void) = default;

    /**
     * @brief Register an entity in the registry.
     * @param entity The entity to register.
     */
    void addEntity(Entity &entity);

    /**
     * @brief Update the signature for a registered entity.
     * @param identityIdentifier The entity identifier.
     * @param signature The entity signature.
     */
    void setEntitySignature(const Entity::Identifier &identityIdentifier,
                            const Entity::Signature &signature);

    /**
     * @brief Get all registered entity identifiers.
     * @return Const reference to entity identifiers.
     */
    const std::vector<Entity::Identifier> &getEntities(void) const {
        return _entities;
    }

    /**
     * @brief Get tracked entity signatures.
     * @return Const reference to the signatures map.
     */
    const std::map<Entity::Identifier, Entity::Signature> &
    getEntitySignatures(void) const {
        return _entitySignatures;
    }
};

} // namespace guillaume::ecs
