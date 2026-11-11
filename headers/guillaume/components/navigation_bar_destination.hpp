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

#include <functional>
#include <string>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of one destination of a navigation bar.
	 *
	 * This component is the whole state of a destination: its glyphs, label
	 * and badge, whether the label is shown, and the identifiers of the
	 * active-indicator pill, icon, label and badge children. Whether the
	 * destination is selected is its components::Selectable.
	 * systems::NavigationBarDestination turns it into the colors, the glyph
	 * and the placement of those children.
	 */
	class NavigationBarDestination: public ecs::Component
	{
		public:
		/// Called when the user presses the destination.
		using PressHandler = std::function<void(void)>;

		/// M3 active indicator width, in dp.
		static constexpr float IndicatorWidth = 64.0f;

		/// M3 active indicator height, in dp.
		static constexpr float IndicatorHeight = 32.0f;

		/// Icon size, in dp.
		static constexpr float IconSize = 24.0f;

		/// Horizontal badge offset from the indicator origin, so the badge
		/// sits on the top-right corner of the icon.
		static constexpr float BadgeOffsetX = 36.0f;

		/// Vertical badge offset from the indicator origin.
		static constexpr float BadgeOffsetY = 2.0f;

		private:
		std::string _icon {};			///< Unselected icon glyph
		std::string _focusedIcon {};	///< Icon glyph while selected
		std::string _label {};			///< Label text
		std::string _badge {};			///< Badge label
		bool _showLabel { true };		///< Whether the label is shown
		ecs::Entity::Identifier _indicator {
			ecs::Entity::InvalidIdentifier
		};	  ///< Active indicator pill child

		ecs::Entity::Identifier _iconEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Icon child

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child

		ecs::Entity::Identifier _badgeEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Badge child

		PressHandler _onPress {};	 ///< User press callback

		public:
		/**
		 * @brief Default constructor.
		 */
		NavigationBarDestination(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationBarDestination(void) = default;

		/** @brief Get the unselected icon glyph. @return The glyph name. */
		const std::string &getIcon(void) const noexcept;

		/** @brief Set the unselected icon glyph. @param icon It. @return It.
		 */
		NavigationBarDestination &setIcon(const std::string &icon);

		/** @brief Get the icon glyph shown while selected. @return The glyph
		 * name ("" keeps the unselected glyph). */
		const std::string &getFocusedIcon(void) const noexcept;

		/** @brief Set the icon glyph shown while selected. @param icon It.
		 * @return It. */
		NavigationBarDestination &setFocusedIcon(const std::string &icon);

		/** @brief Get the label text. @return The label. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set the label text. @param label It. @return It. */
		NavigationBarDestination &setLabel(const std::string &label);

		/** @brief Get the badge label. @return The label ("" shows a dot on
		 * an existing badge). */
		const std::string &getBadge(void) const noexcept;

		/** @brief Set the badge label. @param badge It. @return It. */
		NavigationBarDestination &setBadge(const std::string &badge);

		/** @brief Whether the label is shown. @return True when shown. */
		bool isLabelShown(void) const noexcept;

		/** @brief Set whether the label is shown. @param show It. @return It.
		 */
		NavigationBarDestination &setShowLabel(bool show);

		/** @brief Get the active indicator child. @return Its identifier. */
		ecs::Entity::Identifier getIndicator(void) const noexcept;

		/** @brief Set the active indicator child. @param identifier It.
		 * @return It. */
		NavigationBarDestination &
			setIndicator(ecs::Entity::Identifier identifier);

		/** @brief Get the icon child. @return Its identifier. */
		ecs::Entity::Identifier getIconEntity(void) const noexcept;

		/** @brief Set the icon child. @param identifier It. @return It. */
		NavigationBarDestination &
			setIconEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the label child. @return Its identifier. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set the label child. @param identifier It. @return It. */
		NavigationBarDestination &
			setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the badge child. @return Its identifier. */
		ecs::Entity::Identifier getBadgeEntity(void) const noexcept;

		/** @brief Set the badge child. @param identifier It. @return It. */
		NavigationBarDestination &
			setBadgeEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the press callback. @return The callback. */
		const PressHandler &getOnPress(void) const noexcept;

		/** @brief Set the press callback. @param handler It. @return It. */
		NavigationBarDestination &setOnPress(PressHandler handler);
	};

}	 // namespace guillaume::components
