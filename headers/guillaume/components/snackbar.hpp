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
	 * @brief State of a snackbar: a short message at the bottom of the
	 * screen, with an optional action, that goes away by itself.
	 *
	 * The entities::Snackbar widget writes it: the message, the action
	 * label, the duration, the callbacks and the identifiers of the two
	 * children it builds. systems::Snackbar reads it every frame in the
	 * Layout phase and turns it into the bar's Layout, Color, Borders,
	 * Elevation, Overlay and Accessibility, and into the text and color of
	 * its message child and the color of its action child.
	 *
	 * The visibility flag here is the one to set (through
	 * systems::Snackbar::show and systems::Snackbar::hide). The system
	 * copies it to the components::Overlay of the same entity, whose
	 * countdown then dismisses the bar after the duration. A hidden
	 * snackbar is drawn transparent.
	 *
	 * @see systems::Snackbar, components::Overlay, entities::Snackbar
	 */
	class Snackbar: public ecs::Component
	{
		public:
		/**
		 * @brief Kind of snackbar.
		 *
		 * MultiLine makes the bar 68 dp high; SingleLine and WithAction
		 * are 48 dp high.
		 */
		enum class Variant { SingleLine, MultiLine, WithAction };

		/// Callback that takes no argument, used for dismissal and for the
		/// action.
		using Handler = std::function<void(void)>;

		/// Short display time, in seconds.
		static constexpr float DurationShort = 4.0f;

		/// Medium display time, in seconds. This is the default.
		static constexpr float DurationMedium = 7.0f;

		/// Long display time, in seconds.
		static constexpr float DurationLong = 10.0f;

		private:
		Variant _variant { Variant::SingleLine };	 ///< Kind of snackbar
		std::string _message {};					 ///< Message text
		std::string _action {};						 ///< Action label
		ecs::Entity::Identifier _messageEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Message child

		ecs::Entity::Identifier _actionEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Action child

		bool _visible { true };				   ///< Whether the bar is shown
		float _duration { DurationMedium };	   ///< Display time (0: never)
		Handler _onDismiss {};				   ///< Dismiss callback
		Handler _onAction {};				   ///< Action callback

		public:
		/**
		 * @brief Create the configuration of a visible, empty single-line
		 * snackbar that lasts DurationMedium.
		 */
		Snackbar(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Snackbar(void) = default;

		/**
		 * @brief Get the kind of snackbar.
		 * @return The variant, Variant::SingleLine by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the kind of snackbar, which decides its height.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		Snackbar &setVariant(Variant variant);

		/**
		 * @brief Get the message shown in the bar.
		 * @return The message; it is also the bar's accessible name.
		 */
		const std::string &getMessage(void) const noexcept;

		/**
		 * @brief Set the message shown in the bar.
		 *
		 * systems::Snackbar copies it to the message child. Marks the
		 * component changed only when the text differs.
		 * @param message The new message.
		 * @return This component, for chaining.
		 */
		Snackbar &setMessage(const std::string &message);

		/**
		 * @brief Get the label of the action.
		 * @return The label; empty when the snackbar has no action.
		 */
		const std::string &getAction(void) const noexcept;

		/**
		 * @brief Set the label of the action, such as "Undo".
		 *
		 * The widget uses it when it builds the action child, and it is
		 * the accessible name of that child. Marks the component changed
		 * only when the label differs.
		 * @param action The new label.
		 * @return This component, for chaining.
		 */
		Snackbar &setAction(const std::string &action);

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
		Snackbar &setMessageEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the pressable child that draws the action label.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when
		 * the snackbar has no action.
		 */
		ecs::Entity::Identifier getActionEntity(void) const noexcept;

		/**
		 * @brief Record the pressable child that draws the action label.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Snackbar &setActionEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Tell whether the snackbar is shown.
		 * @return True when shown, which is the default.
		 */
		bool isVisible(void) const noexcept;

		/**
		 * @brief Record whether the snackbar is shown.
		 *
		 * This only stores the flag; the screen follows on the next pass of
		 * systems::Snackbar. Prefer systems::Snackbar::show and
		 * systems::Snackbar::hide, which also restyle the bar at once.
		 * Marks the component changed only when the flag differs.
		 * @param visible True to show the snackbar.
		 * @return This component, for chaining.
		 */
		Snackbar &setVisible(bool visible);

		/**
		 * @brief Get how long the snackbar stays on screen.
		 * @return The display time, in seconds; 0 means it stays until it
		 * is dismissed.
		 */
		float getDuration(void) const noexcept;

		/**
		 * @brief Set how long the snackbar stays on screen once shown.
		 *
		 * Marks the component changed only when the value differs.
		 * @param seconds The display time, in seconds; 0 keeps the bar
		 * until it is dismissed.
		 * @return This component, for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Snackbar &setDuration(float seconds);

		/**
		 * @brief Get the callback run when the snackbar is dismissed.
		 * @return The callback; empty when none is set.
		 */
		const Handler &getOnDismiss(void) const noexcept;

		/**
		 * @brief Set the callback run when the snackbar is dismissed.
		 *
		 * It runs when the display time ends, on Escape, and after the
		 * action is pressed. It does not run when the snackbar is hidden
		 * with systems::Snackbar::hide. Does not mark the component
		 * changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		Snackbar &setOnDismiss(Handler handler);

		/**
		 * @brief Get the callback run when the action is pressed.
		 * @return The callback; empty when none is set.
		 */
		const Handler &getOnAction(void) const noexcept;

		/**
		 * @brief Set the callback run when the action is pressed.
		 *
		 * The snackbar is dismissed right after the callback returns. Does
		 * not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		Snackbar &setOnAction(Handler handler);
	};

}	 // namespace guillaume::components
