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

#include "guillaume/entities/navigation_bar_destination.hpp"

#include "guillaume/entities/navigation_bar.hpp"

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

#include "guillaume/entities/badge.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
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
		 * @brief M3 active indicator width.
		 */
		constexpr float IndicatorWidth = 64.0f;

		/**
		 * @brief M3 active indicator height.
		 */
		constexpr float IndicatorHeight = 32.0f;

		/**
		 * @brief Horizontal badge offset from the indicator origin, so the
		 * badge sits on the icon's top-right corner.
		 */
		constexpr float BadgeOffsetX = 36.0f;

		/**
		 * @brief Vertical badge offset from the indicator origin.
		 */
		constexpr float BadgeOffsetY = 2.0f;

	}	 // namespace

	BarDestination::BarDestination(ecs::ComponentRegistry &registry,
								   const NavigationBar::Destination &spec)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders, components::Selectable>(
			  registry)
		, _spec(spec)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Center)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(theme::SpacingScale::get(1))
			.setPadding(0.0f);

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(transparentColor());

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getShapeRadius(theme::ShapeRole::None))
			.setColor(transparentColor());

		makeInteractive(
			getComponentRegistry(), getIdentifier(),
			components::AccessibilityRole::Tab,
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
			.setLabel(_spec.label);
	}

	void BarDestination::initialize(void)
	{
		View::Builder pillBuilder(getComponentRegistry(), *this);
		View::Director pillDirector;
		_pill = pillDirector.makeDefaultView(pillBuilder, shared_from_this(),
											 utility::graphic::PoseF(), {});
		_pill->setColor(transparentColor());
		_pill->setBorderRadius(getShapeRadius(theme::ShapeRole::Full));
		_pill->setAxis(components::Layout::Axis::Horizontal);
		_pill->setMainAxisAlignment(
			components::Layout::MainAxisAlignment::Center);
		_pill->setCrossAxisAlignment(
			components::Layout::CrossAxisAlignment::Center);
		_pill->setPadding(0.0f);
		_pill->setFixedWidth(IndicatorWidth);
		_pill->setFixedHeight(IndicatorHeight);

		_icon =
			buildIcon(getComponentRegistry(), *_pill, _pill, _spec.icon, 24.0f,
					  schemeColor(SchemeColorRole::OnSurfaceVariant));
		if (_icon != nullptr) {
			_pill->setEntities({ _icon });
		}

		if (!_spec.label.empty()) {
			_labelEntity =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  _spec.label, theme::TypeRole::LabelMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		setBadge(_spec.badge);
		applyGeometry();
	}

	void BarDestination::setOnPress(std::function<void(void)> onPress)
	{
		_onPress = std::move(onPress);
	}

	void BarDestination::setBadge(const std::string &badge)
	{
		_spec.badge = badge;
		if (_badge == nullptr && !badge.empty() && _pill != nullptr) {
			Badge::Builder builder(getComponentRegistry(), *_pill);
			_badge = builder.withVariant(Badge::Variant::Large)
						 .withLabel(badge)
						 .registerEntity(_pill);
		} else if (_badge != nullptr) {
			_badge->setVariant(badge.empty() ? Badge::Variant::Dot
											 : Badge::Variant::Large);
			_badge->setLabel(badge);
		}
		applyGeometry();
	}

	void BarDestination::setShowLabel(bool show)
	{
		_showLabel = show;
	}

	void BarDestination::setOnSelectionChanged(
		const components::Selectable::Handler &handler)
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setOnSelectionChangedHandler(handler);
	}

	bool BarDestination::isSelected(void)
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	void BarDestination::applySelectionColor(bool selected)
	{
		const auto pillColor  = selected
			 ? schemeColor(SchemeColorRole::SecondaryContainer)
			 : transparentColor();
		const auto iconColor  = selected
			 ? schemeColor(SchemeColorRole::OnSecondaryContainer)
			 : schemeColor(SchemeColorRole::OnSurfaceVariant);
		const auto labelColor = selected
			? schemeColor(SchemeColorRole::OnSurface)
			: schemeColor(SchemeColorRole::OnSurfaceVariant);

		if (_pill != nullptr) {
			_pill->setColor(stateLayerColor(
				getComponentRegistry(), getIdentifier(), pillColor, iconColor));
		}

		if (_icon != nullptr) {
			_icon->setGlyphName(selected && !_spec.focusedIcon.empty()
									? _spec.focusedIcon
									: _spec.icon);
			_icon->setColor(iconColor);
		}

		if (_labelEntity != nullptr) {
			_labelEntity->setColor(_showLabel ? labelColor
											  : transparentColor());
		}

		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setSelected(selected);
	}

	void BarDestination::applyGeometry(void)
	{
		std::vector<ecs::Entity::Identifier> children;

		if (_pill != nullptr) {
			children.push_back(_pill->getIdentifier());
		}

		if (_labelEntity != nullptr) {
			children.push_back(_labelEntity->getIdentifier());
		}

		systems::Layout::apply(getComponentRegistry(), getIdentifier(),
							   children,
							   static_cast<std::uint32_t>(getLayer()));

		if (_badge != nullptr && _pill != nullptr) {
			placeChild(
				getComponentRegistry(), _pill->getIdentifier(),
				_badge->getIdentifier(),
				utility::graphic::PositionF(BadgeOffsetX, BadgeOffsetY, 0.0f),
				static_cast<std::uint32_t>(_pill->getLayer()));
		}
	}

	ecs::Entity::Identifier BarDestination::getIndicatorIdentifier(void) const
	{
		return _pill != nullptr ? _pill->getIdentifier()
								: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier BarDestination::getIconIdentifier(void) const
	{
		return _icon != nullptr ? _icon->getIdentifier()
								: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier BarDestination::getLabelIdentifier(void) const
	{
		return _labelEntity != nullptr ? _labelEntity->getIdentifier()
									   : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier BarDestination::getBadgeIdentifier(void) const
	{
		return _badge != nullptr ? _badge->getIdentifier()
								 : ecs::Entity::InvalidIdentifier;
	}
}	 // namespace guillaume::entities::detail
