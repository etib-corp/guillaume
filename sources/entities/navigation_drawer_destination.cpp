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

#include "guillaume/entities/navigation_drawer_destination.hpp"

#include "guillaume/entities/navigation_drawer.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities::detail
{
	namespace
	{
		/**
		 * @brief M3 drawer destination height.
		 */
		constexpr float DrawerDestinationHeight = 56.0f;

		/**
		 * @brief Collapsed item indicator width.
		 */
		constexpr float CollapsedIndicatorWidth = 56.0f;

		/**
		 * @brief Collapsed item indicator height.
		 */
		constexpr float CollapsedIndicatorHeight = 32.0f;

		/**
		 * @brief Make a transparent nested layout view.
		 */
		std::shared_ptr<View> makeGroup(ecs::ComponentRegistry &registry,
										ecs::EntityRegistry &entityRegistry,
										std::shared_ptr<ecs::Entity> parent,
										components::Layout::Axis axis)
		{
			View::Builder builder(registry, entityRegistry);
			View::Director director;
			auto view = director.makeDefaultView(builder, parent,
												 utility::graphic::PoseF(), {});
			view->setColor(transparentColor());
			view->setAxis(axis);
			view->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::Center);
			view->setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center);
			view->setPadding(0.0f);
			view->setSpacing(theme::SpacingScale::get(3));
			return view;
		}

	}	 // namespace

	DrawerDestination::DrawerDestination(ecs::ComponentRegistry &registry,
										 const NavigationDrawer::Entry &entry,
										 bool collapsed, float width)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders, components::Selectable>(
			  registry)
		, _entry(entry)
		, _collapsed(collapsed)
	{
		auto &layout = getComponentRegistry().getComponent<components::Layout>(
			getIdentifier());
		if (_collapsed) {
			layout.setAxis(components::Layout::Axis::Vertical)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Center)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(theme::SpacingScale::get(1))
				.setPadding(0.0f)
				.setFixedWidth(width)
				.setFixedHeight(DrawerDestinationHeight
								+ theme::SpacingScale::get(2));
		} else {
			layout.setAxis(components::Layout::Axis::Horizontal)
				.setMainAxisAlignment(
					entry.right.empty()
						? components::Layout::MainAxisAlignment::Start
						: components::Layout::MainAxisAlignment::SpaceBetween)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(theme::SpacingScale::get(3))
				.setPadding(theme::SpacingScale::get(4))
				.setFixedWidth(width)
				.setFixedHeight(DrawerDestinationHeight);
		}

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(transparentColor());

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getShapeRadius(theme::ShapeRole::Full))
			.setColor(transparentColor());

		makeInteractive(
			getComponentRegistry(), getIdentifier(),
			components::AccessibilityRole::Button,
			[this]() {
				if (_onPress) {
					_onPress();
				}
			},
			[this]() {
				applySelectionColor(isSelected());
			});
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(_entry.label);
		setEnabled(getComponentRegistry(), getIdentifier(), !_entry.disabled);
	}

	void DrawerDestination::initialize(void)
	{
		if (_collapsed) {
			_lead = makeGroup(getComponentRegistry(), *this, shared_from_this(),
							  components::Layout::Axis::Horizontal);
			_lead->setBorderRadius(getShapeRadius(theme::ShapeRole::Full));
			_lead->setFixedWidth(CollapsedIndicatorWidth);
			_lead->setFixedHeight(CollapsedIndicatorHeight);
			_icon = buildIcon(getComponentRegistry(), *_lead, _lead,
							  _entry.icon, 24.0f,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (_icon != nullptr) {
				_lead->setEntities({ _icon });
			}
			if (!_entry.label.empty()) {
				_labelEntity =
					buildText(getComponentRegistry(), *this, shared_from_this(),
							  _entry.label, theme::TypeRole::LabelMedium,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
			}
		} else {
			_lead = makeGroup(getComponentRegistry(), *this, shared_from_this(),
							  components::Layout::Axis::Horizontal);
			std::vector<std::shared_ptr<ecs::Entity>> lead;
			_icon = buildIcon(getComponentRegistry(), *_lead, _lead,
							  _entry.icon, 24.0f,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (_icon != nullptr) {
				lead.push_back(_icon);
			}
			if (!_entry.label.empty()) {
				_labelEntity =
					buildText(getComponentRegistry(), *_lead, _lead,
							  _entry.label, theme::TypeRole::LabelLarge,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
				if (_labelEntity != nullptr) {
					lead.push_back(_labelEntity);
				}
			}
			_lead->setEntities(lead);

			if (!_entry.right.empty()) {
				_rightEntity =
					buildText(getComponentRegistry(), *this, shared_from_this(),
							  _entry.right, theme::TypeRole::LabelLarge,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
			}
		}

		applyGeometry();
	}

	void DrawerDestination::setOnPress(std::function<void(void)> onPress)
	{
		_onPress = std::move(onPress);
	}

	void DrawerDestination::setDisabled(bool disabled)
	{
		_entry.disabled = disabled;
		setEnabled(getComponentRegistry(), getIdentifier(), !disabled);
		applySelectionColor(isSelected());
	}

	bool DrawerDestination::isDisabled(void) const
	{
		return !isEnabled(getComponentRegistry(), getIdentifier());
	}

	void DrawerDestination::setOnSelectionChanged(
		const components::Selectable::Handler &handler)
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setOnSelectionChangedHandler(handler);
	}

	bool DrawerDestination::isSelected(void)
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	void DrawerDestination::applySelectionColor(bool selected)
	{
		const bool disabled = isDisabled();
		const auto pill		= selected && !disabled
				? schemeColor(SchemeColorRole::SecondaryContainer)
				: transparentColor();
		auto content		= selected
				   ? schemeColor(SchemeColorRole::OnSecondaryContainer)
				   : schemeColor(SchemeColorRole::OnSurfaceVariant);
		if (disabled) {
			content = disabledContentColor();
		}

		const auto pillColor = stateLayerColor(getComponentRegistry(),
											   getIdentifier(), pill, content);
		if (_collapsed) {
			if (_lead != nullptr) {
				_lead->setColor(pillColor);
			}
		} else {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(pillColor);
		}

		if (_icon != nullptr) {
			_icon->setColor(content);
		}
		if (_labelEntity != nullptr) {
			_labelEntity->setColor(content);
		}
		if (_rightEntity != nullptr) {
			_rightEntity->setColor(content);
		}

		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setSelected(selected);
	}

	void DrawerDestination::applyGeometry(void)
	{
		std::vector<ecs::Entity::Identifier> children;

		if (_lead != nullptr) {
			children.push_back(_lead->getIdentifier());
		}

		if (_collapsed && _labelEntity != nullptr) {
			children.push_back(_labelEntity->getIdentifier());
		}

		if (_rightEntity != nullptr) {
			children.push_back(_rightEntity->getIdentifier());
		}

		systems::Layout::apply(getComponentRegistry(), getIdentifier(),
							   children,
							   static_cast<std::uint32_t>(getLayer()));
	}

	ecs::Entity::Identifier DrawerDestination::getRightIdentifier(void) const
	{
		return _rightEntity != nullptr ? _rightEntity->getIdentifier()
									   : ecs::Entity::InvalidIdentifier;
	}
}	 // namespace guillaume::entities::detail
