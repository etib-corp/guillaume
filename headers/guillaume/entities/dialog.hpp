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

#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/icon.hpp"
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
	class Dialog: public SurfaceBase<components::Overlay>
	{
		public:
		/**
		 * @brief Dialog variant.
		 */
		enum class Variant { Alert, Simple, Confirmation, FullScreen };

		/**
		 * @brief Builder used to configure and create `Dialog` entities.
		 */
		class Builder: public EntityBuilderBase<Dialog>
		{
			private:
			SurfaceConfig _config;					 ///< Surface configuration.
			Variant _variant { Variant::Alert };	 ///< Dialog variant.
			std::string _title {};					 ///< Dialog title.
			std::string _message {};				 ///< Dialog message.
			std::vector<std::string> _actions {};	 ///< Dialog actions.

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
			Builder &addAction(const std::string &label);
		};

		/**
		 * @brief Director that orchestrates `Dialog::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a dialog using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The dialog variant.
			 * @param title The dialog title.
			 * @param message The dialog message.
			 * @param actions The dialog action labels.
			 * @return The newly created dialog.
			 */
			std::shared_ptr<Dialog>
				makeDialog(Builder &builder,
						   std::shared_ptr<ecs::Entity> parent, Variant variant,
						   const std::string &title, const std::string &message,
						   const std::vector<std::string> &actions = {});
		};

		private:
		Variant _variant { Variant::Alert };	 ///< Dialog variant.
		std::string _title {};					 ///< Dialog title.
		std::string _message {};				 ///< Dialog message.
		std::vector<std::string> _actions {};	 ///< Dialog actions.
		std::shared_ptr<Icon> _iconEntity;		 ///< Leading icon child.
		std::shared_ptr<Text> _titleEntity;		 ///< Title child.
		std::shared_ptr<Text> _messageEntity;	 ///< Message child.
		std::vector<std::shared_ptr<Text>>
			_actionEntities {};	   ///< Action children.

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
		 * @brief Initialize the dialog derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the dialog derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
