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

#include "entities/test_checkbox.hpp"

#include <guillaume/entities/style_helpers.hpp>

#include <guillaume/components/transform.hpp>

#include <guillaume/components/text.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_hover_interaction.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/accessibility.hpp>

#include <optional>

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/line.hpp>
#include <guillaume/components/selection.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

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
		std::shared_ptr<Checkbox>
			makeCheckbox(ecs::ComponentRegistry &registry,
						 Checkbox::State state	   = Checkbox::State::Unchecked,
						 Checkbox::Variant variant = Checkbox::Variant::Enabled,
						 const std::string &label  = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Checkbox::Builder builder(registry, entityRegistry);

			return builder.makeCheckbox(nullptr, state, variant, label);
		}
	}	 // namespace

	TEST_F(TestCheckbox, DefaultGeometryIsSquareBox)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->initialize();
		checkbox->update();

		const auto &bound =
			registry.getComponent<components::Bound>(checkbox->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 18.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 18.0f);

		const auto &borders = registry.getComponent<components::Borders>(
			checkbox->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 2.0f);
	}

	TEST_F(TestCheckbox, StateRoundTripUpdatesSelectable)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->initialize();
		checkbox->setState(Checkbox::State::Checked);

		EXPECT_EQ(checkbox->getState(), Checkbox::State::Checked);
		EXPECT_TRUE(checkbox->isChecked());

		checkbox->setState(Checkbox::State::Unchecked);
		EXPECT_EQ(checkbox->getState(), Checkbox::State::Unchecked);
		EXPECT_FALSE(checkbox->isChecked());
	}

	TEST_F(TestCheckbox, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->setVariant(Checkbox::Variant::Error);
		EXPECT_EQ(checkbox->getVariant(), Checkbox::Variant::Error);

		checkbox->setVariant(Checkbox::Variant::Disabled);
		EXPECT_EQ(checkbox->getVariant(), Checkbox::Variant::Disabled);
	}

	TEST_F(TestCheckbox, IndeterminateEnablesLineStroke)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry);

		checkbox->initialize();
		checkbox->setState(Checkbox::State::Indeterminate);

		const auto &line =
			registry.getComponent<components::Line>(checkbox->getIdentifier());
		EXPECT_GT(line.getThickness(), 0.0f);
	}

	TEST_F(TestCheckbox, LabelSitsOutsideTheBox)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry, Checkbox::State::Checked,
									 Checkbox::Variant::Enabled, "Accept");

		checkbox->initialize();
		checkbox->update();

		// Only the check glyph is laid out inside the 18 dp box.
		EXPECT_EQ(checkbox->getChildren().size(), 1U);
		EXPECT_GE(labelOffsetX(registry, checkbox),
				  (18.0f + Checkbox::TouchTarget) / 2.0f);
	}

	TEST_F(TestCheckbox, PressTogglesAndExposesSemantics)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry, Checkbox::State::Indeterminate,
									 Checkbox::Variant::Enabled, "Accept");
		checkbox->initialize();

		std::optional<Checkbox::State> reported;
		checkbox->setOnValueChange([&reported](Checkbox::State state) {
			reported = state;
		});

		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				checkbox->getIdentifier());
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Checkbox);
		EXPECT_EQ(accessibility.getChecked(), components::CheckedState::Mixed);

		const auto release =
			registry
				.getComponent<components::MouseButtonInteraction>(
					checkbox->getIdentifier())
				.getOnButtonReleaseHandler(
					utility::event::MouseButtonEvent::Button::Left);
		release();
		EXPECT_EQ(checkbox->getState(), Checkbox::State::Checked);
		EXPECT_EQ(reported, Checkbox::State::Checked);
		EXPECT_EQ(accessibility.getChecked(),
				  components::CheckedState::Checked);
		EXPECT_EQ(
			registry.getComponent<components::Color>(checkbox->getIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::Primary));

		release();
		EXPECT_EQ(checkbox->getState(), Checkbox::State::Unchecked);
		EXPECT_EQ(
			registry
				.getComponent<components::Borders>(checkbox->getIdentifier())
				.getColor(),
			schemeColor(SchemeColorRole::OnSurfaceVariant));
	}

	TEST_F(TestCheckbox, DisabledCheckboxIgnoresPress)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry, Checkbox::State::Unchecked,
									 Checkbox::Variant::Disabled);
		checkbox->initialize();

		checkbox->toggle();
		registry
			.getComponent<components::MouseButtonInteraction>(
				checkbox->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_EQ(checkbox->getState(), Checkbox::State::Unchecked);
		EXPECT_FALSE(
			registry
				.getComponent<components::StyleState>(checkbox->getIdentifier())
				.isEnabled());
		EXPECT_EQ(
			registry
				.getComponent<components::Borders>(checkbox->getIdentifier())
				.getColor(),
			disabledContentColor());
	}

	TEST_F(TestCheckbox, TargetAndColorOverrides)
	{
		ecs::ComponentRegistry registry;
		auto checkbox = makeCheckbox(registry, Checkbox::State::Checked);
		checkbox->initialize();

		const auto margin =
			registry
				.getComponent<components::MouseHoverInteraction>(
					checkbox->getIdentifier())
				.getAccessibilityMargin();
		EXPECT_FLOAT_EQ(18.0f + 2.0f * static_cast<float>(margin.getWidth()),
						Checkbox::TouchTarget);

		const utility::graphic::Color32Bit custom(10, 120, 30, 255);
		checkbox->setCheckedColor(custom);
		EXPECT_EQ(
			registry.getComponent<components::Color>(checkbox->getIdentifier())
				.getColor(),
			custom);
	}

	TEST_F(TestCheckbox, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Checkbox::Builder builder(registry, entityRegistry);

		auto checkbox = builder.makeCheckbox(parent, Checkbox::State::Checked,
											 Checkbox::Variant::Error, "Terms");

		ASSERT_NE(checkbox, nullptr);
		EXPECT_EQ(checkbox->getParent(), parent);
		EXPECT_EQ(checkbox->getState(), Checkbox::State::Checked);
		EXPECT_EQ(checkbox->getVariant(), Checkbox::Variant::Error);
	}

}	 // namespace guillaume::entities::tests
