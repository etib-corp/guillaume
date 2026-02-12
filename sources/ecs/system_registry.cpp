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

#include "guillaume/ecs/system_registry.hpp"

namespace guillaume::ecs {

void SystemRegistry::addEntityToSystems(Entity &entity) {
    const auto entitySignature = entity.getSignature();
    const auto entityIdentifier = entity.getIdentifier();

    onEntitySignatureChanged(entityIdentifier, entitySignature);
}

void SystemRegistry::onEntitySignatureChanged(
    const Entity::Identifier &identityIdentifier,
    const Entity::Signature &signature) {
    getLogger().debug("Updating entity " + std::to_string(identityIdentifier) +
                      " system membership");

    for (const auto &[systemType, system] : _systems) {
        const auto systemSignature = system->getSignature();
        const bool matches = (signature & systemSignature) == systemSignature;
        if (matches) {
            system->addEntity(identityIdentifier);
            getLogger().debug("Entity " + std::to_string(identityIdentifier) +
                              " added to system " +
                              std::string(systemType.name()));
        } else if (system->hasEntity(identityIdentifier)) {
            system->removeEntity(identityIdentifier);
            getLogger().debug("Entity " + std::to_string(identityIdentifier) +
                              " removed from system " +
                              std::string(systemType.name()));
        }
    }
}

} // namespace guillaume::ecs