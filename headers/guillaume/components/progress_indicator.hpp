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

#include <optional>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a progress indicator: a bar or a ring that fills up
	 * with a value from 0 to 1, or loops when the value is unknown.
	 *
	 * It holds two kinds of state. The configuration (variant, track
	 * length, diameter, color, visibility) and the identifiers of the track
	 * and fill children are written by the entities::ProgressIndicator
	 * widget. The running state (drawn value, fade opacity, loop progress)
	 * is written by systems::ProgressIndicator from the animations it
	 * installs. In the Layout phase, systems::ProgressIndicator turns all
	 * of it into the size of the widget and the geometry and color of the
	 * two children.
	 *
	 * The value to reach is not stored here: it is the components::Value of
	 * the same entity. Set it with systems::ProgressIndicator::setValue;
	 * the drawn value then moves to it over a short animation.
	 *
	 * @see systems::ProgressIndicator, components::Value,
	 * entities::ProgressIndicator
	 */
	class ProgressIndicator: public ecs::Component
	{
		public:
		/**
		 * @brief Shape of the indicator and whether it shows a value.
		 */
		enum class Variant {
			LinearDeterminate,		 ///< Bar filled up to the value.
			LinearIndeterminate,	 ///< Bar with a segment that slides in
									 ///< a loop.
			CircularDeterminate,	 ///< Ring filled up to the value.
			CircularIndeterminate	 ///< Ring with an arc that turns in a
									 ///< loop.
		};

		/// Default length of the linear track, in dp.
		static constexpr float DefaultTrackLength = 200.0f;

		/// Thickness of the linear track, in dp.
		static constexpr float LinearHeight = 4.0f;

		/// Default diameter of the circular variants, in dp.
		static constexpr float DefaultDiameter = 48.0f;

		/// Stroke width of the circular track and fill, in dp.
		static constexpr float RingThickness = 4.0f;

		/// Length of the sliding segment of the linear indeterminate
		/// variant, as a fraction of the track (0 to 1).
		static constexpr float IndeterminateSegment = 0.6f;

		/// Length of the turning arc of the circular indeterminate variant,
		/// as a fraction of the ring (0 to 1).
		static constexpr float IndeterminateSweep = 0.25f;

		/**
		 * @brief Create the state of a visible linear determinate indicator
		 * with the default sizes and no child.
		 */
		ProgressIndicator(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~ProgressIndicator(void) = default;

		/**
		 * @brief Get the shape of the indicator and whether it shows a
		 * value.
		 * @return The variant, Variant::LinearDeterminate by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the shape of the indicator and whether it shows a
		 * value.
		 *
		 * The children depend on the variant; this setter does not rebuild
		 * them, the widget does. Marks the component changed only when the
		 * variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setVariant(Variant variant);

		/**
		 * @brief Tell whether the indicator is a ring.
		 * @return True for the two circular variants.
		 */
		bool isCircular(void) const noexcept;

		/**
		 * @brief Tell whether the indicator loops instead of showing a
		 * value.
		 * @return True for the two indeterminate variants.
		 */
		bool isIndeterminate(void) const noexcept;

		/**
		 * @brief Get the length of the track of the linear variants.
		 * @return The length, in dp.
		 */
		float getTrackLength(void) const noexcept;

		/**
		 * @brief Set the length of the track of the linear variants.
		 *
		 * The circular variants ignore it. Marks the component changed only
		 * when the value differs.
		 * @param length The new length, in dp.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setTrackLength(float length);

		/**
		 * @brief Get the diameter of the circular variants.
		 * @return The diameter, in dp.
		 */
		float getDiameter(void) const noexcept;

		/**
		 * @brief Set the diameter of the circular variants.
		 *
		 * The linear variants ignore it. Marks the component changed only
		 * when the value differs.
		 * @param diameter The new diameter, in dp.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setDiameter(float diameter);

		/**
		 * @brief Get the child that draws the track: the full bar or ring
		 * behind the fill.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getTrack(void) const noexcept;

		/**
		 * @brief Record the child that draws the track.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setTrack(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the fill: the part of the bar or
		 * ring that shows the progress.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getFill(void) const noexcept;

		/**
		 * @brief Record the child that draws the fill.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setFill(ecs::Entity::Identifier identifier);

		/**
		 * @brief Tell whether the track and fill children match the
		 * current variant.
		 * @return True once the widget has built them; false for a new
		 * component and after the widget changes the variant.
		 */
		bool isBuilt(void) const noexcept;

		/**
		 * @brief Record whether the track and fill children match the
		 * current variant.
		 *
		 * The widget lowers the flag when it changes the variant and raises
		 * it once it has rebuilt the children. Marks the component changed
		 * only when the flag differs.
		 * @param built True when the children are up to date.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setBuilt(bool built);

		/**
		 * @brief Get the value currently drawn.
		 * @return The drawn value, 0 to 1. It trails the target value of
		 * components::Value while the transition runs.
		 */
		float getDisplayed(void) const noexcept;

		/**
		 * @brief Store the value currently drawn.
		 *
		 * Running state written by systems::ProgressIndicator on every
		 * sample of the value transition. Marks the component changed only
		 * when the value differs.
		 * @param displayed The drawn value, 0 to 1.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setDisplayed(float displayed);

		/**
		 * @brief Get the value that was drawn when the running transition
		 * began.
		 * @return The starting value, 0 to 1.
		 */
		float getValueFrom(void) const noexcept;

		/**
		 * @brief Store the value drawn at the start of a transition.
		 *
		 * The drawn value is interpolated from this one to the target.
		 * Marks the component changed only when the value differs.
		 * @param value The starting value, 0 to 1.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setValueFrom(float value);

		/**
		 * @brief Tell whether the indicator is meant to be visible.
		 * @return True when visible, which is the default. The indicator
		 * may still be fading; see getOpacity().
		 */
		bool isVisible(void) const noexcept;

		/**
		 * @brief Record whether the indicator is meant to be visible.
		 *
		 * This only stores the flag. Use
		 * systems::ProgressIndicator::setVisible instead: it also starts
		 * the fade. Marks the component changed only when the flag differs.
		 * @param visible True to show the indicator.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setVisible(bool visible);

		/**
		 * @brief Get the opacity the indicator is drawn with.
		 * @return The opacity, 0 (hidden) to 1 (fully visible).
		 */
		float getOpacity(void) const noexcept;

		/**
		 * @brief Store the opacity the indicator is drawn with.
		 *
		 * Running state written by systems::ProgressIndicator on every
		 * sample of the fade. Marks the component changed only when the
		 * value differs.
		 * @param opacity The opacity, 0 to 1.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setOpacity(float opacity);

		/**
		 * @brief Get the opacity the indicator had when the running fade
		 * began.
		 * @return The starting opacity, 0 to 1.
		 */
		float getOpacityFrom(void) const noexcept;

		/**
		 * @brief Store the opacity at the start of a fade.
		 *
		 * The opacity is interpolated from this one to 1 (visible) or 0
		 * (hidden). Marks the component changed only when the value
		 * differs.
		 * @param opacity The starting opacity, 0 to 1.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setOpacityFrom(float opacity);

		/**
		 * @brief Get how far the loop of an indeterminate variant has
		 * advanced.
		 * @return The eased progress of the current loop, 0 to 1.
		 */
		float getCycle(void) const noexcept;

		/**
		 * @brief Store how far the loop of an indeterminate variant has
		 * advanced.
		 *
		 * Running state written by the looping animation of the fill
		 * child. It sets the position of the sliding segment or the start
		 * angle of the turning arc. Marks the component changed only when
		 * the value differs.
		 * @param cycle The eased progress of the current loop, 0 to 1.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setCycle(float cycle);

		/**
		 * @brief Get the color chosen for the fill.
		 * @return The color, or std::nullopt when the fill uses the theme's
		 * `Primary` color.
		 */
		const std::optional<utility::graphic::Color32Bit> &
			getIndicatorColor(void) const noexcept;

		/**
		 * @brief Choose the color of the fill.
		 *
		 * Marks the component changed only when the value differs.
		 * @param color The color, or std::nullopt to use the theme's
		 * `Primary` color.
		 * @return This component, for chaining.
		 */
		ProgressIndicator &setIndicatorColor(
			const std::optional<utility::graphic::Color32Bit> &color);

		private:
		Variant _variant { Variant::LinearDeterminate };	///< Visual variant
		float _trackLength { DefaultTrackLength };	  ///< Linear track length
		float _diameter { DefaultDiameter };		  ///< Circular diameter
		ecs::Entity::Identifier _track {
			ecs::Entity::InvalidIdentifier
		};	  ///< Track child

		ecs::Entity::Identifier _fill {
			ecs::Entity::InvalidIdentifier
		};	  ///< Fill child

		bool _built { false };			///< Whether the children are built
		float _displayed { 0.0f };		///< Value currently drawn
		float _valueFrom { 0.0f };		///< Drawn value at transition start
		bool _visible { true };			///< Target visibility
		float _opacity { 1.0f };		///< Current fade opacity
		float _opacityFrom { 1.0f };	///< Opacity when the fade began
		float _cycle { 0.0f };			///< Indeterminate animation progress
		std::optional<utility::graphic::Color32Bit>
			_indicatorColor {};	   ///< Fill color override
	};

}	 // namespace guillaume::components
