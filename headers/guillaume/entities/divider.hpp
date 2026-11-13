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

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/divider.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A thin horizontal line separating two pieces of content.
	 *
	 * Divider is a leaf widget: it has no children. Its state lives in
	 * components::Divider (variant, length, thickness, inset, color, origin
	 * pose); systems::Divider turns it into the entity's Transform, Bound,
	 * Line and Color each frame, in the Measure phase, and
	 * systems::LineRender draws the stroke. The class itself holds no
	 * field: its setters write the component and run systems::Divider at
	 * once.
	 *
	 * The variant chooses which ends are shortened by the inset:
	 * - `FullWidth`: spans the whole length.
	 * - `Inset`: leaves the inset on the leading end.
	 * - `Middle`: leaves the inset on both ends.
	 *
	 * @code
	 * Divider::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto divider = builder.makeDivider(parent, 320.0f,
	 * 	Divider::Variant::Inset);
	 * divider->setBold(true);
	 * @endcode
	 *
	 * `Divider::create(registry, pose, color, length, thickness, inset,
	 * variant)` builds the same widget without registering it in an entity
	 * registry.
	 *
	 * Watch out:
	 * - Lengths, thicknesses and insets are in dp.
	 * - The Bound, Line thickness and Color of the entity are overwritten
	 *   every frame from components::Divider: change them with the setters
	 *   of this class, not by writing those components.
	 * - The Transform pose is the origin pose shifted by the leading
	 *   inset. When something else moves the entity (a parent layout for
	 *   example), the system takes the new position as the drawn start of
	 *   the line and recomputes the origin from it.
	 *
	 * @see components::Divider, systems::Divider, systems::LineRender
	 */
	class Divider:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Color, components::Line,
								 components::Divider>
	{
		public:
		/**
		 * @brief Which ends of the line are shortened by the inset:
		 * `FullWidth` (none), `Inset` (leading end) or `Middle` (both).
		 */
		using Variant = components::Divider::Variant;

		/**
		 * @brief Default stroke thickness, in dp: a hairline (Paper
		 * `StyleSheet.hairlineWidth` on a 2x display).
		 */
		static constexpr float HairlineThickness =
			components::Divider::HairlineThickness;

		/**
		 * @brief Stroke thickness of a bold divider, in dp (Paper `bold`).
		 */
		static constexpr float BoldThickness =
			components::Divider::BoldThickness;

		/**
		 * @brief Configures Divider widgets and registers them in the
		 * scene's entity registry.
		 */
		class Builder: public EntityBuilderBase<Divider>
		{
			private:
			utility::graphic::PoseF
				_pose {};	 ///< Pose of the line before the leading inset.
			utility::graphic::Color32Bit _color {};	   ///< Stroke color.
			float _length { 100.0f };	 ///< Total length, insets included,
										 ///< in dp.
			float _thickness { HairlineThickness };	   ///< Stroke, in dp.
			float _inset { 16.0f };	   ///< Inset of an inset end, in dp.
			Variant _variant { Variant::FullWidth };	///< Which ends are
														///< inset.

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built dividers are stored in.
			 * @param entityRegistry The registry that owns the dividers
			 * built by `makeDivider`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a divider from the current configuration,
			 * without registering it or giving it a parent.
			 * @return The new divider.
			 */
			std::shared_ptr<Divider> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: default pose, the OutlineVariant
			 * color of the active theme, length 100 dp, hairline thickness
			 * (`HairlineThickness`), inset 16 dp, and the `FullWidth`
			 * variant.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the line starts.
			 * @param pose The pose of the line before the leading inset is
			 * applied. A default-constructed pose by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the stroke color.
			 * @param color The color (RGBA). By default the OutlineVariant
			 * color the active theme had when the builder was last reset.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the total length.
			 * @param length The length, insets included, in dp. 100 by
			 * default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withLength(float length);

			/**
			 * @brief Set the stroke thickness.
			 * @param thickness The thickness, in dp. `HairlineThickness`
			 * by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withThickness(float thickness);

			/**
			 * @brief Set how much an inset end is shortened.
			 * @param inset The inset, in dp, applied to the ends the
			 * variant insets. 16 by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withInset(float inset);

			/**
			 * @brief Choose which ends of the line are inset.
			 * @param variant The variant. `FullWidth` by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Build a divider, register it under `parent` and reset
			 * the builder.
			 *
			 * The pose, color, thickness and inset set with the `with...`
			 * methods apply to the built divider.
			 * @param parent The entity the divider is attached to, or
			 * nullptr for a root entity.
			 * @param length The total length, insets included, in dp.
			 * @param variant Which ends of the line are inset.
			 * @return The registered divider.
			 */
			std::shared_ptr<Divider> makeDivider(std::shared_ptr<Entity> parent,
												 float length, Variant variant);
		};

		public:
		/**
		 * @brief Build a divider without registering it in an entity
		 * registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, pose, color, length, thickness, inset, variant)`. A
		 * divider has no children to build.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new divider. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Divider> create(Arguments &&...arguments)
		{
			std::shared_ptr<Divider> entity(
				new Divider(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Divider(void) override;

		/**
		 * @brief Choose which ends of the line are inset. Moves and
		 * resizes the line at once.
		 * @param variant The new variant.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setVariant(Variant variant);

		/**
		 * @brief Set the total length. Resizes the line at once.
		 * @param length The new length, insets included, in dp. The drawn
		 * line is this length minus the insets of the variant, and never
		 * negative.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setLength(float length);

		/**
		 * @brief Set the stroke thickness. Restyles at once.
		 * @param thickness The new thickness, in dp.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setThickness(float thickness);

		/**
		 * @brief Switch between the hairline and the bold stroke (Paper
		 * `bold`). Restyles at once.
		 * @param bold True for `BoldThickness` (1 dp), false for
		 * `HairlineThickness` (0.5 dp).
		 * @return Reference to this divider for chaining.
		 */
		Divider &setBold(bool bold);

		/**
		 * @brief Check whether the divider uses the bold stroke.
		 * @return True when the thickness is at least `BoldThickness`,
		 * whether it was set by setBold() or setThickness().
		 */
		bool isBold(void) const;

		/**
		 * @brief Set how much an inset end is shortened. Moves and resizes
		 * the line at once.
		 * @param inset The new inset, in dp. It has no effect with the
		 * `FullWidth` variant.
		 * @return Reference to this divider for chaining.
		 */
		Divider &setInset(float inset);

		/**
		 * @brief Set the stroke color. Restyles at once.
		 * @param color The new color (RGBA).
		 * @return Reference to this divider for chaining.
		 */
		Divider &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Tell which ends of the line are inset.
		 * @return The variant stored in components::Divider.
		 */
		Variant getVariant(void) const;

		private:
		/**
		 * @brief Access the divider configuration component.
		 * @return The component holding this divider's state.
		 */
		components::Divider &config(void);

		/**
		 * @brief Access the divider configuration component.
		 * @return The component holding this divider's state.
		 */
		const components::Divider &config(void) const;

		/**
		 * @brief Run systems::Divider on this entity right now, so the
		 * Transform, Bound, Line and Color match the configuration without
		 * waiting for the next frame.
		 */
		void apply(void);

		private:
		/**
		 * @brief Construct a divider and apply its configuration.
		 * @param registry The registry the components are stored in.
		 * @param pose The pose of the line before the leading inset.
		 * @param color The stroke color (RGBA).
		 * @param length The total length, insets included, in dp.
		 * @param thickness The stroke thickness, in dp.
		 * @param inset The inset of an inset end, in dp.
		 * @param variant Which ends of the line are inset.
		 */
		Divider(ecs::ComponentRegistry &registry,
				const utility::graphic::PoseF &pose,
				const utility::graphic::Color32Bit &color, float length,
				float thickness, float inset, Variant variant);
	};

}	 // namespace guillaume::entities
