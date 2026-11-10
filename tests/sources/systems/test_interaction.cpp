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
		registry.registerComponentsForEntity<
			components::MouseHoverInteraction>(entity.getIdentifier());
		registry
			.getComponent<components::MouseHoverInteraction>(
				entity.getIdentifier())
			.setAccessibilityMargin(utility::graphic::SizeUI(8, 4));

		const auto margin = hitMargin(registry, entity.getIdentifier());
		EXPECT_FLOAT_EQ(margin[0], 8.0f);
		EXPECT_FLOAT_EQ(margin[1], 4.0f);
	}

}	 // namespace guillaume::systems::tests