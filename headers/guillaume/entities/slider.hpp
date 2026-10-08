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

#include <functional>
#include <memory>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/range.hpp"
#include "guillaume/components/value.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Slider entity: a track with a draggable value handle.
	 *
	 * The slider reuses `SurfaceBase` for its geometry, `components::Value`
	 * for the single-handle value, `components::Range` for the two-handle
	 * interval and `components::DragInteraction` (driven by
	 * `systems::Drag` via the mouse/hand button interactions) to update the
	 * value while dragging. The variants are:
	 * - `Continuous`: a single handle over a free value.
	 * - `Discrete`: a single handle snapped to a step.
	 * - `Range`: two handles selecting an interval.
	 *
	 * @see SurfaceBase
	 * @see components::Value
	 * @see components::Range
	 * @see components::DragInteraction
	 */
	class Slider:
		public SurfaceBase<components::Value, components::Range,
						   components::DragInteraction,
						   components::MouseButtonInteraction,
						   components::HandButtonInteraction>
	{
		public:
		/**
		 * @brief Slider visual variant.
		 */
		enum class Variant {
			Continuous,	   ///< Single free handle.
			Discrete,	   ///< Single stepped handle.
			Range		   ///< Two-handle interval.
		};

		/**
		 * @brief Builder used to configure and create `Slider` entities.
		 */
		class Builder: public EntityBuilderBase<Slider>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Continuous };	 ///< Visual variant.
			float _min { 0.0f };						 ///< Minimum value.
			float _max { 100.0f };						 ///< Maximum value.
			float _value { 0.0f };						 ///< Initial value.
			float _step { 0.0f };						 ///< Snapping step.
			float _width { 200.0f };					 ///< Track length.
			std::function<void(float)> _onChanged {};	 ///< Change callback.

			public:
			/**
			 * @brief Construct a new Slider Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Slider entity from the current configuration.
			 * @return The newly created Slider.
			 */
			std::shared_ptr<Slider> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the slider pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the slider variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the slider value range.
			 * @param minimum The new minimum value.
			 * @param maximum The new maximum value.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withRange(float minimum, float maximum);

			/**
			 * @brief Set the initial value.
			 * @param value The new value.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withValue(float value);

			/**
			 * @brief Set the snapping step.
			 * @param step The new step (0 disables snapping).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withStep(float step);

			/**
			 * @brief Set the track length.
			 * @param width The new track length.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withWidth(float width);

			/**
			 * @brief Set the value change callback.
			 * @param onChanged The callback invoked when the value changes.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnChanged(const std::function<void(float)> &onChanged);
		};

		/**
		 * @brief Director that orchestrates `Slider::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a slider using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The slider variant.
			 * @param minimum The minimum value.
			 * @param maximum The maximum value.
			 * @param value The initial value.
			 * @return The newly created slider.
			 */
			std::shared_ptr<Slider>
				makeSlider(Builder &builder,
						   std::shared_ptr<ecs::Entity> parent, Variant variant,
						   float minimum, float maximum, float value);
		};

		private:
		Variant _variant { Variant::Continuous };	 ///< Visual variant.
		float _min { 0.0f };						 ///< Minimum value.
		float _max { 100.0f };						 ///< Maximum value.
		float _step { 0.0f };						 ///< Snapping step.
		float _trackLength { 200.0f };				 ///< Track length.
		std::function<void(float)> _onChanged {};	 ///< User change callback.
		std::shared_ptr<ecs::Entity> _track;		 ///< Track child.
		std::shared_ptr<ecs::Entity> _fill;			 ///< Active fill child.
		std::shared_ptr<ecs::Entity> _lowThumb;		 ///< Low handle child.
		std::shared_ptr<ecs::Entity> _highThumb;	 ///< High handle child.
		bool _built { false };	  ///< Whether children exist.

		private:
		/**
		 * @brief Build (and attach) the track, fill and handle children.
		 */
		void buildChildren(void);

		/**
		 * @brief Refresh the track, fill and handles from the current state.
		 */
		void applyState(void);

		/**
		 * @brief Normalize a value to `[0, 1]` within the slider range.
		 * @param value The value to normalize.
		 * @return The normalized value.
		 */
		float normalize(float value) const;

		/**
		 * @brief Position one handle at a normalized location.
		 * @param thumb The handle entity.
		 * @param normalized The normalized location, in `[0, 1]`.
		 */
		void placeThumb(const std::shared_ptr<ecs::Entity> &thumb,
						float normalized);

		public:
		/**
		 * @brief Construct a Slider entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The visual variant.
		 * @param minimum The minimum value.
		 * @param maximum The maximum value.
		 * @param value The initial value.
		 * @param step The snapping step (0 disables snapping).
		 * @param onChanged The value change callback.
		 */
		Slider(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, float minimum, float maximum, float value,
			   float step, const std::function<void(float)> &onChanged);

		/**
		 * @brief Default destructor.
		 */
		~Slider(void) override;

		/**
		 * @brief Set the slider variant.
		 * @param variant The new variant.
		 * @return Reference to this slider for chaining.
		 */
		Slider &setVariant(Variant variant);

		/**
		 * @brief Get the slider variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the current value.
		 * @param value The new value.
		 * @return Reference to this slider for chaining.
		 */
		Slider &setValue(float value);

		/**
		 * @brief Get the current value.
		 * @return The current value.
		 */
		float getValue(void) const;

		/**
		 * @brief Set the slider value range.
		 * @param minimum The new minimum value.
		 * @param maximum The new maximum value.
		 * @return Reference to this slider for chaining.
		 */
		Slider &setRange(float minimum, float maximum);

		/**
		 * @brief Get the low end of the range interval.
		 * @return The low value.
		 */
		float getLow(void) const;

		/**
		 * @brief Get the high end of the range interval.
		 * @return The high value.
		 */
		float getHigh(void) const;

		/**
		 * @brief Set the value change callback.
		 * @param onChanged The new callback.
		 * @return Reference to this slider for chaining.
		 */
		Slider &
			setOnChangedHandler(const std::function<void(float)> &onChanged);

		/**
		 * @brief Initialize the slider derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the slider derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
