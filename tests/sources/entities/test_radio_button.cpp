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

#include "entities/test_radio_button.hpp"

#include <guillaume/entities/style_helpers.hpp>

#include <guillaume/entities/radio_group.hpp>

#include <guillaume/components/transform.hpp>

#include <guillaume/components/text.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/accessibility.hpp>

#include <stdexcept>

#include <memory>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/ring.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/systems/layout.hpp>

namespace guillaume::entities::tests
{

	namespace
	{
		template<typename Parent>
		float labelOffsetX(ecs::ComponentRegistry &registry,
						   const std::shared_ptr<Parent> &parent)
		{
			for (const auto &child: parent->getEntities()) {
				if (registry.hasComponent<components::Text>(
						child->getIdentifier())) {
					return registry
						.getComponent<components::Transform>(
							child->getIdentifier())
						.getPose()
						.getPosition()
						.getX();
				}
			}
			return -1.0f;
		}
	}	 // namespace
	namespace
	{
		std::shared_ptr<RadioButton> makeRadio(
			ecs::ComponentRegistry &registry,
			RadioButton::Variant variant = RadioButton::Variant::Enabled,
			const std::string &label	 = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			RadioButton::Builder builder(registry, entityRegistry);

			return builder.makeRadioButton(nullptr, variant, label);
		}
	}	 // namespace

	TEST_F(TestRadioButton, DefaultGeometryIsCircle)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->initialize();
		radio->update();

		const auto &bound =
			registry.getComponent<components::Bound>(radio->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 20.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 20.0f);

		const auto &ring =
			registry.getComponent<components::Ring>(radio->getIdentifier());
		EXPECT_FLOAT_EQ(ring.getThickness(), 2.0f);
	}

	TEST_F(TestRadioButton, ChildrenAreLiftedByTheContentLayer)
	{
		ecs::ComponentRegistry registry;
		auto radio =
			makeRadio(registry, RadioButton::Variant::Enabled, "Label");

		radio->select(true);
		radio->update();

		const float baseZ =
			registry.getComponent<components::Transform>(radio->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();
		const auto depths = directChildDepths(registry, radio);

		ASSERT_FALSE(depths.empty());
		for (const float depth: depths) {
			EXPECT_GE(
				depth,
				baseZ
					+ static_cast<float>(systems::Layout::ContentLayerOffset));
		}
	}

	TEST_F(TestRadioButton, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->setVariant(RadioButton::Variant::Error);
		EXPECT_EQ(radio->getVariant(), RadioButton::Variant::Error);

		radio->setVariant(RadioButton::Variant::Disabled);
		EXPECT_EQ(radio->getVariant(), RadioButton::Variant::Disabled);
	}

	TEST_F(TestRadioButton, SelectRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->initialize();

		EXPECT_FALSE(radio->isSelected());
		radio->select(true);
		EXPECT_TRUE(radio->isSelected());

		radio->select(false);
		EXPECT_FALSE(radio->isSelected());
	}

	TEST_F(TestRadioButton, SelectedDotIsOpaque)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);

		radio->initialize();
		radio->select(true);

		const auto &color =
			registry.getComponent<components::Color>(radio->getIdentifier());
		EXPECT_GT(color.getColor().getAlpha(), 0);
	}

	TEST_F(TestRadioButton, LabelSitsOutsideTheRing)
	{
		ecs::ComponentRegistry registry;
		auto radio =
			makeRadio(registry, RadioButton::Variant::Enabled, "Choice");

		radio->initialize();
		radio->update();

		EXPECT_TRUE(radio->getChildren().empty());
		EXPECT_GE(labelOffsetX(registry, radio),
				  (20.0f + RadioButton::TouchTarget) / 2.0f);
	}

	TEST_F(TestRadioButton, PressSelectsAndExposesSemantics)
	{
		ecs::ComponentRegistry registry;
		auto radio =
			makeRadio(registry, RadioButton::Variant::Enabled, "Choice");
		radio->initialize();

		int presses = 0;
		radio->setOnPress([&presses]() {
			++presses;
		});
		registry
			.getComponent<components::MouseButtonInteraction>(
				radio->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_TRUE(radio->isSelected());
		EXPECT_EQ(presses, 1);
		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				radio->getIdentifier());
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Radio);
		EXPECT_EQ(accessibility.getChecked(),
				  components::CheckedState::Checked);
		EXPECT_EQ(accessibility.getLabel(), "Choice");

		const auto margin =
			registry
				.getComponent<components::MouseHoverInteraction>(
					radio->getIdentifier())
				.getAccessibilityMargin();
		EXPECT_FLOAT_EQ(20.0f + 2.0f * static_cast<float>(margin.getWidth()),
						RadioButton::TouchTarget);
	}

	TEST_F(TestRadioButton, DisabledRadioIgnoresPress)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry, RadioButton::Variant::Disabled, "No");
		radio->initialize();

		registry
			.getComponent<components::MouseButtonInteraction>(
				radio->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_FALSE(radio->isSelected());
		EXPECT_TRUE(radio->isDisabled());
		EXPECT_FALSE(
			registry
				.getComponent<components::StyleState>(radio->getIdentifier())
				.isEnabled());
		EXPECT_EQ(
			registry.getComponent<components::Color>(radio->getIdentifier())
				.getColor(),
			disabledContentColor());
	}

	TEST_F(TestRadioButton, SelectedUsesPrimaryAndTenDpDot)
	{
		ecs::ComponentRegistry registry;
		auto radio = makeRadio(registry);
		radio->initialize();
		radio->select(true);

		EXPECT_EQ(
			registry.getComponent<components::Color>(radio->getIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::Primary));
		EXPECT_FLOAT_EQ(RadioButton::DotSize, 10.0f);
	}

	TEST_F(TestRadioButton, GroupKeepsOneSelectionAndReportsIndex)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		RadioGroup::Builder builder(registry, entityRegistry);
		std::size_t reported = 99;
		auto group			 = builder.addOption("A")
						 .addOption("B")
						 .addOption("C")
						 .withValue(0)
						 .withOnValueChange([&reported](std::size_t index) {
							 reported = index;
						 })
						 .registerEntity(nullptr);

		ASSERT_EQ(group->getOptionCount(), 3u);
		ASSERT_NE(group->getOption(2), nullptr);
		EXPECT_TRUE(group->getOption(0)->isSelected());

		registry
			.getComponent<components::MouseButtonInteraction>(
				group->getOption(2)->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(reported, 2u);
		EXPECT_EQ(group->getValue(), 2u);
		EXPECT_FALSE(group->getOption(0)->isSelected());
		EXPECT_TRUE(group->getOption(2)->isSelected());
		EXPECT_EQ(
			registry
				.getComponent<components::Accessibility>(group->getIdentifier())
				.getRole(),
			components::AccessibilityRole::RadioGroup);
		EXPECT_THROW(group->setValue(5), std::out_of_range);
	}

	TEST_F(TestRadioButton, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		RadioButton::Builder builder(registry, entityRegistry);

		auto radio = builder.makeRadioButton(
			parent, RadioButton::Variant::Error, "Option");

		ASSERT_NE(radio, nullptr);
		EXPECT_EQ(radio->getParent(), parent);
		EXPECT_EQ(radio->getVariant(), RadioButton::Variant::Error);
	}

}	 // namespace guillaume::entities::tests
