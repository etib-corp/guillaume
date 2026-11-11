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

#include "entities/test_switch.hpp"

#include <guillaume/components/transform.hpp>

#include <guillaume/components/text.hpp>

#include <guillaume/components/style_state.hpp>

#include <guillaume/components/mouse_button_interaction.hpp>

#include <guillaume/components/accessibility.hpp>

#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/value.hpp>
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
		std::shared_ptr<Switch>
			makeSwitch(ecs::ComponentRegistry &registry,
					   Switch::Variant variant = Switch::Variant::Enabled,
					   bool checked = false, const std::string &iconGlyph = "",
					   const std::string &label = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Switch::Builder builder(registry, entityRegistry);

			return builder.makeSwitch(nullptr, variant, checked, iconGlyph,
									  label);
		}
	}	 // namespace

	TEST_F(TestSwitch, DefaultGeometryIsPill)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		const auto &bound =
			registry.getComponent<components::Bound>(toggle->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 52.0f);
		EXPECT_FLOAT_EQ(bound.getHeight(), 32.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(toggle->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 16.0f);
	}

	TEST_F(TestSwitch, CheckedRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		EXPECT_FALSE(toggle->isChecked());
		toggle->setChecked(true);
		EXPECT_TRUE(toggle->isChecked());

		toggle->toggle();
		EXPECT_FALSE(toggle->isChecked());
	}

	TEST_F(TestSwitch, ValueReflectsCheckedState)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry, Switch::Variant::Enabled, true);

		const auto &value =
			registry.getComponent<components::Value>(toggle->getIdentifier());
		EXPECT_FLOAT_EQ(value.getValue(), 1.0f);
		EXPECT_FLOAT_EQ(value.getMax(), 1.0f);
		EXPECT_FLOAT_EQ(value.getMin(), 0.0f);
	}

	TEST_F(TestSwitch, ChildrenAreLiftedByTheContentLayer)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry, Switch::Variant::Enabled, false,
								 "check", "Label");

		const float baseZ =
			registry
				.getComponent<components::Transform>(toggle->getIdentifier())
				.getPose()
				.getPosition()
				.getZ();
		const auto depths = directChildDepths(registry, toggle);

		ASSERT_FALSE(depths.empty());
		for (const float depth: depths) {
			EXPECT_GE(
				depth,
				baseZ
					+ static_cast<float>(systems::Layout::ContentLayerOffset));
		}
	}

	TEST_F(TestSwitch, NoChildIsRegisteredAsALayoutChild)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry, Switch::Variant::Enabled, false,
								 "check", "Dark mode");

		const auto &layoutChildren =
			registry.getComponent<components::Layout>(toggle->getIdentifier())
				.getChildren();

		// The thumb, its icon and the label are positioned manually and stay
		// out of the layout list, so nothing is squeezed into the track.
		EXPECT_EQ(layoutChildren.size(), 0u);
		EXPECT_GT(toggle->getEntities().size(), layoutChildren.size());
	}

	TEST_F(TestSwitch, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		toggle->setVariant(Switch::Variant::Disabled);
		EXPECT_EQ(toggle->getVariant(), Switch::Variant::Disabled);

		toggle->setVariant(Switch::Variant::WithIcon);
		EXPECT_EQ(toggle->getVariant(), Switch::Variant::WithIcon);
	}

	TEST_F(TestSwitch, LabelSitsOutsideTheTrack)
	{
		ecs::ComponentRegistry registry;
		auto toggle =
			makeSwitch(registry, Switch::Variant::Enabled, false, "", "Wifi");

		const float trackWidth =
			registry.getComponent<components::Bound>(toggle->getIdentifier())
				.getWidth();
		EXPECT_GE(labelOffsetX(registry, toggle),
				  trackWidth + Switch::LabelGap);
		EXPECT_EQ(registry
					  .getComponent<components::Accessibility>(
						  toggle->getIdentifier())
					  .getLabel(),
				  "Wifi");
	}

	TEST_F(TestSwitch, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Switch::Builder builder(registry, entityRegistry);

		auto toggle = builder.makeSwitch(parent, Switch::Variant::WithIcon,
										 true, "check", "On");

		ASSERT_NE(toggle, nullptr);
		EXPECT_EQ(toggle->getParent(), parent);
		EXPECT_EQ(toggle->getVariant(), Switch::Variant::WithIcon);
		EXPECT_TRUE(toggle->isChecked());
	}

	TEST_F(TestSwitch, DisabledSwitchDoesNotToggle)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry, Switch::Variant::Disabled);

		int changes = 0;
		toggle->setOnValueChange([&changes](bool) {
			++changes;
		});

		toggle->toggle();
		registry
			.getComponent<components::MouseButtonInteraction>(
				toggle->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_FALSE(toggle->isChecked());
		EXPECT_EQ(changes, 0);
		EXPECT_TRUE(toggle->isDisabled());
		EXPECT_FALSE(
			registry
				.getComponent<components::StyleState>(toggle->getIdentifier())
				.isEnabled());
	}

	TEST_F(TestSwitch, ReleaseTogglesAndReportsValue)
	{
		ecs::ComponentRegistry registry;
		auto toggle = makeSwitch(registry);

		bool reported = false;
		toggle->setOnValueChange([&reported](bool value) {
			reported = value;
		});

		registry
			.getComponent<components::MouseButtonInteraction>(
				toggle->getIdentifier())
			.getOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left)();

		EXPECT_TRUE(toggle->isChecked());
		EXPECT_TRUE(reported);
		const auto &accessibility =
			registry.getComponent<components::Accessibility>(
				toggle->getIdentifier());
		EXPECT_EQ(accessibility.getRole(),
				  components::AccessibilityRole::Switch);
		EXPECT_EQ(accessibility.getChecked(),
				  components::CheckedState::Checked);
	}

}	 // namespace guillaume::entities::tests
