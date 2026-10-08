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

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/elevation.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Card entity: a surface grouping related content.
	 *
	 * Cards reuse `SurfaceBase` for the layout/geometry and `components::Color`
	 * + `components::Elevation` for the M3 variants:
	 * - `Elevated`: surface container color with a soft shadow.
	 * - `Filled`: surface container color without a shadow.
	 * - `Outlined`: transparent fill with an outline border.
	 *
	 * @see SurfaceBase
	 * @see components::Elevation
	 */
	class Card: public SurfaceBase<components::Elevation>
	{
		public:
		/**
		 * @brief Card variant.
		 */
		enum class Variant { Elevated, Filled, Outlined };

		/**
		 * @brief Builder used to configure and create `Card` entities.
		 */
		class Builder: public EntityBuilderBase<Card>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Elevated };	   ///< Card variant.

			public:
			/**
			 * @brief Construct a new Card Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Card entity from the current configuration.
			 * @return The newly created Card.
			 */
			std::shared_ptr<Card> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the card pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the card variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Force the card width.
			 * @param width The new width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withWidth(float width);

			/**
			 * @brief Force the card height.
			 * @param height The new height.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withHeight(float height);

			/**
			 * @brief Set the corner radius.
			 * @param radius The new corner radius.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withBorderRadius(float radius);
		};

		/**
		 * @brief Director that orchestrates `Card::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a card using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The card variant.
			 * @return The newly created card.
			 */
			std::shared_ptr<Card> makeCard(Builder &builder,
										   std::shared_ptr<ecs::Entity> parent,
										   Variant variant);
		};

		private:
		Variant _variant { Variant::Elevated };	   ///< Card variant.

		private:
		/**
		 * @brief Apply the variant-specific colors, borders and elevation.
		 */
		void applyVariant(void);

		public:
		/**
		 * @brief Construct a Card entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The card variant.
		 */
		Card(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant);

		/**
		 * @brief Default destructor.
		 */
		~Card(void) override;

		/**
		 * @brief Set the card variant.
		 * @param variant The new variant.
		 * @return Reference to this card for chaining.
		 */
		Card &setVariant(Variant variant);

		/**
		 * @brief Get the card variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Initialize the card derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the card derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
