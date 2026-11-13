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

#include "guillaume/systems/badge.hpp"

#include <algorithm>
#include <cstdint>

#include "guillaume/components/animation.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/typography.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/typography.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Badge::Variant;
		using style::schemeColor;

		/**
		 * @brief Scale the alpha of a color by the fade opacity.
		 * @param color The opaque color.
		 * @param opacity The fade opacity, in [0, 1].
		 * @return The faded color.
		 */
		utility::graphic::Color32Bit
			faded(const utility::graphic::Color32Bit &color, float opacity)
		{
			return style::withAlpha(
				color,
				static_cast<std::uint8_t>(
					static_cast<float>(color.getAlpha()) * opacity + 0.5f));
		}

		/**
		 * @brief Write the content, font size and color of the label child.
		 * @param registry The registry holding the components.
		 * @param label The label child (may be invalid).
		 * @param content The label content.
		 * @param fontSize The label font size, in dp.
		 * @param color The label color.
		 */
		void styleLabel(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier label,
						const std::string &content, float fontSize,
						const utility::graphic::Color32Bit &color)
		{
			if (registry.hasComponent<components::Text>(label)) {
				registry.getComponent<components::Text>(label).setContent(
					content);
				if (registry.hasComponent<components::Typography>(label)) {
					registry.getComponent<components::Typography>(label)
						.setFontSize(fontSize);
					Typography::apply(registry, label);
				}
			}
			if (registry.hasComponent<components::Color>(label)) {
				registry.getComponent<components::Color>(label).setColor(color);
			}
		}
	}	 // namespace

	Badge::Badge(void)
		: ecs::SystemFiller<components::Badge>(ecs::Phase::Layout)
	{
	}

	void Badge::install(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Animation>(identifier)
			.setDuration(getMotionDuration(theme::MotionDuration::Short3))
			.setEasing(theme::MotionScale::standard())
			.setOnSampleHandler([registry, identifier](float progress) {
				fade(*registry, identifier, progress);
			});
	}

	float Badge::sizeOf(const ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &badge =
			componentRegistry.getComponent<components::Badge>(entityIdentifier);

		if (badge.getSize().has_value()) {
			return *badge.getSize();
		}
		switch (badge.getVariant()) {
			case Variant::Dot:
				return components::Badge::DotSize;
			case Variant::Small:
				return components::Badge::SmallSize;
			case Variant::Large:
			default:
				return components::Badge::LargeSize;
		}
	}

	bool Badge::needsLabel(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &badge =
			componentRegistry.getComponent<components::Badge>(entityIdentifier);

		return badge.getVariant() != Variant::Dot && !badge.getLabel().empty();
	}

	void Badge::setVisible(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   bool visible)
	{
		auto &badge =
			componentRegistry.getComponent<components::Badge>(entityIdentifier);

		if (badge.isVisible() == visible) {
			return;
		}
		badge.setVisible(visible).setFadeFrom(badge.getOpacity());
		componentRegistry.getComponent<components::Animation>(entityIdentifier)
			.play();
	}

	void Badge::fade(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier,
					 float progress)
	{
		auto &badge =
			componentRegistry.getComponent<components::Badge>(entityIdentifier);
		const float target = badge.isVisible() ? 1.0f : 0.0f;
		const float from   = badge.getFadeFrom();

		badge.setOpacity(from + (target - from) * progress);
		apply(componentRegistry, entityIdentifier);
	}

	void Badge::apply(ecs::ComponentRegistry &componentRegistry,
					  const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &badge =
			componentRegistry.getComponent<components::Badge>(entityIdentifier);
		const float size	= sizeOf(componentRegistry, entityIdentifier);
		const float opacity = badge.getOpacity();
		const auto label	= badge.getLabelEntity();
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);

		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(getShapeRadius(theme::ShapeRole::Full));

		// A label wider than the badge grows it into a pill.
		float width = size;
		if (badge.getVariant() != Variant::Dot
			&& componentRegistry.hasComponent<components::Bound>(label)) {
			width = std::max(
				size,
				componentRegistry.getComponent<components::Bound>(label)
						.getWidth()
					+ 2.0f * components::Badge::HorizontalPadding);
		}
		layout.setPadding(0.0f)
			.setSpacing(0.0f)
			.setFixedWidth(width)
			.setFixedHeight(size);

		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(faded(badge.getContainerColor().value_or(
								schemeColor(SchemeColorRole::Error)),
							opacity));

		styleLabel(componentRegistry, label, badge.getLabel(),
				   size * components::Badge::LabelScale,
				   faded(badge.getContentColor().value_or(
							 schemeColor(SchemeColorRole::OnError)),
						 opacity));

		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void Badge::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Badge>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
