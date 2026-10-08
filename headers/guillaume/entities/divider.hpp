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
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Divider entity: a thin horizontal rule.
	 *
	 * A divider is a horizontal stroke rendered by `systems::LineRender`. It
	 * reuses `components::Line` for the stroke and `components::Color` for its
	 * color, so no dedicated render system is required.
	 *
	 * The M3 variants are expressed through the `Variant` enum:
	 * - `FullWidth`: spans the whole length.
	 * - `Inset`: leaves a leading inset.
	 * - `Middle`: leaves both leading and trailing insets.
	 *
	 * @see components::Line
	 * @see systems::LineRender
	 */
	class Divider:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Color, components::Line>
	{
		public:
		/**
		 * @brief Divider variant.
		 */
		enum class Variant { FullWidth, Inset, Middle };

		/**
		 * @brief Builder used to configure and create `Divider` entities.
		 */
		class Builder: public EntityBuilderBase<Divider>
		{
			private:
			utility::graphic::PoseF _pose {};		   ///< Divider pose.
			utility::graphic::Color32Bit _color {};	   ///< Divider color.
			float _length { 100.0f };				   ///< Total length.
			float _thickness { 1.0f };				   ///< Stroke thickness.
			float _inset { 16.0f };	   ///< Leading/trailing inset.
			Variant _variant { Variant::FullWidth };	///< Divider variant.

			public:
			/**
			 * @brief Construct a new Divider Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Divider entity from the current configuration.
			 * @return The newly created Divider.
			 */
			std::shared_ptr<Divider> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the divider pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the divider color.
			 * @param color The new color.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the total length.
			 * @param length The new length.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLength(float length);

			/**
			 * @brief Set the stroke thickness.
			 * @param thickness The new thickness.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withThickness(float thickness);

			/**
			 * @brief Set the leading/trailing inset.
			 * @param inset The new inset.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withInset(float inset);

			/**
			 * @brief Set the divider variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);
		};

		/**
		 * @brief Director that orchestrates `Divider::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a divider using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param length The divider length.
			 * @param variant The divider variant.
			 * @return The newly created divider.
			 */
			std::shared_ptr<Divider> makeDivider(Builder &builder,
												 std::shared_ptr<Entity> parent,
												 float length, Variant variant);
		};

		private:
		utility::graphic::PoseF _pose {};			///< Divider origin pose.
		utility::graphic::PoseF _appliedPose {};	///< Last applied pose.
		utility::graphic::Color32Bit _color {};		///< Divider color.
		float _length { 100.0f };					///< Total length.
		float _thickness { 1.0f };					///< Stroke thickness.
		float _inset { 16.0f };						///< Leading/trailing inset.
		Variant _variant { Variant::FullWidth };	///< Divider variant.

		private:
		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyGeometry(void);

		/**
		 * @brief Compute the effective leading inset for the variant.
		 * @return The leading inset.
		 */
		float getLeadingInset(void) const;

		/**
		 * @brief Compute the effective trailing inset for the variant.
		 * @return The trailing inset.
		 */
		float getTrailingInset(void) const;

		public:
		/**
		 * @brief Construct a Divider entity.
		 * @param registry The component registry.
		 * @param pose The divider pose.
		 * @param color The divider color.
		 * @param length The total length.
		 * @param thickness The stroke thickness.
		 * @param inset The leading/trailing inset.
		 * @param variant The divider variant.
		 */
		Divider(ecs::ComponentRegistry &registry,
				const utility::graphic::PoseF &pose,
				const utility::graphic::Color32Bit &color, float length,
				float thickness, float inset, Variant variant);

		/**
		 * @brief Default destructor.
		 */
		~Divider(void) override;

		/**
		 * @brief Set the divider variant.
		 * @param variant The new variant.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setVariant(Variant variant);

		/**
		 * @brief Set the total length.
		 * @param length The new length.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setLength(float length);

		/**
		 * @brief Set the stroke thickness.
		 * @param thickness The new thickness.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setThickness(float thickness);

		/**
		 * @brief Set the leading/trailing inset.
		 * @param inset The new inset.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setInset(float inset);

		/**
		 * @brief Set the divider color.
		 * @param color The new color.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Get the divider variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Initialize the divider derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the divider derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
