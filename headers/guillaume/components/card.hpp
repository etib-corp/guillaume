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
	 * @brief State of a card: a surface grouping an optional cover image,
	 * title, text and row of action buttons.
	 *
	 * It holds the variant, the content (cover path, title, subtitle,
	 * text, actions), the identifiers of the children that draw that
	 * content, the press callback and the progress of the press motion.
	 * Whether the card is enabled lives in the components::StyleState of
	 * the same entity.
	 *
	 * Role in the frame:
	 * - entities::Card writes the content, creates the children and records
	 *   their identifiers here.
	 * - systems::Card (Layout phase) reads it and writes the card's Color,
	 *   Borders and Elevation, copies the texts and the cover path to the
	 *   children, and stacks the sections top to bottom: cover, title,
	 *   content, actions. On input it writes the pressed flag and, through
	 *   the entity's components::Animation, the press progress.
	 *
	 * Watch out:
	 * - To make a card pressable call systems::Card::setOnPress(), not the
	 *   setter here: only the system wires the input.
	 * - The press fields (pressed, press progress, press from) belong to
	 *   systems::Card and are overwritten while the motion runs.
	 *
	 * Unless a member says otherwise, a setter compares before writing and
	 * marks the component changed only when the value differs.
	 *
	 * @see systems::Card, components::Animation, components::Layout
	 */
	class Card: public ecs::Component
	{
		public:
		/**
		 * @brief Look of the card surface.
		 */
		enum class Variant {
			/// Tonal level-1 surface with a shadow; rises to level 2 while
			/// pressed.
			Elevated,
			Filled,		///< Surface-variant fill, no shadow, no outline.
			Outlined	///< Surface fill with a 1 dp outline-variant stroke.
		};

		/**
		 * @brief One button of the actions row: its label and what it does.
		 */
		struct Action {
			std::string label;	  ///< Button label.
			std::function<void(void)>
				onPress;	///< Called when the button is clicked.
		};

		/// Callback run when the card is pressed.
		using Handler = std::function<void(void)>;

		/// List of child entity identifiers.
		using Identifiers = std::vector<ecs::Entity::Identifier>;

		/**
		 * @brief Height of the cover image, in dp.
		 */
		static constexpr float CoverHeight = 195.0f;

		/**
		 * @brief Minimum height of the title section, in dp.
		 */
		static constexpr float TitleMinHeight = 72.0f;

		private:
		Variant _variant { Variant::Elevated };	   ///< Card variant
		bool _interactive { false };			   ///< Whether pressable
		bool _pressed { false };				   ///< Last pressed state
		float _pressProgress { 0.0f };	  ///< Press motion progress, 0 to 1
		float _pressFrom { 0.0f };		  ///< Progress when the motion began
		std::string _coverPath {};		  ///< Cover texture path
		std::string _title {};			  ///< Title text
		std::string _subtitle {};		  ///< Subtitle text
		std::string _contentText {};	  ///< Content text
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
		 * @brief Create the state of an empty, elevated card that cannot be
		 * pressed.
		 */
		Card(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Card(void) = default;

		/**
		 * @brief Get the look of the card surface.
		 * @return The variant (Elevated by default).
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose the look of the card surface.
		 * @param variant Elevated, Filled or Outlined.
		 * @return This component, for chaining.
		 */
		Card &setVariant(Variant variant);

		/**
		 * @brief Check whether the card reacts to hover and press.
		 * @return True once systems::Card::setOnPress() wired the input.
		 */
		bool isInteractive(void) const noexcept;

		/**
		 * @brief Record that the card reacts to hover and press.
		 *
		 * An interactive card blends the hover and press state layers over
		 * its container color. Written by systems::Card::setOnPress(); the
		 * flag alone does not wire any input.
		 * @param interactive True when the input is wired.
		 * @return This component, for chaining.
		 */
		Card &setInteractive(bool interactive);

		/**
		 * @brief Check whether the press motion is heading to the raised
		 * state.
		 * @return True while an elevated card is held down.
		 */
		bool isPressed(void) const noexcept;

		/**
		 * @brief Record the pressed state the press motion heads to.
		 *
		 * Written by systems::Card when the input changes; only an Elevated
		 * card is ever recorded as pressed.
		 * @param pressed True when the card is held down.
		 * @return This component, for chaining.
		 */
		Card &setPressed(bool pressed);

		/**
		 * @brief Get how far the card is raised by a press.
		 * @return The progress, 0 at rest to 1 fully pressed.
		 */
		float getPressProgress(void) const noexcept;

		/**
		 * @brief Set how far the card is raised by a press.
		 *
		 * Written by systems::Card on each sample of the press animation.
		 * An Elevated card blends its surface tone and shadow between
		 * elevation levels 1 and 2 by this amount.
		 * @param pressProgress The progress, 0 at rest to 1 fully pressed.
		 * @return This component, for chaining.
		 */
		Card &setPressProgress(float pressProgress);

		/**
		 * @brief Get the press progress at which the running motion began.
		 * @return The starting progress, 0 to 1.
		 */
		float getPressFrom(void) const noexcept;

		/**
		 * @brief Record the press progress at which a motion begins.
		 *
		 * Written by systems::Card when the pressed state flips, so a press
		 * released half-way animates back from where it was.
		 * @param pressFrom The starting progress, 0 to 1.
		 * @return This component, for chaining.
		 */
		Card &setPressFrom(float pressFrom);

		/**
		 * @brief Get the path of the cover texture.
		 * @return The path, empty when the card has no cover.
		 */
		const std::string &getCoverPath(void) const noexcept;

		/**
		 * @brief Set the path of the cover texture.
		 *
		 * The system copies it to the Image of the cover child.
		 * @param coverPath The path of the image file.
		 * @return This component, for chaining.
		 */
		Card &setCoverPath(const std::string &coverPath);

		/**
		 * @brief Get the title text.
		 * @return The text, empty when the card has no title.
		 */
		const std::string &getTitle(void) const noexcept;

		/**
		 * @brief Set the title text.
		 *
		 * The system copies it to the title text child and to the
		 * Accessibility label of the title section.
		 * @param title The title text.
		 * @return This component, for chaining.
		 */
		Card &setTitle(const std::string &title);

		/**
		 * @brief Get the subtitle text.
		 * @return The text, empty when the card has no subtitle.
		 */
		const std::string &getSubtitle(void) const noexcept;

		/**
		 * @brief Set the subtitle text.
		 *
		 * The system copies it to the subtitle text child.
		 * @param subtitle The subtitle text, shown under the title.
		 * @return This component, for chaining.
		 */
		Card &setSubtitle(const std::string &subtitle);

		/**
		 * @brief Get the content text.
		 * @return The text, empty when the card has no content text.
		 */
		const std::string &getContentText(void) const noexcept;

		/**
		 * @brief Set the content text.
		 *
		 * The system copies it to the content text child.
		 * @param contentText The body text of the card.
		 * @return This component, for chaining.
		 */
		Card &setContentText(const std::string &contentText);

		/**
		 * @brief Get the child entity that draws the cover image.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * card has no cover.
		 */
		ecs::Entity::Identifier getCover(void) const noexcept;

		/**
		 * @brief Record which child entity draws the cover image.
		 * @param identifier The cover child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Card &setCover(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that contains the title and the
		 * subtitle.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * card has no title section.
		 */
		ecs::Entity::Identifier getTitleSection(void) const noexcept;

		/**
		 * @brief Record which child entity contains the title and the
		 * subtitle.
		 * @param identifier The title section child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Card &setTitleSection(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the entity that draws the title text.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * card has no title.
		 */
		ecs::Entity::Identifier getTitleText(void) const noexcept;

		/**
		 * @brief Record which entity draws the title text.
		 * @param identifier The title text entity, a child of the title
		 * section, or ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Card &setTitleText(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the entity that draws the subtitle text.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * card has no subtitle.
		 */
		ecs::Entity::Identifier getSubtitleText(void) const noexcept;

		/**
		 * @brief Record which entity draws the subtitle text.
		 * @param identifier The subtitle text entity, a child of the title
		 * section, or ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Card &setSubtitleText(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that draws the content text.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * card has no content text.
		 */
		ecs::Entity::Identifier getContentEntity(void) const noexcept;

		/**
		 * @brief Record which child entity draws the content text.
		 * @param identifier The content text child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Card &setContentEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child entity that contains the action buttons.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when the
		 * card has no actions row.
		 */
		ecs::Entity::Identifier getActionsSection(void) const noexcept;

		/**
		 * @brief Record which child entity contains the action buttons.
		 * @param identifier The actions row child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Card &setActionsSection(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the actions the card was asked to show.
		 * @return The actions, in the order they were added.
		 */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Append an action to the actions row.
		 *
		 * Always marks the component changed. The widget class creates the
		 * button of each action from this list.
		 * @param action The label and the click handler of the new button.
		 * @return This component, for chaining.
		 */
		Card &addAction(Action action);

		/**
		 * @brief Get the entities of the action buttons.
		 * @return Their identifiers, in action order.
		 */
		const Identifiers &getActionButtons(void) const noexcept;

		/**
		 * @brief Record which entities are the action buttons.
		 *
		 * The system arranges them inside the actions row.
		 * @param identifiers Their identifiers, in action order.
		 * @return This component, for chaining.
		 */
		Card &setActionButtons(const Identifiers &identifiers);

		/**
		 * @brief Get the callback run when the card is pressed.
		 * @return The callback, empty when none is set.
		 */
		const Handler &getOnPress(void) const noexcept;

		/**
		 * @brief Store the callback run when the card is pressed.
		 *
		 * It only stores the callback and does not mark the component
		 * changed. Use systems::Card::setOnPress() so the card also starts
		 * reacting to input.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		Card &setOnPress(Handler handler);
	};

}	 // namespace guillaume::components
