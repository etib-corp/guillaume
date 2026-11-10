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

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Snackbar entity: a transient message bar.
	 *
	 * Snackbars reuse `SurfaceBase` for the layout, geometry and background
	 * surface and `components::Overlay` for the transient overlay behaviour
	 * (auto-dismiss). The M3 variants are:
	 * - `SingleLine`: a single message line, height 48.
	 * - `MultiLine`: a taller message bar, height 68.
	 * - `WithAction`: a message line followed by a trailing action, height 48.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 */
	class Snackbar:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Accessibility>
	{
		public:
		/**
		 * @brief Snackbar variant.
		 */
		enum class Variant { SingleLine, MultiLine, WithAction };

		/**
		 * @brief Paper `Snackbar.DURATION_SHORT`, in seconds.
		 */
		static constexpr float DurationShort = 4.0f;

		/**
		 * @brief Paper `Snackbar.DURATION_MEDIUM` (the default), in seconds.
		 */
		static constexpr float DurationMedium = 7.0f;

		/**
		 * @brief Paper `Snackbar.DURATION_LONG`, in seconds.
		 */
		static constexpr float DurationLong = 10.0f;

		/**
		 * @brief Builder used to configure and create `Snackbar` entities.
		 */
		class Builder: public EntityBuilderBase<Snackbar>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SingleLine };	 ///< Snackbar variant.
			std::string _message {};					 ///< Message text.
			std::string _action {};						 ///< Action label.
			bool _visible { true };	   ///< Initial visibility.
			float _duration { DurationMedium };	   ///< Display time.
			std::function<void(void)> _onDismiss {};	///< Dismiss handler.
			std::function<void(void)> _onAction {};		///< Action handler.

			public:
			/**
			 * @brief Construct a new Snackbar Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Snackbar entity from the current configuration.
			 * @return The newly created Snackbar.
			 */
			std::shared_ptr<Snackbar> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the snackbar pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the snackbar variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the snackbar message.
			 * @param message The new message text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMessage(const std::string &message);

			/**
			 * @brief Set the snackbar action label.
			 * @param action The new action label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withAction(const std::string &action);

			/**
			 * @brief Set the initial visibility (default shown).
			 * @param visible Visibility.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVisible(bool visible);

			/**
			 * @brief Set the display time (Paper `duration`).
			 * @param seconds Seconds before auto-dismiss (0: never).
			 * @return Reference to this builder for chaining.
			 */
			Builder &withDuration(float seconds);

			/**
			 * @brief Set the dismiss handler (Paper `onDismiss`).
			 * @param onDismiss Handler.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnDismiss(std::function<void(void)> onDismiss);

			/**
			 * @brief Set the action handler (Paper `action.onPress`).
			 * @param onAction Handler.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnAction(std::function<void(void)> onAction);
		};

		/**
		 * @brief Director that orchestrates `Snackbar::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a snackbar using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The snackbar variant.
			 * @param message The snackbar message.
			 * @param action The snackbar action label.
			 * @return The newly created snackbar.
			 */
			std::shared_ptr<Snackbar>
				makeSnackbar(Builder &builder,
							 std::shared_ptr<ecs::Entity> parent,
							 Variant variant, const std::string &message,
							 const std::string &action = "");
		};

		private:
		Variant _variant { Variant::SingleLine };	 ///< Snackbar variant.
		std::string _message {};					 ///< Message text.
		std::string _action {};						 ///< Action label.
		std::shared_ptr<Text> _messageEntity;		 ///< Message child entity.
		std::shared_ptr<Text> _actionEntity;		 ///< Action child entity.
		bool _visible { true };	   ///< Whether the snackbar is shown.
		float _duration { DurationMedium };		  ///< Display time.
		std::function<void(void)> _onDismiss {};	///< Dismiss handler.
		std::function<void(void)> _onAction {};		///< Action handler.

		/**
		 * @brief Mirror visibility on the overlay and the visuals.
		 */
		void applyVisibility(void);

		/**
		 * @brief Handle a dismissal reported by the overlay system.
		 */
		void handleDismiss(void);

		private:
		/**
		 * @brief Get the fixed height for the current variant.
		 * @return The snackbar height.
		 */
		float getVariantHeight(void) const;

		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for a snackbar.
		 */
		void applyOverlay(void);

		/**
		 * @brief Build the message and action child entities when needed.
		 */
		void buildContent(void);

		public:
		/**
		 * @brief Construct a Snackbar entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The snackbar variant.
		 * @param message The snackbar message.
		 * @param action The snackbar action label.
		 */
		Snackbar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 Variant variant, const std::string &message,
				 const std::string &action);

		/**
		 * @brief Default destructor.
		 */
		~Snackbar(void) override;

		/**
		 * @brief Set the snackbar variant.
		 * @param variant The new variant.
		 * @return Reference to this snackbar for chaining.
		 */
		Snackbar &setVariant(Variant variant);

		/**
		 * @brief Get the snackbar variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the snackbar message.
		 * @param message The new message text.
		 * @return Reference to this snackbar for chaining.
		 */
		Snackbar &setMessage(const std::string &message);

		/**
		 * @brief Show or hide the snackbar (Paper `visible`).
		 *
		 * Showing restarts the auto-dismiss countdown run by
		 * `systems::Overlay`; a hidden snackbar paints nothing.
		 *
		 * @param visible Visibility.
		 * @return Reference to this snackbar for chaining.
		 */
		Snackbar &setVisible(bool visible);

		/** @brief Show the snackbar. @return *this. */
		Snackbar &show(void);

		/** @brief Hide the snackbar without calling onDismiss. @return *this. */
		Snackbar &hide(void);

		/** @brief Check the visibility. @return True when shown. */
		bool isVisible(void) const;

		/**
		 * @brief Set the display time (Paper `duration`).
		 * @param seconds Seconds before auto-dismiss (0: never).
		 * @return Reference to this snackbar for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Snackbar &setDuration(float seconds);

		/** @brief Get the display time. @return Seconds (0: never). */
		float getDuration(void) const;

		/**
		 * @brief Set the dismiss handler (Paper `onDismiss`), called on
		 * auto-dismiss, Escape and action press.
		 * @param onDismiss Handler.
		 * @return Reference to this snackbar for chaining.
		 */
		Snackbar &setOnDismiss(std::function<void(void)> onDismiss);

		/**
		 * @brief Set the action handler (Paper `action.onPress`).
		 * @param onAction Handler.
		 * @return Reference to this snackbar for chaining.
		 */
		Snackbar &setOnAction(std::function<void(void)> onAction);

		/**
		 * @brief Activate the action as a press would: run the action handler,
		 * then dismiss.
		 * @return Reference to this snackbar for chaining.
		 */
		Snackbar &pressAction(void);

		/**
		 * @brief Get the action label entity.
		 * @return The action entity, or nullptr.
		 */
		std::shared_ptr<Text> getActionEntity(void) const;

		/**
		 * @brief Initialize the snackbar derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the snackbar derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
