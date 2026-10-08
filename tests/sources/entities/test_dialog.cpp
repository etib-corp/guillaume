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

#include "entities/test_dialog.hpp"

#include <memory>
#include <string>
#include <vector>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/overlay.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Dialog>
			makeDialog(ecs::ComponentRegistry &registry,
					   Dialog::Variant variant = Dialog::Variant::Alert)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Dialog::Builder builder(registry, entityRegistry);
			Dialog::Director director;

			return director.makeDialog(builder, nullptr, variant, "Title",
									   "Message", { "OK", "Cancel" });
		}
	}	 // namespace

	TEST_F(TestDialog, AlertGeometryIsCenteredPanel)
	{
		ecs::ComponentRegistry registry;
		auto dialog = makeDialog(registry, Dialog::Variant::Alert);

		dialog->initialize();
		dialog->update();

		const auto &bound =
			registry.getComponent<components::Bound>(dialog->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getWidth(), 320.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(dialog->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 28.0f);
	}

	TEST_F(TestDialog, FullScreenIsUnrounded)
	{
		ecs::ComponentRegistry registry;
		auto dialog = makeDialog(registry, Dialog::Variant::FullScreen);

		dialog->initialize();
		dialog->update();

		const auto &bound =
			registry.getComponent<components::Bound>(dialog->getIdentifier());
		EXPECT_FLOAT_EQ(bound.getHeight(), 640.0f);

		const auto &borders =
			registry.getComponent<components::Borders>(dialog->getIdentifier());
		EXPECT_FLOAT_EQ(borders.getTopLeftRadius(), 0.0f);
	}

	TEST_F(TestDialog, OverlayIsModalAndEscapable)
	{
		ecs::ComponentRegistry registry;
		auto dialog = makeDialog(registry, Dialog::Variant::Alert);

		dialog->initialize();
		dialog->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(dialog->getIdentifier());
		EXPECT_TRUE(overlay.isModal());
		EXPECT_TRUE(overlay.dismissesOnOutsideClick());
		EXPECT_TRUE(overlay.dismissesOnEscape());
	}

	TEST_F(TestDialog, FullScreenDoesNotDismissOnOutsideClick)
	{
		ecs::ComponentRegistry registry;
		auto dialog = makeDialog(registry, Dialog::Variant::FullScreen);

		dialog->initialize();
		dialog->update();

		const auto &overlay =
			registry.getComponent<components::Overlay>(dialog->getIdentifier());
		EXPECT_FALSE(overlay.dismissesOnOutsideClick());
	}

	TEST_F(TestDialog, ShowHideTogglesVisibility)
	{
		ecs::ComponentRegistry registry;
		auto dialog = makeDialog(registry, Dialog::Variant::Alert);

		dialog->initialize();

		EXPECT_FALSE(dialog->isVisible());
		dialog->show();
		EXPECT_TRUE(dialog->isVisible());
		dialog->hide();
		EXPECT_FALSE(dialog->isVisible());
	}

	TEST_F(TestDialog, VariantRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto dialog = makeDialog(registry, Dialog::Variant::Alert);

		dialog->setVariant(Dialog::Variant::Confirmation);

		EXPECT_EQ(dialog->getVariant(), Dialog::Variant::Confirmation);
	}

	TEST_F(TestDialog, DirectorBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();

		Dialog::Builder builder(registry, entityRegistry);
		Dialog::Director director;

		auto dialog =
			director.makeDialog(builder, parent, Dialog::Variant::Simple,
								"Title", "Message", { "One" });

		ASSERT_NE(dialog, nullptr);
		EXPECT_EQ(dialog->getParent(), parent);
		EXPECT_EQ(dialog->getVariant(), Dialog::Variant::Simple);
	}

}	 // namespace guillaume::entities::tests
