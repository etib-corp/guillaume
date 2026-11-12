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
	 * @brief Configuration of a snackbar.
	 *
	 * This component is the whole state of a snackbar. systems::Snackbar
	 * turns it into the bar's Layout, Color, Borders, Elevation, Overlay and
	 * Accessibility, and into the color of its message and action children.
	 */
	class Snackbar: public ecs::Component
	{
		public:
		/**
		 * @brief Snackbar variant.
		 */
		enum class Variant { SingleLine, MultiLine, WithAction };

		/// Called when the snackbar is dismissed or its action is pressed.
		using Handler = std::function<void(void)>;

		/// Paper `Snackbar.DURATION_SHORT`, in seconds.
		static constexpr float DurationShort = 4.0f;

		/// Paper `Snackbar.DURATION_MEDIUM` (the default), in seconds.
		static constexpr float DurationMedium = 7.0f;

		/// Paper `Snackbar.DURATION_LONG`, in seconds.
		static constexpr float DurationLong = 10.0f;

		private:
		Variant _variant { Variant::SingleLine };	 ///< Snackbar variant
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
		 * @brief Default constructor.
		 */
		Snackbar(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Snackbar(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Snackbar &setVariant(Variant variant);

		/** @brief Get the message text. @return The message. */
		const std::string &getMessage(void) const noexcept;

		/** @brief Set the message text. @param message It. @return It. */
		Snackbar &setMessage(const std::string &message);

		/** @brief Get the action label. @return The label. */
		const std::string &getAction(void) const noexcept;

		/** @brief Set the action label. @param action It. @return It. */
		Snackbar &setAction(const std::string &action);

		/** @brief Get the message child. @return Its identifier. */
		ecs::Entity::Identifier getMessageEntity(void) const noexcept;

		/** @brief Set the message child. @param identifier It. @return It. */
		Snackbar &setMessageEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the action child. @return Its identifier. */
		ecs::Entity::Identifier getActionEntity(void) const noexcept;

		/** @brief Set the action child. @param identifier It. @return It. */
		Snackbar &setActionEntity(ecs::Entity::Identifier identifier);

		/** @brief Check the visibility. @return True when shown. */
		bool isVisible(void) const noexcept;

		/** @brief Set the visibility. @param visible It. @return It. */
		Snackbar &setVisible(bool visible);

		/** @brief Get the display time. @return Seconds (0: never). */
		float getDuration(void) const noexcept;

		/**
		 * @brief Set the display time.
		 * @param seconds Seconds before auto-dismiss (0: never).
		 * @return Reference to this component for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Snackbar &setDuration(float seconds);

		/** @brief Get the dismiss callback. @return The callback. */
		const Handler &getOnDismiss(void) const noexcept;

		/** @brief Set the dismiss callback. @param handler It. @return It. */
		Snackbar &setOnDismiss(Handler handler);

		/** @brief Get the action callback. @return The callback. */
		const Handler &getOnAction(void) const noexcept;

		/** @brief Set the action callback. @param handler It. @return It. */
		Snackbar &setOnAction(Handler handler);
	};

}	 // namespace guillaume::components
