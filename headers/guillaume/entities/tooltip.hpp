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
	 * @brief Tooltip entity: a small overlay-attached label.
	 *
	 * Tooltips reuse `SurfaceBase` for the layout, geometry and background
	 * surface and `components::Overlay` for the transient overlay behaviour.
	 * The M3 variants are:
	 * - `Plain`: a short single-line text on an inverse surface, pill radius 4.
	 * - `Rich`: a larger title (font 16) followed by a supporting text
	 *   (font 14), corner radius 12.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 */
	class Tooltip: public SurfaceBase<components::Overlay>
	{
		public:
		/**
		 * @brief Tooltip variant.
		 */
		enum class Variant { Plain, Rich };

		/**
		 * @brief Builder used to configure and create `Tooltip` entities.
		 */
		class Builder: public EntityBuilderBase<Tooltip>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Plain };	///< Tooltip variant.
			std::string _text {};					///< Tooltip body text.
			std::string _title {};	  ///< Tooltip title (Rich only).

			public:
			/**
			 * @brief Construct a new Tooltip Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Tooltip entity from the current configuration.
			 * @return The newly created Tooltip.
			 */
			std::shared_ptr<Tooltip> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the tooltip pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the tooltip variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the tooltip body text.
			 * @param text The new body text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withText(const std::string &text);

			/**
			 * @brief Set the tooltip title (Rich variant only).
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Force the tooltip width.
			 * @param width The new fixed width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);
		};

		/**
		 * @brief Director that orchestrates `Tooltip::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a tooltip using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The tooltip variant.
			 * @param text The tooltip body text.
			 * @param title The tooltip title (Rich variant only).
			 * @return The newly created tooltip.
			 */
			std::shared_ptr<Tooltip>
				makeTooltip(Builder &builder,
							std::shared_ptr<ecs::Entity> parent,
							Variant variant, const std::string &text,
							const std::string &title = "");
		};

		private:
		Variant _variant { Variant::Plain };	///< Tooltip variant.
		std::string _text {};					///< Tooltip body text.
		std::string _title {};					///< Tooltip title.
		std::shared_ptr<Text> _titleEntity;		///< Title child entity.
		std::shared_ptr<Text> _textEntity;		///< Body child entity.

		private:
		/**
		 * @brief Get the fixed height for the current variant.
		 * @return The tooltip height.
		 */
		float getVariantHeight(void) const;

		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Build the title and body child entities when needed.
		 */
		void buildContent(void);

		/**
		 * @brief Configure the overlay component for a tooltip.
		 */
		void applyOverlay(void);

		public:
		/**
		 * @brief Construct a Tooltip entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The tooltip variant.
		 * @param text The tooltip body text.
		 * @param title The tooltip title (Rich variant only).
		 */
		Tooltip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				Variant variant, const std::string &text,
				const std::string &title);

		/**
		 * @brief Default destructor.
		 */
		~Tooltip(void) override;

		/**
		 * @brief Set the tooltip variant.
		 * @param variant The new variant.
		 * @return Reference to this tooltip for chaining.
		 */
		Tooltip &setVariant(Variant variant);

		/**
		 * @brief Get the tooltip variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the tooltip body text.
		 * @param text The new body text.
		 * @return Reference to this tooltip for chaining.
		 */
		Tooltip &setText(const std::string &text);

		/**
		 * @brief Initialize the tooltip derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the tooltip derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
