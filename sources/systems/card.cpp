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

#include "guillaume/systems/card.hpp"

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/animation.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/image.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Card::Variant;
		using entities::schemeColor;
		using entities::transparentColor;

		/**
		 * @brief Interpolate between two colors, channel by channel.
		 * @param from The color at 0.
		 * @param to The color at 1.
		 * @param t The interpolation factor in [0, 1].
		 * @return The mixed color.
		 */
		utility::graphic::Color32Bit
			lerpColor(const utility::graphic::Color32Bit &from,
					  const utility::graphic::Color32Bit &to, float t)
		{
			const auto mix = [t](std::uint8_t a, std::uint8_t b) {
				return static_cast<std::uint8_t>(
					static_cast<float>(a)
					+ (static_cast<float>(b) - static_cast<float>(a)) * t
					+ 0.5f);
			};
			return utility::graphic::Color32Bit(
				mix(from.getRed(), to.getRed()),
				mix(from.getGreen(), to.getGreen()),
				mix(from.getBlue(), to.getBlue()),
				mix(from.getAlpha(), to.getAlpha()));
		}

		/**
		 * @brief Set the content of a text child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param content The content to write.
		 */
		void writeText(ecs::ComponentRegistry &registry,
					   ecs::Entity::Identifier child,
					   const std::string &content)
		{
			if (registry.hasComponent<components::Text>(child)) {
				registry.getComponent<components::Text>(child).setContent(
					content);
			}
		}

		/**
		 * @brief Record and arrange the children of a composed section,
		 * when the section exists.
		 * @param registry The registry holding the components.
		 * @param section The section identifier (may be invalid).
		 * @param children The children the section lays out.
		 */
		void
			arrangeSection(ecs::ComponentRegistry &registry,
						   ecs::Entity::Identifier section,
						   const std::vector<ecs::Entity::Identifier> &children)
		{
			if (!registry.hasComponent<components::Layout>(section)
				|| !registry.hasComponent<components::Transform>(section)
				|| !registry.hasComponent<components::Bound>(section)) {
				return;
			}
			registry.getComponent<components::Layout>(section).setChildren(
				children);
			Layout::apply(registry, section, children,
						  Layout::layerOf(registry, section));
		}

		/**
		 * @brief Arrange the title and actions sections of a card.
		 * @param registry The registry holding the components.
		 * @param card The card configuration.
		 */
		void arrangeSections(ecs::ComponentRegistry &registry,
							 const components::Card &card)
		{
			const auto invalid = ecs::Entity::InvalidIdentifier;
			std::vector<ecs::Entity::Identifier> texts;
			std::vector<ecs::Entity::Identifier> buttons;

			if (card.getTitleText() != invalid) {
				texts.push_back(card.getTitleText());
			}
			if (card.getSubtitleText() != invalid) {
				texts.push_back(card.getSubtitleText());
			}
			for (const auto button: card.getActionButtons()) {
				if (button != invalid) {
					buttons.push_back(button);
				}
			}
			arrangeSection(registry, card.getTitleSection(), texts);
			arrangeSection(registry, card.getActionsSection(), buttons);
		}

		/**
		 * @brief Write the colors, stroke and elevation of a card.
		 * @param registry The registry holding the components.
		 * @param identifier The card entity.
		 */
		void style(ecs::ComponentRegistry &registry,
				   ecs::Entity::Identifier identifier)
		{
			const auto &card =
				registry.getComponent<components::Card>(identifier);
			auto &borders =
				registry.getComponent<components::Borders>(identifier);
			auto &elevation =
				registry.getComponent<components::Elevation>(identifier);
			utility::graphic::Color32Bit container;

			switch (card.getVariant()) {
				case Variant::Elevated: {
					// Paper raises an elevated card from level 1 to level 2
					// while pressed; the shadow and the tonal tint follow
					// the motion.
					const auto &rest =
						getElevationToken(theme::ElevationLevel::Level1);
					const auto &raised =
						getElevationToken(theme::ElevationLevel::Level2);
					const float t = card.getPressProgress();
					container	  = lerpColor(
						getElevatedSurfaceColor(theme::ElevationLevel::Level1),
						getElevatedSurfaceColor(theme::ElevationLevel::Level2),
						t);

					const auto level   = t >= 0.5f
						  ? theme::ElevationLevel::Level2
						  : theme::ElevationLevel::Level1;
					const float spread = rest.shadowSpread
						+ (raised.shadowSpread - rest.shadowSpread) * t;
					const utility::graphic::Color32Bit shadow(
						0, 0, 0,
						static_cast<std::uint8_t>(
							static_cast<float>(rest.shadowAlpha)
							+ (static_cast<float>(raised.shadowAlpha)
							   - static_cast<float>(rest.shadowAlpha))
								* t));

					// Setting the level resets the spread and the shadow
					// color to its token: only write on a real change.
					if (elevation.getElevationLevel() != level
						|| elevation.getLevel() != 1.0f
						|| elevation.getSpread() != spread
						|| elevation.getColor() != shadow) {
						elevation.setElevationLevel(level)
							.setSpread(spread)
							.setColor(shadow);
					}
					borders.setColor(transparentColor()).setWidth(0.0f);
					break;
				}
				case Variant::Filled: {
					container = schemeColor(SchemeColorRole::SurfaceVariant);
					borders.setColor(transparentColor()).setWidth(0.0f);
					elevation.setElevationLevel(theme::ElevationLevel::Level0);
					break;
				}
				case Variant::Outlined:
				default: {
					container = schemeColor(SchemeColorRole::Surface);
					borders
						.setColor(schemeColor(SchemeColorRole::OutlineVariant))
						.setWidth(1.0f);
					elevation.setElevationLevel(theme::ElevationLevel::Level0);
					break;
				}
			}

			if (card.isInteractive()) {
				container = entities::stateLayerColor(
					registry, identifier, container,
					schemeColor(SchemeColorRole::OnSurface));
			}
			registry.getComponent<components::Color>(identifier)
				.setColor(container);
		}
	}	 // namespace

	Card::Card(void)
		: ecs::SystemFiller<components::Card>(ecs::Phase::Layout)
	{
	}

	void Card::install(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Animation>(identifier)
			.setDuration(getMotionDuration(theme::MotionDuration::Short3))
			.setEasing(theme::MotionScale::standard())
			.setOnSampleHandler([registry, identifier](float progress) {
				auto &card =
					registry->getComponent<components::Card>(identifier);
				const float target = card.isPressed() ? 1.0f : 0.0f;
				const float from   = card.getPressFrom();

				card.setPressProgress(from + (target - from) * progress);
				apply(*registry, identifier);
			});
	}

	void Card::setOnPress(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  components::Card::Handler handler)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		auto &card =
			componentRegistry.getComponent<components::Card>(identifier);

		card.setOnPress(std::move(handler));
		if (!card.isInteractive()) {
			card.setInteractive(true);
			entities::makeInteractive(
				componentRegistry, identifier,
				components::AccessibilityRole::Button,
				[registry, identifier]() {
					press(*registry, identifier);
				},
				[registry, identifier]() {
					interactionChanged(*registry, identifier);
				});
		}
		apply(componentRegistry, identifier);
	}

	void Card::press(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		// Copy: the callback may replace itself.
		const auto onPress =
			componentRegistry.getComponent<components::Card>(entityIdentifier)
				.getOnPress();
		if (onPress) {
			onPress();
		}
	}

	void Card::interactionChanged(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &card =
			componentRegistry.getComponent<components::Card>(entityIdentifier);
		const bool pressed = card.getVariant() == Variant::Elevated
			&& entities::isPressed(componentRegistry, entityIdentifier);

		if (pressed != card.isPressed()) {
			card.setPressed(pressed).setPressFrom(card.getPressProgress());
			componentRegistry
				.getComponent<components::Animation>(entityIdentifier)
				.play();
		}
		apply(componentRegistry, entityIdentifier);
	}

	void Card::setDisabled(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   bool disabled)
	{
		auto &card =
			componentRegistry.getComponent<components::Card>(entityIdentifier);

		entities::setEnabled(componentRegistry, entityIdentifier, !disabled);
		if (disabled && card.isPressed()) {
			card.setPressed(false).setPressProgress(0.0f);
		}
		apply(componentRegistry, entityIdentifier);
	}

	bool Card::isDisabled(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier)
	{
		return !entities::isEnabled(componentRegistry, entityIdentifier);
	}

	float Card::contentWidth(const ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		const float width =
			layout.hasFixedWidth() ? layout.getFixedWidth() : 0.0f;

		return std::max(0.0f, width - 2.0f * layout.getPadding());
	}

	void Card::apply(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &card =
			componentRegistry.getComponent<components::Card>(entityIdentifier);
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		const auto invalid		  = ecs::Entity::InvalidIdentifier;
		const auto cover		  = card.getCover();
		const auto titleSection	  = card.getTitleSection();
		const auto content		  = card.getContentEntity();
		const auto actionsSection = card.getActionsSection();

		style(componentRegistry, entityIdentifier);

		if (componentRegistry.hasComponent<components::Image>(cover)) {
			componentRegistry.getComponent<components::Image>(cover)
				.setTexturePath(card.getCoverPath());
		}
		writeText(componentRegistry, card.getTitleText(), card.getTitle());
		writeText(componentRegistry, card.getSubtitleText(),
				  card.getSubtitle());
		writeText(componentRegistry, content, card.getContentText());
		if (componentRegistry.hasComponent<components::Accessibility>(
				titleSection)) {
			componentRegistry
				.getComponent<components::Accessibility>(titleSection)
				.setLabel(card.getTitle());
		}

		// The sections are sized first, so the card arranges them with their
		// final bounds.
		arrangeSections(componentRegistry, card);

		// A card without composed content keeps whatever children it was
		// given; otherwise the sections are laid out cover, title, content,
		// actions.
		if (cover != invalid || titleSection != invalid || content != invalid
			|| actionsSection != invalid) {
			std::vector<ecs::Entity::Identifier> children;
			for (const auto child:
				 { cover, titleSection, content, actionsSection }) {
				if (child != invalid) {
					children.push_back(child);
				}
			}
			layout.setChildren(children);
		}
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));

		// The card may have moved its sections: place their content again.
		arrangeSections(componentRegistry, card);
	}

	void Card::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Card>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
