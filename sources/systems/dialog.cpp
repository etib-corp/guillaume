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

#include "guillaume/systems/dialog.hpp"

#include <string>
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
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/backdrop.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Dialog::Variant;
		using entities::schemeColor;

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

		/**
		 * @brief Set the content of a text child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param content The text to write.
		 */
		void writeChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const std::string &content)
		{
			if (registry.hasComponent<components::Text>(child)) {
				registry.getComponent<components::Text>(child).setContent(
					content);
			}
		}
	}	 // namespace

	Dialog::Dialog(void)
		: ecs::SystemFiller<components::Dialog>(ecs::Phase::Layout)
	{
	}

	void Dialog::install(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setRole(components::AccessibilityRole::Dialog)
			.setFocusable(false);

		installTitle(componentRegistry, entityIdentifier);
	}

	void Dialog::installTitle(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &dialog = componentRegistry.getComponent<components::Dialog>(
			entityIdentifier);
		const auto title = dialog.getTitleEntity();

		if (title == ecs::Entity::InvalidIdentifier) {
			return;
		}
		if (!componentRegistry.hasComponent<components::Accessibility>(title)) {
			componentRegistry.addComponent<components::Accessibility>(title);
		}
		componentRegistry.getComponent<components::Accessibility>(title)
			.setRole(components::AccessibilityRole::Header)
			.setLabel(dialog.getTitle())
			.setFocusable(false);
	}

	void Dialog::show(ecs::ComponentRegistry &componentRegistry,
					  const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(true);
		apply(componentRegistry, entityIdentifier);
	}

	void Dialog::hide(ecs::ComponentRegistry &componentRegistry,
					  const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(false);
		apply(componentRegistry, entityIdentifier);
	}

	void Dialog::dismiss(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!componentRegistry
				 .getComponent<components::Overlay>(entityIdentifier)
				 .isVisible()) {
			return;
		}
		hide(componentRegistry, entityIdentifier);

		// Copy: the handler may replace itself.
		const auto onDismiss =
			componentRegistry
				.getComponent<components::Overlay>(entityIdentifier)
				.getOnDismissHandler();
		if (onDismiss) {
			onDismiss();
		}
	}

	void Dialog::pressAction(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier,
							 std::size_t index)
	{
		const auto &actions =
			componentRegistry.getComponent<components::Dialog>(entityIdentifier)
				.getActions();

		if (index >= actions.size()) {
			return;
		}

		// Copy: the handler may replace the actions.
		const auto onPress = actions[index].onPress;
		if (onPress) {
			onPress();
		}
	}

	void Dialog::apply(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &dialog = componentRegistry.getComponent<components::Dialog>(
			entityIdentifier);
		const bool fullScreen = dialog.getVariant() == Variant::FullScreen;
		const auto icon		  = dialog.getIconEntity();
		const auto title	  = dialog.getTitleEntity();
		const auto message	  = dialog.getMessageEntity();
		const bool hasIcon	  = icon != ecs::Entity::InvalidIdentifier;

		std::vector<ecs::Entity::Identifier> children;
		for (const auto child:
			 { icon, title, message, dialog.getActionRow() }) {
			if (child != ecs::Entity::InvalidIdentifier) {
				children.push_back(child);
			}
		}

		// M3: with a hero icon, the icon and the headline are centred.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Center)
			.setCrossAxisAlignment(
				hasIcon ? components::Layout::CrossAxisAlignment::Center
						: components::Layout::CrossAxisAlignment::Start)
			.setPadding(Padding)
			.setSpacing(Spacing)
			.setFixedWidth(components::Dialog::Width)
			.setChildren(children);
		if (fullScreen) {
			layout.setFixedHeight(components::Dialog::FullScreenHeight);
		} else {
			layout.clearFixedHeight();
		}

		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(
				getShapeRadius(fullScreen ? theme::ShapeRole::None
										  : theme::ShapeRole::ExtraLarge));
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(getElevatedSurfaceColor(theme::ElevationLevel::Level3));
		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel(theme::ElevationLevel::Level3);
		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(dialog.getTitle());

		auto &overlay = componentRegistry.getComponent<components::Overlay>(
			entityIdentifier);
		overlay.setModal(true)
			.setDismissOnOutsideClick(!fullScreen)
			.setDismissOnEscape(true);

		colorChild(componentRegistry, icon,
				   schemeColor(SchemeColorRole::Secondary));
		writeChild(componentRegistry, title, dialog.getTitle());
		colorChild(componentRegistry, title,
				   schemeColor(SchemeColorRole::OnSurface));
		if (componentRegistry.hasComponent<components::Accessibility>(title)) {
			componentRegistry.getComponent<components::Accessibility>(title)
				.setLabel(dialog.getTitle());
		}
		writeChild(componentRegistry, message, dialog.getMessage());
		colorChild(componentRegistry, message,
				   schemeColor(SchemeColorRole::OnSurfaceVariant));

		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));

		const auto backdrop = dialog.getBackdrop();
		if (backdrop != ecs::Entity::InvalidIdentifier) {
			// A full-screen dialog covers the viewport and needs no backdrop.
			Backdrop::setVisible(componentRegistry, backdrop,
								 overlay.isVisible() && !fullScreen);
			const auto &bound =
				componentRegistry.getComponent<components::Bound>(
					entityIdentifier);
			Backdrop::follow(
				componentRegistry, backdrop,
				componentRegistry
					.getComponent<components::Transform>(entityIdentifier)
					.getPose(),
				bound.getWidth(), bound.getHeight());
		}
	}

	void Dialog::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Dialog>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
