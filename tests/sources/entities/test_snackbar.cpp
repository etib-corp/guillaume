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

#include "entities/test_snackbar.hpp"

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
		std::shared_ptr<Snackbar> makeSnackbar(
			ecs::ComponentRegistry &registry,
			Snackbar::Variant variant  = Snackbar::Variant::SingleLine,
			const std::string &message = "Message",
			const std::string &action  = "")
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Snackbar::Builder builder(registry, entityRegistry);
			Snackbar::Director director;

			return director.makeSnackbar(builder, nullptr, variant, message,
										 action);
		}
	}	 // namespace

	TEST_F(TestSnackbar, SingleLineHeightIsFortyEight)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->initialize();
		snackbar->update();

		const auto &bound =
			registry.getComponent<components::Bound>(snackbar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 48.0f);
	}

	TEST_F(TestSnackbar, MultiLineHeightIsSixtyEight)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::MultiLine);

		snackbar->initialize();
		snackbar->update();

		const auto &bound =
			registry.getComponent<components::Bound>(snackbar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 68.0f);
	}

	TEST_F(TestSnackbar, UsesInverseSurfaceColor)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->initialize();
		snackbar->update();

		const auto &color =
			registry.getComponent<components::Color>(snackbar->getIdentifier());
		EXPECT_EQ(color.getColor(),
				  schemeColor(SchemeColorRole::InverseSurface));
	}

	TEST_F(TestSnackbar, OverlayAutoDismisses)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->initialize();
		snackbar->update();

		const auto &overlay = registry.getComponent<components::Overlay>(
			snackbar->getIdentifier());
		EXPECT_FALSE(overlay.isModal());
		EXPECT_TRUE(overlay.hasAutoDismiss());
		EXPECT_FLOAT_EQ(overlay.getAutoDismiss(), 4.0f);
	}

	TEST_F(TestSnackbar, WithActionBuildsTwoChildren)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::WithAction,
									 "Message", "Undo");

		snackbar->initialize();
		snackbar->update();

		EXPECT_EQ(snackbar->getChildren().size(), 2U);
	}

	TEST_F(TestSnackbar, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto snackbar = makeSnackbar(registry, Snackbar::Variant::SingleLine);

		snackbar->setVariant(Snackbar::Variant::MultiLine);

		EXPECT_EQ(snackbar->getVariant(), Snackbar::Variant::MultiLine);

		const auto &bound =
			registry.getComponent<components::Bound>(snackbar->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 68.0f);
	}

	TEST_F(TestSnackbar, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Snackbar::Builder builder(registry, entityRegistry);
		Snackbar::Director director;

		auto snackbar = director.makeSnackbar(
			builder, parent, Snackbar::Variant::WithAction, "Message", "Undo");

		ASSERT_NE(snackbar, nullptr);
		EXPECT_EQ(snackbar->getParent(), parent);
		EXPECT_EQ(snackbar->getVariant(), Snackbar::Variant::WithAction);
	}

}	 // namespace guillaume::entities::tests
