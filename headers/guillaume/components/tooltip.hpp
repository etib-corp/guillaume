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
	 * @brief Configuration and transient state of a tooltip.
	 *
	 * This component is the whole state of a tooltip: what it shows, the
	 * entity that triggers it, the hover delays and their running timers.
	 * systems::Tooltip turns it into the tooltip's Layout, Color, Borders,
	 * Elevation, Overlay and Accessibility, and into the color of its title
	 * and body children.
	 */
	class Tooltip: public ecs::Component
	{
		public:
		/**
		 * @brief Tooltip variant.
		 */
		enum class Variant { Plain, Rich };

		/// Clamping area, as (width, height).
		using Viewport = std::pair<float, float>;

		/// Paper `enterTouchDelay`, in seconds.
		static constexpr float DefaultEnterDelay = 0.5f;

		/// Paper `leaveTouchDelay`, in seconds.
		static constexpr float DefaultLeaveDelay = 1.5f;

		private:
		Variant _variant { Variant::Plain };	///< Tooltip variant
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
		 * @brief Default constructor.
		 */
		Tooltip(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Tooltip(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Tooltip &setVariant(Variant variant);

		/** @brief Get the body text. @return The text. */
		const std::string &getText(void) const noexcept;

		/** @brief Set the body text. @param text It. @return It. */
		Tooltip &setText(const std::string &text);

		/** @brief Get the title. @return The title. */
		const std::string &getTitle(void) const noexcept;

		/** @brief Set the title. @param title It. @return It. */
		Tooltip &setTitle(const std::string &title);

		/** @brief Get the title child. @return Its identifier. */
		ecs::Entity::Identifier getTitleEntity(void) const noexcept;

		/** @brief Set the title child. @param identifier It. @return It. */
		Tooltip &setTitleEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the body child. @return Its identifier. */
		ecs::Entity::Identifier getTextEntity(void) const noexcept;

		/** @brief Set the body child. @param identifier It. @return It. */
		Tooltip &setTextEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the trigger entity. @return Its identifier. */
		ecs::Entity::Identifier getAnchor(void) const noexcept;

		/** @brief Set the trigger entity. @param identifier It. @return It. */
		Tooltip &setAnchor(ecs::Entity::Identifier identifier);

		/** @brief Get the hover-in delay. @return Seconds. */
		float getEnterDelay(void) const noexcept;

		/**
		 * @brief Set the hover-in delay.
		 * @param seconds Delay, must be non-negative.
		 * @return Reference to this component for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setEnterDelay(float seconds);

		/** @brief Get the hover-out delay. @return Seconds. */
		float getLeaveDelay(void) const noexcept;

		/**
		 * @brief Set the hover-out delay.
		 * @param seconds Delay, must be non-negative.
		 * @return Reference to this component for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setLeaveDelay(float seconds);

		/** @brief Get the time the anchor has been hovered. @return Seconds. */
		float getHoverTime(void) const noexcept;

		/** @brief Set the hovered time. @param seconds It. @return It. */
		Tooltip &setHoverTime(float seconds);

		/** @brief Get the time since the anchor was left. @return Seconds. */
		float getLeaveTime(void) const noexcept;

		/** @brief Set the left time. @param seconds It. @return It. */
		Tooltip &setLeaveTime(float seconds);

		/** @brief Get the last ticker progress. @return Progress in [0, 1]. */
		float getLastProgress(void) const noexcept;

		/** @brief Set the last ticker progress. @param progress It. @return It.
		 */
		Tooltip &setLastProgress(float progress);

		/** @brief Check the visibility. @return True when shown. */
		bool isVisible(void) const noexcept;

		/** @brief Set the visibility. @param visible It. @return It. */
		Tooltip &setVisible(bool visible);

		/** @brief Get the clamping area. @return The area, if any. */
		const std::optional<Viewport> &getViewport(void) const noexcept;

		/** @brief Set the clamping area. @param viewport It. @return It. */
		Tooltip &setViewport(std::optional<Viewport> viewport);
	};

}	 // namespace guillaume::components
