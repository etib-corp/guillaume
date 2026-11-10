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

#include "systems/test_interaction.hpp"

#include <guillaume/systems/hit_test.hpp>
#include <guillaume/systems/style_state.hpp>

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/entities/interaction_helpers.hpp>

namespace guillaume::systems::tests
{
	TEST_F(TestInteraction, HitTestHonoursAccessibilityMargin)
	{
		components::Bound bound;
		bound.setWidth(32.0f).setHeight(32.0f);
		const utility::graphic::PoseF pose;

		// A ray 4 dp left of the control, vertically centred.
		const utility::graphic::RayF ray(
			utility::math::Vector<float, 3>({ -4.0f, 16.0f, -10.0f }),
			utility::math::Vector<float, 3>({ 0.0f, 0.0f, 1.0f }));

		EXPECT_FALSE(
			hitTest(ray, pose, bound, utility::math::Vector2F({ 0.0f, 0.0f })));
		EXPECT_TRUE(
			hitTest(ray, pose, bound, utility::math::Vector2F({ 8.0f, 8.0f })));
	}

	TEST_F(TestInteraction, HitMarginReadsHoverComponent)
	{
		ecs::ComponentRegistry registry;
		ecs::Entity entity;
		registry.registerComponentsForEntity<components::MouseHoverInteraction>(
			entity.getIdentifier());
		registry
			.getComponent<components::MouseHoverInteraction>(
				entity.getIdentifier())
			.setAccessibilityMargin(utility::graphic::SizeUI(8, 4));

		const auto margin = hitMargin(registry, entity.getIdentifier());
		EXPECT_FLOAT_EQ(margin[0], 8.0f);
		EXPECT_FLOAT_EQ(margin[1], 4.0f);
	}

	TEST_F(TestInteraction, DisabledEntityIsNotInteractive)
	{
		ecs::ComponentRegistry registry;
		ecs::Entity entity;
		const auto id = entity.getIdentifier();
		registry.registerComponentsForEntity<components::Bound>(id);
		EXPECT_TRUE(isInteractive(registry, id));

		registry.addComponent<components::StyleState>(id);
		EXPECT_TRUE(isInteractive(registry, id));
		registry.getComponent<components::StyleState>(id).setEnabled(false);
		EXPECT_FALSE(isInteractive(registry, id));
	}

	TEST_F(TestInteraction, MakeInteractiveGatesPressOnEnabledState)
	{
		ecs::ComponentRegistry registry;
		ecs::Entity entity;
		const auto id = entity.getIdentifier();
		registry.registerComponentsForEntity<components::Bound>(id);

		int presses = 0;
		entities::makeInteractive(registry, id,
								  components::AccessibilityRole::ListItem,
								  [&presses]() {
									  ++presses;
								  });
		const auto release =
			registry.getComponent<components::MouseButtonInteraction>(id)
				.getOnButtonReleaseHandler(
					utility::event::MouseButtonEvent::Button::Left);

		release();
		EXPECT_EQ(presses, 1);

		entities::setEnabled(registry, id, false);
		release();
		EXPECT_EQ(presses, 1);
		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(id).getRole(),
			components::AccessibilityRole::ListItem);
	}

	TEST_F(TestInteraction, StateLayerColorFollowsInteraction)
	{
		ecs::ComponentRegistry registry;
		ecs::Entity entity;
		const auto id = entity.getIdentifier();
		registry.registerComponentsForEntity<components::Bound>(id);
		entities::makeInteractive(registry, id,
								  components::AccessibilityRole::Button, {});

		const utility::graphic::Color32Bit base(255, 255, 255, 255);
		const utility::graphic::Color32Bit content(0, 0, 0, 255);
		EXPECT_EQ(entities::stateLayerColor(registry, id, base, content), base);

		registry.getComponent<components::MouseHoverInteraction>(id).setHovered(
			true);
		const auto hovered =
			entities::stateLayerColor(registry, id, base, content);
		EXPECT_LT(hovered.getRed(), 255);

		registry.getComponent<components::MouseButtonInteraction>(id)
			.setButtonPressed(utility::event::MouseButtonEvent::Button::Left,
							  true);
		const auto pressed =
			entities::stateLayerColor(registry, id, base, content);
		EXPECT_LT(pressed.getRed(), hovered.getRed());

		entities::setEnabled(registry, id, false);
		EXPECT_EQ(entities::stateLayerColor(registry, id, base, content), base);
	}

	TEST_F(TestInteraction, StyleStateSystemMirrorsInteractions)
	{
		ecs::ComponentRegistry registry;
		ecs::Entity entity;
		const auto id = entity.getIdentifier();
		registry.registerComponentsForEntity<components::Bound>(id);
		entities::makeInteractive(registry, id,
								  components::AccessibilityRole::Button, {});

		StyleState system;
		system.bindComponentRegistry(registry);

		registry.getComponent<components::MouseHoverInteraction>(id).setHovered(
			true);
		system.update(id);
		EXPECT_TRUE(
			registry.getComponent<components::StyleState>(id).isHovered());

		registry.getComponent<components::MouseHoverInteraction>(id).setHovered(
			false);
		system.update(id);
		EXPECT_FALSE(
			registry.getComponent<components::StyleState>(id).isHovered());
		system.unbindComponentRegistry();
	}

}	 // namespace guillaume::systems::tests