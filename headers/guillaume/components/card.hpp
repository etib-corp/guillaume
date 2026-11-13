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
	 * @brief Configuration and press state of a card.
	 *
	 * This component is the whole state of a card, apart from its enabled
	 * state (components::StyleState). systems::Card turns it into the card's
	 * Color, Borders, Elevation, Layout and Bound, and into the content and
	 * arrangement of its cover, title, content and actions children.
	 */
	class Card: public ecs::Component
	{
		public:
		/**
		 * @brief Card variants.
		 */
		enum class Variant {
			Elevated,	 ///< Tonal level-1 surface with shadow.
			Filled,		 ///< Paper `contained`: surfaceVariant.
			Outlined	 ///< Surface with an outlineVariant stroke.
		};

		/**
		 * @brief Composition data of one action of the `Card.Actions` row.
		 */
		struct Action {
			std::string label;					  ///< Button label.
			std::function<void(void)> onPress;	  ///< Button handler.
		};

		/// Called when the card is pressed.
		using Handler = std::function<void(void)>;

		/// Child identifiers.
		using Identifiers = std::vector<ecs::Entity::Identifier>;

		/**
		 * @brief Height of `Card.Cover` (Paper: 195 dp).
		 */
		static constexpr float CoverHeight = 195.0f;

		/**
		 * @brief Minimum height of `Card.Title` (Paper: 72 dp).
		 */
		static constexpr float TitleMinHeight = 72.0f;

		private:
		Variant _variant { Variant::Elevated };	   ///< Card variant
		bool _interactive { false };			   ///< Whether pressable
		bool _pressed { false };				   ///< Last pressed state
		float _pressProgress { 0.0f };			   ///< Elevation progress
		float _pressFrom { 0.0f };		///< Progress when the motion began
		std::string _coverPath {};		///< Cover texture
		std::string _title {};			///< Title text
		std::string _subtitle {};		///< Subtitle text
		std::string _contentText {};	///< Content text
		ecs::Entity::Identifier _cover {
			ecs::Entity::InvalidIdentifier
		};	  ///< Cover image child

		ecs::Entity::Identifier _titleSection {
			ecs::Entity::InvalidIdentifier
		};	  ///< Title section child

		ecs::Entity::Identifier _titleText {
			ecs::Entity::InvalidIdentifier
		};	  ///< Title text child

		ecs::Entity::Identifier _subtitleText {
			ecs::Entity::InvalidIdentifier
		};	  ///< Subtitle text child

		ecs::Entity::Identifier _contentEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Content text child

		ecs::Entity::Identifier _actionsSection {
			ecs::Entity::InvalidIdentifier
		};	  ///< Actions row child

		std::vector<Action> _actions {};	///< Requested actions
		Identifiers _actionButtons {};		///< Action button children
		Handler _onPress {};				///< Press handler

		public:
		/**
		 * @brief Default constructor.
		 */
		Card(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Card(void) = default;

		/** @brief Get the variant. @return It. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return This. */
		Card &setVariant(Variant variant);

		/** @brief Get the interactive flag. @return It. */
		bool isInteractive(void) const noexcept;

		/** @brief Set the interactive flag. @param interactive It. @return
		 * This. */
		Card &setInteractive(bool interactive);

		/** @brief Get the last pressed state. @return It. */
		bool isPressed(void) const noexcept;

		/** @brief Set the last pressed state. @param pressed It. @return This.
		 */
		Card &setPressed(bool pressed);

		/** @brief Get the press-elevation progress (0 at rest, 1 pressed).
		 * @return It. */
		float getPressProgress(void) const noexcept;

		/** @brief Set the press-elevation progress (0 at rest, 1 pressed).
		 * @param pressProgress It. @return This. */
		Card &setPressProgress(float pressProgress);

		/** @brief Get the progress at which the press motion began. @return It.
		 */
		float getPressFrom(void) const noexcept;

		/** @brief Set the progress at which the press motion began. @param
		 * pressFrom It. @return This. */
		Card &setPressFrom(float pressFrom);

		/** @brief Get the cover texture path. @return It. */
		const std::string &getCoverPath(void) const noexcept;

		/** @brief Set the cover texture path. @param coverPath It. @return
		 * This. */
		Card &setCoverPath(const std::string &coverPath);

		/** @brief Get the title text. @return It. */
		const std::string &getTitle(void) const noexcept;

		/** @brief Set the title text. @param title It. @return This. */
		Card &setTitle(const std::string &title);

		/** @brief Get the subtitle text. @return It. */
		const std::string &getSubtitle(void) const noexcept;

		/** @brief Set the subtitle text. @param subtitle It. @return This. */
		Card &setSubtitle(const std::string &subtitle);

		/** @brief Get the content text. @return It. */
		const std::string &getContentText(void) const noexcept;

		/** @brief Set the content text. @param contentText It. @return This. */
		Card &setContentText(const std::string &contentText);

		/** @brief Get the cover image child. @return Its identifier. */
		ecs::Entity::Identifier getCover(void) const noexcept;

		/** @brief Set the cover image child. @param identifier It. @return
		 * This.
		 */
		Card &setCover(ecs::Entity::Identifier identifier);

		/** @brief Get the title section child. @return Its identifier. */
		ecs::Entity::Identifier getTitleSection(void) const noexcept;

		/** @brief Set the title section child. @param identifier It. @return
		 * This.
		 */
		Card &setTitleSection(ecs::Entity::Identifier identifier);

		/** @brief Get the title text child. @return Its identifier. */
		ecs::Entity::Identifier getTitleText(void) const noexcept;

		/** @brief Set the title text child. @param identifier It. @return This.
		 */
		Card &setTitleText(ecs::Entity::Identifier identifier);

		/** @brief Get the subtitle text child. @return Its identifier. */
		ecs::Entity::Identifier getSubtitleText(void) const noexcept;

		/** @brief Set the subtitle text child. @param identifier It. @return
		 * This.
		 */
		Card &setSubtitleText(ecs::Entity::Identifier identifier);

		/** @brief Get the content text child. @return Its identifier. */
		ecs::Entity::Identifier getContentEntity(void) const noexcept;

		/** @brief Set the content text child. @param identifier It. @return
		 * This.
		 */
		Card &setContentEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the actions row child. @return Its identifier. */
		ecs::Entity::Identifier getActionsSection(void) const noexcept;

		/** @brief Set the actions row child. @param identifier It. @return
		 * This.
		 */
		Card &setActionsSection(ecs::Entity::Identifier identifier);

		/** @brief Get the requested actions. @return The actions. */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Append a requested action.
		 * @param action The action to append.
		 * @return Reference to this component for chaining.
		 */
		Card &addAction(Action action);

		/** @brief Get the action button children. @return Their identifiers.
		 */
		const Identifiers &getActionButtons(void) const noexcept;

		/** @brief Set the action button children. @param identifiers They.
		 * @return This. */
		Card &setActionButtons(const Identifiers &identifiers);

		/** @brief Get the press handler. @return The handler. */
		const Handler &getOnPress(void) const noexcept;

		/** @brief Set the press handler. @param handler It. @return This. */
		Card &setOnPress(Handler handler);
	};

}	 // namespace guillaume::components
