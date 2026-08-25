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

#include "entities/test_button.hpp"

#include <guillaume/components/transform.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestButton, ChildrenHaveDistinctLayersAndDepthOffsets)
	{
		ComponentRegistry registry;
		Button button(registry, "home", "Save", false, Button::Color::Filled,
					  Button::Shape::Round, Button::Size::Medium, false, {});

		auto &buttonTransform = registry.getComponent<components::Transform>(
			button.getIdentifier());
		buttonTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
			utility::graphic::OrientationF()));

		button.update();

		const auto iconIdentifier  = button.getIconIdentifier();
		const auto labelIdentifier = button.getLabelIdentifier();

		ASSERT_NE(iconIdentifier, ecs::Entity::InvalidIdentifier);
		ASSERT_NE(labelIdentifier, ecs::Entity::InvalidIdentifier);

		const auto iconEntity  = button.getEntity(iconIdentifier);
		const auto labelEntity = button.getEntity(labelIdentifier);
		ASSERT_NE(iconEntity, nullptr);
		ASSERT_NE(labelEntity, nullptr);

		EXPECT_EQ(iconEntity->getLayer(), 1);
		EXPECT_EQ(labelEntity->getLayer(), 2);

		const float baseZ = buttonTransform.getPose().getPosition().getZ();
		const auto iconZ =
			registry.getComponent<components::Transform>(iconIdentifier)
				.getPose()
				.getPosition()
				.getZ();
		const auto textZ =
			registry.getComponent<components::Transform>(labelIdentifier)
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(iconZ, baseZ + 1.0f);
		EXPECT_FLOAT_EQ(textZ, baseZ + 2.0f);
		EXPECT_LT(baseZ, iconZ);
		EXPECT_LT(iconZ, textZ);
	}

	TEST_F(TestButton, TextOnlyChildUsesLayerOffset)
	{
		ComponentRegistry registry;
		Button button(registry, "", "Save", false, Button::Color::Filled,
					  Button::Shape::Round, Button::Size::Medium, false, {});

		auto &buttonTransform = registry.getComponent<components::Transform>(
			button.getIdentifier());
		buttonTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
			utility::graphic::OrientationF()));

		button.update();

		const auto iconIdentifier  = button.getIconIdentifier();
		const auto labelIdentifier = button.getLabelIdentifier();

		EXPECT_EQ(iconIdentifier, ecs::Entity::InvalidIdentifier);
		ASSERT_NE(labelIdentifier, ecs::Entity::InvalidIdentifier);

		const auto labelEntity = button.getEntity(labelIdentifier);
		ASSERT_NE(labelEntity, nullptr);
		EXPECT_EQ(labelEntity->getLayer(), 2);

		const float baseZ = buttonTransform.getPose().getPosition().getZ();
		const auto textZ =
			registry.getComponent<components::Transform>(labelIdentifier)
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(textZ, baseZ + 2.0f);
	}

	TEST_F(TestButton, IconOnlyChildUsesLayerOffset)
	{
		ComponentRegistry registry;
		Button button(registry, "home", "", false, Button::Color::Filled,
					  Button::Shape::Round, Button::Size::Medium, false, {});

		auto &buttonTransform = registry.getComponent<components::Transform>(
			button.getIdentifier());
		buttonTransform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(0.0f, 0.0f, -300.0f),
			utility::graphic::OrientationF()));

		button.update();

		const auto iconIdentifier  = button.getIconIdentifier();
		const auto labelIdentifier = button.getLabelIdentifier();

		ASSERT_NE(iconIdentifier, ecs::Entity::InvalidIdentifier);
		EXPECT_EQ(labelIdentifier, ecs::Entity::InvalidIdentifier);

		const auto iconEntity = button.getEntity(iconIdentifier);
		ASSERT_NE(iconEntity, nullptr);
		EXPECT_EQ(iconEntity->getLayer(), 1);

		const float baseZ = buttonTransform.getPose().getPosition().getZ();
		const auto iconZ =
			registry.getComponent<components::Transform>(iconIdentifier)
				.getPose()
				.getPosition()
				.getZ();

		EXPECT_FLOAT_EQ(iconZ, baseZ + 1.0f);
	}
}	 // namespace guillaume::entities::tests
