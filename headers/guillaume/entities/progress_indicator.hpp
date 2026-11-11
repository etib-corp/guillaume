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
#include <optional>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/animation.hpp"
#include "guillaume/components/progress_indicator.hpp"
#include "guillaume/components/value.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A bar or a ring that fills up to show how far a task has
	 * progressed, or that loops when the progress is unknown.
	 *
	 * The progress, 0 (nothing done) to 1 (complete), lives in
	 * components::Value. The rest of the state lives in
	 * components::ProgressIndicator (variant, dimensions, color,
	 * visibility, running transition values).
	 * systems::ProgressIndicator sizes the widget and draws its track and
	 * fill each frame, in the Layout phase. The class itself holds no
	 * field: its setters write the components and redraw at once.
	 *
	 * @code
	 * ProgressIndicator::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto progress = builder.withWidth(240.0f).makeProgressIndicator(parent,
	 * 	ProgressIndicator::Variant::LinearDeterminate, 0.0f);
	 * // later, as the task advances:
	 * progress->setValue(0.5f);
	 * @endcode
	 *
	 * `ProgressIndicator::create(registry, config, variant, value)` builds
	 * the same widget without registering it in an entity registry.
	 *
	 * The widget has two children: the track, drawn in full, and the fill,
	 * drawn over it. The variants are:
	 * - `LinearDeterminate`: a 4 dp thick bar, 200 dp long by default,
	 *   filled from its start up to the value.
	 * - `LinearIndeterminate`: the same bar, with a segment of 60% of its
	 *   length sliding along it.
	 * - `CircularDeterminate`: a 4 dp thick ring, 48 dp wide by default,
	 *   whose arc starts at the top and covers the value.
	 * - `CircularIndeterminate`: the same ring, with a quarter-turn arc
	 *   rotating around it.
	 *
	 * What drives the animations: the widget's own components::Animation
	 * runs the 200 ms transition of the drawn value toward a new value,
	 * and the 200 ms fade of setVisible(). In the indeterminate variants
	 * the fill child carries a looping components::Animation of 1.5
	 * second per cycle, started when the child is built. systems::Animation
	 * samples both every frame, in the Event phase.
	 *
	 * Things to know:
	 * - Values outside 0 to 1 are clamped. The indeterminate variants
	 *   store the value but do not draw it.
	 * - The value given at creation is drawn as is; only later changes
	 *   are animated.
	 * - The length and the diameter are chosen in the builder and cannot
	 *   be changed afterwards. After a setVariant() from a bar to a ring
	 *   or back, the shape the indicator was not created with has its
	 *   default size (200 dp long or 48 dp wide).
	 * - A hidden indicator keeps its place in its parent's layout.
	 * - systems::ProgressIndicator overwrites the widget's size, padding,
	 *   color and corner radius every frame.
	 *
	 * @see components::ProgressIndicator, systems::ProgressIndicator,
	 * LoadingIndicator
	 */
	class ProgressIndicator:
		public SurfaceBase<components::Value, components::Animation,
						   components::Accessibility,
						   components::ProgressIndicator>
	{
		public:
		/// Shape and kind of the indicator: a bar or a ring, determinate
		/// (shows a value) or indeterminate (loops).
		using Variant = components::ProgressIndicator::Variant;

		/**
		 * @brief Configures a ProgressIndicator, then builds it and
		 * registers it in the entity registry given at construction.
		 */
		class Builder: public EntityBuilderBase<ProgressIndicator>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and size of the widget.
			Variant _variant { Variant::LinearDeterminate };	///< Variant.
			float _value { 0.0f };		///< Initial progress, 0 to 1.
			float _width { 200.0f };	///< Length of the bar, in dp.
			float _size { 48.0f };		///< Diameter of the ring, in dp.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry the indicator's
			 * components are created in.
			 * @param entityRegistry The registry the built indicators are
			 * added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a progress indicator and its track and fill
			 * children from the current configuration, without registering
			 * it or setting its parent.
			 * @return The new indicator, visible.
			 */
			std::shared_ptr<ProgressIndicator> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: LinearDeterminate variant, value
			 * 0, a 200 dp long bar, a 48 dp wide ring and a default pose.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose the indicator is created with.
			 * @param pose The position and orientation of the widget.
			 * Default: a default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the indicator variant.
			 * @param variant The variant. Default:
			 * `Variant::LinearDeterminate`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the progress the indicator starts with.
			 * @param value The progress, 0 to 1; a value outside is
			 * clamped. Default: 0.
			 * @return This builder, for chaining.
			 */
			Builder &withValue(float value);

			/**
			 * @brief Set the length of the bar of the linear variants.
			 * @param width The length, in dp. Default: 200. The circular
			 * variants ignore it.
			 * @return This builder, for chaining.
			 */
			Builder &withWidth(float width);

			/**
			 * @brief Set the diameter of the ring of the circular variants.
			 * @param size The diameter, in dp. Default: 48. The linear
			 * variants ignore it.
			 * @return This builder, for chaining.
			 */
			Builder &withSize(float size);

			/**
			 * @brief Build a progress indicator, register it under
			 * @p parent and reset the builder.
			 *
			 * The arguments replace the variant and value set before; the
			 * pose, length and diameter are kept.
			 * @param parent The entity the indicator is attached to.
			 * @param variant The indicator variant.
			 * @param value The progress it starts with, 0 to 1; a value
			 * outside is clamped. Not drawn by the indeterminate variants.
			 * @return The registered indicator, visible.
			 */
			std::shared_ptr<ProgressIndicator>
				makeProgressIndicator(std::shared_ptr<ecs::Entity> parent,
									  Variant variant, float value);
		};

		public:
		/**
		 * @brief Create a progress indicator and its track and fill
		 * children, without registering it in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * (registry, config, variant, value). A fixed width in the config
		 * is taken as the length of the bar (linear variants) and a fixed
		 * height as the diameter of the ring (circular variants). The
		 * children are built here, by build(), because a child needs its
		 * parent to be owned by a shared pointer.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new indicator, visible, with no parent.
		 */
		template<typename... Arguments>
		static std::shared_ptr<ProgressIndicator>
			create(Arguments &&...arguments)
		{
			std::shared_ptr<ProgressIndicator> entity(
				new ProgressIndicator(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~ProgressIndicator(void) override;

		/**
		 * @brief Change the indicator variant.
		 *
		 * Does nothing when @p variant is the current one. Otherwise
		 * replaces the track and fill children by those of the new
		 * variant, starts the looping animation when it is indeterminate,
		 * and redraws at once. The value is kept.
		 * @param variant The new variant.
		 * @return This indicator, for chaining.
		 */
		ProgressIndicator &setVariant(Variant variant);

		/**
		 * @brief Get the indicator variant.
		 * @return The variant stored in components::ProgressIndicator.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the progress.
		 *
		 * Writes components::Value. The fill does not jump: the drawn
		 * value moves to the new one over 200 ms (see
		 * getDisplayedValue()).
		 * @param value The new progress, 0 (nothing done) to 1 (complete);
		 * a value outside is clamped.
		 * @return This indicator, for chaining.
		 */
		ProgressIndicator &setValue(float value);

		/**
		 * @brief Get the progress last set.
		 * @return The progress, 0 to 1. It is the target value, not the
		 * one being drawn during a transition.
		 */
		float getValue(void) const;

		/**
		 * @brief Get the progress drawn at the moment.
		 * @return The drawn progress, 0 to 1. It follows getValue() with a
		 * 200 ms transition and equals it once the transition is over.
		 */
		float getDisplayedValue(void) const;

		/**
		 * @brief Show or hide the indicator with a 200 ms fade (Paper
		 * `visible`).
		 *
		 * Does nothing when the indicator is already in that state.
		 * @param visible True to fade in, false to fade out.
		 * @return This indicator, for chaining.
		 */
		ProgressIndicator &setVisible(bool visible);

		/**
		 * @brief Check whether the indicator is meant to be visible.
		 * @return The value last given to setVisible(), true by default. It
		 * changes at once, before the fade ends; see getOpacity().
		 */
		bool isVisible(void) const;

		/**
		 * @brief Get the opacity of the fade.
		 * @return The opacity, 0 (hidden) to 1 (fully drawn); in between
		 * while a fade is running.
		 */
		float getOpacity(void) const;

		/**
		 * @brief Set the color of the fill. Redraws at once.
		 * @param color The color, used in place of the theme's primary
		 * color. The track keeps its theme color.
		 * @return This indicator, for chaining.
		 */
		ProgressIndicator &
			setIndicatorColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Get the color of the fill.
		 * @return The color given to setIndicatorColor(), or the theme's
		 * primary color when none was given. The fade opacity is not
		 * applied to it.
		 */
		utility::graphic::Color32Bit getIndicatorColor(void) const;

		/**
		 * @brief Length of the sliding segment of the LinearIndeterminate
		 * variant, as a fraction of the bar length: 0.6 (Paper uses 60%).
		 */
		static constexpr float IndeterminateSegment =
			components::ProgressIndicator::IndeterminateSegment;

		private:
		/**
		 * @brief Build (and attach) the track and fill children of the
		 * current variant, unless they already exist.
		 */
		void buildChildren(void);

		private:
		/**
		 * @brief Construct a progress indicator, without its children.
		 * @param registry The registry the indicator's components are
		 * created in.
		 * @param config Pose and size of the widget. A fixed width is
		 * taken as the length of the bar (linear variants), a fixed height
		 * as the diameter of the ring (circular variants).
		 * @param variant The visual variant.
		 * @param value The progress it starts with, 0 to 1; a value
		 * outside is clamped.
		 */
		ProgressIndicator(ecs::ComponentRegistry &registry,
						  const SurfaceConfig &config, Variant variant,
						  float value);

		/**
		 * @brief Build the track and fill children and redraw.
		 *
		 * Called once by create(), when the indicator is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
