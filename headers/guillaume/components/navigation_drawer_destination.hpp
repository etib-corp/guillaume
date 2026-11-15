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
	 * @brief State of one destination of a navigation drawer: an icon, a
	 * label and an optional text at the trailing edge.
	 *
	 * It has two renderings. A regular item is a 56 dp row whose whole
	 * surface is the active pill. A collapsed item (the rail of a collapsed
	 * drawer) stacks a small pill holding the icon above the label.
	 *
	 * The entities::detail::DrawerDestination widget writes the content and
	 * the identifiers of the children it builds.
	 * systems::NavigationDrawerDestination reads everything in the Layout
	 * phase and turns it into the pill color, the content colors and the
	 * placement of those children.
	 *
	 * Whether the destination is selected is not stored here: it is the
	 * components::Selectable of the same entity.
	 *
	 * @see systems::NavigationDrawerDestination,
	 * components::NavigationDrawer
	 */
	class NavigationDrawerDestination: public ecs::Component
	{
		public:
		/// Callback run when the user presses the destination.
		using PressHandler = std::function<void(void)>;

		/// Height of a regular destination, in dp.
		static constexpr float Height = 56.0f;

		/// Width of the active indicator pill of a collapsed item, in dp.
		static constexpr float CollapsedIndicatorWidth = 56.0f;

		/// Height of the active indicator pill of a collapsed item, in dp.
		static constexpr float CollapsedIndicatorHeight = 32.0f;

		/// Size of the icon, in dp.
		static constexpr float IconSize = 24.0f;

		private:
		std::string _icon {};		  ///< Icon glyph
		std::string _label {};		  ///< Label text
		std::string _right {};		  ///< Trailing text
		bool _disabled { false };	  ///< Whether the destination is disabled
		bool _collapsed { false };	  ///< Collapsed (rail) rendering
		ecs::Entity::Identifier _lead {
			ecs::Entity::InvalidIdentifier
		};	  ///< Icon and label group, or the pill of a collapsed item

		ecs::Entity::Identifier _iconEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Icon child

		ecs::Entity::Identifier _labelEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Label child

		ecs::Entity::Identifier _rightEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Trailing text child

		PressHandler _onPress {};	 ///< User press callback

		public:
		/**
		 * @brief Create the configuration of an enabled, regular
		 * destination with no icon and no text.
		 */
		NavigationDrawerDestination(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawerDestination(void) = default;

		/**
		 * @brief Get the icon of the destination.
		 * @return The glyph name.
		 */
		const std::string &getIcon(void) const noexcept;

		/**
		 * @brief Set the icon of the destination.
		 *
		 * Marks the component changed only when the name differs.
		 * @param icon The glyph name.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &setIcon(const std::string &icon);

		/**
		 * @brief Get the label of the destination.
		 * @return The label; it is also the destination's accessible name.
		 */
		const std::string &getLabel(void) const noexcept;

		/**
		 * @brief Set the label of the destination.
		 *
		 * Marks the component changed only when the text differs.
		 * @param label The new label.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &setLabel(const std::string &label);

		/**
		 * @brief Get the text shown at the trailing edge of a regular item.
		 * @return The text; empty when there is none.
		 */
		const std::string &getRight(void) const noexcept;

		/**
		 * @brief Set the text shown at the trailing edge of a regular item,
		 * such as an unread count.
		 *
		 * When it is not empty, the row spreads its content so this text
		 * reaches the far edge. Marks the component changed only when the
		 * text differs.
		 * @param right The new text; empty for none.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &setRight(const std::string &right);

		/**
		 * @brief Tell whether the destination ignores presses.
		 * @return True when disabled.
		 */
		bool isDisabled(void) const noexcept;

		/**
		 * @brief Disable or enable the destination.
		 *
		 * A disabled destination is drawn with the disabled content color,
		 * shows no active pill and does not run its press callback.
		 * systems::NavigationDrawerDestination copies the flag to the
		 * entity's components::StyleState. Marks the component changed only
		 * when the flag differs.
		 * @param disabled True to disable the destination.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &setDisabled(bool disabled);

		/**
		 * @brief Tell which rendering the destination uses.
		 * @return True for a collapsed (rail) item, false for a regular row.
		 */
		bool isCollapsed(void) const noexcept;

		/**
		 * @brief Choose between the regular row and the collapsed (rail)
		 * rendering.
		 *
		 * Marks the component changed only when the flag differs.
		 * @param collapsed True for a collapsed item.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &setCollapsed(bool collapsed);

		/**
		 * @brief Get the leading child of the destination.
		 * @return For a regular item, the group that holds the icon and the
		 * label; for a collapsed item, the pill that holds the icon.
		 * ecs::Entity::InvalidIdentifier when it has not been built.
		 */
		ecs::Entity::Identifier getLead(void) const noexcept;

		/**
		 * @brief Record the leading child of the destination.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The identifier of the icon-and-label group
		 * (regular item) or of the pill (collapsed item).
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &
			setLead(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getIconEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &
			setIconEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the label.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &
			setLabelEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the trailing text.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * destination has none.
		 */
		ecs::Entity::Identifier getRightEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the trailing text.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &
			setRightEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the callback run when the user presses the
		 * destination.
		 * @return The callback; empty when none is set.
		 */
		const PressHandler &getOnPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the user presses the
		 * destination while it is enabled.
		 *
		 * Inside a navigation drawer, systems::NavigationDrawer::install
		 * sets it to select the destination, so a callback set by hand is
		 * replaced. Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		NavigationDrawerDestination &setOnPress(PressHandler handler);
	};

}	 // namespace guillaume::components
