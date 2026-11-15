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
#include <memory>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Everything needed to set up a surface in one value: where it
	 * is, how it looks and how it arranges its children.
	 *
	 * A surface is a rectangle that contains other widgets. This struct is a
	 * plain value used to pass its settings around: SurfaceBase copies each
	 * field into the matching component (Transform, Color, Borders, Layout)
	 * and keeps no copy. Every widget constructor that derives from
	 * SurfaceBase takes one.
	 *
	 * The setters return the configuration, so a value can be written in one
	 * expression:
	 * @code
	 * auto config = SurfaceConfig()
	 * 	.setAxis(components::Layout::Axis::Vertical)
	 * 	.setSpacing(8.0f)
	 * 	.setPadding(16.0f)
	 * 	.setFixedWidth(320.0f);
	 * @endcode
	 *
	 * Without a fixed width or height, the surface takes the size of its
	 * children plus the padding on both sides.
	 *
	 * @see SurfaceBase, components::Layout, systems::Layout
	 */
	struct SurfaceConfig {
		utility::graphic::PoseF
			pose {};	///< Position and orientation of the surface.
		utility::graphic::Color32Bit color {
			0, 0, 0, 0
		};	  ///< Fill color (transparent by default).
		float borderRadius { 0.0f };	///< Radius of the four corners, in dp.
		components::Layout::Axis axis {
			components::Layout::Axis::Horizontal
		};	  ///< Direction the children are arranged in.
		components::Layout::MainAxisAlignment mainAxisAlignment {
			components::Layout::MainAxisAlignment::Start
		};	  ///< Where the children go along the axis.
		components::Layout::CrossAxisAlignment crossAxisAlignment {
			components::Layout::CrossAxisAlignment::Center
		};	  ///< Where the children go across the axis.
		float spacing { 0.0f };	   ///< Gap between two children, in dp.
		float padding {
			0.0f
		};	  ///< Gap between the edges and the children, in dp.
		bool hasFixedWidth {
			false
		};	  ///< True when `fixedWidth` replaces the content width.
		float fixedWidth {
			0.0f
		};	  ///< Width in dp, used only when `hasFixedWidth`.
		bool hasFixedHeight {
			false
		};	  ///< True when `fixedHeight` replaces the content height.
		float fixedHeight {
			0.0f
		};	  ///< Height in dp, used only when `hasFixedHeight`.

		/**
		 * @brief Set where the surface is.
		 * @param value The position and orientation of the surface.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setPose(const utility::graphic::PoseF &value);

		/**
		 * @brief Set the fill color.
		 * @param value The color painted behind the children.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setColor(const utility::graphic::Color32Bit &value);

		/**
		 * @brief Set the radius of the four corners.
		 * @param value The radius, in dp. 0 gives square corners.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setBorderRadius(float value);

		/**
		 * @brief Choose between a row and a column.
		 * @param value Horizontal to arrange the children left to right,
		 * Vertical to arrange them top to bottom.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setAxis(components::Layout::Axis value);

		/**
		 * @brief Set where the children go along the axis.
		 * @param value Start, Center, End or SpaceBetween.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &
			setMainAxisAlignment(components::Layout::MainAxisAlignment value);

		/**
		 * @brief Set where the children go across the axis.
		 * @param value Start, Center or End.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &
			setCrossAxisAlignment(components::Layout::CrossAxisAlignment value);

		/**
		 * @brief Set the gap between two consecutive children.
		 * @param value The gap, in dp.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setSpacing(float value);

		/**
		 * @brief Set the gap between the edges of the surface and its
		 * children.
		 * @param value The gap, in dp, applied on the four sides.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setPadding(float value);

		/**
		 * @brief Give the surface a width that does not depend on its
		 * children.
		 *
		 * Sets both `fixedWidth` and `hasFixedWidth`.
		 * @param value The width, in dp.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setFixedWidth(float value);

		/**
		 * @brief Give the surface a height that does not depend on its
		 * children.
		 *
		 * Sets both `fixedHeight` and `hasFixedHeight`.
		 * @param value The height, in dp.
		 * @return This configuration, for chaining.
		 */
		SurfaceConfig &setFixedHeight(float value);

		/**
		 * @brief Get the configuration shared by dialog and picker panels.
		 *
		 * The result has a fixed width, 24 dp of padding, the extra-large
		 * corner radius of the active theme (28 dp by default) and children
		 * centered on both axes. The color is left transparent and the
		 * height follows the content: the caller sets what differs.
		 *
		 * @param width The panel width, in dp.
		 * @param axis The direction the children are arranged in.
		 * @param spacing The gap between two children, in dp.
		 * @return The panel configuration.
		 */
		static SurfaceConfig panel(float width, components::Layout::Axis axis,
								   float spacing);
	};

	/**
	 * @brief Base of the widgets that are a rectangle containing other
	 * widgets (Card, Dialog, Checkbox, ...).
	 *
	 * It has no component of its own. Its state is the five components it
	 * always carries: Transform (pose), Bound (size), Color (fill), Borders
	 * (corner radius) and Layout (axis, alignment, spacing, padding, fixed
	 * size and the list of children to arrange). systems::Layout holds the
	 * rules: in the Layout phase of every frame it positions the children
	 * listed in the Layout component and writes the size of the surface in
	 * its Bound. The setters of this class write the components and run the
	 * same arrangement at once, so the result is visible while a scene is
	 * being built.
	 *
	 * A widget derives from it and lists its own components in the template
	 * pack:
	 * @code
	 * class Checkbox:
	 * 	public SurfaceBase<components::Selectable, components::Line,
	 * 					   components::Checkbox>
	 * @endcode
	 *
	 * Watch out for:
	 * - Only the children given to setChildren() are arranged. A child
	 *   attached with `ecs::Entity::setParent()` alone keeps the position
	 *   its own code gives it.
	 * - The size of the surface (Bound) is overwritten every frame from its
	 *   children, its padding and its fixed size.
	 * - setChildren() needs the surface to be owned by a `std::shared_ptr`,
	 *   which is the case for every widget built with `create()` or a
	 *   Builder.
	 *
	 * @tparam ExtraComponents The components the derived widget adds to the
	 * five common ones.
	 *
	 * @see SurfaceConfig, components::Layout, systems::Layout
	 */
	template<ecs::InheritFromComponent... ExtraComponents> class SurfaceBase:
		public std::enable_shared_from_this<SurfaceBase<ExtraComponents...>>,
		public ecs::ParentEntityFiller<
			components::Layout, components::Transform, components::Bound,
			components::Color, components::Borders, ExtraComponents...>
	{
		public:
		using Children =
			std::vector<std::shared_ptr<ecs::Entity>>;	  ///< List of child
														  ///< entities.

		/**
		 * @brief Default destructor.
		 */
		~SurfaceBase(void) override = default;

		/**
		 * @brief Move the surface.
		 *
		 * Writes the Transform component, then arranges the layout children
		 * again so they follow.
		 * @param pose The new position and orientation.
		 * @return This surface, for chaining.
		 */
		SurfaceBase &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Set the fill color (Color component).
		 *
		 * Widgets whose system computes the color from the theme (Surface,
		 * Checkbox, ...) overwrite it on their next restyle.
		 * @param color The new fill color.
		 * @return This surface, for chaining.
		 */
		SurfaceBase &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the radius of the four corners (Borders component).
		 * @param radius The radius, in dp.
		 * @return This surface, for chaining.
		 */
		SurfaceBase &setBorderRadius(float radius);

		/**
		 * @brief Apply every field of a configuration at once.
		 *
		 * Writes the Transform, Color, Borders and Layout components. A
		 * fixed width or height that the configuration does not have is
		 * cleared. The list of layout children is kept, and the children are
		 * arranged again at once.
		 * @param config The configuration to copy into the components.
		 * @return This surface, for chaining.
		 */
		SurfaceBase &setSurfaceConfig(const SurfaceConfig &config);

		/**
		 * @brief Read the current configuration back from the components.
		 *
		 * The surface keeps no copy of the configuration it was given: the
		 * value is rebuilt from the Transform, Color, Borders and Layout
		 * components, so it reflects changes made through them. The corner
		 * radius reported is the top-left one.
		 * @return The configuration matching the current components.
		 */
		SurfaceConfig getSurfaceConfig(void) const;

		/**
		 * @brief Choose the children this surface arranges, in order.
		 *
		 * Each child becomes a child entity of the surface and its
		 * identifier is recorded in the Layout component, replacing the
		 * previous list. The children are arranged at once. Null pointers
		 * and entities without a valid identifier are skipped. The surface
		 * does not keep the children alive: each one must be owned by an
		 * entity registry, which is the case when it was created by a
		 * Builder.
		 * @param children The children, in layout order (left to right or
		 * top to bottom).
		 * @return This surface, for chaining.
		 */
		SurfaceBase &setChildren(const Children &children);

		/**
		 * @brief Get the children this surface arranges.
		 * @return The identifiers recorded in the Layout component, in
		 * layout order. Empty when setChildren() was never called. Children
		 * attached without setChildren() are not listed.
		 */
		std::vector<ecs::Entity::Identifier> getChildren(void) const;

		protected:
		/**
		 * @brief Create the entity with its components and apply a
		 * configuration.
		 * @param registry The registry that stores the components.
		 * @param config The initial configuration, applied with
		 * setSurfaceConfig().
		 */
		SurfaceBase(ecs::ComponentRegistry &registry,
					const SurfaceConfig &config);

		/**
		 * @brief Arrange the layout children now with systems::Layout::apply.
		 *
		 * The Layout system does the same every frame. Calling it from a
		 * setter makes the new positions and the new size of the surface
		 * available immediately, before the first frame.
		 */
		void applyGeometry(void);
	};

}	 // namespace guillaume::entities

#include "guillaume/entities/surface.tpp"

#include "guillaume/components/elevation.hpp"
#include "guillaume/components/surface_tone.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/theme/tokens.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A plain container whose background gets lighter or darker with
	 * its elevation, and which can cast a shadow.
	 *
	 * Use it to group widgets on a visible panel. Its state lives in
	 * components::SurfaceTone (elevation level and whether a shadow is
	 * cast). systems::SurfaceTone holds the rule: in the Measure phase of
	 * every frame it writes the fill color (the Surface color of the active
	 * theme tinted for the level) and the shadow level. Children are
	 * arranged by systems::Layout, as for every SurfaceBase.
	 *
	 * @code
	 * Surface::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto panel = builder.withShape(theme::ShapeRole::Medium)
	 * 	.makeSurface(nullptr, theme::ElevationLevel::Level2);
	 * panel->setChildren({ title, body });
	 * @endcode
	 *
	 * `Surface::create(registry, config, elevation, mode)` builds the same
	 * widget without registering it in an entity registry.
	 *
	 * A color set with setColor() does not last: the system recomputes the
	 * fill color from the elevation every frame.
	 *
	 * @see components::SurfaceTone, systems::SurfaceTone, SurfaceBase
	 */
	class Surface:
		public SurfaceBase<components::Elevation, components::SurfaceTone>
	{
		public:
		/**
		 * @brief Whether the surface casts a shadow.
		 */
		enum class Mode {
			Flat,		///< Tinted background, no shadow.
			Elevated	///< Tinted background and the shadow of the level.
		};

		/**
		 * @brief Configures and registers Surface widgets.
		 */
		class Builder: public EntityBuilderBase<Surface>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, corner radius and layout.
			theme::ElevationLevel _elevation {
				theme::ElevationLevel::Level1
			};	  ///< Elevation level of the next surface.
			Mode _mode {
				Mode::Elevated
			};	  ///< Whether the next surface casts a shadow.

			public:
			/**
			 * @brief Construct a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the surfaces built.
			 * @param entityRegistry The registry that owns the surfaces
			 * built.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Create a surface from the current configuration,
			 * without attaching nor registering it.
			 * @return The new surface.
			 */
			std::shared_ptr<Surface> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: elevation level 1, a shadow
			 * (Mode::Elevated), square corners, children in a column
			 * aligned at the start and centered across, no spacing, no
			 * padding, no fixed size, default pose.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the surface is.
			 * @param pose The position and orientation (default: the
			 * default pose).
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the elevation level, which tints the background
			 * and sizes the shadow.
			 * @param elevation The level, from Level0 to Level5 (default:
			 * Level1).
			 * @return This builder, for chaining.
			 */
			Builder &withElevation(theme::ElevationLevel elevation);

			/**
			 * @brief Choose whether the surface casts a shadow.
			 * @param mode Elevated for a shadow, Flat for none (default:
			 * Elevated).
			 * @return This builder, for chaining.
			 */
			Builder &withMode(Mode mode);

			/**
			 * @brief Set the corner radius from a shape role of the theme.
			 * @param role The shape role, such as theme::ShapeRole::Medium
			 * (default: None, square corners).
			 * @return This builder, for chaining.
			 */
			Builder &withShape(theme::ShapeRole role);

			/**
			 * @brief Replace the layout configuration: axis, alignment,
			 * spacing, padding, fixed size and corner radius.
			 *
			 * The pose already given to the builder is kept, so call
			 * withShape() after this method, not before. The color of the
			 * configuration has no lasting effect: the fill color comes
			 * from the elevation.
			 * @param config The configuration to copy, except its pose.
			 * @return This builder, for chaining.
			 */
			Builder &withConfig(const SurfaceConfig &config);

			/**
			 * @brief Build a surface with the given elevation and register
			 * it under a parent.
			 *
			 * The other settings are those given with the `with...()`
			 * methods. The builder is back to its defaults afterwards.
			 * @param parent The entity the surface becomes a child of, or
			 * nullptr.
			 * @param elevation The elevation level, from Level0 to Level5.
			 * @param mode Elevated for a shadow, Flat for none.
			 * @return The registered surface.
			 */
			std::shared_ptr<Surface>
				makeSurface(std::shared_ptr<ecs::Entity> parent,
							theme::ElevationLevel elevation,
							Mode mode = Mode::Elevated);
		};

		/**
		 * @brief Create a surface without registering it.
		 *
		 * The constructor is private so that a surface is always owned by a
		 * `std::shared_ptr`, which setChildren() requires. The arguments are
		 * those of the constructor: `(registry, config, elevation, mode)`.
		 * The caller keeps the surface alive, or hands it to an entity
		 * registry; Builder does both steps.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The constructor arguments.
		 * @return The new surface, already styled for its elevation.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Surface> create(Arguments &&...arguments)
		{
			std::shared_ptr<Surface> entity(
				new Surface(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Surface(void) override = default;

		/**
		 * @brief Change the elevation level.
		 *
		 * Writes components::SurfaceTone and restyles at once: the fill
		 * color and, in Mode::Elevated, the shadow follow.
		 * @param elevation The level, from Level0 to Level5.
		 * @return This surface, for chaining.
		 */
		Surface &setElevation(theme::ElevationLevel elevation);

		/**
		 * @brief Get the elevation level.
		 * @return The level stored in components::SurfaceTone. It tints the
		 * background even in Mode::Flat.
		 */
		theme::ElevationLevel getElevation(void) const;

		/**
		 * @brief Turn the shadow on or off, and restyle at once.
		 * @param mode Elevated for a shadow, Flat for none.
		 * @return This surface, for chaining.
		 */
		Surface &setMode(Mode mode);

		/**
		 * @brief Tell whether the surface casts a shadow.
		 * @return Mode::Elevated when it does, Mode::Flat otherwise.
		 */
		Mode getMode(void) const;

		private:
		/**
		 * @brief Construct a surface and style it for its elevation.
		 * @param registry The registry that stores the components.
		 * @param config The pose, corner radius and layout. Its color is
		 * replaced by the color of the elevation.
		 * @param elevation The elevation level, from Level0 to Level5.
		 * @param mode Elevated for a shadow, Flat for none.
		 */
		Surface(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				theme::ElevationLevel elevation, Mode mode);
	};

}	 // namespace guillaume::entities
