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
	 * @brief State of a floating action button menu (speed dial): a trigger
	 * button that expands into a column of action buttons above it.
	 *
	 * It holds the trigger's icon and accessibility label, the list of
	 * actions, whether the menu is open, and the identifiers of the button
	 * children.
	 *
	 * Role in the frame:
	 * - entities::FloatingActionButtonMenu writes the configuration and
	 *   creates one button child per action plus the trigger, then records
	 *   their identifiers here.
	 * - systems::FloatingActionButtonMenu (Layout phase) reads it: it shows
	 *   the action buttons only while open, swaps the trigger icon for a
	 *   close glyph, and gives the visible buttons to the Layout component
	 *   of the menu so they are stacked in a column.
	 *
	 * setOpen() only stores the flag. To open or close the menu from code
	 * and have the state-change callback run, call
	 * systems::FloatingActionButtonMenu::setOpen(), `open()`, `close()` or
	 * `toggle()`.
	 *
	 * @see systems::FloatingActionButtonMenu, components::Button,
	 * components::Layout
	 */
	class FloatingActionButtonMenu: public ecs::Component
	{
		public:
		/**
		 * @brief One entry of the menu: what its button shows and does.
		 */
		struct Action {
			/// Text shown beside the icon and used as accessibility label;
			/// empty for an icon-only action.
			std::string label;
			std::string iconGlyphName;	  ///< Icon glyph name
			std::function<void(void)>
				onClick;	///< Called when the action button is clicked
		};

		/// Called with the new open state (true when open) each time the
		/// menu opens or closes.
		using StateChangeHandler = std::function<void(bool)>;

		/// Gap between the buttons of the column, in dp.
		static constexpr float Spacing = 16.0f;

		private:
		std::string _iconGlyphName {};		   ///< Trigger icon when closed
		std::vector<Action> _actions {};	   ///< Menu actions, top to bottom
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
		 * @brief Create the state of a closed menu with no action.
		 */
		FloatingActionButtonMenu(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButtonMenu(void) = default;

		/**
		 * @brief Get the icon the trigger shows while the menu is closed.
		 * @return The glyph name.
		 */
		const std::string &getIconGlyphName(void) const noexcept;

		/**
		 * @brief Set the icon the trigger shows while the menu is closed.
		 *
		 * While open, the system shows
		 * systems::FloatingActionButtonMenu::CloseGlyph instead. Marks the
		 * component changed when the name differs.
		 * @param name The glyph name in the icon font.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &setIconGlyphName(const std::string &name);

		/**
		 * @brief Get the actions of the menu.
		 * @return The actions, in the order their buttons are stacked.
		 */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Replace the actions of the menu.
		 *
		 * The widget class creates one button child per action from this
		 * list. Does not mark the component changed (actions carry
		 * callbacks, which cannot be compared).
		 * @param actions The new actions.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &setActions(std::vector<Action> actions);

		/**
		 * @brief Get the accessibility label of the trigger button.
		 * @return The label, empty when none was given.
		 */
		const std::string &getAccessibilityLabel(void) const noexcept;

		/**
		 * @brief Set the accessibility label of the trigger button.
		 *
		 * systems::FloatingActionButtonMenu::install() copies it to the
		 * trigger's Accessibility. Marks the component changed when the
		 * label differs.
		 * @param label The text announced for the trigger.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &
			setAccessibilityLabel(const std::string &label);

		/**
		 * @brief Check whether the menu is open.
		 * @return True when the action buttons are shown.
		 */
		bool isOpen(void) const noexcept;

		/**
		 * @brief Store whether the menu is open.
		 *
		 * Marks the component changed when the state differs. It does not
		 * run the state-change callback; use
		 * systems::FloatingActionButtonMenu::setOpen() for that.
		 * @param open True to show the action buttons.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &setOpen(bool open);

		/**
		 * @brief Get the callback run when the menu opens or closes.
		 * @return The callback, empty when none is set.
		 */
		const StateChangeHandler &getOnStateChange(void) const noexcept;

		/**
		 * @brief Set the callback run when the menu opens or closes.
		 *
		 * Does not mark the component changed.
		 * @param handler The callback; it receives true when the menu just
		 * opened and false when it just closed.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &setOnStateChange(StateChangeHandler handler);

		/**
		 * @brief Get the child entity of the trigger button.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier before
		 * the widget is built.
		 */
		ecs::Entity::Identifier getTrigger(void) const noexcept;

		/**
		 * @brief Record which child entity is the trigger button.
		 *
		 * Marks the component changed when the identifier differs.
		 * @param identifier The trigger button child.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &
			setTrigger(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entities of the action buttons.
		 * @return Their identifiers, in action order; entry `i` is the
		 * button of `getActions()[i]`.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getActionButtons(void) const noexcept;

		/**
		 * @brief Record which child entities are the action buttons.
		 *
		 * Marks the component changed when the list differs.
		 * @param buttons Their identifiers, in action order.
		 * @return This component, for chaining.
		 */
		FloatingActionButtonMenu &setActionButtons(
			const std::vector<ecs::Entity::Identifier> &buttons);
	};

}	 // namespace guillaume::components
