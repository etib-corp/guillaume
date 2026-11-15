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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/floating_action_button_menu.hpp"
#include "guillaume/components/layout.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A floating action button menu (speed dial): a trigger FAB that
	 * shows a column of smaller action buttons above it when opened.
	 *
	 * Its state lives in components::FloatingActionButtonMenu (trigger icon,
	 * actions, open flag, callback, child identifiers);
	 * systems::FloatingActionButtonMenu shows or hides the action buttons,
	 * swaps the trigger icon for a close glyph while open and fills the
	 * column each frame, in the Layout phase. systems::Layout then places
	 * the column. The class itself holds no field: open(), close() and
	 * toggle() call the system.
	 *
	 * @code
	 * FloatingActionButtonMenu::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto menu = builder.addAction("mail", "Send", [] {
	 * 		// called when the user clicks the "Send" action
	 * 	})
	 * 	.addAction("alarm", [] {})
	 * 	.withAccessibilityLabel("Actions")
	 * 	.makeFloatingActionButtonMenu(parent, "add");
	 * menu->setOnStateChange([](bool open) {
	 * 	// called when the menu opens or closes
	 * });
	 * @endcode
	 *
	 * `FloatingActionButtonMenu::create(registry, iconGlyphName, actions,
	 * accessibilityLabel)` builds the same widget without registering it in
	 * an entity registry.
	 *
	 * The menu owns its trigger and action buttons: it is the entity
	 * registry they are registered in. The actions are fixed at creation.
	 * Clicking an action calls its handler and leaves the menu open; call
	 * close() from the handler to close it. open(), close() and toggle()
	 * call the state-change handler like a click on the trigger does. The
	 * click handler of the trigger and the menu's Layout configuration are
	 * set by the system; do not replace them.
	 *
	 * @see components::FloatingActionButtonMenu,
	 * systems::FloatingActionButtonMenu, FloatingActionButton
	 */
	class FloatingActionButtonMenu:
		public std::enable_shared_from_this<FloatingActionButtonMenu>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound,
									   components::Layout,
									   components::FloatingActionButtonMenu>
	{
		public:
		/// One action of the menu: a label (may be empty), an icon glyph
		/// name and a click handler.
		using Action = components::FloatingActionButtonMenu::Action;

		/**
		 * @brief Configures, creates and registers FloatingActionButtonMenu
		 * widgets.
		 *
		 * Add the actions with addAction(), then call
		 * makeFloatingActionButtonMenu() or registerEntity().
		 */
		class Builder: public EntityBuilderBase<FloatingActionButtonMenu>
		{
			private:
			std::string _iconGlyphName;	   ///< Trigger icon glyph name
			std::vector<Action>
				_actions;	 ///< Actions, in the order they were added
			std::string _accessibilityLabel;	///< Trigger accessibility label

			public:
			/**
			 * @brief Construct a builder with the default configuration (see
			 * reset()).
			 * @param componentRegistry The registry that stores the
			 * components of the menus to build.
			 * @param entityRegistry The registry the built menus are added
			 * to (usually the scene's).
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a FloatingActionButtonMenu, with its trigger and
			 * action buttons, from the current configuration, without
			 * registering it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * menu and resets the builder.
			 * @return The new menu.
			 */
			std::shared_ptr<FloatingActionButtonMenu>
				buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: empty trigger icon, no action, empty accessibility
			 * label.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon of the trigger while the menu is closed.
			 * @param iconGlyphName The glyph name of the trigger icon (for
			 * example "add"). Default: empty.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyphName);

			/**
			 * @brief Add an action shown as a small icon-only FAB.
			 * @param iconGlyphName The glyph name of the action icon.
			 * @param onClick The function called when the user clicks the
			 * action (may be empty).
			 * @return Reference to the builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName,
							   std::function<void(void)> onClick);

			/**
			 * @brief Add an action shown with a label next to its icon (Paper
			 * FAB.Group `label`).
			 * @param iconGlyphName The glyph name of the action icon.
			 * @param label The text shown next to the icon, also used as the
			 * accessibility label. When empty, the action is icon-only.
			 * @param onClick The function called when the user clicks the
			 * action (may be empty).
			 * @return Reference to the builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName,
							   const std::string &label,
							   std::function<void(void)> onClick);

			/**
			 * @brief Set the label of the trigger read by assistive
			 * technology.
			 * @param accessibilityLabel The accessibility label of the
			 * trigger. Default: empty.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withAccessibilityLabel(const std::string &accessibilityLabel);

			/**
			 * @brief Build a menu with the actions added so far, register it
			 * under `parent` and reset the builder.
			 * @param parent The entity the menu becomes a child of.
			 * @param iconGlyphName The glyph name of the trigger icon while
			 * the menu is closed.
			 * @return The new menu, closed and registered in the entity
			 * registry.
			 */
			std::shared_ptr<FloatingActionButtonMenu>
				makeFloatingActionButtonMenu(std::shared_ptr<Entity> parent,
											 const std::string &iconGlyphName);
		};

		/**
		 * @brief Create a menu with its trigger and action buttons, without
		 * registering it in an entity registry.
		 *
		 * The constructor is private because the buttons need a menu already
		 * held by a `std::shared_ptr` as parent: create() constructs the
		 * menu then calls build(), which creates them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, iconGlyphName, actions, accessibilityLabel).
		 * @return The new menu, closed.
		 */
		template<typename... Arguments>
		static std::shared_ptr<FloatingActionButtonMenu>
			create(Arguments &&...arguments)
		{
			std::shared_ptr<FloatingActionButtonMenu> entity(
				new FloatingActionButtonMenu(
					std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~FloatingActionButtonMenu(void);

		/**
		 * @brief Open the menu: show the action buttons and the close glyph
		 * on the trigger, at once.
		 *
		 * Calls the state-change handler with `true`. Does nothing when the
		 * menu is already open.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &open(void);

		/**
		 * @brief Close the menu: hide the action buttons and restore the
		 * trigger icon, at once.
		 *
		 * Calls the state-change handler with `false`. Does nothing when the
		 * menu is already closed.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &close(void);

		/**
		 * @brief Open the menu when it is closed, close it when it is open.
		 *
		 * Calls the state-change handler with the new state. A click on the
		 * trigger does the same.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &toggle(void);

		/**
		 * @brief Get whether the menu is open.
		 * @return True when the action buttons are shown. A new menu is
		 * closed.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Set the handler called when the menu opens or closes (Paper
		 * `onStateChange`).
		 *
		 * It is called after the change, by a click on the trigger and by
		 * open(), close() and toggle().
		 * @param onStateChange The function that receives the new open state
		 * (true when open). Pass an empty function to remove the handler.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &
			setOnStateChange(std::function<void(bool)> onStateChange);

		/**
		 * @brief Get the number of actions.
		 * @return The number of action buttons, shown or not.
		 */
		std::size_t getActionCount(void) const;

		/**
		 * @brief Get the identifier of the trigger button, a
		 * FloatingActionButton child of the menu.
		 * @return The trigger identifier, or ecs::Entity::InvalidIdentifier
		 * before the children are built.
		 */
		ecs::Entity::Identifier getTriggerIdentifier(void) const;

		/**
		 * @brief Get the identifier of the button of one action.
		 * @param index The index of the action, in the order the actions
		 * were added (0 is the first).
		 * @return The identifier of the action button, or
		 * ecs::Entity::InvalidIdentifier when @p index is out of range.
		 */
		ecs::Entity::Identifier getActionIdentifier(std::size_t index) const;

		private:
		/**
		 * @brief Access the menu state.
		 * @return The components::FloatingActionButtonMenu of this entity.
		 */
		components::FloatingActionButtonMenu &state(void);

		/**
		 * @brief Access the menu state, read-only.
		 * @return The components::FloatingActionButtonMenu of this entity.
		 */
		const components::FloatingActionButtonMenu &state(void) const;

		private:
		/**
		 * @brief Construct a menu without its buttons; create() calls it
		 * then build().
		 * @param registry The registry that stores the menu's components.
		 * @param iconGlyphName The glyph name of the trigger icon while the
		 * menu is closed.
		 * @param actions The actions, in display order. One button is built
		 * for each.
		 * @param accessibilityLabel The label of the trigger read by
		 * assistive technology.
		 */
		FloatingActionButtonMenu(ecs::ComponentRegistry &registry,
								 const std::string &iconGlyphName,
								 const std::vector<Action> &actions,
								 const std::string &accessibilityLabel);

		/**
		 * @brief Create the trigger and one button per action, then wire
		 * their clicks through systems::FloatingActionButtonMenu::install.
		 *
		 * The trigger is a medium primary FloatingActionButton. An action
		 * without label is a small surface FloatingActionButton; an action
		 * with a label is a small surface ExtendedFloatingActionButton.
		 * Called once by create(), when the menu is held by a
		 * `std::shared_ptr` and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
