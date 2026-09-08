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

#include "entities/test_standard_button_group.hpp"

#include <guillaume/components/transform.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestStandardButtonGroup, CreatesButtons)
	{
		ecs::ComponentRegistry registry;
		auto group = std::make_shared<StandardButtonGroup>(
			registry, std::vector<std::string> { "A", "B", "C" }, 8.0f);

		group->initialize();
		group->update();

		EXPECT_EQ(group->getButtonCount(), 3);
		EXPECT_NE(group->getButtonIdentifier(0),
				  ecs::Entity::InvalidIdentifier);
		EXPECT_EQ(group->getButtonIdentifier(9),
				  ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestStandardButtonGroup, ButtonsAreSpacedWithGap)
	{
		ecs::ComponentRegistry registry;
		auto group = std::make_shared<StandardButtonGroup>(
			registry, std::vector<std::string> { "A", "B", "C" }, 16.0f);

		group->initialize();
		group->update();

		const auto firstX  = registry
								 .getComponent<components::Transform>(
									 group->getButtonIdentifier(0))
								 .getPose()
								 .getPosition()
								 .getX();
		const auto secondX = registry
								 .getComponent<components::Transform>(
									 group->getButtonIdentifier(1))
								 .getPose()
								 .getPosition()
								 .getX();
		const auto thirdX  = registry
								 .getComponent<components::Transform>(
									 group->getButtonIdentifier(2))
								 .getPose()
								 .getPosition()
								 .getX();

		EXPECT_EQ(firstX, 0.0f);
		EXPECT_GT(secondX, firstX);
		EXPECT_GT(thirdX, secondX);
	}
}	 // namespace guillaume::entities::tests
