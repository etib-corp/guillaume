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

#include "entities/test_tooltip.hpp"

#include <memory>
#include <string>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/entities/style_helpers.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Tooltip>
			makeTooltip(ecs::ComponentRegistry &registry,
						Tooltip::Variant variant = Tooltip::Variant::Plain,
						const std::string &text	 = "Tooltip")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Tooltip::Builder builder(registry, entityRegistry);
			Tooltip::Director director;

			return director.makeTooltip(builder, nullptr, variant, text, "");
		}
	}	 // namespace

	TEST_F(TestTooltip, PlainIsPillWithRadiusFour)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->initialize();
		tooltip->update();

		const auto &borders = registry.getComponent<components::Borders>(
			tooltip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 4.0f);
	}

	TEST_F(TestTooltip, RichUsesRadiusTwelve)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Rich);

		tooltip->initialize();
		tooltip->update();

		const auto &borders = registry.getComponent<components::Borders>(
			tooltip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 12.0f);
	}

	TEST_F(TestTooltip, PlainUsesInverseSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->initialize();
		tooltip->update();

		const auto &color =
			registry.getComponent<components::Color>(tooltip->getIdentifier());
		EXPECT_EQ(color.getColor(),
				  schemeColor(SchemeColorRole::InverseSurface));
	}

	TEST_F(TestTooltip, OverlayConfiguration)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->initialize();
		tooltip->update();

		const auto &overlay = registry.getComponent<components::Overlay>(
			tooltip->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestTooltip, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto tooltip = makeTooltip(registry, Tooltip::Variant::Plain);

		tooltip->setVariant(Tooltip::Variant::Rich);

		EXPECT_EQ(tooltip->getVariant(), Tooltip::Variant::Rich);

		const auto &borders = registry.getComponent<components::Borders>(
			tooltip->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 12.0f);
	}

	TEST_F(TestTooltip, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Tooltip::Builder builder(registry, entityRegistry);
		Tooltip::Director director;

		auto tooltip = director.makeTooltip(
			builder, parent, Tooltip::Variant::Rich, "Text", "Title");

		ASSERT_NE(tooltip, nullptr);
		EXPECT_EQ(tooltip->getParent(), parent);
		EXPECT_EQ(tooltip->getVariant(), Tooltip::Variant::Rich);
	}

}	 // namespace guillaume::entities::tests
