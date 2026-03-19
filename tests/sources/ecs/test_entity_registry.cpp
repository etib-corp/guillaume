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

#include "ecs/test_entity_registry.hpp"

#include <memory>

namespace guillaume::ecs::tests {

class DummyEntity : public Entity {};

TEST_F(TestEntityRegistry, AddEntityStoresIdentifierAndSignature) {
    EntityRegistry registry;
    auto entity = std::make_unique<DummyEntity>();

    Entity::Signature signature;
    signature.set(3);
    entity->setSignature(signature);

    const auto identifier = entity->getIdentifier();

    registry.addEntity(std::move(entity));

    EXPECT_EQ(entity, nullptr);

    ASSERT_EQ(registry.getEntities().size(), 1U);
    EXPECT_EQ(registry.getEntities().front()->getIdentifier(), identifier);
    EXPECT_EQ(registry.getEntities().front()->getSignature(), signature);

    ASSERT_EQ(registry.getEntities().size(), 1U);
    EXPECT_EQ(registry.getEntities().front()->getIdentifier(), identifier);
}

TEST_F(TestEntityRegistry, AddEntityTracksAllEntityIdentifiersInOrder) {
    EntityRegistry registry;
    auto firstEntity = std::make_unique<DummyEntity>();
    auto secondEntity = std::make_unique<DummyEntity>();

    const auto firstIdentifier = firstEntity->getIdentifier();
    const auto secondIdentifier = secondEntity->getIdentifier();

    registry.addEntity(std::move(firstEntity));
    registry.addEntity(std::move(secondEntity));

    const auto &entities = registry.getEntities();
    ASSERT_EQ(entities.size(), 2U);
    EXPECT_EQ(entities[0]->getIdentifier(), firstIdentifier);
    EXPECT_EQ(entities[1]->getIdentifier(), secondIdentifier);
}

} // namespace guillaume::ecs::tests
