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
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a floating action button menu (speed dial).
	 *
	 * This component is the whole state of the menu: its trigger icon and
	 * label, its actions, whether it is open, and the button children.
	 * systems::FloatingActionButtonMenu turns it into the visibility of the
	 * action buttons, the trigger's icon and expanded state, and the column
	 * the Layout system arranges.
	 */
	class FloatingActionButtonMenu: public ecs::Component
	{
		public:
		/**
		 * @brief Describes a single action in the menu.
		 */
		struct Action {
			std::string label;			  ///< Accessibility/action label
			std::string iconGlyphName;	  ///< Icon glyph name
			std::function<void(void)>
				onClick;	///< Click handler for the action
		};

		/// Called with the new open state when the menu opens or closes.
		using StateChangeHandler = std::function<void(bool)>;

		/// Gap between the buttons of the column, in dp.
		static constexpr float Spacing = 16.0f;

		private:
		std::string _iconGlyphName {};		   ///< Trigger icon glyph name
		std::vector<Action> _actions {};	   ///< Menu actions
		std::string _accessibilityLabel {};	   ///< Trigger accessibility label
		bool _open { false };				   ///< Whether the menu is expanded
		StateChangeHandler _onStateChange {};	 ///< Open-state callback
		ecs::Entity::Identifier _trigger {
			ecs::Entity::InvalidIdentifier
		};	  ///< Trigger button child
		std::vector<ecs::Entity::Identifier>
			_actionButtons {};	  ///< Button child of each action

		public:
		/**
		 * @brief Default constructor.
		 */
		FloatingActionButtonMenu(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButtonMenu(void) = default;

		/** @brief Get the trigger icon. @return Its glyph name. */
		const std::string &getIconGlyphName(void) const noexcept;

		/** @brief Set the trigger icon. @param name Its glyph name. @return
		 * It. */
		FloatingActionButtonMenu &setIconGlyphName(const std::string &name);

		/** @brief Get the actions. @return The actions. */
		const std::vector<Action> &getActions(void) const noexcept;

		/** @brief Set the actions. @param actions They. @return It. */
		FloatingActionButtonMenu &setActions(std::vector<Action> actions);

		/** @brief Get the trigger accessibility label. @return The label. */
		const std::string &getAccessibilityLabel(void) const noexcept;

		/** @brief Set the trigger accessibility label. @param label It.
		 * @return It. */
		FloatingActionButtonMenu &
			setAccessibilityLabel(const std::string &label);

		/** @brief Get whether the menu is open. @return True when open. */
		bool isOpen(void) const noexcept;

		/** @brief Set whether the menu is open. @param open It. @return It.
		 */
		FloatingActionButtonMenu &setOpen(bool open);

		/** @brief Get the open-state callback. @return The callback. */
		const StateChangeHandler &getOnStateChange(void) const noexcept;

		/** @brief Set the open-state callback. @param handler It. @return It.
		 */
		FloatingActionButtonMenu &setOnStateChange(StateChangeHandler handler);

		/** @brief Get the trigger child. @return Its identifier. */
		ecs::Entity::Identifier getTrigger(void) const noexcept;

		/** @brief Set the trigger child. @param identifier It. @return It. */
		FloatingActionButtonMenu &
			setTrigger(ecs::Entity::Identifier identifier);

		/** @brief Get the action button children. @return Their identifiers,
		 * in action order. */
		const std::vector<ecs::Entity::Identifier> &
			getActionButtons(void) const noexcept;

		/** @brief Set the action button children. @param buttons Their
		 * identifiers, in action order. @return It. */
		FloatingActionButtonMenu &setActionButtons(
			const std::vector<ecs::Entity::Identifier> &buttons);
	};

}	 // namespace guillaume::components
