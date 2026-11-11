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
	 * @brief Configuration of one destination of a navigation drawer (Paper
	 * `Drawer.Item` or `Drawer.CollapsedItem`).
	 *
	 * This component is the whole state of a destination: its glyph, label
	 * and trailing text, whether it is disabled, whether it is rendered as a
	 * collapsed (rail) item, and the identifiers of its lead group, icon,
	 * label and trailing text children. Whether the destination is selected
	 * is its components::Selectable. systems::NavigationDrawerDestination
	 * turns it into the active pill, the content colors and the placement of
	 * those children.
	 */
	class NavigationDrawerDestination: public ecs::Component
	{
		public:
		/// Called when the user presses the destination.
		using PressHandler = std::function<void(void)>;

		/// M3 drawer destination height, in dp.
		static constexpr float Height = 56.0f;

		/// Width of the active indicator of a collapsed item, in dp.
		static constexpr float CollapsedIndicatorWidth = 56.0f;

		/// Height of the active indicator of a collapsed item, in dp.
		static constexpr float CollapsedIndicatorHeight = 32.0f;

		/// Icon size, in dp.
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
		 * @brief Default constructor.
		 */
		NavigationDrawerDestination(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawerDestination(void) = default;

		/** @brief Get the icon glyph. @return The glyph name. */
		const std::string &getIcon(void) const noexcept;

		/** @brief Set the icon glyph. @param icon It. @return It. */
		NavigationDrawerDestination &setIcon(const std::string &icon);

		/** @brief Get the label text. @return The label. */
		const std::string &getLabel(void) const noexcept;

		/** @brief Set the label text. @param label It. @return It. */
		NavigationDrawerDestination &setLabel(const std::string &label);

		/** @brief Get the trailing text. @return The text ("" for none). */
		const std::string &getRight(void) const noexcept;

		/** @brief Set the trailing text. @param right It. @return It. */
		NavigationDrawerDestination &setRight(const std::string &right);

		/** @brief Whether the destination is disabled. @return True when
		 * disabled. */
		bool isDisabled(void) const noexcept;

		/** @brief Set whether the destination is disabled. @param disabled
		 * It. @return It. */
		NavigationDrawerDestination &setDisabled(bool disabled);

		/** @brief Whether the destination is a collapsed (rail) item. @return
		 * True when collapsed. */
		bool isCollapsed(void) const noexcept;

		/** @brief Set whether the destination is a collapsed item. @param
		 * collapsed It. @return It. */
		NavigationDrawerDestination &setCollapsed(bool collapsed);

		/** @brief Get the lead group child. @return Its identifier. */
		ecs::Entity::Identifier getLead(void) const noexcept;

		/** @brief Set the lead group child. @param identifier It. @return It.
		 */
		NavigationDrawerDestination &
			setLead(ecs::Entity::Identifier identifier);

		/** @brief Get the icon child. @return Its identifier. */
		ecs::Entity::Identifier getIconEntity(void) const noexcept;

		/** @brief Set the icon child. @param identifier It. @return It. */
		NavigationDrawerDestination &
			setIconEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the label child. @return Its identifier. */
		ecs::Entity::Identifier getLabelEntity(void) const noexcept;

		/** @brief Set the label child. @param identifier It. @return It. */
		NavigationDrawerDestination &
			setLabelEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the trailing text child. @return Its identifier. */
		ecs::Entity::Identifier getRightEntity(void) const noexcept;

		/** @brief Set the trailing text child. @param identifier It. @return
		 * It. */
		NavigationDrawerDestination &
			setRightEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the press callback. @return The callback. */
		const PressHandler &getOnPress(void) const noexcept;

		/** @brief Set the press callback. @param handler It. @return It. */
		NavigationDrawerDestination &setOnPress(PressHandler handler);
	};

}	 // namespace guillaume::components
