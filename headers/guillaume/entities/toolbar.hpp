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
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Toolbar entity: a horizontal surface row hosting arbitrary
	 * content.
	 *
	 * Toolbars reuse `SurfaceBase` for the layout, geometry and background
	 * surface. They arrange an arbitrary list of content entities (a
	 * navigation icon button, a title text, action icon buttons, ...) along a
	 * horizontal axis, centered on the cross axis. The M3 variants are:
	 * - `Docked`: a rectangular surface using the surface container color.
	 * - `Floating`: a pill-shaped surface using the higher surface container
	 *   color.
	 *
	 * @see SurfaceBase
	 */
	class Toolbar: public SurfaceBase<>
	{
		public:
		/**
		 * @brief Toolbar variant.
		 */
		enum class Variant { Docked, Floating };

		/**
		 * @brief Builder used to configure and create `Toolbar` entities.
		 */
		class Builder: public EntityBuilderBase<Toolbar>
		{
			private:
			SurfaceConfig _config;					 ///< Surface configuration.
			Variant _variant { Variant::Docked };	 ///< Toolbar variant.
			std::vector<std::shared_ptr<ecs::Entity>>
				_children {};	 ///< Content entities to attach.

			public:
			/**
			 * @brief Construct a new Toolbar Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Toolbar entity from the current configuration.
			 * @return The newly created Toolbar.
			 */
			std::shared_ptr<Toolbar> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the toolbar pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the toolbar variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the spacing between children.
			 * @param spacing The new spacing.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSpacing(float spacing);

			/**
			 * @brief Set the padding around the children.
			 * @param padding The new padding.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPadding(float padding);

			/**
			 * @brief Force the toolbar width.
			 * @param width The new width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Force the toolbar height.
			 * @param height The new height.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withHeight(float height);

			/**
			 * @brief Set the content entities to attach.
			 * @param children The entities to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withChildren(
				const std::vector<std::shared_ptr<ecs::Entity>> &children);
		};

		/**
		 * @brief Director that orchestrates `Toolbar::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a toolbar using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The toolbar variant.
			 * @return The newly created toolbar.
			 */
			std::shared_ptr<Toolbar>
				makeToolbar(Builder &builder,
							std::shared_ptr<ecs::Entity> parent,
							Variant variant);
		};

		private:
		Variant _variant { Variant::Docked };	 ///< Toolbar variant.
		std::vector<std::shared_ptr<ecs::Entity>>
			_children {};	 ///< Content entities attached to the toolbar.

		private:
		/**
		 * @brief Apply the variant-specific radius, color and layout.
		 */
		void applyVariant(void);

		public:
		/**
		 * @brief Construct a Toolbar entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The toolbar variant.
		 * @param children The content entities to attach.
		 */
		Toolbar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				Variant variant,
				const std::vector<std::shared_ptr<ecs::Entity>> &children);

		/**
		 * @brief Default destructor.
		 */
		~Toolbar(void) override;

		/**
		 * @brief Set the toolbar variant.
		 * @param variant The new variant.
		 * @return Reference to this toolbar for chaining.
		 */
		Toolbar &setVariant(Variant variant);

		/**
		 * @brief Get the toolbar variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Attach the content entities arranged by this toolbar.
		 * @param children The entities to attach.
		 * @return Reference to this toolbar for chaining.
		 */
		Toolbar &setChildren(
			const std::vector<std::shared_ptr<ecs::Entity>> &children);

		/**
		 * @brief Initialize the toolbar derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the toolbar derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
