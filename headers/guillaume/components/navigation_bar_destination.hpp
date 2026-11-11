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
	 * @brief State of one destination of a navigation bar: an icon in a
	 * pill, a label under it and an optional badge.
	 *
	 * The entities::detail::BarDestination widget writes the glyphs, the
	 * label and the badge text, and records the identifiers of the children
	 * it builds (pill, icon, label, badge). systems::NavigationBar writes the
	 * label visibility and the press callback.
	 * systems::NavigationBarDestination reads everything in the Layout phase
	 * and turns it into the colors, the glyph and the placement of those
	 * children.
	 *
	 * Whether the destination is selected is not stored here: it is the
	 * components::Selectable of the same entity.
	 *
	 * @see systems::NavigationBarDestination, components::NavigationBar
	 */
	class NavigationBarDestination: public ecs::Component
	{
		public:
		/// Callback run when the user presses the destination.
		using PressHandler = std::function<void(void)>;

		/// Width of the active indicator pill, in dp.
		static constexpr float IndicatorWidth = 64.0f;

		/// Height of the active indicator pill, in dp.
		static constexpr float IndicatorHeight = 32.0f;

		/// Size of the icon, in dp.
		static constexpr float IconSize = 24.0f;

		/// Horizontal offset of the badge from the indicator origin, in dp,
		/// so the badge sits on the top-right corner of the icon.
		static constexpr float BadgeOffsetX = 36.0f;

		/// Vertical offset of the badge from the indicator origin, in dp.
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
		 * @brief Create the configuration of a destination with no icon, no
		 * text and a visible label.
		 */
		NavigationBarDestination(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationBarDestination(void) = default;

		/**
		 * @brief Get the icon shown while the destination is not selected.
		 * @return The glyph name.
		 */
		const std::string &getIcon(void) const noexcept;

		/**
		 * @brief Set the icon shown while the destination is not selected.
		 *
		 * It is also the icon of a selected destination that has no focused
		 * icon. Marks the component changed only when the name differs.
		 * @param icon The glyph name.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &setIcon(const std::string &icon);

		/**
		 * @brief Get the icon shown while the destination is selected.
		 * @return The glyph name; empty when the destination keeps its
		 * unselected icon.
		 */
		const std::string &getFocusedIcon(void) const noexcept;

		/**
		 * @brief Set the icon shown while the destination is selected,
		 * typically the filled version of the unselected one.
		 *
		 * Marks the component changed only when the name differs.
		 * @param icon The glyph name; empty keeps the unselected icon.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &setFocusedIcon(const std::string &icon);

		/**
		 * @brief Get the text shown under the icon.
		 * @return The label; it is also the destination's accessible name.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the text shown under the icon.
		 *
		 * Marks the component changed only when the text differs.
		 * @param label The new label.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &setLabel(const std::string &label);

		/**
		 * @brief Get the text of the badge.
		 * @return The badge text. When the badge child exists, an empty
		 * text draws it as a dot.
		 */
		const std::string &getBadge(void) const noexcept;

		/**
		 * @brief Set the text of the badge.
		 *
		 * systems::NavigationBarDestination shows it on the badge child,
		 * when that child exists: a large badge with the text, or a dot
		 * when the text is empty. Marks the component changed only when the
		 * text differs.
		 * @param badge The new badge text.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &setBadge(const std::string &badge);

		/**
		 * @brief Tell whether the label is visible.
		 * @return True when the label is drawn; false when it is
		 * transparent.
		 */
		bool isLabelShown(void) const noexcept;

		/**
		 * @brief Show or hide the label.
		 *
		 * A hidden label is drawn transparent and keeps its place in the
		 * column. systems::NavigationBar overwrites this flag every frame
		 * from the bar's variant and the selection. Marks the component
		 * changed only when the flag differs.
		 * @param show True to draw the label.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &setShowLabel(bool show);

		/**
		 * @brief Get the pill drawn behind the icon of the selected
		 * destination.
		 * @return The child's identifier, or ecs::Entity::InvalidIdentifier
		 * when it has not been built.
		 */
		ecs::Entity::Identifier getIndicator(void) const noexcept;

		/**
		 * @brief Record the active indicator pill child.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &
			setIndicator(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the icon.
		 * @return The child's identifier, or ecs::Entity::InvalidIdentifier
		 * when it has not been built.
		 */
		ecs::Entity::Identifier getIconEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &
			setIconEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the label.
		 * @return The child's identifier, or ecs::Entity::InvalidIdentifier
		 * when it has not been built.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the label.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &
			setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the badge.
		 * @return The child's identifier, or ecs::Entity::InvalidIdentifier
		 * when the destination has no badge.
		 */
		ecs::Entity::Identifier getBadgeEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the badge.
		 *
		 * The widget builds it the first time the badge text is not empty.
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &
			setBadgeEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the callback run when the user presses the
		 * destination.
		 * @return The callback; empty when none is set.
		 */
		const PressHandler &getOnPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the user presses the
		 * destination.
		 *
		 * Inside a navigation bar, systems::NavigationBar::install sets it
		 * to select the destination, so a callback set by hand is replaced.
		 * Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		NavigationBarDestination &setOnPress(PressHandler handler);
	};

}	 // namespace guillaume::components
