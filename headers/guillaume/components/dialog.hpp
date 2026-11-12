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
	 * @brief Configuration of a dialog.
	 *
	 * This component is the whole state of a dialog, apart from its
	 * visibility and dismiss callback, which live in components::Overlay.
	 * systems::Dialog turns it into the panel's Layout, Color, Borders,
	 * Elevation, Overlay and Accessibility, into the content and color of
	 * its icon, title and message children, and into the state of its
	 * backdrop.
	 */
	class Dialog: public ecs::Component
	{
		public:
		/**
		 * @brief Dialog variant.
		 */
		enum class Variant { Alert, Simple, Confirmation, FullScreen };

		/**
		 * @brief One dialog action, rendered as a text button.
		 */
		struct Action {
			std::string label;					  ///< Button label.
			std::function<void(void)> onPress;	  ///< Press handler.
		};

		/// M3 dialog width, in dp (basic dialogs are 280-560 dp wide).
		static constexpr float Width = 320.0f;

		/// Height of a full-screen dialog, in dp.
		static constexpr float FullScreenHeight = 640.0f;

		private:
		Variant _variant { Variant::Alert };	///< Dialog variant
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
		 * @brief Default constructor.
		 */
		Dialog(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Dialog(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Dialog &setVariant(Variant variant);

		/** @brief Get the title. @return The title. */
		const std::string &getTitle(void) const noexcept;

		/** @brief Set the title. @param title It. @return It. */
		Dialog &setTitle(const std::string &title);

		/** @brief Get the message. @return The message. */
		const std::string &getMessage(void) const noexcept;

		/** @brief Set the message. @param message It. @return It. */
		Dialog &setMessage(const std::string &message);

		/** @brief Get the actions. @return The actions. */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Replace the actions.
		 *
		 * The component is marked changed only when the labels differ; the
		 * press handlers are always replaced.
		 * @param actions The new actions.
		 * @return Reference to this component for chaining.
		 */
		Dialog &setActions(std::vector<Action> actions);

		/** @brief Get the hero icon glyph. @return The glyph name. */
		const std::string &getIcon(void) const noexcept;

		/** @brief Set the hero icon glyph. @param icon It. @return It. */
		Dialog &setIcon(const std::string &icon);

		/**
		 * @brief Get the glyph the hero icon shows.
		 * @return The configured glyph, "info" for a Confirmation dialog
		 * without one, or an empty string when there is no hero icon.
		 */
		std::string getEffectiveIcon(void) const;

		/** @brief Get the hero icon child. @return Its identifier. */
		ecs::Entity::Identifier getIconEntity(void) const noexcept;

		/** @brief Set the hero icon child. @param identifier It. @return It. */
		Dialog &setIconEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the title child. @return Its identifier. */
		ecs::Entity::Identifier getTitleEntity(void) const noexcept;

		/** @brief Set the title child. @param identifier It. @return It. */
		Dialog &setTitleEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the message child. @return Its identifier. */
		ecs::Entity::Identifier getMessageEntity(void) const noexcept;

		/** @brief Set the message child. @param identifier It. @return It. */
		Dialog &setMessageEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the action buttons. @return Their identifiers. */
		const std::vector<ecs::Entity::Identifier> &
			getActionEntities(void) const noexcept;

		/** @brief Set the action buttons. @param identifiers They. @return It.
		 */
		Dialog &setActionEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/** @brief Get the action row child. @return Its identifier. */
		ecs::Entity::Identifier getActionRow(void) const noexcept;

		/** @brief Set the action row child. @param identifier It. @return It.
		 */
		Dialog &setActionRow(ecs::Entity::Identifier identifier);

		/** @brief Get the backdrop child. @return Its identifier. */
		ecs::Entity::Identifier getBackdrop(void) const noexcept;

		/** @brief Set the backdrop child. @param identifier It. @return It. */
		Dialog &setBackdrop(ecs::Entity::Identifier identifier);
	};

}	 // namespace guillaume::components
