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

#include <memory>
#include <utility>

#include "guillaume/entities/badge.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/systems/navigation_bar_destination.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities::detail
{
	BarDestination::BarDestination(ecs::ComponentRegistry &registry,
								   const NavigationBar::Destination &spec)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders, components::Selectable,
								  components::NavigationBarDestination>(
			  registry)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.setIcon(spec.icon)
			.setFocusedIcon(spec.focusedIcon)
			.setLabel(spec.label)
			.setBadge(spec.badge);
		systems::NavigationBarDestination::install(getComponentRegistry(),
												   getIdentifier());
		systems::NavigationBarDestination::apply(getComponentRegistry(),
												 getIdentifier());
	}

	void BarDestination::initialize(void)
	{
		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const auto spec =
			registry.getComponent<components::NavigationBarDestination>(
				getIdentifier());

		if (spec.getIndicator() == ecs::Entity::InvalidIdentifier) {
			View::Builder pillBuilder(registry, *this);
			const auto pill = pillBuilder.makeDefaultView(
				shared_from_this(), utility::graphic::PoseF(), {});
			pill->setColor(transparentColor());
			pill->setBorderRadius(getShapeRadius(theme::ShapeRole::Full));
			pill->setAxis(components::Layout::Axis::Horizontal);
			pill->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::Center);
			pill->setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center);
			pill->setPadding(0.0f);
			pill->setFixedWidth(
				components::NavigationBarDestination::IndicatorWidth);
			pill->setFixedHeight(
				components::NavigationBarDestination::IndicatorHeight);

			const auto icon =
				buildIcon(registry, *pill, pill, spec.getIcon(),
						  components::NavigationBarDestination::IconSize,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (icon != nullptr) {
				pill->setEntities({ icon });
			}

			const auto label =
				buildText(registry, *this, shared_from_this(), spec.getLabel(),
						  theme::TypeRole::LabelMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));

			registry
				.getComponent<components::NavigationBarDestination>(
					getIdentifier())
				.setIndicator(pill->getIdentifier())
				.setIconEntity(icon != nullptr ? icon->getIdentifier()
											   : ecs::Entity::InvalidIdentifier)
				.setLabelEntity(label != nullptr
									? label->getIdentifier()
									: ecs::Entity::InvalidIdentifier);
		}

		buildBadge();
		systems::NavigationBarDestination::apply(registry, getIdentifier());
	}

	void BarDestination::buildBadge(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const auto spec =
			registry.getComponent<components::NavigationBarDestination>(
				getIdentifier());

		if (spec.getBadgeEntity() != ecs::Entity::InvalidIdentifier
			|| spec.getBadge().empty()) {
			return;
		}

		const auto pill = getEntity<View>(spec.getIndicator());
		if (pill == nullptr) {
			return;
		}

		Badge::Builder builder(registry, *pill);
		const auto badge = builder.withVariant(Badge::Variant::Large)
							   .withLabel(spec.getBadge())
							   .registerEntity(pill);
		if (badge != nullptr) {
			registry
				.getComponent<components::NavigationBarDestination>(
					getIdentifier())
				.setBadgeEntity(badge->getIdentifier());
		}
	}

	void BarDestination::setOnPress(std::function<void(void)> onPress)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.setOnPress(std::move(onPress));
	}

	void BarDestination::setBadge(const std::string &badge)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.setBadge(badge);
		// A first badge label needs the badge child.
		buildBadge();
		systems::NavigationBarDestination::apply(getComponentRegistry(),
												 getIdentifier());
	}

	void BarDestination::setShowLabel(bool show)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.setShowLabel(show);
		systems::NavigationBarDestination::apply(getComponentRegistry(),
												 getIdentifier());
	}

	void BarDestination::setOnSelectionChanged(
		const components::Selectable::Handler &handler)
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setOnSelectionChangedHandler(handler);
	}

	bool BarDestination::isSelected(void) const
	{
		return systems::NavigationBarDestination::isSelected(
			getComponentRegistry(), getIdentifier());
	}

	ecs::Entity::Identifier BarDestination::getIndicatorIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.getIndicator();
	}

	ecs::Entity::Identifier BarDestination::getIconIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.getIconEntity();
	}

	ecs::Entity::Identifier BarDestination::getLabelIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.getLabelEntity();
	}

	ecs::Entity::Identifier BarDestination::getBadgeIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationBarDestination>(getIdentifier())
			.getBadgeEntity();
	}
}	 // namespace guillaume::entities::detail
