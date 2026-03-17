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

namespace guillaume::ecs::tests {

class DummyEntity : public Entity {};

TEST_F(TestEntityRegistry, AddEntityStoresIdentifierAndSignature) {
    EntityRegistry registry;
    DummyEntity entity;

    Entity::Signature signature;
    signature.set(3);
    entity.setSignature(signature);

    registry.addEntity(entity);

    ASSERT_EQ(registry.getEntities().size(), 1U);
    EXPECT_EQ(registry.getEntities().front(), entity.getIdentifier());

    const auto &entitySignatures = registry.getEntitySignatures();
    ASSERT_TRUE(entitySignatures.contains(entity.getIdentifier()));
    EXPECT_EQ(entitySignatures.at(entity.getIdentifier()), signature);
}

TEST_F(TestEntityRegistry, SetEntitySignatureUpdatesTrackedSignature) {
    EntityRegistry registry;
    DummyEntity entity;

    registry.addEntity(entity);

    Entity::Signature updatedSignature;
    updatedSignature.set(7);
    updatedSignature.set(9);

    registry.setEntitySignature(entity.getIdentifier(), updatedSignature);

    const auto &entitySignatures = registry.getEntitySignatures();
    ASSERT_TRUE(entitySignatures.contains(entity.getIdentifier()));
    EXPECT_EQ(entitySignatures.at(entity.getIdentifier()), updatedSignature);
}

} // namespace guillaume::ecs::tests
