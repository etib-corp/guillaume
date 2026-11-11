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
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/dialog.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/backdrop.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A modal panel shown above the rest of the interface, with an
	 * optional icon, a title, a message and a row of text buttons.
	 *
	 * Its configuration lives in components::Dialog (variant, title,
	 * message, actions, icon, identifiers of the children) and its
	 * visibility and dismiss handler in components::Overlay. systems::Dialog
	 * styles the panel, arranges its children and shows or hides its
	 * backdrop each frame, in the Layout phase. The class itself holds no
	 * field: its setters write the components and restyle at once.
	 *
	 * @code
	 * Dialog::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto dialog = builder.withOnDismiss([]() {
	 * 		// called when the user dismisses the dialog
	 * 	})
	 * 	.addAction("Cancel")
	 * 	.addAction("Delete", []() {
	 * 		// called when the button is pressed
	 * 	})
	 * 	.makeDialog(parent, Dialog::Variant::Alert, "Delete the file?",
	 * 		"This cannot be undone.");
	 * dialog->show();
	 * @endcode
	 *
	 * `Dialog::create(registry, config, variant, title, message, actions,
	 * icon, onDismiss)` builds the same widget without registering it in an
	 * entity registry.
	 *
	 * Showing and hiding: a dialog is created hidden. show() and hide()
	 * switch it; dismiss() hides it and calls the dismiss handler.
	 * systems::Overlay dismisses the topmost visible overlay when the user
	 * presses Escape. Pressing an action button only calls the action's
	 * handler: the dialog stays open until the handler (or other code)
	 * hides it.
	 *
	 * The variants differ as follows:
	 * - `Alert`, `Simple`: the default look, a 320 dp wide panel with
	 *   rounded corners.
	 * - `Confirmation`: same look; shows the "info" icon when no icon is
	 *   set.
	 * - `FullScreen`: a 640 dp high panel without rounded corners and
	 *   without backdrop.
	 *
	 * Things to know:
	 * - The dialog is the entity registry of its children (backdrop, icon,
	 *   title, message, action row); they are built by this class and
	 *   destroyed with it.
	 * - systems::Dialog overwrites the panel's layout settings, width,
	 *   color, corner radius and elevation every frame: values set through
	 *   the SurfaceBase setters do not last.
	 * - The actions are fixed at creation.
	 *
	 * @see components::Dialog, components::Overlay, systems::Dialog
	 */
	class Dialog:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Accessibility, components::Dialog>
	{
		public:
		/// Look of the dialog: Alert, Simple, Confirmation or FullScreen.
		using Variant = components::Dialog::Variant;

		/// One dialog action: a label and a press handler, shown as a text
		/// button in the action row.
		using Action = components::Dialog::Action;

		/**
		 * @brief Configures a Dialog, then builds it and registers it in the
		 * entity registry given at construction.
		 */
		class Builder: public EntityBuilderBase<Dialog>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and layout of the panel.
			Variant _variant { Variant::Alert };	///< Dialog variant.
			std::string _title {};					///< Title text.
			std::string _message {};				///< Message text.
			std::vector<Action> _actions {};		///< Actions, in row order.
			std::string _icon {};					///< Icon glyph name.
			std::function<void(void)> _onDismiss {};	///< Dismiss handler.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry the dialog's components
			 * are created in.
			 * @param entityRegistry The registry the built dialogs are added
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a dialog and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new dialog, hidden.
			 */
			std::shared_ptr<Dialog> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: Alert variant, no title, no
			 * message, no action, no icon, no dismiss handler, and a
			 * default pose.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose the dialog is created with.
			 * @param pose The position and orientation of the panel.
			 * Default: a default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the dialog variant.
			 * @param variant The variant. Default: `Variant::Alert`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the title, shown above the message.
			 * @param title The title text. Default: empty, which builds no
			 * title.
			 * @return This builder, for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the message, shown under the title.
			 * @param message The message text. Default: empty, which builds
			 * no message.
			 * @return This builder, for chaining.
			 */
			Builder &withMessage(const std::string &message);

			/**
			 * @brief Append an action, shown as a text button at the end of
			 * the action row.
			 * @param label The button label. An action with an empty label
			 * gets no button.
			 * @param onPress Called when the button is pressed. May be
			 * empty. The dialog stays open after the call.
			 * @return This builder, for chaining.
			 */
			Builder &addAction(const std::string &label,
							   std::function<void(void)> onPress = {});

			/**
			 * @brief Set the icon shown above the title.
			 * @param iconGlyph The glyph name. Default: empty, which builds
			 * no icon, except for a Confirmation dialog, which then shows
			 * "info".
			 * @return This builder, for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the handler called when the dialog is dismissed
			 * (see Dialog::setOnDismiss()).
			 * @param onDismiss The handler. Default: none.
			 * @return This builder, for chaining.
			 */
			Builder &withOnDismiss(std::function<void(void)> onDismiss);

			/**
			 * @brief Build a dialog, register it under @p parent and reset
			 * the builder.
			 *
			 * The arguments replace the variant, title and message set
			 * before; @p actions are appended to the actions already added
			 * with addAction(). The icon and dismiss handler set before are
			 * kept.
			 * @param parent The entity the dialog is attached to.
			 * @param variant The dialog variant.
			 * @param title The title text (empty: no title).
			 * @param message The message text (empty: no message).
			 * @param actions Labels of extra actions without a press
			 * handler.
			 * @return The registered dialog, hidden until show() is called.
			 */
			std::shared_ptr<Dialog>
				makeDialog(std::shared_ptr<ecs::Entity> parent, Variant variant,
						   const std::string &title, const std::string &message,
						   const std::vector<std::string> &actions = {});
		};

		/**
		 * @brief Construct a dialog with pressable actions and an icon.
		 *
		 * Writes the components and styles the panel, but builds no child:
		 * use create() or the Builder, which also build the children.
		 * @param registry The registry the dialog's components are created
		 * in.
		 * @param config Pose and layout of the panel. systems::Dialog
		 * overwrites the layout settings.
		 * @param variant The dialog variant.
		 * @param title The title text, in the HeadlineSmall type role.
		 * @param message The message text, in the BodyMedium type role.
		 * @param actions The actions, shown as a row of text buttons aligned
		 * to the end of the panel.
		 * @param icon The glyph name of the icon above the title. When
		 * empty, a Confirmation dialog shows "info" and the other variants
		 * show no icon.
		 * @param onDismiss Called when the dialog is dismissed; stored in
		 * components::Overlay.
		 */
		Dialog(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &message, const std::vector<Action> &actions,
			   const std::string &icon,
			   std::function<void(void)> onDismiss = {});

		/**
		 * @brief Create a dialog and its children, without registering it
		 * in an entity registry.
		 *
		 * The arguments are forwarded to a constructor: (registry, config,
		 * variant, title, message, actions, icon, onDismiss) with a vector
		 * of Action, or (registry, config, variant, title, message, labels)
		 * with a vector of labels. The children are built here, by build(),
		 * because a child needs its parent to be owned by a shared pointer.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new dialog, hidden, with no parent.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Dialog> create(Arguments &&...arguments)
		{
			std::shared_ptr<Dialog> entity(
				new Dialog(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Dialog(void) override = default;

		/**
		 * @brief Change the dialog variant.
		 *
		 * Builds the default "info" icon when the new variant is
		 * Confirmation and the dialog has no icon yet, then restyles at
		 * once.
		 * @param variant The new variant.
		 * @return This dialog, for chaining.
		 */
		Dialog &setVariant(Variant variant);

		/**
		 * @brief Get the dialog variant.
		 * @return The variant stored in components::Dialog.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the title text.
		 *
		 * Builds the title child when the dialog had none and @p title is
		 * not empty, then restyles at once. An empty title leaves an
		 * existing title child in place, with no text.
		 * @param title The new title text.
		 * @return This dialog, for chaining.
		 */
		Dialog &setTitle(const std::string &title);

		/**
		 * @brief Change the message text.
		 *
		 * Builds the message child when the dialog had none and @p message
		 * is not empty, then restyles at once. An empty message leaves an
		 * existing message child in place, with no text.
		 * @param message The new message text.
		 * @return This dialog, for chaining.
		 */
		Dialog &setMessage(const std::string &message);

		/**
		 * @brief Show the dialog and, except for the FullScreen variant,
		 * its backdrop.
		 *
		 * Sets components::Overlay visible and restyles at once.
		 * @return This dialog, for chaining.
		 */
		Dialog &show(void);

		/**
		 * @brief Hide the dialog and its backdrop.
		 *
		 * Sets components::Overlay hidden and restyles at once. Does not
		 * itself call the dismiss handler; use dismiss() for that.
		 * @return This dialog, for chaining.
		 */
		Dialog &hide(void);

		/**
		 * @brief Check whether the dialog is shown.
		 * @return True when components::Overlay is visible. False for a
		 * dialog that was never shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Show the dialog; same as show().
		 * @return This dialog, for chaining.
		 */
		Dialog &open(void);

		/**
		 * @brief Hide the dialog; same as hide().
		 * @return This dialog, for chaining.
		 */
		Dialog &close(void);

		/**
		 * @brief Check whether the dialog is shown; same as isVisible().
		 * @return True when the dialog is shown.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Dismiss the dialog as the user would: hide it, then call
		 * the dismiss handler.
		 *
		 * Does nothing while the dialog is hidden.
		 * @return This dialog, for chaining.
		 */
		Dialog &dismiss(void);

		/**
		 * @brief Set the handler called when the dialog is dismissed
		 * (Paper `onDismiss`).
		 *
		 * The handler is stored in components::Overlay. It is called by
		 * dismiss() and by systems::Overlay when it dismisses the dialog,
		 * for example on Escape.
		 * @param onDismiss The handler; an empty one removes it.
		 * @return This dialog, for chaining.
		 */
		Dialog &setOnDismiss(std::function<void(void)> onDismiss);

		/**
		 * @brief Get the number of action buttons.
		 * @return The number of buttons built; an action with an empty
		 * label has none.
		 */
		std::size_t getActionCount(void) const;

		/**
		 * @brief Get the identifier of an action button.
		 * @param index Index of the button in the action row, from 0.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * @p index is out of range.
		 */
		ecs::Entity::Identifier getActionIdentifier(std::size_t index) const;

		/**
		 * @brief Get the identifier of the row that holds the action
		 * buttons.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * the dialog has no action.
		 */
		ecs::Entity::Identifier getActionRowIdentifier(void) const;

		/**
		 * @brief Get the identifier of the backdrop drawn behind the
		 * panel.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier`
		 * before the children are built.
		 */
		ecs::Entity::Identifier getBackdropIdentifier(void) const;

		/**
		 * @brief Get the identifier of the icon shown above the title.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * the dialog has no icon.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		private:
		/**
		 * @brief Build the backdrop, icon, title, message and action
		 * children that are missing, record their identifiers in
		 * components::Dialog and hand the layout children to the surface.
		 *
		 * Does nothing until the dialog is owned by a shared pointer.
		 */
		void buildContent(void);

		/**
		 * @brief Create the action row and one text button per action, and
		 * record their identifiers in components::Dialog.
		 */
		void buildActionRow(void);

		private:
		/**
		 * @brief Construct a dialog whose actions have no press handler,
		 * with no icon and no dismiss handler.
		 * @param registry The registry the dialog's components are created
		 * in.
		 * @param config Pose and layout of the panel.
		 * @param variant The dialog variant.
		 * @param title The title text.
		 * @param message The message text.
		 * @param actions The labels of the action buttons.
		 */
		Dialog(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &message,
			   const std::vector<std::string> &actions);

		/**
		 * @brief Build the children and restyle.
		 *
		 * Called once by create(), when the dialog is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
