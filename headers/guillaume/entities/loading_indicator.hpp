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

#include "guillaume/components/animation.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Loading indicator entity: an indeterminate activity animation.
	 *
	 * The loading indicator reuses `SurfaceBase` for its geometry and
	 * `components::Animation` to drive an animated child indefinitely. Unlike
	 * the progress indicator it carries no value: it simply conveys that work
	 * is in progress. The variants are:
	 * - `Linear`: a 200x4 sliding bar.
	 * - `Circular`: a 48x48 rotating arc.
	 *
	 * @see SurfaceBase
	 * @see components::Animation
	 */
	class LoadingIndicator: public SurfaceBase<components::Animation>
	{
		public:
		/**
		 * @brief Loading indicator visual variant.
		 */
		enum class Variant {
			Linear,		///< Linear sliding bar.
			Circular	///< Circular rotating arc.
		};

		/**
		 * @brief Builder used to configure and create `LoadingIndicator`
		 * entities.
		 */
		class Builder: public EntityBuilderBase<LoadingIndicator>
		{
			private:
			SurfaceConfig _config;					 ///< Surface configuration.
			Variant _variant { Variant::Linear };	 ///< Visual variant.
			float _size { 48.0f };					 ///< Circular diameter.

			public:
			/**
			 * @brief Construct a new LoadingIndicator Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the LoadingIndicator entity from the current
			 * configuration.
			 * @return The newly created LoadingIndicator.
			 */
			std::shared_ptr<LoadingIndicator> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the loading indicator pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the loading indicator variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the circular diameter.
			 * @param size The new diameter.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSize(float size);
		};

		/**
		 * @brief Director that orchestrates `LoadingIndicator::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a loading indicator using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The loading indicator variant.
			 * @return The newly created loading indicator.
			 */
			std::shared_ptr<LoadingIndicator>
				makeLoadingIndicator(Builder &builder,
									 std::shared_ptr<ecs::Entity> parent,
									 Variant variant);
		};

		private:
		Variant _variant { Variant::Linear };		///< Visual variant.
		float _diameter { 48.0f };					///< Circular diameter.
		std::shared_ptr<ecs::Entity> _animation;	///< Animated child.
		bool _built { false };		 ///< Whether the child exists.
		float _progress { 0.0f };	 ///< Latest eased animation progress.

		private:
		/**
		 * @brief Build (and attach) the animated child.
		 */
		void buildChild(void);

		/**
		 * @brief Refresh the animated child geometry from the variant.
		 */
		void applyState(void);

		public:
		/**
		 * @brief Construct a LoadingIndicator entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The visual variant.
		 */
		LoadingIndicator(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant);

		/**
		 * @brief Default destructor.
		 */
		~LoadingIndicator(void) override;

		/**
		 * @brief Set the loading indicator variant.
		 * @param variant The new variant.
		 * @return Reference to this loading indicator for chaining.
		 */
		LoadingIndicator &setVariant(Variant variant);

		/**
		 * @brief Get the loading indicator variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Start the loading animation.
		 * @return Reference to this loading indicator for chaining.
		 */
		LoadingIndicator &start(void);

		/**
		 * @brief Stop the loading animation.
		 * @return Reference to this loading indicator for chaining.
		 */
		LoadingIndicator &stop(void);

		/**
		 * @brief Whether the loading animation is running.
		 * @return True when running.
		 */
		bool isRunning(void) const;

		/**
		 * @brief Initialize the loading indicator derived state and child.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the loading indicator derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
