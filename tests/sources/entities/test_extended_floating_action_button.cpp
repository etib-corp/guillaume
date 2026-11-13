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

#include "entities/test_extended_floating_action_button.hpp"

#include <guillaume/theme.hpp>

#include <guillaume/components/elevation.hpp>

#include <guillaume/components/color.hpp>

#include <guillaume/components/bound.hpp>

#include <guillaume/components/borders.hpp>

#include <functional>
#include <memory>

#include <guillaume/components/text.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestExtendedFloatingActionButton, CreatesLabelChild)
	{
		ecs::ComponentRegistry registry;
		auto fab = ExtendedFloatingActionButton::create(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			ExtendedFloatingActionButton::Variant::Primary,
			ExtendedFloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		EXPECT_NE(fab->getIconIdentifier(), ecs::Entity::InvalidIdentifier);
		EXPECT_NE(fab->getLabelIdentifier(), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestExtendedFloatingActionButton, SetLabelContentRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto fab = ExtendedFloatingActionButton::create(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			ExtendedFloatingActionButton::Variant::Surface,
			ExtendedFloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		fab->setLabelContent("Compose");

		EXPECT_EQ(
			registry.getComponent<components::Text>(fab->getLabelIdentifier())
				.getContent(),
			"Compose");
	}

	TEST_F(TestExtendedFloatingActionButton,
		   BuilderPresetBuildsAndRegistersExtendedFab)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		ExtendedFloatingActionButton::Builder builder(registry, entityRegistry);

		auto fab = builder.makeExtendedFloatingActionButton(
			parent, "Compose", std::function<void(void)>());

		ASSERT_NE(fab, nullptr);
		EXPECT_EQ(fab->getParent(), parent);
		EXPECT_EQ(fab->getVariant(),
				  ExtendedFloatingActionButton::Variant::Primary);
	}
	TEST_F(TestExtendedFloatingActionButton, RadiusLabelAndCollapse)
	{
		ecs::ComponentRegistry registry;
		auto fab = ExtendedFloatingActionButton::create(
			registry, "edit", components::Glyph::Style::Outlined, "Compose",
			ExtendedFloatingActionButton::Variant::Primary,
			ExtendedFloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Borders>(fab->getIdentifier())
				.getTopLeftRadius(),
			getShapeRadius(theme::ShapeRole::Large));
		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Text>(fab->getLabelIdentifier())
				.getFontSize(),
			getTypeToken(theme::TypeRole::LabelLarge).fontSize);
		EXPECT_TRUE(
			registry.getComponent<components::Elevation>(fab->getIdentifier())
				.hasShadow());

		fab->setExtended(false);
		EXPECT_FALSE(fab->isExtended());
		const auto &bound =
			registry.getComponent<components::Bound>(fab->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), bound.getHeight());
		EXPECT_EQ(
			registry.getComponent<components::Color>(fab->getLabelIdentifier())
				.getColor()
				.getAlpha(),
			0);
	}

}	 // namespace guillaume::entities::tests
