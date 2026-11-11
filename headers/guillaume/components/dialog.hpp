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
	 * @brief State of a dialog: a modal panel with an optional icon, a
	 * title, a message and a row of text buttons.
	 *
	 * The entities::Dialog widget writes it: the content, and the
	 * identifiers of the children it builds (icon, title, message, action
	 * buttons, action row, backdrop). systems::Dialog reads it every frame
	 * in the Layout phase and turns it into the panel's Layout, Color,
	 * Borders, Elevation, Overlay flags and Accessibility, into the text of
	 * the title and message children, and into the state of the backdrop.
	 *
	 * Whether the dialog is shown, and the callback run when it is
	 * dismissed, are not stored here: they are in the components::Overlay
	 * of the same entity (see systems::Dialog::show and
	 * systems::Dialog::hide).
	 *
	 * @see systems::Dialog, components::Overlay, entities::Dialog
	 */
	class Dialog: public ecs::Component
	{
		public:
		/**
		 * @brief Kind of dialog.
		 *
		 * systems::Dialog treats two of them specially. A Confirmation
		 * dialog shows an "info" icon when no icon is set. A FullScreen
		 * dialog has a fixed height, square corners and no backdrop, and a
		 * click outside does not dismiss it. Alert and Simple are styled
		 * alike.
		 */
		enum class Variant { Alert, Simple, Confirmation, FullScreen };

		/**
		 * @brief One action of the dialog, drawn as a text button in the
		 * action row.
		 */
		struct Action {
			std::string label;					  ///< Text of the button.
			std::function<void(void)> onPress;	  ///< Called on press; may be
												  ///< empty.
		};

		/// Width of the dialog panel, in dp.
		static constexpr float Width = 320.0f;

		/// Height of a full-screen dialog, in dp.
		static constexpr float FullScreenHeight = 640.0f;

		private:
		Variant _variant { Variant::Alert };	///< Kind of dialog
		std::string _title {};					///< Dialog title
		std::string _message {};				///< Dialog message
		std::vector<Action> _actions {};		///< Dialog actions
		std::string _icon {};					///< Hero icon glyph
		ecs::Entity::Identifier _iconEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Hero icon child

		ecs::Entity::Identifier _titleEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Title child

		ecs::Entity::Identifier _messageEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Message child

		std::vector<ecs::Entity::Identifier>
			_actionEntities {};	   ///< Action button children
		ecs::Entity::Identifier _actionRow {
			ecs::Entity::InvalidIdentifier
		};	  ///< Right-aligned action row child

		ecs::Entity::Identifier _backdrop {
			ecs::Entity::InvalidIdentifier
		};	  ///< Modal backdrop child

		public:
		/**
		 * @brief Create the configuration of an empty alert dialog.
		 */
		Dialog(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Dialog(void) = default;

		/**
		 * @brief Get the kind of dialog.
		 * @return The variant, Variant::Alert by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the kind of dialog.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		Dialog &setVariant(Variant variant);

		/**
		 * @brief Get the title (headline) of the dialog.
		 * @return The title; it is also the dialog's accessible name.
		 */
		const std::string &getTitle(void) const noexcept;

		/**
		 * @brief Set the title (headline) of the dialog.
		 *
		 * systems::Dialog copies it to the title child. Marks the component
		 * changed only when the text differs.
		 * @param title The new title.
		 * @return This component, for chaining.
		 */
		Dialog &setTitle(const std::string &title);

		/**
		 * @brief Get the message (supporting text) of the dialog.
		 * @return The message.
		 */
		const std::string &getMessage(void) const noexcept;

		/**
		 * @brief Set the message (supporting text) of the dialog.
		 *
		 * systems::Dialog copies it to the message child. Marks the
		 * component changed only when the text differs.
		 * @param message The new message.
		 * @return This component, for chaining.
		 */
		Dialog &setMessage(const std::string &message);

		/**
		 * @brief Get the actions of the dialog.
		 * @return The actions, in the order of their buttons.
		 */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Replace the actions of the dialog.
		 *
		 * The press handlers are always replaced. The component is marked
		 * changed only when the labels differ, because handlers cannot be
		 * compared. This does not build the buttons; the widget does.
		 * @param actions The new actions, in the order of their buttons.
		 * @return This component, for chaining.
		 */
		Dialog &setActions(std::vector<Action> actions);

		/**
		 * @brief Get the icon chosen for the top of the dialog.
		 * @return The glyph name; empty when none was set. See
		 * getEffectiveIcon() for the icon actually shown.
		 */
		const std::string &getIcon(void) const noexcept;

		/**
		 * @brief Choose the icon shown above the title (the hero icon).
		 *
		 * With a hero icon, the icon and the title are centered. Marks the
		 * component changed only when the name differs.
		 * @param icon The glyph name; empty for the variant's default.
		 * @return This component, for chaining.
		 */
		Dialog &setIcon(const std::string &icon);

		/**
		 * @brief Get the icon the dialog actually shows.
		 * @return The icon set with setIcon(), "info" for a Confirmation
		 * dialog without one, or an empty string when there is no hero
		 * icon.
		 */
		std::string getEffectiveIcon(void) const;

		/**
		 * @brief Get the child that draws the hero icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * dialog has none.
		 */
		ecs::Entity::Identifier getIconEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the hero icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Dialog &setIconEntity(ecs::Entity::Identifier identifier);

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
		Dialog &setTitleEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the message.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getMessageEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the message.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Dialog &setMessageEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the text buttons built for the actions.
		 * @return Their identifiers, in action order.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getActionEntities(void) const noexcept;

		/**
		 * @brief Record the text buttons built for the actions.
		 *
		 * Marks the component changed only when the list differs.
		 * @param identifiers Their identifiers, in action order.
		 * @return This component, for chaining.
		 */
		Dialog &setActionEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/**
		 * @brief Get the row that holds the action buttons, at the bottom
		 * of the panel.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getActionRow(void) const noexcept;

		/**
		 * @brief Record the row that holds the action buttons.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The row's identifier.
		 * @return This component, for chaining.
		 */
		Dialog &setActionRow(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that dims the content behind the dialog.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getBackdrop(void) const noexcept;

		/**
		 * @brief Record the child that dims the content behind the dialog.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The backdrop's identifier.
		 * @return This component, for chaining.
		 */
		Dialog &setBackdrop(ecs::Entity::Identifier identifier);
	};

}	 // namespace guillaume::components
