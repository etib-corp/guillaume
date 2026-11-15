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

#include "guillaume/systems/app_bar.hpp"

#include <optional>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/button.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/typography.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/button.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/typography.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::AppBar::Variant;
		using style::schemeColor;

		/**
		 * @brief Push the title text, its typescale role and its accessible
		 * name to the title child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param appBar The app bar configuration.
		 */
		void styleTitle(ecs::ComponentRegistry &registry,
						const components::AppBar &appBar)
		{
			const auto title = appBar.getTitleEntity();

			if (registry.hasComponent<components::Text>(title)) {
				registry.getComponent<components::Text>(title).setContent(
					appBar.getTitle());
			}
			if (registry.hasComponent<components::Typography>(title)
				&& registry.hasComponent<components::Text>(title)) {
				auto &typography =
					registry.getComponent<components::Typography>(title);
				const auto role = AppBar::titleRole(appBar.getVariant());

				// Resolving the typescale again would mark the title changed.
				if (typography.getRole() != role
					|| typography.getFontSize().has_value()) {
					typography.setRole(role).setFontSize(std::nullopt);
					Typography::apply(registry, title);
				}
			}
			if (registry.hasComponent<components::Accessibility>(title)) {
				registry.getComponent<components::Accessibility>(title)
					.setLabel(appBar.getTitle());
			}
		}

		/**
		 * @brief Give the navigation button the onSurface content color
		 * (Paper: the leading icon uses onSurface, actions
		 * onSurfaceVariant).
		 * @param registry The registry holding the components.
		 * @param navigation The navigation button (may be invalid).
		 */
		void styleNavigation(ecs::ComponentRegistry &registry,
							 ecs::Entity::Identifier navigation)
		{
			if (!registry.hasComponent<components::Button>(navigation)) {
				return;
			}

			auto &button =
				registry.getComponent<components::Button>(navigation);
			const components::Button::OptionalColor color =
				schemeColor(SchemeColorRole::OnSurface);

			if (button.getContentColorOverride() != color) {
				button.setContentColorOverride(color);
				Button::restyle(registry, navigation);
			}
		}
	}	 // namespace

	AppBar::AppBar(void)
		: ecs::SystemFiller<components::AppBar>(ecs::Phase::Layout)
	{
	}

	void AppBar::installTitle(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &appBar = componentRegistry.getComponent<components::AppBar>(
			entityIdentifier);
		const auto title = appBar.getTitleEntity();

		if (title == ecs::Entity::InvalidIdentifier) {
			return;
		}
		if (!componentRegistry.hasComponent<components::Accessibility>(title)) {
			componentRegistry.addComponent<components::Accessibility>(title);
		}
		componentRegistry.getComponent<components::Accessibility>(title)
			.setRole(components::AccessibilityRole::Header)
			.setLabel(appBar.getTitle())
			.setFocusable(false);
	}

	float AppBar::height(components::AppBar::Variant variant)
	{
		// Paper Appbar/utils.ts: 64 small/center-aligned, 112 medium,
		// 152 large; the bottom app bar is 80.
		switch (variant) {
			case Variant::Medium:
				return 112.0f;
			case Variant::Large:
				return 152.0f;
			case Variant::Bottom:
				return 80.0f;
			case Variant::Small:
			case Variant::CenterAligned:
			default:
				return 64.0f;
		}
	}

	theme::TypeRole AppBar::titleRole(components::AppBar::Variant variant)
	{
		switch (variant) {
			case Variant::Medium:
				return theme::TypeRole::HeadlineSmall;
			case Variant::Large:
				return theme::TypeRole::HeadlineMedium;
			case Variant::Small:
			case Variant::CenterAligned:
			case Variant::Bottom:
			default:
				return theme::TypeRole::TitleLarge;
		}
	}

	bool AppBar::needsNavigationButton(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &appBar = componentRegistry.getComponent<components::AppBar>(
			entityIdentifier);

		return appBar.getNavigationButton() == ecs::Entity::InvalidIdentifier
			&& !appBar.getNavigation().icon.empty();
	}

	bool AppBar::needsTitle(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &appBar = componentRegistry.getComponent<components::AppBar>(
			entityIdentifier);

		return appBar.getTitleEntity() == ecs::Entity::InvalidIdentifier
			&& !appBar.getTitle().empty();
	}

	bool AppBar::needsActionButtons(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &appBar = componentRegistry.getComponent<components::AppBar>(
			entityIdentifier);

		return appBar.getActionButtons().size() != appBar.getActions().size();
	}

	void AppBar::apply(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &appBar = componentRegistry.getComponent<components::AppBar>(
			entityIdentifier);
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		const auto variant	  = appBar.getVariant();
		const auto invalid	  = ecs::Entity::InvalidIdentifier;
		const auto navigation = appBar.getNavigationButton();
		const auto title	  = appBar.getTitleEntity();

		// A center-aligned bar spreads its children, or centers a lone title.
		auto mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		if (variant == Variant::CenterAligned) {
			mainAxisAlignment = (appBar.getNavigation().icon.empty()
								 && appBar.getActions().empty())
				? components::Layout::MainAxisAlignment::Center
				: components::Layout::MainAxisAlignment::SpaceBetween;
		}

		std::vector<ecs::Entity::Identifier> children;
		if (navigation != invalid) {
			children.push_back(navigation);
		}
		if (title != invalid) {
			children.push_back(title);
		}
		for (const auto action: appBar.getActionButtons()) {
			if (action != invalid) {
				children.push_back(action);
			}
		}

		layout.setAxis(components::Layout::Axis::Horizontal)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setMainAxisAlignment(mainAxisAlignment)
			.setPadding(components::AppBar::HorizontalPadding)
			.setSpacing(components::AppBar::HorizontalPadding)
			.setFixedHeight(height(variant))
			.setChildren(children);

		// Paper: elevated bars (and the bottom bar) use elevation.level2.
		const bool tinted = appBar.isElevated() || variant == Variant::Bottom;
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(
				tinted ? getElevatedSurfaceColor(theme::ElevationLevel::Level2)
					   : schemeColor(SchemeColorRole::Surface));
		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel(appBar.isElevated()
								   ? theme::ElevationLevel::Level2
								   : theme::ElevationLevel::Level0);

		styleNavigation(componentRegistry, navigation);
		styleTitle(componentRegistry, appBar);

		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void AppBar::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::AppBar>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
