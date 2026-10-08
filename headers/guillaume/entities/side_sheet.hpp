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

#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Side sheet entity: a panel anchored to a left or right edge.
	 *
	 * Side sheets reuse `SurfaceBase` for the layout, geometry and background
	 * surface and `components::Overlay` for the transient overlay behaviour.
	 * The Material Design variants are:
	 * - `Standard`: a non-modal sheet docked on the edge.
	 * - `Modal`: a modal sheet that captures input.
	 *
	 * The anchored edge is selected with the `Side` enumeration.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 */
	class SideSheet: public SurfaceBase<components::Overlay>
	{
		public:
		/**
		 * @brief Side sheet variant.
		 */
		enum class Variant { Standard, Modal };

		/**
		 * @brief Edge the side sheet is anchored to.
		 */
		enum class Side { Left, Right };

		/**
		 * @brief Builder used to configure and create `SideSheet` entities.
		 */
		class Builder: public EntityBuilderBase<SideSheet>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Standard };	   ///< Sheet variant.
			Side _side { Side::Right };				   ///< Anchored edge.
			std::string _title {};					   ///< Sheet title.
			std::string _content {};				   ///< Sheet content text.

			public:
			/**
			 * @brief Construct a new SideSheet Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the SideSheet entity from the current configuration.
			 * @return The newly created SideSheet.
			 */
			std::shared_ptr<SideSheet> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the side sheet pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the side sheet variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the anchored edge.
			 * @param side The new anchored edge.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSide(Side side);

			/**
			 * @brief Set the side sheet title.
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the side sheet content text.
			 * @param content The new content.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withContent(const std::string &content);
		};

		/**
		 * @brief Director that orchestrates `SideSheet::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a side sheet using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The sheet variant.
			 * @param side The anchored edge.
			 * @param title The sheet title.
			 * @param content The sheet content.
			 * @return The newly created side sheet.
			 */
			std::shared_ptr<SideSheet> makeSideSheet(
				Builder &builder, std::shared_ptr<ecs::Entity> parent,
				Variant variant, Side side, const std::string &title,
				const std::string &content);
		};

		private:
		Variant _variant { Variant::Standard };	   ///< Sheet variant.
		Side _side { Side::Right };				   ///< Anchored edge.
		std::string _title {};					   ///< Sheet title.
		std::string _content {};				   ///< Sheet content text.
		std::shared_ptr<Text> _titleEntity;		   ///< Title child.
		std::shared_ptr<Text> _contentEntity;	   ///< Content child.

		private:
		/**
		 * @brief Apply the variant and side specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for the current variant.
		 */
		void applyOverlay(void);

		/**
		 * @brief Build the title and content child entities.
		 */
		void buildContent(void);

		public:
		/**
		 * @brief Construct a SideSheet entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The sheet variant.
		 * @param side The anchored edge.
		 * @param title The sheet title.
		 * @param content The sheet content.
		 */
		SideSheet(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				  Variant variant, Side side, const std::string &title,
				  const std::string &content);

		/**
		 * @brief Default destructor.
		 */
		~SideSheet(void) override;

		/**
		 * @brief Set the side sheet variant.
		 * @param variant The new variant.
		 * @return Reference to this sheet for chaining.
		 */
		SideSheet &setVariant(Variant variant);

		/**
		 * @brief Get the side sheet variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the anchored edge.
		 * @param side The new anchored edge.
		 * @return Reference to this sheet for chaining.
		 */
		SideSheet &setSide(Side side);

		/**
		 * @brief Get the anchored edge.
		 * @return The anchored edge.
		 */
		Side getSide(void) const;

		/**
		 * @brief Set the side sheet title.
		 * @param title The new title.
		 * @return Reference to this sheet for chaining.
		 */
		SideSheet &setTitle(const std::string &title);

		/**
		 * @brief Set the side sheet content text.
		 * @param content The new content.
		 * @return Reference to this sheet for chaining.
		 */
		SideSheet &setContent(const std::string &content);

		/**
		 * @brief Show the side sheet.
		 * @return Reference to this sheet for chaining.
		 */
		SideSheet &show(void);

		/**
		 * @brief Hide the side sheet.
		 * @return Reference to this sheet for chaining.
		 */
		SideSheet &hide(void);

		/**
		 * @brief Whether the side sheet is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Open (show) the entity.
		 * @return Reference to this entity for chaining.
		 */
		SideSheet &open(void);

		/**
		 * @brief Close (hide) the entity.
		 * @return Reference to this entity for chaining.
		 */
		SideSheet &close(void);

		/**
		 * @brief Whether the entity is open.
		 * @return True when visible.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Initialize the side sheet derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the side sheet derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
