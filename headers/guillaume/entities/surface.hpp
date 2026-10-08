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
	 * @brief Shared configuration describing a component surface.
	 *
	 * The configuration is consumed by `SurfaceBase` (and its concrete
	 * component families) to configure the common `components::Layout`,
	 * `components::Transform`, `components::Bound`, `components::Color` and
	 * `components::Borders` components. Arrangement is delegated to
	 * `systems::Layout`, so families never hand-roll child positioning.
	 *
	 * @see SurfaceBase
	 * @see systems::Layout
	 */
	struct SurfaceConfig {
		utility::graphic::PoseF pose {};	///< Surface pose.
		utility::graphic::Color32Bit color { 0, 0, 0,
											 0 };	 ///< Surface fill color.
		float borderRadius { 0.0f };				 ///< Uniform corner radius.
		components::Layout::Axis axis {
			components::Layout::Axis::Horizontal
		};	  ///< Arrangement axis.
		components::Layout::MainAxisAlignment mainAxisAlignment {
			components::Layout::MainAxisAlignment::Start
		};	  ///< Main axis alignment.
		components::Layout::CrossAxisAlignment crossAxisAlignment {
			components::Layout::CrossAxisAlignment::Center
		};	  ///< Cross axis alignment.
		float spacing { 0.0f };			  ///< Space between children.
		float padding { 0.0f };			  ///< Space around children.
		bool hasFixedWidth { false };	  ///< Whether the width is forced.
		float fixedWidth { 0.0f };		  ///< Forced width.
		bool hasFixedHeight { false };	  ///< Whether the height is forced.
		float fixedHeight { 0.0f };		  ///< Forced height.

		/**
		 * @brief Set the surface pose.
		 * @param value The new pose.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setPose(const utility::graphic::PoseF &value);

		/**
		 * @brief Set the surface fill color.
		 * @param value The new fill color.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setColor(const utility::graphic::Color32Bit &value);

		/**
		 * @brief Set the uniform corner radius.
		 * @param value The new corner radius.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setBorderRadius(float value);

		/**
		 * @brief Set the arrangement axis.
		 * @param value The new axis.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setAxis(components::Layout::Axis value);

		/**
		 * @brief Set the main axis alignment.
		 * @param value The new alignment.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &
			setMainAxisAlignment(components::Layout::MainAxisAlignment value);

		/**
		 * @brief Set the cross axis alignment.
		 * @param value The new alignment.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &
			setCrossAxisAlignment(components::Layout::CrossAxisAlignment value);

		/**
		 * @brief Set the spacing between children.
		 * @param value The new spacing.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setSpacing(float value);

		/**
		 * @brief Set the padding around children.
		 * @param value The new padding.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setPadding(float value);

		/**
		 * @brief Force the surface width.
		 * @param value The new fixed width.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setFixedWidth(float value);

		/**
		 * @brief Force the surface height.
		 * @param value The new fixed height.
		 * @return Reference to this configuration for chaining.
		 */
		SurfaceConfig &setFixedHeight(float value);

		/**
		 * @brief Create the common M3 dialog/picker panel configuration.
		 *
		 * Returns a centered surface using the surface-container-high color,
		 * a 28px corner radius, 24px padding and a fixed width. Variants that
		 * differ only in their dimensions/axis can start from this preset and
		 * override the relevant fields.
		 *
		 * @param width The panel width.
		 * @param axis The arrangement axis.
		 * @param spacing The spacing between children.
		 * @return A preconfigured surface configuration.
		 */
		static SurfaceConfig panel(float width, components::Layout::Axis axis,
								   float spacing);
	};

	/**
	 * @brief Reusable entity base for component families.
	 *
	 * `SurfaceBase` owns the common component signature (layout, transform,
	 * bound, color, borders) plus any extra components injected by the
	 * concrete family through the template pack. It arranges its children with
	 * `systems::Layout`, so derived families only configure the surface and
	 * attach content.
	 *
	 * @tparam ExtraComponents Additional component types owned by the family.
	 *
	 * @see SurfaceConfig
	 */
	template<ecs::InheritFromComponent... ExtraComponents> class SurfaceBase:
		public std::enable_shared_from_this<SurfaceBase<ExtraComponents...>>,
		public ecs::ParentEntityFiller<
			components::Layout, components::Transform, components::Bound,
			components::Color, components::Borders, ExtraComponents...>
	{
		public:
		using Children =
			std::vector<std::shared_ptr<ecs::Entity>>;	  ///< Child entities.

		/**
		 * @brief Default destructor.
		 */
		~SurfaceBase(void) override = default;

		/**
		 * @brief Set the surface pose.
		 * @param pose The new pose.
		 * @return Reference to this surface for chaining.
		 */
		SurfaceBase &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Set the surface fill color.
		 * @param color The new fill color.
		 * @return Reference to this surface for chaining.
		 */
		SurfaceBase &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the uniform corner radius.
		 * @param radius The new corner radius.
		 * @return Reference to this surface for chaining.
		 */
		SurfaceBase &setBorderRadius(float radius);

		/**
		 * @brief Apply a full surface configuration.
		 * @param config The configuration to apply.
		 * @return Reference to this surface for chaining.
		 */
		SurfaceBase &setSurfaceConfig(const SurfaceConfig &config);

		/**
		 * @brief Get the current surface configuration.
		 * @return The surface configuration.
		 */
		const SurfaceConfig &getSurfaceConfig(void) const;

		/**
		 * @brief Attach the child entities arranged by this surface.
		 * @param children The children to attach.
		 * @return Reference to this surface for chaining.
		 */
		SurfaceBase &setChildren(const Children &children);

		/**
		 * @brief Get the attached child entities.
		 * @return The attached children.
		 */
		const Children &getChildren(void) const;

		/**
		 * @brief Arrange the surface children and update its bound.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the surface derived state.
		 */
		void update(void) override;

		protected:
		/**
		 * @brief Construct a surface base.
		 * @param registry The component registry.
		 * @param config The initial surface configuration.
		 */
		SurfaceBase(ecs::ComponentRegistry &registry,
					const SurfaceConfig &config);

		/**
		 * @brief Arrange the attached children with `systems::Layout`.
		 */
		void applyGeometry(void);

		private:
		SurfaceConfig _surfaceConfig;	 ///< Current surface configuration.
		Children _children {};			 ///< Attached children.
	};

}	 // namespace guillaume::entities

#include "guillaume/entities/surface.tpp"
