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

#include "guillaume/systems/tooltip.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/animation.hpp"
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
#include "guillaume/motion/easing.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Tooltip::Variant;
		using style::schemeColor;
		using style::transparentColor;

		/// Height of a plain tooltip, in dp.
		constexpr float PlainHeight = 32.0f;

		/// Height of a rich tooltip, in dp.
		constexpr float RichHeight = 64.0f;

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
		 * @brief Arrange the layout children of the tooltip.
		 * @param registry The registry holding the components.
		 * @param identifier The tooltip entity.
		 */
		void arrange(ecs::ComponentRegistry &registry,
					 const ecs::Entity::Identifier &identifier)
		{
			Layout::apply(registry, identifier,
						  registry.getComponent<components::Layout>(identifier)
							  .getChildren(),
						  Layout::layerOf(registry, identifier));
		}
	}	 // namespace

	Tooltip::Tooltip(void)
		: ecs::SystemFiller<components::Tooltip>(ecs::Phase::Layout)
	{
	}

	void Tooltip::install(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Accessibility>(identifier)
			.setRole(components::AccessibilityRole::Text)
			.setLiveRegion(components::LiveRegion::Polite);

		// The animation is a one-second looping ticker: the progress it
		// reports is turned back into the elapsed time.
		componentRegistry.getComponent<components::Animation>(identifier)
			.setDuration(1.0f)
			.setEasing(motion::Easing::Linear)
			.setLooping(true)
			.setOnSampleHandler([registry, identifier](float progress) {
				auto &tooltip =
					registry->getComponent<components::Tooltip>(identifier);
				float delta = progress - tooltip.getLastProgress();
				if (delta < 0.0f) {
					delta += 1.0f;
				}
				tooltip.setLastProgress(progress);
				advance(*registry, identifier, delta);
			});

		componentRegistry.getComponent<components::Overlay>(identifier)
			.setOnDismissHandler([registry, identifier]() {
				hide(*registry, identifier);
			});
	}

	bool Tooltip::hasAnchor(const ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto anchor =
			componentRegistry
				.getComponent<components::Tooltip>(entityIdentifier)
				.getAnchor();

		return anchor != ecs::Entity::InvalidIdentifier
			&& componentRegistry.hasComponent<components::Transform>(anchor);
	}

	void Tooltip::setAnchor(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							ecs::Entity::Identifier anchorIdentifier)
	{
		componentRegistry.getComponent<components::Tooltip>(entityIdentifier)
			.setAnchor(anchorIdentifier)
			.setHoverTime(0.0f)
			.setLeaveTime(0.0f)
			.setLastProgress(0.0f);

		auto &ticker = componentRegistry.getComponent<components::Animation>(
			entityIdentifier);
		if (anchorIdentifier != ecs::Entity::InvalidIdentifier) {
			ticker.play();
		} else {
			ticker.stop();
		}
	}

	void Tooltip::setViewport(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  float width, float height)
	{
		auto &tooltip = componentRegistry.getComponent<components::Tooltip>(
			entityIdentifier);

		tooltip.setViewport(std::make_pair(width, height));
		if (tooltip.isVisible()) {
			place(componentRegistry, entityIdentifier);
		}
	}

	void Tooltip::show(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Tooltip>(entityIdentifier)
			.setVisible(true);
		place(componentRegistry, entityIdentifier);
		apply(componentRegistry, entityIdentifier);
	}

	void Tooltip::hide(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Tooltip>(entityIdentifier)
			.setVisible(false);
		apply(componentRegistry, entityIdentifier);
	}

	void Tooltip::advance(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  float deltaTime)
	{
		if (!hasAnchor(componentRegistry, entityIdentifier)) {
			return;
		}

		auto &tooltip = componentRegistry.getComponent<components::Tooltip>(
			entityIdentifier);

		if (interaction::isHovered(componentRegistry, tooltip.getAnchor())) {
			tooltip.setLeaveTime(0.0f);
			tooltip.setHoverTime(tooltip.getHoverTime() + deltaTime);
			if (!tooltip.isVisible()
				&& tooltip.getHoverTime() >= tooltip.getEnterDelay()) {
				show(componentRegistry, entityIdentifier);
			}
			return;
		}

		tooltip.setHoverTime(0.0f);
		if (tooltip.isVisible()) {
			tooltip.setLeaveTime(tooltip.getLeaveTime() + deltaTime);
			if (tooltip.getLeaveTime() >= tooltip.getLeaveDelay()) {
				tooltip.setLeaveTime(0.0f);
				hide(componentRegistry, entityIdentifier);
			}
		}
	}

	void Tooltip::place(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!hasAnchor(componentRegistry, entityIdentifier)) {
			return;
		}

		const auto &tooltip =
			componentRegistry.getComponent<components::Tooltip>(
				entityIdentifier);
		const auto anchor = tooltip.getAnchor();
		if (!componentRegistry.hasComponent<components::Bound>(anchor)) {
			return;
		}

		const auto anchorPose =
			componentRegistry.getComponent<components::Transform>(anchor)
				.getPose();
		const auto &anchorBound =
			componentRegistry.getComponent<components::Bound>(anchor);
		const auto &bound =
			componentRegistry.getComponent<components::Bound>(entityIdentifier);

		const float width  = bound.getWidth();
		const float height = bound.getHeight();
		float x			   = anchorPose.getPosition().getX()
			+ (anchorBound.getWidth() - width) / 2.0f;
		// Positions are top-left corners and y points up: below the anchor
		// is its top minus its height.
		float y = anchorPose.getPosition().getY() - anchorBound.getHeight();

		if (tooltip.getViewport().has_value()) {
			// The viewport is measured downward from y = 0.
			const float viewportWidth  = tooltip.getViewport()->first;
			const float viewportHeight = tooltip.getViewport()->second;
			float down				   = -y;
			if (down + height > viewportHeight) {
				down = -anchorPose.getPosition().getY() - height;
			}
			x = std::clamp(x, 0.0f, std::max(0.0f, viewportWidth - width));
			down =
				std::clamp(down, 0.0f, std::max(0.0f, viewportHeight - height));
			y = -down;
		}

		componentRegistry.getComponent<components::Transform>(entityIdentifier)
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(x, y,
											anchorPose.getPosition().getZ()),
				anchorPose.getOrientation()));
		arrange(componentRegistry, entityIdentifier);
	}

	void Tooltip::apply(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &tooltip =
			componentRegistry.getComponent<components::Tooltip>(
				entityIdentifier);
		const bool rich	   = (tooltip.getVariant() == Variant::Rich);
		const bool visible = tooltip.isVisible();
		const auto title   = tooltip.getTitleEntity();
		const auto text	   = tooltip.getTextEntity();

		// The title only takes part in the layout of a rich tooltip.
		std::vector<ecs::Entity::Identifier> children;
		if (rich && title != ecs::Entity::InvalidIdentifier) {
			children.push_back(title);
		}
		if (text != ecs::Entity::InvalidIdentifier) {
			children.push_back(text);
		}

		componentRegistry.getComponent<components::Layout>(entityIdentifier)
			.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Center)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Start)
			.setPadding(8.0f)
			.setSpacing(2.0f)
			.setFixedHeight(rich ? RichHeight : PlainHeight)
			.setChildren(children);

		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel(rich && visible ? theme::ElevationLevel::Level2
											   : theme::ElevationLevel::Level0);
		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(
				getShapeRadius(rich ? theme::ShapeRole::Medium
									: theme::ShapeRole::ExtraSmall));
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(visible
						  ? schemeColor(rich ? SchemeColorRole::SurfaceContainer
											 : SchemeColorRole::InverseSurface)
						  : transparentColor());

		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setModal(false)
			.setDismissOnOutsideClick(false)
			.setDismissOnEscape(true)
			.setVisible(visible);

		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(tooltip.getText());

		if (componentRegistry.hasComponent<components::Text>(text)) {
			componentRegistry.getComponent<components::Text>(text).setContent(
				tooltip.getText());
		}
		colorChild(componentRegistry, title,
				   visible ? schemeColor(SchemeColorRole::OnSurface)
						   : transparentColor());
		colorChild(componentRegistry, text,
				   visible
					   ? schemeColor(rich ? SchemeColorRole::OnSurfaceVariant
										  : SchemeColorRole::InverseOnSurface)
					   : transparentColor());

		arrange(componentRegistry, entityIdentifier);
	}

	void Tooltip::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Tooltip>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
