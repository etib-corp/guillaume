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

#include <guillaume/components/text.hpp>

namespace guillaume::entities::tests
{
	TEST_F(TestExtendedFloatingActionButton, CreatesLabelChild)
	{
		ecs::ComponentRegistry registry;
		auto fab = std::make_shared<ExtendedFloatingActionButton>(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			ExtendedFloatingActionButton::Variant::Primary,
			ExtendedFloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		fab->initialize();
		fab->update();

		EXPECT_NE(fab->getIconIdentifier(), ecs::Entity::InvalidIdentifier);
		EXPECT_NE(fab->getLabelIdentifier(), ecs::Entity::InvalidIdentifier);
	}

	TEST_F(TestExtendedFloatingActionButton, SetLabelContentRoundTrip)
	{
		ecs::ComponentRegistry registry;
		auto fab = std::make_shared<ExtendedFloatingActionButton>(
			registry, "edit", components::Glyph::Style::Outlined, "Edit",
			ExtendedFloatingActionButton::Variant::Surface,
			ExtendedFloatingActionButton::Size::Medium, false, false, "",
			std::function<void(void)>());

		fab->initialize();
		fab->update();

		fab->setLabelContent("Compose");

		EXPECT_EQ(
			registry.getComponent<components::Text>(fab->getLabelIdentifier())
				.getContent(),
			"Compose");
	}
}	 // namespace guillaume::entities::tests
