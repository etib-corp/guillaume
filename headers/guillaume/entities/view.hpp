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

#include <utility>
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
	 * @brief A rectangle, colored or transparent, that lays its children
	 * out in a row or a column.
	 *
	 * View has no component of its own. Its state lives in
	 * components::Transform (pose of its top-left corner), components::Color
	 * (background), components::Borders (corner radius) and
	 * components::Layout (axis, alignments, spacing, padding, fixed size
	 * and the ordered identifiers of the children to arrange).
	 * systems::Layout holds the rules: each frame, in the Layout phase, it
	 * places every layout child along the axis and writes the size of the
	 * view into components::Bound. The class itself holds no field: its
	 * layout setters write the components and arrange the children at
	 * once.
	 *
	 * A view is also an entity registry (ecs::ParentEntityFiller), so it
	 * can own the widgets built inside it: pass `*view` as the entity
	 * registry of their builder.
	 *
	 * @code
	 * View::Builder viewBuilder(getComponentRegistry(), getEntityRegistry());
	 * auto row = viewBuilder.withAxis(View::Axis::Horizontal)
	 * 	.withSpacing(8.0f)
	 * 	.makeDefaultView(nullptr, utility::graphic::PoseF(), {});
	 *
	 * Text::Builder textBuilder(getComponentRegistry(), *row);
	 * auto name = textBuilder.makeText(row, "Name",
	 * 	theme::TypeRole::BodyLarge,
	 * 	getActiveColor(SchemeColorRole::OnSurface));
	 * row->setEntities({ name });
	 * @endcode
	 *
	 * `View::create(registry, pose, color, borderRadius, axis,
	 * mainAxisAlignment, crossAxisAlignment, spacing, padding,
	 * hasFixedWidth, fixedWidth, hasFixedHeight, fixedHeight, entities)`
	 * builds the same widget without registering it in an entity registry.
	 *
	 * Watch out:
	 * - Sizes, spacing, padding and radius are in dp.
	 * - Only the entities given to setEntities() (or to the builder) are
	 *   arranged. Parenting an entity to the view is not enough.
	 * - Attaching an entity does not make the view own it. Each child must
	 *   be owned by an entity registry (the view itself or another one),
	 *   or it is destroyed when the last pointer to it goes away.
	 * - The position of each layout child and the Bound of the view are
	 *   overwritten every frame. The size of a child is never changed.
	 * - Without a fixed width or height, the view takes the size of its
	 *   children plus the padding.
	 *
	 * @see components::Layout, systems::Layout, SurfaceBase
	 */
	class View:
		public std::enable_shared_from_this<View>,
		public ecs::ParentEntityFiller<components::Layout,
									   components::Transform, components::Bound,
									   components::Color, components::Borders>
	{
		public:
		/// Direction the children are arranged in: `Horizontal` (a row) or
		/// `Vertical` (a column).
		using Axis = components::Layout::Axis;
		/// How the children are distributed along the axis.
		using MainAxisAlignment = components::Layout::MainAxisAlignment;
		/// How each child is aligned across the axis.
		using CrossAxisAlignment = components::Layout::CrossAxisAlignment;

		/**
		 * @brief Configures View widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<View>
		{
			private:
			utility::graphic::PoseF _pose;	  ///< Pose of the top-left corner.
			utility::graphic::Color32Bit _color;	///< Background color.
			float _borderRadius;					///< Corner radius, in dp.
			Axis _axis;	   ///< Axis along which children are arranged.
			MainAxisAlignment _mainAxisAlignment;	 ///< Main axis alignment.
			CrossAxisAlignment
				_crossAxisAlignment;	///< Cross axis alignment.
			float _spacing;				///< Space between children, in dp.
			float _padding;				///< Space around children, in dp.
			bool _hasFixedWidth;		///< Whether the width is forced.
			bool _hasFixedHeight;		///< Whether the height is forced.
			float _fixedWidth;			///< Forced width, in dp.
			float _fixedHeight;			///< Forced height, in dp.
			std::vector<std::shared_ptr<ecs::Entity>>
				_entities;	  ///< Entities to arrange, in layout order.

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built views are stored in.
			 * @param entityRegistry The registry that owns the views built
			 * by the `make...` methods.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a view from the current configuration, without
			 * registering it or giving it a parent.
			 *
			 * The entities of withEntities() are parented to the new view
			 * and arranged.
			 * @return The new view.
			 */
			std::shared_ptr<View> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: default pose, transparent
			 * background, corner radius 16 dp, horizontal axis, main axis
			 * alignment `Start`, cross axis alignment `Center`, spacing
			 * 8 dp, padding 16 dp, no fixed width or height, and no entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the view is.
			 * @param pose The pose of the top-left corner of the view. A
			 * default-constructed pose by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the background color.
			 * @param color The color (RGBA). Transparent by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the corner radius of the background.
			 * @param borderRadius The radius of the four corners, in dp.
			 * 16 by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withBorderRadius(float borderRadius);

			/**
			 * @brief Choose between a row and a column.
			 * @param axis `Horizontal` to arrange the children left to
			 * right, `Vertical` for top to bottom. `Horizontal` by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withAxis(Axis axis);

			/**
			 * @brief Set how the children are distributed along the axis.
			 * @param alignment `Start`, `Center`, `End` or `SpaceBetween`.
			 * `Start` by default. It only shows when the view is larger
			 * than its content, that is with a fixed size on that axis.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withMainAxisAlignment(MainAxisAlignment alignment);

			/**
			 * @brief Set how each child is aligned across the axis.
			 * @param alignment `Start`, `Center` or `End`. `Center` by
			 * default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withCrossAxisAlignment(CrossAxisAlignment alignment);

			/**
			 * @brief Set the gap between two consecutive children.
			 * @param spacing The gap, in dp. 8 by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withSpacing(float spacing);

			/**
			 * @brief Set the space between the edges of the view and its
			 * children.
			 * @param padding The padding, in dp, the same on the four
			 * sides. 16 by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPadding(float padding);

			/**
			 * @brief Force the width of the view.
			 * @param width The width, in dp. By default the width follows
			 * the children.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Force the height of the view.
			 * @param height The height, in dp. By default the height
			 * follows the children.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFixedHeight(float height);

			/**
			 * @brief Set the entities the view arranges, in layout order.
			 *
			 * The builder keeps the pointers only until the view is built.
			 * The view does not own them either: each entity must be owned
			 * by an entity registry.
			 * @param entities The entities to arrange. Null entries are
			 * skipped. Empty by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withEntities(
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);

			/**
			 * @brief Build a transparent view, register it under `parent`
			 * and reset the builder.
			 *
			 * The background stays transparent unless withColor() was
			 * called. The other settings of the `with...` methods apply.
			 * @param parent The entity the view is attached to, or nullptr
			 * for a root entity.
			 * @param pose The pose of the top-left corner of the view.
			 * @param entities The entities to arrange, in layout order.
			 * They are parented to the view; each must be owned by an
			 * entity registry. May be empty.
			 * @return The registered view.
			 */
			std::shared_ptr<View> makeDefaultView(
				std::shared_ptr<Entity> parent,
				const utility::graphic::PoseF &pose,
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);

			/**
			 * @brief Build a view with a background color, register it
			 * under `parent` and reset the builder.
			 * @param parent The entity the view is attached to, or nullptr
			 * for a root entity.
			 * @param pose The pose of the top-left corner of the view.
			 * @param color The background color (RGBA).
			 * @param entities The entities to arrange, in layout order.
			 * They are parented to the view; each must be owned by an
			 * entity registry. May be empty.
			 * @return The registered view.
			 */
			std::shared_ptr<View> makeColorView(
				std::shared_ptr<Entity> parent,
				const utility::graphic::PoseF &pose,
				const utility::graphic::Color32Bit &color,
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);
		};

		/**
		 * @brief Build a view without registering it in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, pose, color, borderRadius, axis, mainAxisAlignment,
		 * crossAxisAlignment, spacing, padding, hasFixedWidth, fixedWidth,
		 * hasFixedHeight, fixedHeight, entities)`. It then calls build(),
		 * which parents the given entities to the view and arranges them.
		 * A view creates no child of its own.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new view. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<View> create(Arguments &&...arguments)
		{
			std::shared_ptr<View> entity(
				new View(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~View(void);

		/**
		 * @brief Move the view. Moves its layout children at once.
		 * @param pose The new pose of the top-left corner of the view.
		 * @return Reference to this view for chaining.
		 */
		View &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Set the background color.
		 * @param color The new color (RGBA), written to components::Color.
		 * A fully transparent color hides the background.
		 * @return Reference to this view for chaining.
		 */
		View &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the corner radius of the background.
		 * @param borderRadius The new radius of the four corners, in dp,
		 * written to components::Borders.
		 * @return Reference to this view for chaining.
		 */
		View &setBorderRadius(float borderRadius);

		/**
		 * @brief Choose between a row and a column. Arranges the children
		 * at once.
		 * @param axis `Horizontal` to arrange the children left to right,
		 * `Vertical` for top to bottom.
		 * @return Reference to this view for chaining.
		 */
		View &setAxis(Axis axis);

		/**
		 * @brief Set how the children are distributed along the axis.
		 * Arranges the children at once.
		 * @param alignment `Start`, `Center`, `End` or `SpaceBetween`. It
		 * only shows when the view has a fixed size on that axis.
		 * @return Reference to this view for chaining.
		 */
		View &setMainAxisAlignment(MainAxisAlignment alignment);

		/**
		 * @brief Set how each child is aligned across the axis. Arranges
		 * the children at once.
		 * @param alignment `Start`, `Center` or `End`.
		 * @return Reference to this view for chaining.
		 */
		View &setCrossAxisAlignment(CrossAxisAlignment alignment);

		/**
		 * @brief Set the gap between two consecutive children. Arranges
		 * the children at once.
		 * @param spacing The new gap, in dp.
		 * @return Reference to this view for chaining.
		 */
		View &setSpacing(float spacing);

		/**
		 * @brief Set the space between the edges of the view and its
		 * children. Arranges the children at once.
		 * @param padding The new padding, in dp, the same on the four
		 * sides.
		 * @return Reference to this view for chaining.
		 */
		View &setPadding(float padding);

		/**
		 * @brief Force the width of the view, whatever its children.
		 * Arranges the children at once.
		 * @param width The new width, in dp.
		 * @return Reference to this view for chaining.
		 */
		View &setFixedWidth(float width);

		/**
		 * @brief Let the width follow the children again: their extent
		 * plus the padding. Arranges the children at once.
		 * @return Reference to this view for chaining.
		 */
		View &clearFixedWidth(void);

		/**
		 * @brief Force the height of the view, whatever its children.
		 * Arranges the children at once.
		 * @param height The new height, in dp.
		 * @return Reference to this view for chaining.
		 */
		View &setFixedHeight(float height);

		/**
		 * @brief Let the height follow the children again: their extent
		 * plus the padding. Arranges the children at once.
		 * @return Reference to this view for chaining.
		 */
		View &clearFixedHeight(void);

		/**
		 * @brief Replace the entities the view arranges.
		 *
		 * Each entity is parented to the view and its identifier recorded,
		 * in order, as a layout child in `components::Layout`; the children
		 * are arranged at once. The view keeps no pointer to the entities:
		 * each must be owned by an entity registry. An entity of the
		 * previous list that is left out of the new one stays parented to
		 * the view but is not arranged.
		 * @param entities The entities to arrange, in layout order. Null
		 * entries are skipped.
		 * @return Reference to this view for chaining.
		 */
		View &setEntities(
			const std::vector<std::shared_ptr<ecs::Entity>> &entities);

		private:
		/**
		 * @brief Arrange the layout children now with `systems::Layout`.
		 *
		 * The Layout system does the same every frame; this makes the result
		 * visible immediately while a scene is being built.
		 */
		void applyGeometry(void);

		private:
		/**
		 * @brief Construct a view and write its components.
		 * @param registry The registry the components are stored in.
		 * @param pose The pose of the top-left corner of the view.
		 * @param color The background color (RGBA).
		 * @param borderRadius The corner radius, in dp.
		 * @param axis The axis along which children are arranged.
		 * @param mainAxisAlignment How the children are distributed along
		 * the axis.
		 * @param crossAxisAlignment How each child is aligned across the
		 * axis.
		 * @param spacing The gap between two consecutive children, in dp.
		 * @param padding The space around the children, in dp.
		 * @param hasFixedWidth Whether `fixedWidth` is applied.
		 * @param fixedWidth The forced width, in dp. Ignored when
		 * `hasFixedWidth` is false.
		 * @param hasFixedHeight Whether `fixedHeight` is applied.
		 * @param fixedHeight The forced height, in dp. Ignored when
		 * `hasFixedHeight` is false.
		 * @param entities The entities to arrange, in layout order. Their
		 * identifiers are recorded as layout children at once; they are
		 * parented to the view by build().
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
		 * @brief Parent the entities given to the constructor to the view,
		 * then arrange them.
		 *
		 * Called once by create(), when the view is held by a shared
		 * pointer and can be given as parent. The view stops holding the
		 * entities here: they stay alive through their entity registry.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
