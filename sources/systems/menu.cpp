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

#include "guillaume/systems/menu.hpp"

#include <algorithm>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Mode			 = components::Menu::Mode;
		using AnchorPosition = components::Menu::AnchorPosition;
		using Row			 = components::Menu::Row;
		using style::disabledContentColor;
		using style::schemeColor;
		using style::transparentColor;

		/// M3 menu item height, in dp.
		constexpr float ItemHeight = 48.0f;

		/// Paper dense menu item height, in dp.
		constexpr float DenseItemHeight = 32.0f;

		/// M3 menu item horizontal padding, in dp.
		constexpr float ItemPadding = theme::SpacingScale::get(3);

		/// M3 gap between the icons and the label, in dp.
		constexpr float ItemSpacing = theme::SpacingScale::get(3);

		/// Corner radius of the state layer of a row, in dp.
		constexpr float RowRadius = 16.0f;

		/// Gap between the anchor and the menu, in dp.
		constexpr float AnchorGap = theme::SpacingScale::get(1);

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
		 * @brief Get the width a row needs to show its content.
		 * @param registry The registry holding the components.
		 * @param row The row.
		 * @return The padded width of its measured children.
		 */
		float contentWidthOf(const ecs::ComponentRegistry &registry,
							 const Row &row)
		{
			float width		  = 2.0f * ItemPadding;
			std::size_t count = 0;

			for (const auto child: row.content) {
				if (!registry.hasComponent<components::Bound>(child)) {
					continue;
				}
				width +=
					registry.getComponent<components::Bound>(child).getWidth();
				++count;
			}
			if (count > 1) {
				width += ItemSpacing * static_cast<float>(count - 1);
			}
			return width;
		}

		/**
		 * @brief Arrange the content of every row inside its row.
		 * @param registry The registry holding the components.
		 * @param rows The rows.
		 */
		void arrangeRows(ecs::ComponentRegistry &registry,
						 const std::vector<Row> &rows)
		{
			for (const auto &row: rows) {
				Layout::apply(registry, row.view, row.content,
							  Layout::layerOf(registry, row.view));
			}
		}

		/**
		 * @brief Move the menu below or above its anchor.
		 * @param registry The registry holding the components.
		 * @param identifier The menu entity.
		 * @param menu Its configuration.
		 */
		void applyAnchor(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier,
						 const components::Menu &menu)
		{
			const auto anchor = menu.getAnchor();

			if (anchor == ecs::Entity::InvalidIdentifier
				|| !registry.hasComponent<components::Transform>(anchor)
				|| !registry.hasComponent<components::Bound>(anchor)) {
				return;
			}

			const auto anchorPose =
				registry.getComponent<components::Transform>(anchor).getPose();
			const auto &anchorBound =
				registry.getComponent<components::Bound>(anchor);
			const auto &bound =
				registry.getComponent<components::Bound>(identifier);

			const float offsetY =
				menu.getAnchorPosition() == AnchorPosition::Bottom
				? anchorBound.getHeight() + AnchorGap
				: -(bound.getHeight() + AnchorGap);

			const utility::graphic::PositionF position(
				anchorPose.getPosition().getX(),
				anchorPose.getPosition().getY() + offsetY,
				anchorPose.getPosition().getZ());
			registry.getComponent<components::Transform>(identifier)
				.setPose(utility::graphic::PoseF(position,
												 anchorPose.getOrientation()));
		}
	}	 // namespace

	Menu::Menu(void)
		: ecs::SystemFiller<components::Menu>(ecs::Phase::Layout)
	{
	}

	void Menu::install(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setRole(components::AccessibilityRole::Menu)
			.setFocusable(false);

		const std::size_t count =
			componentRegistry.getComponent<components::Menu>(entityIdentifier)
				.getRows()
				.size();
		for (std::size_t index = 0; index < count; ++index) {
			installRow(componentRegistry, entityIdentifier, index);
		}
	}

	void Menu::installRow(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const auto &menu =
			componentRegistry.getComponent<components::Menu>(identifier);

		if (index >= menu.getRows().size()) {
			return;
		}

		const auto row = menu.getRows()[index].view;
		interaction::makeInteractive(
			componentRegistry, row, components::AccessibilityRole::MenuItem,
			[registry, identifier, index]() {
				pressItem(*registry, identifier, index);
			},
			[registry, identifier, index]() {
				styleRow(*registry, identifier, index);
			});
		if (index < menu.getItems().size()) {
			const auto &item = menu.getItems()[index];
			componentRegistry.getComponent<components::Accessibility>(row)
				.setLabel(item.label);
			interaction::setEnabled(componentRegistry, row, !item.disabled);
		}
	}

	void Menu::styleRow(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier,
						std::size_t index)
	{
		const auto &rows =
			componentRegistry.getComponent<components::Menu>(entityIdentifier)
				.getRows();

		if (index >= rows.size()) {
			return;
		}

		const auto &row = rows[index];
		const bool enabled =
			interaction::isEnabled(componentRegistry, row.view);

		colorChild(componentRegistry, row.view,
				   interaction::stateLayerColor(
					   componentRegistry, row.view, transparentColor(),
					   schemeColor(SchemeColorRole::OnSurface)));
		colorChild(componentRegistry, row.label,
				   enabled ? schemeColor(SchemeColorRole::OnSurface)
						   : disabledContentColor());
		for (const auto icon: row.icons) {
			colorChild(componentRegistry, icon,
					   enabled ? schemeColor(SchemeColorRole::OnSurfaceVariant)
							   : disabledContentColor());
		}
	}

	void Menu::pressItem(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 std::size_t index)
	{
		const auto &items =
			componentRegistry.getComponent<components::Menu>(entityIdentifier)
				.getItems();

		if (index >= items.size() || items[index].disabled) {
			return;
		}

		// Copy: the handler may replace the items.
		const auto onPress = items[index].onPress;
		if (onPress) {
			onPress();
		}
	}

	void Menu::setItemDisabled(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index, bool disabled)
	{
		componentRegistry.getComponent<components::Menu>(entityIdentifier)
			.setItemDisabled(index, disabled);
		apply(componentRegistry, entityIdentifier);
	}

	void Menu::show(ecs::ComponentRegistry &componentRegistry,
					const ecs::Entity::Identifier &entityIdentifier)
	{
		apply(componentRegistry, entityIdentifier);
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(true);
	}

	void Menu::hide(ecs::ComponentRegistry &componentRegistry,
					const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(false);
	}

	void Menu::dismiss(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &overlay = componentRegistry.getComponent<components::Overlay>(
			entityIdentifier);

		if (!overlay.isVisible()) {
			return;
		}
		hide(componentRegistry, entityIdentifier);

		// Copy: the handler may replace itself.
		const auto onDismiss = overlay.getOnDismissHandler();
		if (onDismiss) {
			onDismiss();
		}
	}

	void Menu::apply(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &menu =
			componentRegistry.getComponent<components::Menu>(entityIdentifier);
		const auto &rows	= menu.getRows();
		const auto &items	= menu.getItems();
		const bool elevated = menu.getMode() == Mode::Elevated;

		std::vector<ecs::Entity::Identifier> children;
		children.reserve(rows.size());
		float contentWidth = 0.0f;
		for (const auto &row: rows) {
			children.push_back(row.view);
			contentWidth =
				std::max(contentWidth, contentWidthOf(componentRegistry, row));
		}

		// The width follows the widest row within the M3 range.
		const float width = std::clamp(contentWidth, components::Menu::MinWidth,
									   components::Menu::MaxWidth);

		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Start)
			.setPadding(0.0f)
			.setSpacing(0.0f)
			.setFixedWidth(width)
			.setChildren(children);

		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(getShapeRadius(theme::ShapeRole::ExtraSmall));
		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel(elevated ? theme::ElevationLevel::Level2
										: theme::ElevationLevel::Level0);
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(
				elevated
					? getElevatedSurfaceColor(theme::ElevationLevel::Level2)
					: schemeColor(SchemeColorRole::SurfaceContainer));

		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setModal(false)
			.setDismissOnOutsideClick(true)
			.setDismissOnEscape(true);

		for (std::size_t index = 0; index < rows.size(); ++index) {
			const auto &row	 = rows[index];
			const bool dense = index < items.size() && items[index].dense;

			componentRegistry.getComponent<components::Layout>(row.view)
				.setAxis(components::Layout::Axis::Horizontal)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Start)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(ItemSpacing)
				.setPadding(ItemPadding)
				.setFixedWidth(width)
				.setFixedHeight(dense ? DenseItemHeight : ItemHeight)
				.setChildren(row.content);
			if (componentRegistry.hasComponent<components::Borders>(row.view)) {
				componentRegistry.getComponent<components::Borders>(row.view)
					.setBorderRadius(RowRadius);
			}
			if (index < items.size()) {
				interaction::setEnabled(componentRegistry, row.view,
										!items[index].disabled);
			}
			styleRow(componentRegistry, entityIdentifier, index);
		}

		// The rows are sized first, so the menu knows its height before it
		// is anchored; everything is then placed at the anchored pose.
		const auto layer = Layout::layerOf(componentRegistry, entityIdentifier);
		arrangeRows(componentRegistry, rows);
		Layout::apply(componentRegistry, entityIdentifier, children, layer);
		applyAnchor(componentRegistry, entityIdentifier, menu);
		Layout::apply(componentRegistry, entityIdentifier, children, layer);
		arrangeRows(componentRegistry, rows);
	}

	void Menu::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Menu>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
