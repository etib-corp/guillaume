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

#include <memory>
#include <utility>
#include <vector>

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/systems/navigation_drawer_destination.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities::detail
{
	namespace
	{
		/**
		 * @brief Make a transparent nested layout view.
		 * @param registry The component registry.
		 * @param entityRegistry The entity registry receiving the view.
		 * @param parent The parent entity.
		 * @param axis The arrangement axis.
		 * @return The view.
		 */
		std::shared_ptr<View> makeGroup(ecs::ComponentRegistry &registry,
										ecs::EntityRegistry &entityRegistry,
										std::shared_ptr<ecs::Entity> parent,
										components::Layout::Axis axis)
		{
			View::Builder builder(registry, entityRegistry);
			auto view =
				builder.makeDefaultView(parent, utility::graphic::PoseF(), {});
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

		/**
		 * @brief Get the identifier of an optional child.
		 * @param child The child (may be null).
		 * @return Its identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier
			identifierOf(const std::shared_ptr<ecs::Entity> &child)
		{
			return child != nullptr ? child->getIdentifier()
									: ecs::Entity::InvalidIdentifier;
		}
	}	 // namespace

	DrawerDestination::DrawerDestination(ecs::ComponentRegistry &registry,
										 const NavigationDrawer::Entry &entry,
										 bool collapsed, float width)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders, components::Selectable,
								  components::NavigationDrawerDestination>(
			  registry)
	{
		getComponentRegistry()
			.getComponent<components::NavigationDrawerDestination>(
				getIdentifier())
			.setIcon(entry.icon)
			.setLabel(entry.label)
			.setRight(entry.right)
			.setDisabled(entry.disabled)
			.setCollapsed(collapsed);
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setFixedWidth(width);
		systems::NavigationDrawerDestination::install(getComponentRegistry(),
													  getIdentifier());
		systems::NavigationDrawerDestination::apply(getComponentRegistry(),
													getIdentifier());
	}

	void DrawerDestination::initialize(void)
	{
		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const auto entry =
			registry.getComponent<components::NavigationDrawerDestination>(
				getIdentifier());

		if (entry.getLead() == ecs::Entity::InvalidIdentifier) {
			const auto lead = makeGroup(registry, *this, shared_from_this(),
										components::Layout::Axis::Horizontal);
			const auto icon =
				buildIcon(registry, *lead, lead, entry.getIcon(),
						  components::NavigationDrawerDestination::IconSize,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			std::shared_ptr<Text> label;
			std::shared_ptr<Text> right;
			std::vector<std::shared_ptr<ecs::Entity>> content;

			if (icon != nullptr) {
				content.push_back(icon);
			}

			if (entry.isCollapsed()) {
				// The lead group is the indicator around the icon; the
				// label sits under it.
				lead->setBorderRadius(getShapeRadius(theme::ShapeRole::Full));
				lead->setFixedWidth(components::NavigationDrawerDestination::
										CollapsedIndicatorWidth);
				lead->setFixedHeight(components::NavigationDrawerDestination::
										 CollapsedIndicatorHeight);
				label =
					buildText(registry, *this, shared_from_this(),
							  entry.getLabel(), theme::TypeRole::LabelMedium,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
			} else {
				// The lead group holds the icon and the label; the trailing
				// text sits at the far edge of the row.
				label =
					buildText(registry, *lead, lead, entry.getLabel(),
							  theme::TypeRole::LabelLarge,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
				if (label != nullptr) {
					content.push_back(label);
				}
				right =
					buildText(registry, *this, shared_from_this(),
							  entry.getRight(), theme::TypeRole::LabelLarge,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
			}
			if (!content.empty() || !entry.isCollapsed()) {
				lead->setEntities(content);
			}

			registry
				.getComponent<components::NavigationDrawerDestination>(
					getIdentifier())
				.setLead(lead->getIdentifier())
				.setIconEntity(identifierOf(icon))
				.setLabelEntity(identifierOf(label))
				.setRightEntity(identifierOf(right));
		}

		systems::NavigationDrawerDestination::apply(registry, getIdentifier());
	}

	void DrawerDestination::setOnPress(std::function<void(void)> onPress)
	{
		getComponentRegistry()
			.getComponent<components::NavigationDrawerDestination>(
				getIdentifier())
			.setOnPress(std::move(onPress));
	}

	void DrawerDestination::setDisabled(bool disabled)
	{
		systems::NavigationDrawerDestination::setDisabled(
			getComponentRegistry(), getIdentifier(), disabled);
	}

	bool DrawerDestination::isDisabled(void) const
	{
		return systems::NavigationDrawerDestination::isDisabled(
			getComponentRegistry(), getIdentifier());
	}

	void DrawerDestination::setOnSelectionChanged(
		const components::Selectable::Handler &handler)
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setOnSelectionChangedHandler(handler);
	}

	bool DrawerDestination::isSelected(void) const
	{
		return systems::NavigationDrawerDestination::isSelected(
			getComponentRegistry(), getIdentifier());
	}

	ecs::Entity::Identifier DrawerDestination::getRightIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationDrawerDestination>(
				getIdentifier())
			.getRightEntity();
	}
}	 // namespace guillaume::entities::detail
