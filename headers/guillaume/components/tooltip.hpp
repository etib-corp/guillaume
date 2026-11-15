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

#pragma once

#include <optional>
#include <string>
#include <utility>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a tooltip: a small label that appears under an entity
	 * (its anchor) after the pointer has rested on it.
	 *
	 * It holds two kinds of state. The configuration (variant, texts,
	 * anchor, delays, viewport) is written by the entities::Tooltip widget.
	 * The running state (hover timers, visibility) is written by
	 * systems::Tooltip, which a looping components::Animation on the same
	 * entity calls each frame to advance the timers. In the Layout phase,
	 * systems::Tooltip turns all of it into the tooltip's Layout, Color,
	 * Borders, Elevation, Overlay and Accessibility, and into the text and
	 * color of its children.
	 *
	 * The tooltip shows once its anchor has been hovered for the enter
	 * delay, and hides once the anchor has been left for the leave delay.
	 * A hidden tooltip is drawn transparent.
	 *
	 * @see systems::Tooltip, components::Overlay, entities::Tooltip
	 */
	class Tooltip: public ecs::Component
	{
		public:
		/**
		 * @brief Kind of tooltip.
		 *
		 * Plain is a 32 dp high dark label with one text. Rich is a 64 dp
		 * high elevated card with a title above the text.
		 */
		enum class Variant { Plain, Rich };

		/// Area the tooltip must stay inside, as (width, height) in dp,
		/// with its origin at (0, 0).
		using Viewport = std::pair<float, float>;

		/// Default time the anchor must be hovered before the tooltip
		/// shows, in seconds.
		static constexpr float DefaultEnterDelay = 0.5f;

		/// Default time the tooltip stays after the anchor is left, in
		/// seconds.
		static constexpr float DefaultLeaveDelay = 1.5f;

		private:
		Variant _variant { Variant::Plain };	///< Kind of tooltip
		std::string _text {};					///< Body text
		std::string _title {};					///< Title (Rich only)
		ecs::Entity::Identifier _titleEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Title child

		ecs::Entity::Identifier _textEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Body child

		ecs::Entity::Identifier _anchor {
			ecs::Entity::InvalidIdentifier
		};	  ///< Trigger entity

		float _enterDelay { DefaultEnterDelay };	///< Hover-in delay
		float _leaveDelay { DefaultLeaveDelay };	///< Hover-out delay
		float _hoverTime { 0.0f };		 ///< Time the anchor has been hovered
		float _leaveTime { 0.0f };		 ///< Time since the anchor was left
		float _lastProgress { 0.0f };	 ///< Last ticker progress
		bool _visible { false };		 ///< Whether the tooltip is shown
		std::optional<Viewport> _viewport {};	 ///< Clamping area

		public:
		/**
		 * @brief Create the configuration of a hidden, empty plain tooltip
		 * with no anchor and the default delays.
		 */
		Tooltip(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Tooltip(void) = default;

		/**
		 * @brief Get the kind of tooltip.
		 * @return The variant, Variant::Plain by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the kind of tooltip.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		Tooltip &setVariant(Variant variant);

		/**
		 * @brief Get the body text.
		 * @return The text; it is also the tooltip's accessible name.
		 */
		const std::string &getText(void) const noexcept;

		/**
		 * @brief Set the body text.
		 *
		 * systems::Tooltip copies it to the body child. Marks the component
		 * changed only when the text differs.
		 * @param text The new text.
		 * @return This component, for chaining.
		 */
		Tooltip &setText(const std::string &text);

		/**
		 * @brief Get the title of a rich tooltip.
		 * @return The title; empty when there is none.
		 */
		const std::string &getTitle(void) const noexcept;

		/**
		 * @brief Set the title of a rich tooltip.
		 *
		 * The widget uses it when it builds the title child. A plain
		 * tooltip does not show it. Marks the component changed only when
		 * the text differs.
		 * @param title The new title.
		 * @return This component, for chaining.
		 */
		Tooltip &setTitle(const std::string &title);

		/**
		 * @brief Get the child that draws the title.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getTitleEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the title.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Tooltip &setTitleEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the body text.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getTextEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the body text.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Tooltip &setTextEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the entity whose hover shows the tooltip.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when
		 * the tooltip has no anchor.
		 */
		ecs::Entity::Identifier getAnchor(void) const noexcept;

		/**
		 * @brief Record the entity whose hover shows the tooltip.
		 *
		 * This only stores the identifier. Use systems::Tooltip::setAnchor
		 * instead: it also resets the timers and starts or stops the
		 * animation that advances them. Marks the component changed only
		 * when the identifier differs.
		 * @param identifier The anchor's identifier, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Tooltip &setAnchor(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get how long the anchor must be hovered before the
		 * tooltip shows.
		 * @return The delay, in seconds.
		 */
		float getEnterDelay(void) const noexcept;

		/**
		 * @brief Set how long the anchor must be hovered before the
		 * tooltip shows.
		 *
		 * Marks the component changed only when the value differs.
		 * @param seconds The delay, in seconds; 0 shows the tooltip at
		 * once.
		 * @return This component, for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setEnterDelay(float seconds);

		/**
		 * @brief Get how long the tooltip stays after the pointer leaves
		 * the anchor.
		 * @return The delay, in seconds.
		 */
		float getLeaveDelay(void) const noexcept;

		/**
		 * @brief Set how long the tooltip stays after the pointer leaves
		 * the anchor.
		 *
		 * Marks the component changed only when the value differs.
		 * @param seconds The delay, in seconds; 0 hides the tooltip at
		 * once.
		 * @return This component, for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setLeaveDelay(float seconds);

		/**
		 * @brief Get how long the anchor has been hovered without a break.
		 * @return The time, in seconds; 0 while the anchor is not hovered.
		 */
		float getHoverTime(void) const noexcept;

		/**
		 * @brief Store how long the anchor has been hovered.
		 *
		 * Running state written by systems::Tooltip::advance. Marks the
		 * component changed only when the value differs.
		 * @param seconds The time, in seconds.
		 * @return This component, for chaining.
		 */
		Tooltip &setHoverTime(float seconds);

		/**
		 * @brief Get how long ago the pointer left the anchor while the
		 * tooltip was shown.
		 * @return The time, in seconds; 0 while the anchor is hovered or
		 * the tooltip is hidden.
		 */
		float getLeaveTime(void) const noexcept;

		/**
		 * @brief Store how long ago the pointer left the anchor.
		 *
		 * Running state written by systems::Tooltip::advance. Marks the
		 * component changed only when the value differs.
		 * @param seconds The time, in seconds.
		 * @return This component, for chaining.
		 */
		Tooltip &setLeaveTime(float seconds);

		/**
		 * @brief Get the progress the timer animation reported last.
		 * @return The progress, 0 to 1.
		 */
		float getLastProgress(void) const noexcept;

		/**
		 * @brief Store the progress the timer animation reported last.
		 *
		 * The timers are driven by a one-second looping animation; the
		 * difference between two reported progresses is the time elapsed,
		 * in seconds. Marks the component changed only when the value
		 * differs.
		 * @param progress The progress, 0 to 1.
		 * @return This component, for chaining.
		 */
		Tooltip &setLastProgress(float progress);

		/**
		 * @brief Tell whether the tooltip is shown.
		 * @return True when shown; false by default.
		 */
		bool isVisible(void) const noexcept;

		/**
		 * @brief Record whether the tooltip is shown.
		 *
		 * This only stores the flag; the screen follows on the next pass of
		 * systems::Tooltip. Prefer systems::Tooltip::show and
		 * systems::Tooltip::hide, which also place and restyle the tooltip
		 * at once. Marks the component changed only when the flag differs.
		 * @param visible True to show the tooltip.
		 * @return This component, for chaining.
		 */
		Tooltip &setVisible(bool visible);

		/**
		 * @brief Get the area the tooltip must stay inside.
		 * @return The area as (width, height) in dp, or std::nullopt when
		 * the tooltip is not kept inside any area.
		 */
		const std::optional<Viewport> &getViewport(void) const noexcept;

		/**
		 * @brief Set the area the tooltip must stay inside.
		 *
		 * With a viewport, a tooltip that would go past the bottom edge is
		 * placed above its anchor, and its position is clamped to the
		 * area. Marks the component changed only when the value differs.
		 * @param viewport The area as (width, height) in dp, or
		 * std::nullopt to lift the constraint.
		 * @return This component, for chaining.
		 */
		Tooltip &setViewport(std::optional<Viewport> viewport);
	};

}	 // namespace guillaume::components
