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

#include <cstdint>
#include <memory>
#include <vector>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A container surface that arranges its children.
	 *
	 * `View` owns a background surface (color and border radius) and a
	 * `components::Layout` describing how its children are arranged. The actual
	 * geometry is produced by `systems::Layout`, which places every child along
	 * the configured axis, applies the alignments and updates the container
	 * `components::Bound`.
	 *
	 * The class is a facade without fields and without an `update()`
	 * override. Its whole state lives in components that already existed:
	 * the pose in `components::Transform`, the surface color in
	 * `components::Color`, the corner radius in `components::Borders`, and
	 * the axis, alignments, spacing, padding, fixed size and ordered child
	 * identifiers in `components::Layout`. The setters write those components
	 * and arrange the children at once; `systems::Layout` does the same every
	 * frame, so a value written to these components by identifier is never
	 * overwritten by the view.
	 *
	 * The view does not own the entities attached to it: they stay alive
	 * through the entity registry they were built in (usually the view
	 * itself, passed as the entity registry of their builder).
	 *
	 * @see components::Layout
	 * @see systems::Layout
	 */
	class View:
		public std::enable_shared_from_this<View>,
		public ecs::ParentEntityFiller<components::Layout,
									   components::Transform, components::Bound,
									   components::Color, components::Borders>
	{
		public:
		using Axis				 = components::Layout::Axis;
		using MainAxisAlignment	 = components::Layout::MainAxisAlignment;
		using CrossAxisAlignment = components::Layout::CrossAxisAlignment;

		/**
		 * @brief Builder used to configure and create `View` entities.
		 */
		class Builder: public EntityBuilderBase<View>
		{
			private:
			utility::graphic::PoseF _pose;			///< Pose of the container.
			utility::graphic::Color32Bit _color;	///< Surface color.
			float _borderRadius;	///< Border radius of the surface.
			Axis _axis;				///< Axis along which children are arranged.
			MainAxisAlignment _mainAxisAlignment;	 ///< Main axis alignment.
			CrossAxisAlignment
				_crossAxisAlignment;	///< Cross axis alignment.
			float _spacing;				///< Space between children.
			float _padding;				///< Space around children.
			bool _hasFixedWidth;		///< Whether the width is forced.
			bool _hasFixedHeight;		///< Whether the height is forced.
			float _fixedWidth;			///< Forced width.
			float _fixedHeight;			///< Forced height.
			std::vector<std::shared_ptr<ecs::Entity>>
				_entities;	  ///< Entities attached to the container.

			public:
			/**
			 * @brief Construct a new View Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the View entity from the current configuration.
			 * @return The newly created View.
			 */
			std::shared_ptr<View> buildEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose of the container.
			 * @param pose The pose to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the surface color of the container.
			 * @param color The color to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the border radius of the container.
			 * @param borderRadius The border radius to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withBorderRadius(float borderRadius);

			/**
			 * @brief Set the axis along which children are arranged.
			 * @param axis The axis to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withAxis(Axis axis);

			/**
			 * @brief Set the main axis alignment.
			 * @param alignment The alignment to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMainAxisAlignment(MainAxisAlignment alignment);

			/**
			 * @brief Set the cross axis alignment.
			 * @param alignment The alignment to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withCrossAxisAlignment(CrossAxisAlignment alignment);

			/**
			 * @brief Set the spacing between children.
			 * @param spacing The spacing to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSpacing(float spacing);

			/**
			 * @brief Set the padding around the children.
			 * @param padding The padding to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPadding(float padding);

			/**
			 * @brief Force the container width.
			 * @param width The fixed width to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Force the container height.
			 * @param height The fixed height to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedHeight(float height);

			/**
			 * @brief Set the entities to attach to the container.
			 * @param entities The entities to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withEntities(
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);

			/**
			 * @brief Create a default view entity.
			 * @param parent The parent entity to attach the container to.
			 * @param pose The pose to set for the container.
			 * @param entities The entities to attach to the container.
			 * @return A shared pointer to the newly created container entity.
			 */
			std::shared_ptr<View> makeDefaultView(
				std::shared_ptr<Entity> parent,
				const utility::graphic::PoseF &pose,
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);

			/**
			 * @brief Create a colored view entity.
			 * @param parent The parent entity to attach the container to.
			 * @param pose The pose to set for the container.
			 * @param color The surface color to set.
			 * @param entities The entities to attach to the container.
			 * @return A shared pointer to the newly created container entity.
			 */
			std::shared_ptr<View> makeColorView(
				std::shared_ptr<Entity> parent,
				const utility::graphic::PoseF &pose,
				const utility::graphic::Color32Bit &color,
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);
		};

		/**
		 * @brief Construct a View entity.
		 * @param registry Reference to the component registry.
		 * @param pose The pose to initialize the container with.
		 * @param color The surface color to initialize the container with.
		 * @param borderRadius The border radius to initialize the container
		 * with.
		 * @param axis The axis along which children are arranged.
		 * @param mainAxisAlignment The main axis alignment.
		 * @param crossAxisAlignment The cross axis alignment.
		 * @param spacing The spacing between children.
		 * @param padding The padding around the children.
		 * @param hasFixedWidth Whether the width is forced.
		 * @param fixedWidth The forced width.
		 * @param hasFixedHeight Whether the height is forced.
		 * @param fixedHeight The forced height.
		 * @param entities The entities to attach to the container. Their
		 * identifiers are recorded as layout children at once; they are
		 * parented to the view by initialize().
		 */
		View(ecs::ComponentRegistry &registry,
			 const utility::graphic::PoseF &pose,
			 const utility::graphic::Color32Bit &color, float borderRadius,
			 Axis axis, MainAxisAlignment mainAxisAlignment,
			 CrossAxisAlignment crossAxisAlignment, float spacing,
			 float padding, bool hasFixedWidth, float fixedWidth,
			 bool hasFixedHeight, float fixedHeight,
			 const std::vector<std::shared_ptr<ecs::Entity>> &entities);

		/**
		 * @brief Default destructor for the View entity.
		 */
		~View(void);

		/**
		 * @brief Set the pose of the container.
		 * @param pose The new pose.
		 * @return Reference to this View for chaining.
		 */
		View &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Set the surface color of the container.
		 * @param color The new color.
		 * @return Reference to this View for chaining.
		 */
		View &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the border radius of the container.
		 * @param borderRadius The new border radius.
		 * @return Reference to this View for chaining.
		 */
		View &setBorderRadius(float borderRadius);

		/**
		 * @brief Set the arrangement axis.
		 * @param axis The new axis.
		 * @return Reference to this View for chaining.
		 */
		View &setAxis(Axis axis);

		/**
		 * @brief Set the main axis alignment.
		 * @param alignment The new alignment.
		 * @return Reference to this View for chaining.
		 */
		View &setMainAxisAlignment(MainAxisAlignment alignment);

		/**
		 * @brief Set the cross axis alignment.
		 * @param alignment The new alignment.
		 * @return Reference to this View for chaining.
		 */
		View &setCrossAxisAlignment(CrossAxisAlignment alignment);

		/**
		 * @brief Set the spacing between children.
		 * @param spacing The new spacing.
		 * @return Reference to this View for chaining.
		 */
		View &setSpacing(float spacing);

		/**
		 * @brief Set the padding around the children.
		 * @param padding The new padding.
		 * @return Reference to this View for chaining.
		 */
		View &setPadding(float padding);

		/**
		 * @brief Force the container width.
		 * @param width The new fixed width.
		 * @return Reference to this View for chaining.
		 */
		View &setFixedWidth(float width);

		/**
		 * @brief Clear the forced width.
		 * @return Reference to this View for chaining.
		 */
		View &clearFixedWidth(void);

		/**
		 * @brief Force the container height.
		 * @param height The new fixed height.
		 * @return Reference to this View for chaining.
		 */
		View &setFixedHeight(float height);

		/**
		 * @brief Clear the forced height.
		 * @return Reference to this View for chaining.
		 */
		View &clearFixedHeight(void);

		/**
		 * @brief Set the entities attached to the container.
		 *
		 * Each entity is parented to the view and its identifier recorded,
		 * in order, as a layout child in `components::Layout`. The view keeps
		 * no handle on the entities.
		 * @param entities The new entities to attach.
		 * @return Reference to this View for chaining.
		 */
		View &setEntities(
			const std::vector<std::shared_ptr<ecs::Entity>> &entities);

		/**
		 * @brief Parent the entities given to the constructor and arrange
		 * them.
		 *
		 * This holds no rule: parenting needs `shared_from_this()`, which
		 * the constructor cannot use.
		 */
		void initialize(void) override;

		private:
		/**
		 * @brief Arrange the layout children now with `systems::Layout`.
		 *
		 * The Layout system does the same every frame; this makes the result
		 * visible immediately while a scene is being built.
		 */
		void applyGeometry(void);
	};

}	 // namespace guillaume::entities
