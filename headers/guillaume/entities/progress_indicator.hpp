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

#include "guillaume/components/value.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Progress indicator entity: a bar or ring showing task progress.
	 *
	 * The progress indicator reuses `SurfaceBase` for its geometry and
	 * `components::Value` (normalized to `[0, 1]`) as the progress state. The
	 * base surface itself is transparent: a dedicated track child draws the
	 * inactive track while a fill child draws the active progress. The M3
	 * variants are:
	 * - `LinearDeterminate`: a 4 high rounded bar filled from the left.
	 * - `LinearIndeterminate`: a sliding animated bar.
	 * - `CircularDeterminate`: a 48x48 ring whose arc spans the value.
	 * - `CircularIndeterminate`: a 48x48 ring with a rotating arc.
	 *
	 * @see SurfaceBase
	 * @see components::Value
	 */
	class ProgressIndicator: public SurfaceBase<components::Value>
	{
		public:
		/**
		 * @brief Progress indicator visual variant.
		 */
		enum class Variant {
			LinearDeterminate,		 ///< Linear determinate bar.
			LinearIndeterminate,	 ///< Linear indeterminate bar.
			CircularDeterminate,	 ///< Circular determinate ring.
			CircularIndeterminate	 ///< Circular indeterminate ring.
		};

		/**
		 * @brief Builder used to configure and create `ProgressIndicator`
		 * entities.
		 */
		class Builder: public EntityBuilderBase<ProgressIndicator>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::LinearDeterminate };	///< Variant.
			float _value { 0.0f };		///< Initial normalized value.
			float _width { 200.0f };	///< Linear track length.
			float _size { 48.0f };		///< Circular diameter.

			public:
			/**
			 * @brief Construct a new ProgressIndicator Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the ProgressIndicator entity from the current
			 * configuration.
			 * @return The newly created ProgressIndicator.
			 */
			std::shared_ptr<ProgressIndicator> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the progress indicator pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the progress indicator variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the initial normalized progress value.
			 * @param value The new value, in `[0, 1]`.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withValue(float value);

			/**
			 * @brief Set the linear track length.
			 * @param width The new track length.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withWidth(float width);

			/**
			 * @brief Set the circular diameter.
			 * @param size The new diameter.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSize(float size);
		};

		/**
		 * @brief Director that orchestrates `ProgressIndicator::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a progress indicator using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The progress indicator variant.
			 * @param value The initial normalized value, in `[0, 1]`.
			 * @return The newly created progress indicator.
			 */
			std::shared_ptr<ProgressIndicator>
				makeProgressIndicator(Builder &builder,
									  std::shared_ptr<ecs::Entity> parent,
									  Variant variant, float value);
		};

		private:
		Variant _variant { Variant::LinearDeterminate };	///< Variant.
		float _trackLength { 200.0f };			///< Linear track length.
		float _diameter { 48.0f };				///< Circular diameter.
		std::shared_ptr<ecs::Entity> _track;	///< Track child entity.
		std::shared_ptr<ecs::Entity> _fill;		///< Fill child entity.
		bool _built { false };					///< Whether children exist.

		private:
		/**
		 * @brief Build (and attach) the track and fill children.
		 */
		void buildChildren(void);

		/**
		 * @brief Refresh the track and fill geometry from the current value.
		 */
		void applyState(void);

		/**
		 * @brief Whether the current variant is circular.
		 * @return True for circular variants.
		 */
		bool isCircular(void) const;

		/**
		 * @brief Whether the current variant is indeterminate.
		 * @return True for indeterminate variants.
		 */
		bool isIndeterminate(void) const;

		public:
		/**
		 * @brief Construct a ProgressIndicator entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The visual variant.
		 * @param value The initial normalized value, in `[0, 1]`.
		 */
		ProgressIndicator(ecs::ComponentRegistry &registry,
						  const SurfaceConfig &config, Variant variant,
						  float value);

		/**
		 * @brief Default destructor.
		 */
		~ProgressIndicator(void) override;

		/**
		 * @brief Set the progress indicator variant.
		 * @param variant The new variant.
		 * @return Reference to this progress indicator for chaining.
		 */
		ProgressIndicator &setVariant(Variant variant);

		/**
		 * @brief Get the progress indicator variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the normalized progress value.
		 * @param value The new normalized value, in `[0, 1]`.
		 * @return Reference to this progress indicator for chaining.
		 */
		ProgressIndicator &setValue(float value);

		/**
		 * @brief Get the current normalized progress value.
		 * @return The normalized value, in `[0, 1]`.
		 */
		float getValue(void) const;

		/**
		 * @brief Initialize the progress indicator derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the progress indicator derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
