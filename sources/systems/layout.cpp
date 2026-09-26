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

		std::vector<ecs::Entity::Identifier> childIdentifiers;
		std::uint32_t layer = 0;

		for (const auto &entity:
			 getEntityRegistry().getEntitiesBreadthFirst()) {
			const auto parent = entity->getParent();
			if (parent == nullptr
				|| parent->getIdentifier() != entityIdentifier) {
				continue;
			}
			layer = static_cast<std::uint32_t>(parent->getLayer());
			childIdentifiers.push_back(entity->getIdentifier());
		}

		apply(getComponentRegistry(), entityIdentifier, childIdentifiers,
			  layer);
	}

	const utility::graphic::PoseF Layout::applyLayerToPosition(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation,
		const std::uint32_t &layer)
	{
		const auto forwardVector					= orientation.getForward();
		utility::graphic::PositionF forwardPosition = position;

		forwardPosition.translate(utility::graphic::PositionF(
			-forwardVector * static_cast<float>(layer + 1) * LayerDepthStep));

		return utility::graphic::PoseF(forwardPosition, orientation);
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

		std::vector<LayoutChild> children;
		children.reserve(childIdentifiers.size());

		for (const auto &childIdentifier: childIdentifiers) {
			if (childIdentifier == ecs::Entity::InvalidIdentifier
				|| !registry.hasComponent<components::Transform>(
					childIdentifier)
				|| !registry.hasComponent<components::Bound>(childIdentifier)) {
				continue;
			}

			const auto &childBound =
				registry.getComponent<components::Bound>(childIdentifier);
			children.push_back(LayoutChild {
				.identifier = childIdentifier,
				.width		= childBound.getWidth(),
				.height		= childBound.getHeight(),
			});
		}

		float rawMain  = 0.0f;
		float rawCross = 0.0f;

		for (const auto &child: children) {
			rawMain += horizontal ? child.width : child.height;
			rawCross =
				std::max(rawCross, horizontal ? child.height : child.width);
		}

		const float spacing = layout.getSpacing();
		const float padding = layout.getPadding();

		const float gapsTotal	= children.size() > 1
			? spacing * static_cast<float>(children.size() - 1)
			: 0.0f;
		const float mainContent = rawMain + gapsTotal;

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
				childY = originY + crossPosition;
			} else {
				childX = originX + crossPosition;
				childY = originY + cursor;
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

		const float width  = fixedWidth
			? layout.getFixedWidth()
			: (horizontal ? mainContent : rawCross) + 2.0f * padding;
		const float height = fixedHeight
			? layout.getFixedHeight()
			: (horizontal ? rawCross : mainContent) + 2.0f * padding;

		parentBound.setWidth(width).setHeight(height);
	}

}	 // namespace guillaume::systems
