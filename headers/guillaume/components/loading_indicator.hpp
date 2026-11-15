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
	 * @brief State of a loading indicator: an arc that turns, or a bar that
	 * slides, in a loop while something is in progress.
	 *
	 * It holds two kinds of state. The configuration (variant, diameter,
	 * color, whether it hides when stopped) and the identifier of the
	 * animated child are written by the entities::LoadingIndicator widget.
	 * The running state (loop progress, fade opacity) is written by
	 * systems::LoadingIndicator from the animations it installs. In the
	 * Layout phase, systems::LoadingIndicator turns all of it into the size
	 * of the widget and the geometry and color of the child.
	 *
	 * Whether the indicator runs is not stored here: it is whether the
	 * looping components::Animation of the child plays. Use
	 * systems::LoadingIndicator::start and systems::LoadingIndicator::stop.
	 *
	 * @see systems::LoadingIndicator, components::ProgressIndicator,
	 * entities::LoadingIndicator
	 */
	class LoadingIndicator: public ecs::Component
	{
		public:
		/**
		 * @brief Shape of the indicator.
		 */
		enum class Variant {
			Linear,		///< Bar that slides along a 200 dp line.
			Circular	///< Arc that turns around a circle.
		};

		/// Length of the linear variant, in dp.
		static constexpr float LinearWidth = 200.0f;

		/// Thickness of the linear variant, in dp.
		static constexpr float LinearHeight = 4.0f;

		/// Length of the sliding bar, as a fraction of the linear length
		/// (0 to 1).
		static constexpr float LinearSegment = 0.4f;

		/// Default diameter of the circular variant, in dp.
		static constexpr float DefaultSize = 48.0f;

		/// Stroke width of the arc, as a fraction of the diameter: 4 dp for
		/// a 48 dp indicator.
		static constexpr float StrokeRatio = 4.0f / 48.0f;

		/**
		 * @brief Create the state of a circular indicator of the default
		 * size, with no child, that hides when stopped.
		 */
		LoadingIndicator(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~LoadingIndicator(void) = default;

		/**
		 * @brief Get the shape of the indicator.
		 * @return The variant, Variant::Circular by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the shape of the indicator.
		 *
		 * The child depends on the variant; this setter does not rebuild
		 * it, the widget does. Marks the component changed only when the
		 * variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setVariant(Variant variant);

		/**
		 * @brief Get the diameter of the circular variant.
		 * @return The diameter, in dp.
		 */
		float getSize(void) const noexcept;

		/**
		 * @brief Set the diameter of the circular variant.
		 *
		 * The stroke width follows it (StrokeRatio). The linear variant
		 * ignores it. Marks the component changed only when the value
		 * differs.
		 * @param size The new diameter, in dp.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setSize(float size);

		/**
		 * @brief Get the child that draws the turning arc or the sliding
		 * bar.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getIndicator(void) const noexcept;

		/**
		 * @brief Record the child that draws the turning arc or the sliding
		 * bar.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setIndicator(ecs::Entity::Identifier identifier);

		/**
		 * @brief Tell whether the animated child matches the current
		 * variant.
		 * @return True once the widget has built it; false for a new
		 * component and after the widget changes the variant.
		 */
		bool isBuilt(void) const noexcept;

		/**
		 * @brief Record whether the animated child matches the current
		 * variant.
		 *
		 * The widget lowers the flag when it changes the variant and raises
		 * it once it has rebuilt the child. Marks the component changed
		 * only when the flag differs.
		 * @param built True when the child is up to date.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setBuilt(bool built);

		/**
		 * @brief Get how far the loop has advanced.
		 * @return The eased progress of the current loop, 0 to 1.
		 */
		float getProgress(void) const noexcept;

		/**
		 * @brief Store how far the loop has advanced.
		 *
		 * Running state written by the looping animation of the child. It
		 * sets the start angle of the arc or the position of the bar. Marks
		 * the component changed only when the value differs.
		 * @param progress The eased progress of the current loop, 0 to 1.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setProgress(float progress);

		/**
		 * @brief Tell whether the indicator fades out when it is stopped.
		 * @return True when a stopped indicator is hidden, which is the
		 * default; false when it stays visible, frozen in place.
		 */
		bool hidesWhenStopped(void) const noexcept;

		/**
		 * @brief Choose whether the indicator fades out when it is
		 * stopped.
		 *
		 * The choice is applied the next time the indicator starts or
		 * stops. Marks the component changed only when the flag differs.
		 * @param hides True to hide a stopped indicator.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setHidesWhenStopped(bool hides);

		/**
		 * @brief Get the opacity the indicator is drawn with.
		 * @return The opacity, 0 (hidden) to 1 (fully visible).
		 */
		float getOpacity(void) const noexcept;

		/**
		 * @brief Store the opacity the indicator is drawn with.
		 *
		 * Running state written by systems::LoadingIndicator on every
		 * sample of the fade. Marks the component changed only when the
		 * value differs.
		 * @param opacity The opacity, 0 to 1.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setOpacity(float opacity);

		/**
		 * @brief Get the opacity the indicator had when the running fade
		 * began.
		 * @return The starting opacity, 0 to 1.
		 */
		float getOpacityFrom(void) const noexcept;

		/**
		 * @brief Store the opacity at the start of a fade.
		 *
		 * Marks the component changed only when the value differs.
		 * @param opacity The starting opacity, 0 to 1.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setOpacityFrom(float opacity);

		/**
		 * @brief Get the opacity the running fade ends at.
		 * @return 1 while the indicator runs or stays visible when
		 * stopped, 0 when it is stopped and hides.
		 */
		float getOpacityTarget(void) const noexcept;

		/**
		 * @brief Store the opacity a fade ends at.
		 *
		 * Written by systems::LoadingIndicator when the indicator starts
		 * or stops. Marks the component changed only when the value
		 * differs.
		 * @param opacity The final opacity, 0 to 1.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setOpacityTarget(float opacity);

		/**
		 * @brief Get the color chosen for the indicator.
		 * @return The color, or std::nullopt when the indicator uses the
		 * theme's `Primary` color.
		 */
		const std::optional<utility::graphic::Color32Bit> &
			getIndicatorColor(void) const noexcept;

		/**
		 * @brief Choose the color of the indicator.
		 *
		 * Marks the component changed only when the value differs.
		 * @param color The color, or std::nullopt to use the theme's
		 * `Primary` color.
		 * @return This component, for chaining.
		 */
		LoadingIndicator &setIndicatorColor(
			const std::optional<utility::graphic::Color32Bit> &color);

		private:
		Variant _variant { Variant::Circular };	   ///< Visual variant
		float _size { DefaultSize };			   ///< Circular diameter
		ecs::Entity::Identifier _indicator {
			ecs::Entity::InvalidIdentifier
		};	  ///< Animated child

		bool _built { false };				///< Whether the child is built
		float _progress { 0.0f };			///< Looping animation progress
		bool _hidesWhenStopped { true };	///< Fade out when stopped
		float _opacity { 1.0f };			///< Current fade opacity
		float _opacityFrom { 1.0f };		///< Opacity when the fade began
		float _opacityTarget { 1.0f };		///< Opacity the fade moves to
		std::optional<utility::graphic::Color32Bit>
			_indicatorColor {};	   ///< Indicator color override
	};

}	 // namespace guillaume::components
