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

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Badge entity: a small status indicator.
	 *
	 * Badges reuse `SurfaceBase` for their layout and
	 * `systems::RectangleRender` for the pill/circle shape (a fully rounded
	 * rectangle). The variants are:
	 * - `Dot`: a small circle with no label.
	 * - `Small`: a compact pill with a short label.
	 * - `Large`: a larger pill with a label.
	 *
	 * @see SurfaceBase
	 */
	class Badge: public SurfaceBase<>
	{
		public:
		/**
		 * @brief Badge variant.
		 */
		enum class Variant { Dot, Small, Large };

		/**
		 * @brief Builder used to configure and create `Badge` entities.
		 */
		class Builder: public EntityBuilderBase<Badge>
		{
			private:
			SurfaceConfig _config;				  ///< Surface configuration.
			Variant _variant { Variant::Dot };	  ///< Badge variant.
			std::string _label { "1" };			  ///< Badge label.

			public:
			/**
			 * @brief Construct a new Badge Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Badge entity from the current configuration.
			 * @return The newly created Badge.
			 */
			std::shared_ptr<Badge> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the badge pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the badge variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the badge label.
			 * @param label The new label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);
		};

		/**
		 * @brief Director that orchestrates `Badge::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a badge using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param label The badge label.
			 * @param variant The badge variant.
			 * @return The newly created badge.
			 */
			std::shared_ptr<Badge>
				makeBadge(Builder &builder, std::shared_ptr<ecs::Entity> parent,
						  const std::string &label, Variant variant);
		};

		private:
		Variant _variant { Variant::Dot };	   ///< Badge variant.
		std::string _label { "1" };			   ///< Badge label.
		std::shared_ptr<Text> _labelEntity;	   ///< Label child entity.

		private:
		/**
		 * @brief Apply the variant-specific size, color and radius.
		 */
		void applyVariant(void);

		/**
		 * @brief Get the diameter for the current variant.
		 * @return The badge size.
		 */
		float getVariantSize(void) const;

		public:
		/**
		 * @brief Construct a Badge entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The badge variant.
		 * @param label The badge label.
		 */
		Badge(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			  Variant variant, const std::string &label);

		/**
		 * @brief Default destructor.
		 */
		~Badge(void) override;

		/**
		 * @brief Set the badge variant.
		 * @param variant The new variant.
		 * @return Reference to this badge for chaining.
		 */
		Badge &setVariant(Variant variant);

		/**
		 * @brief Get the badge variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the badge label.
		 * @param label The new label.
		 * @return Reference to this badge for chaining.
		 */
		Badge &setLabel(const std::string &label);

		/**
		 * @brief Initialize the badge derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the badge derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
