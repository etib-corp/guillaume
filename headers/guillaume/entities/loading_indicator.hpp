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
#include "guillaume/components/loading_indicator.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief An animation that loops for as long as work is in progress: a
	 * rotating arc or a sliding bar, with no value.
	 *
	 * Its state lives in components::LoadingIndicator (variant, diameter,
	 * color, running animation and fade values). systems::LoadingIndicator
	 * sizes the widget and draws its animated child each frame, in the
	 * Layout phase. The class itself holds no field: its setters write the
	 * component and redraw at once.
	 *
	 * @code
	 * LoadingIndicator::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto spinner = builder.makeLoadingIndicator(parent,
	 * 	LoadingIndicator::Variant::Circular);
	 * // later, when the work is done:
	 * spinner->stop();
	 * @endcode
	 *
	 * `LoadingIndicator::create(registry, config, variant)` builds the same
	 * widget without registering it in an entity registry.
	 *
	 * What drives the animation: the indicator has one child, the arc or
	 * the bar, which carries a looping components::Animation of 1.5 second
	 * per cycle. systems::Animation samples it every frame, in the Event
	 * phase, and the sample moves the child. The widget's own
	 * components::Animation runs the 200 ms fade used by stop() and
	 * start().
	 *
	 * The variants are:
	 * - `Circular`: an arc of three quarters of a turn rotating in a 48 dp
	 *   square by default (see setSize()).
	 * - `Linear`: an 80 dp bar sliding along a 200 dp long, 4 dp thick
	 *   strip.
	 *
	 * Things to know:
	 * - The animation starts as soon as the indicator is created.
	 * - By default a stopped indicator fades out and stays invisible, but
	 *   it keeps its place in its parent's layout.
	 * - systems::LoadingIndicator overwrites the widget's size, padding,
	 *   color and corner radius every frame; set the size with setSize().
	 *
	 * @see components::LoadingIndicator, systems::LoadingIndicator,
	 * ProgressIndicator
	 */
	class LoadingIndicator:
		public SurfaceBase<components::Animation, components::Accessibility,
						   components::LoadingIndicator>
	{
		public:
		/// Shape of the indicator: Linear (sliding bar) or Circular
		/// (rotating arc).
		using Variant = components::LoadingIndicator::Variant;

		/**
		 * @brief Configures a LoadingIndicator, then builds it and registers
		 * it in the entity registry given at construction.
		 */
		class Builder: public EntityBuilderBase<LoadingIndicator>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and size of the widget.
			Variant _variant { Variant::Circular };	   ///< Visual variant.
			float _size { 48.0f };	  ///< Circular diameter, in dp.

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
			 * @brief Build a loading indicator and its animated child from
			 * the current configuration, without registering it or setting
			 * its parent.
			 * @return The new indicator, already animating.
			 */
			std::shared_ptr<LoadingIndicator> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: Circular variant, a 48 dp
			 * diameter and a default pose.
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
			 * @param variant The variant. Default: `Variant::Circular`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the diameter of the Circular variant.
			 * @param size The diameter, in dp; it must be positive, the
			 * builder does not check it. Default: 48. The Linear variant
			 * ignores it.
			 * @return This builder, for chaining.
			 */
			Builder &withSize(float size);

			/**
			 * @brief Build a loading indicator, register it under @p parent
			 * and reset the builder.
			 *
			 * @p variant replaces the variant set before; the pose and the
			 * size are kept.
			 * @param parent The entity the indicator is attached to.
			 * @param variant The indicator variant.
			 * @return The registered indicator, already animating.
			 */
			std::shared_ptr<LoadingIndicator>
				makeLoadingIndicator(std::shared_ptr<ecs::Entity> parent,
									 Variant variant);
		};

		public:
		/**
		 * @brief Create a loading indicator and its animated child, without
		 * registering it in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * (registry, config, variant). For the Circular variant, a fixed
		 * height in @p config is taken as the diameter. The child is built
		 * here, by build(), because a child needs its parent to be owned by
		 * a shared pointer.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new indicator, already animating, with no parent.
		 */
		template<typename... Arguments> static std::shared_ptr<LoadingIndicator>
			create(Arguments &&...arguments)
		{
			std::shared_ptr<LoadingIndicator> entity(
				new LoadingIndicator(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~LoadingIndicator(void) override;

		/**
		 * @brief Change the indicator variant.
		 *
		 * Does nothing when @p variant is the current one. Otherwise
		 * replaces the animated child by the arc or the bar of the new
		 * variant, starts its animation (even if the indicator was
		 * stopped) and redraws at once.
		 * @param variant The new variant.
		 * @return This indicator, for chaining.
		 */
		LoadingIndicator &setVariant(Variant variant);

		/**
		 * @brief Get the indicator variant.
		 * @return The variant stored in components::LoadingIndicator.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Start the looping animation and fade the indicator in
		 * over 200 ms.
		 *
		 * A new indicator is already started.
		 * @return This indicator, for chaining.
		 */
		LoadingIndicator &start(void);

		/**
		 * @brief Stop the looping animation.
		 *
		 * The indicator then fades out over 200 ms, unless
		 * `setHidesWhenStopped(false)` was called, in which case it stays
		 * drawn, still.
		 * @return This indicator, for chaining.
		 */
		LoadingIndicator &stop(void);

		/**
		 * @brief Check whether the looping animation is playing.
		 * @return True between start() and stop(); true for a new
		 * indicator.
		 */
		bool isRunning(void) const;

		/**
		 * @brief Choose whether a stopped indicator is hidden (Paper
		 * `hidesWhenStopped`).
		 *
		 * Takes effect at once: a stopped indicator fades out, or back in,
		 * over 200 ms.
		 * @param hides True to hide the indicator while stopped. Default:
		 * true.
		 * @return This indicator, for chaining.
		 */
		LoadingIndicator &setHidesWhenStopped(bool hides);

		/**
		 * @brief Check whether a stopped indicator is hidden.
		 * @return True when it fades out on stop().
		 */
		bool hidesWhenStopped(void) const;

		/**
		 * @brief Get the opacity of the fade.
		 * @return The opacity, 0 (hidden) to 1 (fully drawn); in between
		 * while a fade is running.
		 */
		float getOpacity(void) const;

		/**
		 * @brief Set the color of the arc or bar. Redraws at once.
		 * @param color The color, used in place of the theme's primary
		 * color.
		 * @return This indicator, for chaining.
		 */
		LoadingIndicator &
			setIndicatorColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Get the color of the arc or bar.
		 * @return The color given to setIndicatorColor(), or the theme's
		 * primary color when none was given. The fade opacity is not
		 * applied to it.
		 */
		utility::graphic::Color32Bit getIndicatorColor(void) const;

		/**
		 * @brief Set the diameter of the Circular variant (Paper `size`).
		 *
		 * For a Circular indicator, rebuilds the arc, starts its animation
		 * and redraws at once. A Linear indicator only stores the value,
		 * for a later switch to Circular.
		 * @param size The diameter, in dp; must be positive.
		 * @return This indicator, for chaining.
		 * @throws std::invalid_argument If @p size is not positive.
		 */
		LoadingIndicator &setSize(float size);

		/**
		 * @brief Get the diameter of the Circular variant.
		 * @return The diameter, in dp (48 by default).
		 */
		float getSize(void) const;

		/**
		 * @brief Get the thickness of the arc of the Circular variant.
		 * @return The thickness, in dp: the diameter divided by 12, which
		 * gives 4 dp at the default 48 dp.
		 */
		float getStrokeWidth(void) const;

		private:
		/**
		 * @brief Build (and attach) the animated child of the current
		 * variant, unless it already exists.
		 */
		void buildChild(void);

		private:
		/**
		 * @brief Construct a loading indicator, without its animated child.
		 * @param registry The registry the indicator's components are
		 * created in.
		 * @param config Pose and size of the widget. For the Circular
		 * variant, a fixed height is taken as the diameter.
		 * @param variant The visual variant.
		 */
		LoadingIndicator(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant);

		/**
		 * @brief Build the animated child and redraw.
		 *
		 * Called once by create(), when the indicator is owned by a shared
		 * pointer and can be given as parent to its child.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
