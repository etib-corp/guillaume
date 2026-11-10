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

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

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
	 * @brief Dialog entity: a centered or full-screen overlay panel.
	 *
	 * Dialogs reuse `SurfaceBase` for the layout, geometry and background
	 * surface and `components::Overlay` for the transient overlay behaviour.
	 * The Material Design variants are:
	 * - `Alert`: a title, a message and a row of textual actions.
	 * - `Simple`: a title followed by a list of option labels.
	 * - `Confirmation`: an icon, a title and a message.
	 * - `FullScreen`: a full-height panel without corners.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 */
	class Dialog: public SurfaceBase<components::Overlay, components::Elevation>
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

		/**
		 * @brief Builder used to configure and create `Dialog` entities.
		 */
		class Builder: public EntityBuilderBase<Dialog>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Alert };	///< Dialog variant.
			std::string _title {};					///< Dialog title.
			std::string _message {};				///< Dialog message.
			std::vector<Action> _actions {};		///< Dialog actions.
			std::string _icon {};					///< Hero icon glyph.
			std::function<void(void)> _onDismiss {};	///< Dismiss handler.

			public:
			/**
			 * @brief Construct a new Dialog Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Dialog entity from the current configuration.
			 * @return The newly created Dialog.
			 */
			std::shared_ptr<Dialog> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the dialog pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the dialog variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the dialog title.
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the dialog message.
			 * @param message The new message.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMessage(const std::string &message);

			/**
			 * @brief Append a dialog action label.
			 * @param label The action label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &addAction(const std::string &label,
							   std::function<void(void)> onPress = {});

			/**
			 * @brief Set the hero icon shown centred above the title.
			 * @param iconGlyph Icon glyph name.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the handler called when the dialog is dismissed.
			 * @param onDismiss Dismiss handler.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnDismiss(std::function<void(void)> onDismiss);

			/**
			 * @brief Create a dialog using the builder.
			 * @param parent The parent entity.
			 * @param variant The dialog variant.
			 * @param title The dialog title.
			 * @param message The dialog message.
			 * @param actions The dialog action labels.
			 * @return The newly created dialog.
			 */
			std::shared_ptr<Dialog>
				makeDialog(std::shared_ptr<ecs::Entity> parent, Variant variant,
						   const std::string &title, const std::string &message,
						   const std::vector<std::string> &actions = {});
		};

		private:
		Variant _variant { Variant::Alert };	 ///< Dialog variant.
		std::string _title {};					 ///< Dialog title.
		std::string _message {};				 ///< Dialog message.
		std::vector<Action> _actions {};		 ///< Dialog actions.
		std::string _icon {};					 ///< Hero icon glyph.
		std::shared_ptr<Icon> _iconEntity;		 ///< Hero icon child.
		std::shared_ptr<Text> _titleEntity;		 ///< Title child.
		std::shared_ptr<Text> _messageEntity;	 ///< Message child.
		std::vector<std::shared_ptr<Button>>
			_actionEntities {};					///< Action buttons.
		std::shared_ptr<View> _actionRow;		///< Right-aligned action row.
		std::shared_ptr<Backdrop> _backdrop;	///< Modal backdrop.

		private:
		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for the current variant.
		 */
		void applyOverlay(void);

		/**
		 * @brief Build the title, message, icon and action child entities.
		 */
		void buildContent(void);

		public:
		/**
		 * @brief Construct a Dialog entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The dialog variant.
		 * @param title The dialog title.
		 * @param message The dialog message.
		 * @param actions The dialog action labels.
		 */
		Dialog(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &message,
			   const std::vector<std::string> &actions);

		/**
		 * @brief Construct a dialog with pressable actions and a hero icon.
		 * @param registry Component registry.
		 * @param config Surface configuration.
		 * @param variant Dialog variant.
		 * @param title Title (headlineSmall).
		 * @param message Supporting text (bodyMedium).
		 * @param actions Actions, rendered right-aligned in a row.
		 * @param icon Hero icon glyph (Confirmation defaults to "info").
		 * @param onDismiss Dismiss handler.
		 */
		Dialog(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &message, const std::vector<Action> &actions,
			   const std::string &icon,
			   std::function<void(void)> onDismiss = {});

		/**
		 * @brief Default destructor.
		 */
		~Dialog(void) override;

		/**
		 * @brief Set the dialog variant.
		 * @param variant The new variant.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &setVariant(Variant variant);

		/**
		 * @brief Get the dialog variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the dialog title.
		 * @param title The new title.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &setTitle(const std::string &title);

		/**
		 * @brief Set the dialog message.
		 * @param message The new message.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &setMessage(const std::string &message);

		/**
		 * @brief Show the dialog.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &show(void);

		/**
		 * @brief Hide the dialog.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &hide(void);

		/**
		 * @brief Whether the dialog is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Open (show) the dialog.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &open(void);

		/**
		 * @brief Close (hide) the dialog.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &close(void);

		/**
		 * @brief Whether the dialog is open.
		 * @return True when visible.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Hide the dialog as a user dismissal and call the dismiss
		 * handler.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &dismiss(void);

		/**
		 * @brief Set the handler called when the dialog is dismissed
		 * (Paper `onDismiss`); it is also given to the Overlay component so
		 * outside clicks and Escape report it.
		 * @param onDismiss Dismiss handler.
		 * @return Reference to this dialog for chaining.
		 */
		Dialog &setOnDismiss(std::function<void(void)> onDismiss);

		/**
		 * @brief Get the number of actions.
		 * @return The action count.
		 */
		std::size_t getActionCount(void) const;

		/**
		 * @brief Get the identifier of an action button.
		 * @param index Action index.
		 * @return The identifier, or InvalidIdentifier when out of range.
		 */
		ecs::Entity::Identifier getActionIdentifier(std::size_t index) const;

		/**
		 * @brief Get the identifier of the action row.
		 * @return The identifier, or InvalidIdentifier before initialization.
		 */
		ecs::Entity::Identifier getActionRowIdentifier(void) const;

		/**
		 * @brief Get the identifier of the modal backdrop.
		 * @return The identifier, or InvalidIdentifier before initialization.
		 */
		ecs::Entity::Identifier getBackdropIdentifier(void) const;

		/**
		 * @brief Get the identifier of the hero icon.
		 * @return The identifier, or InvalidIdentifier when there is none.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Initialize the dialog derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the dialog derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
