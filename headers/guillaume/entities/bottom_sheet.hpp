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

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Bottom sheet entity: a panel anchored to the bottom edge.
	 *
	 * Bottom sheets reuse `SurfaceBase` for the layout, geometry and background
	 * surface, `components::Overlay` for the transient overlay behaviour and
	 * `components::DragInteraction` to let the user drag the sheet down to
	 * dismiss it. The Material Design variants are:
	 * - `Standard`: a non-modal sheet over the content.
	 * - `Modal`: a modal sheet with a dimming scrim.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see components::DragInteraction
	 */
	class BottomSheet:
		public SurfaceBase<components::Overlay, components::DragInteraction>
	{
		public:
		/**
		 * @brief Bottom sheet variant.
		 */
		enum class Variant { Standard, Modal };

		/**
		 * @brief Builder used to configure and create `BottomSheet` entities.
		 */
		class Builder: public EntityBuilderBase<BottomSheet>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Standard };	   ///< Sheet variant.
			std::string _title {};					   ///< Sheet title.
			std::string _content {};				   ///< Sheet content text.
			float _height { 320.0f };				   ///< Sheet height.

			public:
			/**
			 * @brief Construct a new BottomSheet Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the BottomSheet entity from the current
			 * configuration.
			 * @return The newly created BottomSheet.
			 */
			std::shared_ptr<BottomSheet> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the bottom sheet pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the bottom sheet variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the bottom sheet title.
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the bottom sheet content text.
			 * @param content The new content.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withContent(const std::string &content);

			/**
			 * @brief Force the bottom sheet height.
			 * @param height The new height.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withHeight(float height);
		};

		/**
		 * @brief Director that orchestrates `BottomSheet::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a bottom sheet using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The sheet variant.
			 * @param title The sheet title.
			 * @param content The sheet content.
			 * @param height The sheet height.
			 * @return The newly created bottom sheet.
			 */
			std::shared_ptr<BottomSheet>
				makeBottomSheet(Builder &builder,
								std::shared_ptr<ecs::Entity> parent,
								Variant variant, const std::string &title,
								const std::string &content, float height);
		};

		private:
		Variant _variant { Variant::Standard };		  ///< Sheet variant.
		std::string _title {};						  ///< Sheet title.
		std::string _content {};					  ///< Sheet content text.
		float _height { 320.0f };					  ///< Sheet height.
		std::shared_ptr<Text> _titleEntity;			  ///< Title child.
		std::shared_ptr<Text> _contentEntity;		  ///< Content child.
		std::shared_ptr<ecs::Entity> _scrimEntity;	  ///< Modal scrim child.

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
		 * @brief Configure the drag interaction dismiss threshold.
		 */
		void applyDrag(void);

		/**
		 * @brief Build the title, content and modal scrim child entities.
		 */
		void buildContent(void);

		public:
		/**
		 * @brief Construct a BottomSheet entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The sheet variant.
		 * @param title The sheet title.
		 * @param content The sheet content.
		 * @param height The sheet height.
		 */
		BottomSheet(ecs::ComponentRegistry &registry,
					const SurfaceConfig &config, Variant variant,
					const std::string &title, const std::string &content,
					float height);

		/**
		 * @brief Default destructor.
		 */
		~BottomSheet(void) override;

		/**
		 * @brief Set the bottom sheet variant.
		 * @param variant The new variant.
		 * @return Reference to this sheet for chaining.
		 */
		BottomSheet &setVariant(Variant variant);

		/**
		 * @brief Get the bottom sheet variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the bottom sheet title.
		 * @param title The new title.
		 * @return Reference to this sheet for chaining.
		 */
		BottomSheet &setTitle(const std::string &title);

		/**
		 * @brief Set the bottom sheet content text.
		 * @param content The new content.
		 * @return Reference to this sheet for chaining.
		 */
		BottomSheet &setContent(const std::string &content);

		/**
		 * @brief Show the bottom sheet.
		 * @return Reference to this sheet for chaining.
		 */
		BottomSheet &show(void);

		/**
		 * @brief Hide the bottom sheet.
		 * @return Reference to this sheet for chaining.
		 */
		BottomSheet &hide(void);

		/**
		 * @brief Whether the bottom sheet is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Open (show) the entity.
		 * @return Reference to this entity for chaining.
		 */
		BottomSheet &open(void);

		/**
		 * @brief Close (hide) the entity.
		 * @return Reference to this entity for chaining.
		 */
		BottomSheet &close(void);

		/**
		 * @brief Whether the entity is open.
		 * @return True when visible.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Initialize the bottom sheet derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the bottom sheet derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
