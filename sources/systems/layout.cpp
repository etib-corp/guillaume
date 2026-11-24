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

#include "guillaume/systems/layout.hpp"

#include <algorithm>
#include <cstdint>
#include <utility>

#include "guillaume/components/hierarchy.hpp"
#include "guillaume/components/layout_extent.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::systems
{
	namespace
	{
		/**
		 * @brief A child to arrange, with its measured size.
		 */
		struct LayoutChild {
			ecs::Entity::Identifier identifier;
			float width;
			float height;
		};

		/**
		 * @brief Compute the cross axis offset of one child.
		 * @param alignment The cross axis alignment.
		 * @param padding The container padding.
		 * @param crossRegion The usable cross axis region (inside padding).
		 * @param childCross The child cross axis size.
		 * @return The offset of the child along the cross axis.
		 */
		float crossOffset(components::Layout::CrossAxisAlignment alignment,
						  float padding, float crossRegion, float childCross)
		{
			switch (alignment) {
				case components::Layout::CrossAxisAlignment::Start:
					return padding;
				case components::Layout::CrossAxisAlignment::End:
					return padding + crossRegion - childCross;
				case components::Layout::CrossAxisAlignment::Center:
				default:
					return padding + (crossRegion - childCross) / 2.0f;
			}
		}

		/**
		 * @brief Collect the children to arrange, with their measured size.
		 *
		 * Invalid identifiers and children without Transform or Bound are
		 * skipped.
		 */
		std::vector<LayoutChild>
			collectChildren(const ecs::ComponentRegistry &registry,
							const std::vector<ecs::Entity::Identifier> &ids)
		{
			std::vector<LayoutChild> children;
			children.reserve(ids.size());

			for (const auto &childIdentifier: ids) {
				if (childIdentifier == ecs::Entity::InvalidIdentifier
					|| !registry.hasComponent<components::Transform>(
						childIdentifier)
					|| !registry.hasComponent<components::Bound>(
						childIdentifier)) {
					continue;
				}

				const auto &childBound =
					registry.getComponent<components::Bound>(childIdentifier);
				// A child may reserve room beyond what it draws (a checkbox
				// and its label).
				const float trailing =
					registry.hasComponent<components::LayoutExtent>(
						childIdentifier)
					? registry
						  .getComponent<components::LayoutExtent>(
							  childIdentifier)
						  .getTrailingWidth()
					: 0.0f;
				children.push_back(LayoutChild {
					.identifier = childIdentifier,
					.width		= childBound.getWidth() + trailing,
					.height		= childBound.getHeight(),
				});
			}
			return children;
		}

		/**
		 * @brief Sizes of the content of a container along its axes.
		 */
		struct ContentMetrics {
			float rawMain;		  ///< Sum of the children along the axis
			float rawCross;		  ///< Largest child across the axis
			float mainContent;	  ///< rawMain plus the gaps
			float width;		  ///< Resolved width of the container
			float height;		  ///< Resolved height of the container
		};

		/**
		 * @brief Measure the content of a container and resolve its size:
		 * the fixed size when one is set, otherwise the content plus twice
		 * the padding.
		 */
		ContentMetrics measureContent(const components::Layout &layout,
									  const std::vector<LayoutChild> &children)
		{
			const bool horizontal =
				layout.getAxis() == components::Layout::Axis::Horizontal;

			float rawMain  = 0.0f;
			float rawCross = 0.0f;
			for (const auto &child: children) {
				rawMain += horizontal ? child.width : child.height;
				rawCross =
					std::max(rawCross, horizontal ? child.height : child.width);
			}

			const float spacing		= layout.getSpacing();
			const float padding		= layout.getPadding();
			const float gapsTotal	= children.size() > 1
				  ? spacing * static_cast<float>(children.size() - 1)
				  : 0.0f;
			const float mainContent = rawMain + gapsTotal;

			const float width  = layout.hasFixedWidth()
				 ? layout.getFixedWidth()
				 : (horizontal ? mainContent : rawCross) + 2.0f * padding;
			const float height = layout.hasFixedHeight()
				? layout.getFixedHeight()
				: (horizontal ? rawCross : mainContent) + 2.0f * padding;

			return ContentMetrics { rawMain, rawCross, mainContent, width,
									height };
		}

		/**
		 * @brief Deepest nesting resolveNestedSizes() follows. It only
		 * guards against a container listing one of its ancestors.
		 */
		constexpr int MaxNestingDepth = 64;

		/**
		 * @brief Size the nested containers among @p ids, deepest first.
		 *
		 * The Layout phase visits parents before children, so without this
		 * a container would read the Bound its nested containers computed
		 * on the previous frame. Only sizes are written here; each nested
		 * container places its own children when the phase reaches it.
		 */
		void resolveNestedSizes(ecs::ComponentRegistry &registry,
								const std::vector<ecs::Entity::Identifier> &ids,
								int depth)
		{
			if (depth >= MaxNestingDepth) {
				return;
			}
			for (const auto &identifier: ids) {
				if (identifier == ecs::Entity::InvalidIdentifier
					|| !registry.hasComponent<components::Layout>(identifier)
					|| !registry.hasComponent<components::Transform>(identifier)
					|| !registry.hasComponent<components::Bound>(identifier)) {
					continue;
				}
				const auto &layout =
					registry.getComponent<components::Layout>(identifier);
				resolveNestedSizes(registry, layout.getChildren(), depth + 1);
				const auto metrics = measureContent(
					layout, collectChildren(registry, layout.getChildren()));
				registry.getComponent<components::Bound>(identifier)
					.setWidth(metrics.width)
					.setHeight(metrics.height);
			}
		}

		/**
		 * @brief Whether the parent of an entity arranges it as a layout
		 * child, and so has already sized it and its nested containers in
		 * this frame.
		 */
		bool isArrangedByParent(const ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier)
		{
			if (!registry.hasComponent<components::Hierarchy>(identifier)) {
				return false;
			}
			const auto parent =
				registry.getComponent<components::Hierarchy>(identifier)
					.getParentIdentifier();
			if (parent == ecs::Entity::InvalidIdentifier
				|| !registry.hasComponent<components::Layout>(parent)
				|| !registry.hasComponent<components::Transform>(parent)
				|| !registry.hasComponent<components::Bound>(parent)) {
				return false;
			}
			const auto &siblings =
				registry.getComponent<components::Layout>(parent).getChildren();
			return std::find(siblings.begin(), siblings.end(), identifier)
				!= siblings.end();
		}
	}	 // namespace

	Layout::Layout(void)
		: ecs::SystemFiller<components::Layout, components::Transform,
							components::Bound>(ecs::Phase::Layout)
	{
	}

	void Layout::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Layout>(entityIdentifier)) {
			return;
		}

		const auto &layout = getComponent<components::Layout>(entityIdentifier);

		// Size the nested containers first, so a change deep in the tree
		// reaches this container in the same frame. A container arranged by
		// its parent was already handled when the parent was visited.
		if (!isArrangedByParent(getComponentRegistry(), entityIdentifier)) {
			resolveNestedSizes(getComponentRegistry(), layout.getChildren(), 0);
		}

		// Only the container's registered layout children are arranged; any
		// child the container positions manually (a switch thumb, a slider
		// fill, ...) is deliberately absent from the list and keeps its pose.
		apply(getComponentRegistry(), entityIdentifier, layout.getChildren(),
			  layerOf(getComponentRegistry(), entityIdentifier));
	}

	std::uint32_t Layout::layerOf(const ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier)
	{
		// The layer comes from the Hierarchy component, so systems need no
		// entity object.
		if (!registry.hasComponent<components::Hierarchy>(identifier)) {
			return 0;
		}
		return static_cast<std::uint32_t>(
			registry.getComponent<components::Hierarchy>(identifier)
				.getLayer());
	}

	const utility::graphic::PoseF Layout::applyLayerToPosition(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation,
		const std::uint32_t &layer)
	{
		const auto forwardVector					= orientation.getForward();
		utility::graphic::PositionF forwardPosition = position;

		forwardPosition.translate(utility::graphic::PositionF(
			-forwardVector * static_cast<float>(layer + ContentLayerOffset)
			* LayerDepthStep));

		return utility::graphic::PoseF(forwardPosition, orientation);
	}

	const utility::graphic::PoseF Layout::contentLayerPose(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation,
		const std::uint32_t &containerLayer)
	{
		return applyLayerToPosition(position, orientation, containerLayer);
	}

	void Layout::apply(
		ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &parentIdentifier,
		const std::vector<ecs::Entity::Identifier> &childIdentifiers,
		std::uint32_t layer)
	{
		const auto &layout =
			registry.getComponent<components::Layout>(parentIdentifier);
		const auto &parentTransform =
			registry.getComponent<components::Transform>(parentIdentifier);
		auto &parentBound =
			registry.getComponent<components::Bound>(parentIdentifier);

		const auto parentPose  = parentTransform.getPose();
		const auto orientation = parentPose.getOrientation();
		const float originX	   = parentPose.getPosition().getX();
		const float originY	   = parentPose.getPosition().getY();
		const float originZ	   = parentPose.getPosition().getZ();

		const bool horizontal =
			layout.getAxis() == components::Layout::Axis::Horizontal;

		const auto children		= collectChildren(registry, childIdentifiers);
		const auto metrics		= measureContent(layout, children);
		const float rawMain		= metrics.rawMain;
		const float mainContent = metrics.mainContent;
		const float rawCross	= metrics.rawCross;

		const float spacing = layout.getSpacing();
		const float padding = layout.getPadding();

		const bool fixedWidth  = layout.hasFixedWidth();
		const bool fixedHeight = layout.hasFixedHeight();

		const bool hasFixedMain = horizontal ? fixedWidth : fixedHeight;
		const float fixedMain =
			horizontal ? layout.getFixedWidth() : layout.getFixedHeight();
		const float availableMain =
			hasFixedMain ? fixedMain : mainContent + 2.0f * padding;

		const bool hasFixedCross = horizontal ? fixedHeight : fixedWidth;
		const float fixedCross =
			horizontal ? layout.getFixedHeight() : layout.getFixedWidth();
		const float availableCross =
			hasFixedCross ? fixedCross : rawCross + 2.0f * padding;

		const float mainRegion	= availableMain - 2.0f * padding;
		const float crossRegion = availableCross - 2.0f * padding;

		float cursor = padding;
		float gap	 = spacing;

		switch (layout.getMainAxisAlignment()) {
			case components::Layout::MainAxisAlignment::Center:
				cursor = padding + (mainRegion - mainContent) / 2.0f;
				break;
			case components::Layout::MainAxisAlignment::End:
				cursor = padding + (mainRegion - mainContent);
				break;
			case components::Layout::MainAxisAlignment::SpaceBetween:
				cursor = padding;
				gap	   = children.size() > 1 ? (mainRegion - rawMain)
						   / static_cast<float>(children.size() - 1)
											 : 0.0f;
				break;
			case components::Layout::MainAxisAlignment::Start:
			default:
				cursor = padding;
				break;
		}

		for (const auto &child: children) {
			const float childMain  = horizontal ? child.width : child.height;
			const float childCross = horizontal ? child.height : child.width;

			const float crossPosition =
				crossOffset(layout.getCrossAxisAlignment(), padding,
							crossRegion, childCross);

			float childX = originX;
			float childY = originY;

			if (horizontal) {
				childX = originX + cursor;
				childY = originY - crossPosition;
			} else {
				childX = originX + crossPosition;
				childY = originY - cursor;
			}

			auto &childTransform =
				registry.getComponent<components::Transform>(child.identifier);
			auto childPose = childTransform.getPose();

			const utility::graphic::PositionF childBasePosition(childX, childY,
																originZ);
			const auto layeredPose =
				applyLayerToPosition(childBasePosition, orientation, layer);

			childPose.setPosition(layeredPose.getPosition());
			childTransform.setPose(childPose);

			cursor += childMain + gap;
		}

		parentBound.setWidth(metrics.width).setHeight(metrics.height);
	}

}	 // namespace guillaume::systems
