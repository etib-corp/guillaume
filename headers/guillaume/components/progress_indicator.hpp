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
	 * @brief State of a progress indicator.
	 *
	 * This component is the whole state of a progress indicator besides its
	 * target value (components::Value): its configuration (variant, track
	 * length, diameter, color override, visibility), its track and fill
	 * children, and the transient values of its value transition, of its fade
	 * and of its indeterminate animation. systems::ProgressIndicator turns it
	 * into the size of the widget and into the geometry and color of the
	 * children.
	 */
	class ProgressIndicator: public ecs::Component
	{
		public:
		/**
		 * @brief Progress indicator visual variant.
		 */
		enum class Variant {
			LinearDeterminate,		 ///< Linear determinate bar.
			LinearIndeterminate,	 ///< Linear indeterminate bar.
			CircularDeterminate,	 ///< Circular determinate ring.
			CircularIndeterminate	 ///< Circular indeterminate ring.
		};

		/// Default length of the linear track, in dp.
		static constexpr float DefaultTrackLength = 200.0f;

		/// Thickness of the linear track, in dp.
		static constexpr float LinearHeight = 4.0f;

		/// Default diameter of the circular variants, in dp.
		static constexpr float DefaultDiameter = 48.0f;

		/// Stroke width of the circular track and fill, in dp.
		static constexpr float RingThickness = 4.0f;

		/// Indeterminate linear segment length, as a fraction of the track
		/// (Paper uses 60%).
		static constexpr float IndeterminateSegment = 0.6f;

		/// Indeterminate circular arc length, as a fraction of the ring.
		static constexpr float IndeterminateSweep = 0.25f;

		/**
		 * @brief Default constructor.
		 */
		ProgressIndicator(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~ProgressIndicator(void) = default;

		/** @brief Get: visual variant. @return The value. */
		Variant getVariant(void) const noexcept;

		/** @brief Set: visual variant. @param variant The value. @return It. */
		ProgressIndicator &setVariant(Variant variant);

		/** @brief Whether the variant is a ring. @return True for the
		 * circular variants. */
		bool isCircular(void) const noexcept;

		/** @brief Whether the variant carries no value. @return True for the
		 * indeterminate variants. */
		bool isIndeterminate(void) const noexcept;

		/** @brief Get: linear track length, in dp. @return The value. */
		float getTrackLength(void) const noexcept;

		/** @brief Set: linear track length, in dp. @param length The value.
		 * @return It. */
		ProgressIndicator &setTrackLength(float length);

		/** @brief Get: circular diameter, in dp. @return The value. */
		float getDiameter(void) const noexcept;

		/** @brief Set: circular diameter, in dp. @param diameter The value.
		 * @return It. */
		ProgressIndicator &setDiameter(float diameter);

		/** @brief Get: track child. @return The value. */
		ecs::Entity::Identifier getTrack(void) const noexcept;

		/** @brief Set: track child. @param identifier The value. @return It.
		 */
		ProgressIndicator &setTrack(ecs::Entity::Identifier identifier);

		/** @brief Get: fill child. @return The value. */
		ecs::Entity::Identifier getFill(void) const noexcept;

		/** @brief Set: fill child. @param identifier The value. @return It. */
		ProgressIndicator &setFill(ecs::Entity::Identifier identifier);

		/** @brief Get: whether the children have been built for the current
		 * variant. @return The value. */
		bool isBuilt(void) const noexcept;

		/** @brief Set: whether the children have been built for the current
		 * variant. @param built The value. @return It. */
		ProgressIndicator &setBuilt(bool built);

		/** @brief Get: value currently drawn, which trails the target value.
		 * @return The value. */
		float getDisplayed(void) const noexcept;

		/** @brief Set: value currently drawn. @param displayed The value.
		 * @return It. */
		ProgressIndicator &setDisplayed(float displayed);

		/** @brief Get: drawn value when the transition began. @return The
		 * value. */
		float getValueFrom(void) const noexcept;

		/** @brief Set: drawn value when the transition began. @param value
		 * The value. @return It. */
		ProgressIndicator &setValueFrom(float value);

		/** @brief Get: target visibility (Paper `visible`). @return The value.
		 */
		bool isVisible(void) const noexcept;

		/** @brief Set: target visibility. @param visible The value. @return
		 * It. */
		ProgressIndicator &setVisible(bool visible);

		/** @brief Get: current fade opacity, in [0, 1]. @return The value. */
		float getOpacity(void) const noexcept;

		/** @brief Set: current fade opacity. @param opacity The value.
		 * @return It. */
		ProgressIndicator &setOpacity(float opacity);

		/** @brief Get: opacity when the fade began. @return The value. */
		float getOpacityFrom(void) const noexcept;

		/** @brief Set: opacity when the fade began. @param opacity The value.
		 * @return It. */
		ProgressIndicator &setOpacityFrom(float opacity);

		/** @brief Get: latest eased progress of the indeterminate animation,
		 * in [0, 1]. @return The value. */
		float getCycle(void) const noexcept;

		/** @brief Set: latest eased progress of the indeterminate animation.
		 * @param cycle The value. @return It. */
		ProgressIndicator &setCycle(float cycle);

		/** @brief Get: fill color override (Paper `color`), empty for the
		 * theme `Primary`. @return The value. */
		const std::optional<utility::graphic::Color32Bit> &
			getIndicatorColor(void) const noexcept;

		/** @brief Set: fill color override. @param color The value. @return
		 * It. */
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
