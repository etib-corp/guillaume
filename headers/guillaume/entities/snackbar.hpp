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

#include <utility>
#include <functional>
#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/snackbar.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A short message bar that disappears by itself after a few
	 * seconds, with an optional action label the user can press.
	 *
	 * Its state lives in components::Snackbar (variant, message, action
	 * label, visibility, duration, callbacks). systems::Snackbar styles the
	 * bar and arranges its children each frame, in the Layout phase, and
	 * copies the visibility and the duration to components::Overlay;
	 * systems::Overlay runs the countdown. The class itself holds no field:
	 * its setters write the component and restyle at once.
	 *
	 * @code
	 * Snackbar::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto snackbar = builder.withDuration(Snackbar::DurationShort)
	 * 	.withOnAction([]() {
	 * 		// called when the user presses "Undo"
	 * 	})
	 * 	.makeSnackbar(parent, Snackbar::Variant::WithAction,
	 * 		"Message archived", "Undo");
	 * @endcode
	 *
	 * `Snackbar::create(registry, config, variant, message, action)` builds
	 * the same widget without registering it in an entity registry.
	 *
	 * Showing and hiding: a snackbar is shown as soon as it is created,
	 * unless the builder was given `withVisible(false)`. show() shows it
	 * and restarts the countdown; hide() hides it without calling the
	 * dismiss handler. The snackbar dismisses itself, and then calls the
	 * dismiss handler, when its duration has elapsed, when the user
	 * presses Escape, and after the action is pressed. The duration is in
	 * seconds; 0 keeps the snackbar until it is hidden.
	 *
	 * The variants are:
	 * - `SingleLine`: a message, in a 48 dp high bar.
	 * - `MultiLine`: a message, in a 68 dp high bar.
	 * - `WithAction`: a message followed by the action label, in a 48 dp
	 *   high bar. Only this variant builds the action label.
	 *
	 * Things to know:
	 * - The builder makes the bar 344 dp wide.
	 * - A hidden snackbar keeps its place in its parent's layout; it is
	 *   painted transparent.
	 * - systems::Snackbar overwrites the bar's layout settings, height,
	 *   color, corner radius and elevation every frame.
	 * - systems::Overlay only counts down, and only applies Escape to, the
	 *   topmost visible overlay: a snackbar under a dialog waits.
	 * - The action label is fixed at creation.
	 *
	 * @see components::Snackbar, systems::Snackbar, systems::Overlay
	 */
	class Snackbar:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Accessibility, components::Snackbar>
	{
		public:
		/// Look of the snackbar: SingleLine, MultiLine or WithAction.
		using Variant = components::Snackbar::Variant;

		/**
		 * @brief Short display time: 4 seconds (Paper
		 * `Snackbar.DURATION_SHORT`).
		 */
		static constexpr float DurationShort =
			components::Snackbar::DurationShort;

		/**
		 * @brief Default display time: 7 seconds (Paper
		 * `Snackbar.DURATION_MEDIUM`).
		 */
		static constexpr float DurationMedium =
			components::Snackbar::DurationMedium;

		/**
		 * @brief Long display time: 10 seconds (Paper
		 * `Snackbar.DURATION_LONG`).
		 */
		static constexpr float DurationLong =
			components::Snackbar::DurationLong;

		/**
		 * @brief Configures a Snackbar, then builds it and registers it in
		 * the entity registry given at construction.
		 */
		class Builder: public EntityBuilderBase<Snackbar>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and layout of the bar.
			Variant _variant { Variant::SingleLine };	 ///< Snackbar variant.
			std::string _message {};					 ///< Message text.
			std::string _action {};						 ///< Action label.
			bool _visible { true };						 ///< Shown when built.
			float _duration { DurationMedium };	   ///< Display time (seconds).
			std::function<void(void)> _onDismiss {};	///< Dismiss handler.
			std::function<void(void)> _onAction {};		///< Action handler.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry the snackbar's
			 * components are created in.
			 * @param entityRegistry The registry the built snackbars are
			 * added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a snackbar and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new snackbar.
			 * @throws std::invalid_argument If the duration given to
			 * withDuration() is negative.
			 */
			std::shared_ptr<Snackbar> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: SingleLine variant, no message,
			 * no action label, shown, DurationMedium (7 seconds), no
			 * handler, a default pose and a 344 dp wide bar.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose the snackbar is created with.
			 * @param pose The position and orientation of the bar. Default:
			 * a default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the snackbar variant.
			 * @param variant The variant. Default: `Variant::SingleLine`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the message.
			 * @param message The message text. Default: empty, which builds
			 * no message child.
			 * @return This builder, for chaining.
			 */
			Builder &withMessage(const std::string &message);

			/**
			 * @brief Set the label of the action.
			 * @param action The label. Used by the WithAction variant only;
			 * an empty label builds no action. Default: empty.
			 * @return This builder, for chaining.
			 */
			Builder &withAction(const std::string &action);

			/**
			 * @brief Choose whether the snackbar is shown once built.
			 * @param visible True to show it at once, false to build it
			 * hidden. Default: true.
			 * @return This builder, for chaining.
			 */
			Builder &withVisible(bool visible);

			/**
			 * @brief Set how long the snackbar stays shown (Paper
			 * `duration`).
			 * @param seconds The display time, in seconds; 0 never
			 * dismisses. Default: DurationMedium. A negative value makes
			 * the build throw.
			 * @return This builder, for chaining.
			 */
			Builder &withDuration(float seconds);

			/**
			 * @brief Set the handler called when the snackbar dismisses
			 * itself (see Snackbar::setOnDismiss()).
			 * @param onDismiss The handler. Default: none.
			 * @return This builder, for chaining.
			 */
			Builder &withOnDismiss(std::function<void(void)> onDismiss);

			/**
			 * @brief Set the handler called when the action is pressed
			 * (Paper `action.onPress`).
			 * @param onAction The handler. Default: none.
			 * @return This builder, for chaining.
			 */
			Builder &withOnAction(std::function<void(void)> onAction);

			/**
			 * @brief Build a snackbar, register it under @p parent and reset
			 * the builder.
			 *
			 * The arguments replace the variant, message and action label
			 * set before; the other settings are kept.
			 * @param parent The entity the snackbar is attached to.
			 * @param variant The snackbar variant.
			 * @param message The message text.
			 * @param action The action label, used by the WithAction
			 * variant only.
			 * @return The registered snackbar.
			 * @throws std::invalid_argument If the duration given to
			 * withDuration() is negative.
			 */
			std::shared_ptr<Snackbar>
				makeSnackbar(std::shared_ptr<ecs::Entity> parent,
							 Variant variant, const std::string &message,
							 const std::string &action = "");
		};

		public:
		/**
		 * @brief Create a snackbar and its children, without registering it
		 * in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * (registry, config, variant, message, action). The children are
		 * built here, by build(), because a child needs its parent to be
		 * owned by a shared pointer.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new snackbar, shown, with no parent.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Snackbar> create(Arguments &&...arguments)
		{
			std::shared_ptr<Snackbar> entity(
				new Snackbar(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Snackbar(void) override = default;

		/**
		 * @brief Change the snackbar variant.
		 *
		 * Builds the action label when the new variant is WithAction, the
		 * action label is not empty and the child does not exist yet, then
		 * restyles at once.
		 * @param variant The new variant.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &setVariant(Variant variant);

		/**
		 * @brief Get the snackbar variant.
		 * @return The variant stored in components::Snackbar.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the message text.
		 *
		 * Builds the message child when the snackbar had none and
		 * @p message is not empty, then restyles at once.
		 * @param message The new message text.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &setMessage(const std::string &message);

		/**
		 * @brief Show or hide the snackbar (Paper `visible`).
		 *
		 * Restyles at once. Showing a hidden snackbar restarts the
		 * auto-dismiss countdown run by systems::Overlay. Hiding does not
		 * call the dismiss handler. A hidden snackbar is painted
		 * transparent.
		 * @param visible True to show, false to hide.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &setVisible(bool visible);

		/**
		 * @brief Show the snackbar; same as `setVisible(true)`.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &show(void);

		/**
		 * @brief Hide the snackbar without calling the dismiss handler;
		 * same as `setVisible(false)`.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &hide(void);

		/**
		 * @brief Check whether the snackbar is shown.
		 * @return True when shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Set how long the snackbar stays shown (Paper `duration`).
		 *
		 * Restyles at once, which copies the duration to
		 * components::Overlay. Does not restart a running countdown.
		 * @param seconds The display time, in seconds; 0 never dismisses.
		 * @return This snackbar, for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Snackbar &setDuration(float seconds);

		/**
		 * @brief Get how long the snackbar stays shown.
		 * @return The display time, in seconds; 0 means it never dismisses
		 * itself.
		 */
		float getDuration(void) const;

		/**
		 * @brief Set the handler called when the snackbar dismisses itself
		 * (Paper `onDismiss`).
		 *
		 * It is called after the snackbar has been hidden by the end of
		 * its duration, by Escape or by a press on the action. hide() and
		 * `setVisible(false)` do not call it.
		 * @param onDismiss The handler; an empty one removes it.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &setOnDismiss(std::function<void(void)> onDismiss);

		/**
		 * @brief Set the handler called when the action is pressed (Paper
		 * `action.onPress`).
		 * @param onAction The handler; an empty one removes it.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &setOnAction(std::function<void(void)> onAction);

		/**
		 * @brief Activate the action as a press on its label would: call
		 * the action handler, then hide the snackbar and call the dismiss
		 * handler.
		 *
		 * Does nothing while the snackbar is hidden.
		 * @return This snackbar, for chaining.
		 */
		Snackbar &pressAction(void);

		/**
		 * @brief Get the text entity that shows the action label.
		 * @return The action label, or `nullptr` when the snackbar has
		 * none.
		 */
		std::shared_ptr<Text> getActionEntity(void) const;

		private:
		/**
		 * @brief Build the message and action child entities that are
		 * missing and record their identifiers in components::Snackbar.
		 *
		 * Does nothing until the snackbar is owned by a shared pointer.
		 */
		void buildContent(void);

		private:
		/**
		 * @brief Construct a snackbar, without its children.
		 * @param registry The registry the snackbar's components are
		 * created in.
		 * @param config Pose and layout of the bar. systems::Snackbar
		 * overwrites the layout settings and the height.
		 * @param variant The snackbar variant.
		 * @param message The message text.
		 * @param action The action label, used by the WithAction variant
		 * only.
		 */
		Snackbar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 Variant variant, const std::string &message,
				 const std::string &action);

		/**
		 * @brief Build the children and restyle.
		 *
		 * Called once by create(), when the snackbar is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
