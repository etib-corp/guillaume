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

#include "entities/test_segmented_button.hpp"

#include <guillaume/entities/button.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/glyph.hpp>

#include <guillaume/components/bound.hpp>

#include <guillaume/components/borders.hpp>

#include <guillaume/components/accessibility.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestSegmentedButton, CreatesSegments)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "Day", "Week", "Month" },
			SegmentedButton::SelectionMode::Single,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		EXPECT_EQ(segmented->getSegmentCount(), 3);
	}

	TEST_F(TestSegmentedButton, SegmentsAreLaidOutSideBySide)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "A", "B", "C" },
			SegmentedButton::SelectionMode::Single,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		const auto firstX = registry
								.getComponent<components::Transform>(
									segmented->getSegmentIdentifier(0))
								.getPose()
								.getPosition()
								.getX();
		const auto secondX = registry
								 .getComponent<components::Transform>(
									 segmented->getSegmentIdentifier(1))
								 .getPose()
								 .getPosition()
								 .getX();
		const auto thirdX = registry
								.getComponent<components::Transform>(
									segmented->getSegmentIdentifier(2))
								.getPose()
								.getPosition()
								.getX();

		EXPECT_GT(secondX, firstX);
		EXPECT_GT(thirdX, secondX);
	}

	TEST_F(TestSegmentedButton, ConnectedCornersAppliedToOuterSegments)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "A", "B", "C" },
			SegmentedButton::SelectionMode::Single,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		const auto &firstBorders = registry.getComponent<components::Borders>(
			segmented->getSegmentIdentifier(0));
		const auto &middleBorders = registry.getComponent<components::Borders>(
			segmented->getSegmentIdentifier(1));
		const auto &lastBorders = registry.getComponent<components::Borders>(
			segmented->getSegmentIdentifier(2));

		EXPECT_GT(firstBorders.getTopLeftRadius(), 0.0f);
		EXPECT_GT(firstBorders.getBottomLeftRadius(), 0.0f);
		EXPECT_EQ(firstBorders.getTopRightRadius(), 0.0f);
		EXPECT_EQ(firstBorders.getBottomRightRadius(), 0.0f);

		EXPECT_EQ(middleBorders.getTopLeftRadius(), 0.0f);
		EXPECT_EQ(middleBorders.getTopRightRadius(), 0.0f);
		EXPECT_EQ(middleBorders.getBottomLeftRadius(), 0.0f);
		EXPECT_EQ(middleBorders.getBottomRightRadius(), 0.0f);

		EXPECT_EQ(lastBorders.getTopLeftRadius(), 0.0f);
		EXPECT_EQ(lastBorders.getBottomLeftRadius(), 0.0f);
		EXPECT_GT(lastBorders.getTopRightRadius(), 0.0f);
		EXPECT_GT(lastBorders.getBottomRightRadius(), 0.0f);
	}

	TEST_F(TestSegmentedButton, LayoutAppliedToSegmentedBound)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "A", "B", "C" },
			SegmentedButton::SelectionMode::Single,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		const auto &bound = registry.getComponent<components::Bound>(
			segmented->getIdentifier());

		EXPECT_GT(bound.getWidth(), 0.0f);
		EXPECT_GT(bound.getHeight(), 0.0f);
	}

	TEST_F(TestSegmentedButton, SingleSelectionExclusive)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "Day", "Week", "Month" },
			SegmentedButton::SelectionMode::Single,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		segmented->select(0);
		EXPECT_TRUE(segmented->isSelected(0));
		EXPECT_FALSE(segmented->isSelected(1));

		segmented->select(1);
		EXPECT_FALSE(segmented->isSelected(0));
		EXPECT_TRUE(segmented->isSelected(1));
	}

	TEST_F(TestSegmentedButton, MultiSelectionAllowsMultiple)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "A", "B", "C" },
			SegmentedButton::SelectionMode::Multi,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		segmented->select(0);
		segmented->select(2);

		EXPECT_TRUE(segmented->isSelected(0));
		EXPECT_FALSE(segmented->isSelected(1));
		EXPECT_TRUE(segmented->isSelected(2));

		auto indices = segmented->getSelectedIndices();
		EXPECT_EQ(indices.size(), 2);
	}

	TEST_F(TestSegmentedButton, DeselectClearsSelection)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "A", "B" },
			SegmentedButton::SelectionMode::Multi,
			std::function<void(std::size_t)>());

		segmented->initialize();
		segmented->update();

		segmented->select(0);
		EXPECT_TRUE(segmented->isSelected(0));

		segmented->deselect(0);
		EXPECT_FALSE(segmented->isSelected(0));
	}

	TEST_F(TestSegmentedButton, DirectorBuildsAndRegistersSegmentedButton)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		SegmentedButton::Builder builder(registry, entityRegistry);
		SegmentedButton::Director director;

		auto segmented = director.makeSegmentedButton(
			builder, parent,
			std::vector<std::string> { "Day", "Week", "Month" },
			SegmentedButton::SelectionMode::Multi,
			std::function<void(std::size_t)>());

		ASSERT_NE(segmented, nullptr);
		EXPECT_EQ(segmented->getParent(), parent);
		EXPECT_EQ(segmented->getSegmentCount(), 3U);
	}
	TEST_F(TestSegmentedButton, SingleSelectCommitsAndShowsCheck)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry, std::vector<std::string> { "Day", "Week" },
			SegmentedButton::SelectionMode::Single,
			std::function<void(std::size_t)>());
		segmented->initialize();
		segmented->update();

		const auto first = segmented->getSegmentIdentifier(0);
		registry.getComponent<components::MouseButtonInteraction>(first)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(segmented->isSelected(0));
		// Pressing the selected segment again keeps it selected.
		registry.getComponent<components::MouseButtonInteraction>(first)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_TRUE(segmented->isSelected(0));

		auto segment = segmented->getEntity<Button>(first);
		ASSERT_NE(segment, nullptr);
		EXPECT_EQ(
			registry
				.getComponent<components::Glyph>(segment->getIconIdentifier())
				.getName(),
			"check");
		EXPECT_TRUE(
			registry.getComponent<components::Borders>(first).hasStroke());
		EXPECT_TRUE(registry.getComponent<components::Accessibility>(first)
						.isSelected());
	}

	TEST_F(TestSegmentedButton, SegmentsHaveEqualWidthAndPerSegmentOptions)
	{
		ecs::ComponentRegistry registry;
		auto segmented = std::make_shared<SegmentedButton>(
			registry,
			std::vector<SegmentedButton::Segment> {
				{ "A", "sunny", false, "Sunny" },
				{ "Longer label", "", true, "" },
			},
			SegmentedButton::SelectionMode::Multi,
			std::function<void(std::size_t)>());
		segmented->initialize();
		segmented->update();
		segmented->update();

		const auto first  = segmented->getSegmentIdentifier(0);
		const auto second = segmented->getSegmentIdentifier(1);
		const float firstWidth =
			registry.getComponent<components::Bound>(first).getWidth();
		EXPECT_FLOAT_EQ(
			firstWidth,
			registry.getComponent<components::Bound>(second).getWidth());
		EXPECT_GE(firstWidth, SegmentedButton::MinSegmentWidth);

		EXPECT_EQ(
			registry.getComponent<components::Accessibility>(first).getLabel(),
			"Sunny");
		EXPECT_FALSE(
			registry.getComponent<components::StyleState>(second).isEnabled());
		registry.getComponent<components::MouseButtonInteraction>(second)
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();
		EXPECT_FALSE(segmented->isSelected(1));
	}

}	 // namespace guillaume::entities::tests
