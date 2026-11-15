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

#include "guillaume/systems/snackbar.hpp"

#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Snackbar::Variant;
		using style::schemeColor;
		using style::transparentColor;

		/// Height of a single-line snackbar, in dp.
		constexpr float SingleLineHeight = 48.0f;

		/// Height of a multi-line snackbar, in dp.
		constexpr float MultiLineHeight = 68.0f;

		/**
		 * @brief Set the color of a child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param color The color to write.
		 */
		void colorChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const utility::graphic::Color32Bit &color)
		{
			if (registry.hasComponent<components::Color>(child)) {
				registry.getComponent<components::Color>(child).setColor(color);
			}
		}
	}	 // namespace

	Snackbar::Snackbar(void)
		: ecs::SystemFiller<components::Snackbar>(ecs::Phase::Layout)
	{
	}

	void Snackbar::install(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Accessibility>(identifier)
			.setRole(components::AccessibilityRole::Alert)
			.setLiveRegion(components::LiveRegion::Polite);

		// systems::Overlay reports the timeout and Escape through this.
		componentRegistry.getComponent<components::Overlay>(identifier)
			.setOnDismissHandler([registry, identifier]() {
				dismiss(*registry, identifier);
			});

		installAction(componentRegistry, identifier);
	}

	void
		Snackbar::installAction(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const auto &snackbar =
			componentRegistry.getComponent<components::Snackbar>(identifier);
		const auto action = snackbar.getActionEntity();

		if (action == ecs::Entity::InvalidIdentifier) {
			return;
		}

		interaction::makeInteractive(componentRegistry, action,
									 components::AccessibilityRole::Button,
									 [registry, identifier]() {
										 pressAction(*registry, identifier);
									 });
		componentRegistry.getComponent<components::Accessibility>(action)
			.setLabel(snackbar.getAction());
	}

	void Snackbar::show(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Snackbar>(entityIdentifier)
			.setVisible(true);
		apply(componentRegistry, entityIdentifier);
	}

	void Snackbar::hide(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Snackbar>(entityIdentifier)
			.setVisible(false);
		apply(componentRegistry, entityIdentifier);
	}

	void Snackbar::dismiss(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &snackbar = componentRegistry.getComponent<components::Snackbar>(
			entityIdentifier);

		// A dismissal the snackbar requested itself (hide) is not reported:
		// like Paper, onDismiss only fires on timeout, Escape or action.
		if (!snackbar.isVisible()) {
			return;
		}
		snackbar.setVisible(false);
		apply(componentRegistry, entityIdentifier);

		// Copy: the callback may replace itself.
		const auto onDismiss = snackbar.getOnDismiss();
		if (onDismiss) {
			onDismiss();
		}
	}

	void Snackbar::pressAction(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &snackbar =
			componentRegistry.getComponent<components::Snackbar>(
				entityIdentifier);

		if (!snackbar.isVisible()) {
			return;
		}

		// Copy: the callback may replace itself.
		const auto onAction = snackbar.getOnAction();
		if (onAction) {
			onAction();
		}
		dismiss(componentRegistry, entityIdentifier);
	}

	void Snackbar::apply(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &snackbar =
			componentRegistry.getComponent<components::Snackbar>(
				entityIdentifier);
		const bool visible = snackbar.isVisible();
		const auto message = snackbar.getMessageEntity();
		const auto action  = snackbar.getActionEntity();

		std::vector<ecs::Entity::Identifier> children;
		if (message != ecs::Entity::InvalidIdentifier) {
			children.push_back(message);
		}
		if (action != ecs::Entity::InvalidIdentifier) {
			children.push_back(action);
		}

		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setAxis(components::Layout::Axis::Horizontal)
			.setMainAxisAlignment(
				components::Layout::MainAxisAlignment::SpaceBetween)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setPadding(16.0f)
			.setSpacing(8.0f)
			.setFixedHeight(snackbar.getVariant() == Variant::MultiLine
								? MultiLineHeight
								: SingleLineHeight)
			.setChildren(children);

		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(getShapeRadius(theme::ShapeRole::ExtraSmall));
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(visible ? schemeColor(SchemeColorRole::InverseSurface)
							  : transparentColor());
		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel(visible ? theme::ElevationLevel::Level3
									   : theme::ElevationLevel::Level0);

		auto &overlay = componentRegistry.getComponent<components::Overlay>(
			entityIdentifier);
		overlay.setModal(false).setDismissOnOutsideClick(false).setAutoDismiss(
			snackbar.getDuration());
		if (overlay.isVisible() != visible) {
			// Showing restarts the auto-dismiss countdown.
			overlay.setElapsed(0.0f);
			overlay.setVisible(visible);
		}

		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(snackbar.getMessage());

		if (componentRegistry.hasComponent<components::Text>(message)) {
			componentRegistry.getComponent<components::Text>(message)
				.setContent(snackbar.getMessage());
		}
		colorChild(componentRegistry, message,
				   visible ? schemeColor(SchemeColorRole::InverseOnSurface)
						   : transparentColor());
		colorChild(componentRegistry, action,
				   visible ? schemeColor(SchemeColorRole::InversePrimary)
						   : transparentColor());

		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void Snackbar::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Snackbar>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
