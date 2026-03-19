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

#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs {

void EntityRegistry::addEntity(std::unique_ptr<Entity> entity) {
    _entities.push_back(std::move(entity));
}

std::vector<Entity::Identifier> EntityRegistry::getEntityWithSignature(
    Entity::Signature systemSignature) const {
    std::vector<Entity::Identifier> matchingIdentifiers;
    for (const auto &entity : _entities) {
        if ((entity->getSignature() & systemSignature) == systemSignature) {
            matchingIdentifiers.push_back(entity->getIdentifier());
        }
    }
    return matchingIdentifiers;
}

} // namespace guillaume::ecs
