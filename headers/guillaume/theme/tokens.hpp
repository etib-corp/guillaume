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

#include <array>
#include <cstddef>
#include <cstdint>

#include <utility/graphic/color.hpp>

#include "guillaume/motion/easing.hpp"

namespace guillaume::theme
{

	/**
	 * @brief Material 3 corner shape roles.
	 */
	enum class ShapeRole : std::size_t {
		None,		   ///< 0 dp.
		ExtraSmall,	   ///< 4 dp (text fields, menus, snackbars).
		Small,		   ///< 8 dp (chips).
		Medium,		   ///< 12 dp (cards).
		Large,		   ///< 16 dp (FABs, drawers).
		ExtraLarge,	   ///< 28 dp (dialogs, search bars, sheets).
		Full,		   ///< Fully rounded (buttons, icon buttons, badges).
		Count
	};

	/**
	 * @brief Material 3 shape scale scaled by Paper's `roundness` token.
	 *
	 * Paper's MD3 themes use `roundness = 4`; every role scales linearly with
	 * it (`radius = base * roundness / 4`), except None and Full.
	 */
	class ShapeScale
	{
		public:
		/**
		 * @brief Radius used for ShapeRole::Full. Renderers clamp radii to
		 * half the smallest extent, which yields a pill or a circle.
		 */
		static constexpr float FullRadius = 10000.0f;

		/**
		 * @brief Paper's MD3 roundness.
		 */
		static constexpr float DefaultRoundness = 4.0f;

		/**
		 * @brief Construct the M3 baseline shape scale.
		 */
		ShapeScale(void) = default;

		/**
		 * @brief Get the roundness token.
		 * @return The roundness.
		 */
		float getRoundness(void) const;

		/**
		 * @brief Set the roundness token.
		 * @param roundness New roundness, must be non-negative.
		 * @throws std::invalid_argument If @p roundness is negative.
		 */
		void setRoundness(float roundness);

		/**
		 * @brief Resolve a shape role to a corner radius.
		 * @param role Shape role.
		 * @return The corner radius, in dp.
		 * @throws std::out_of_range If @p role is `ShapeRole::Count`.
		 */
		float getRadius(ShapeRole role) const;

		private:
		float _roundness { DefaultRoundness };	  ///< Roundness token.
	};

	/**
	 * @brief Material 3 elevation levels.
	 */
	enum class ElevationLevel : std::size_t {
		Level0,
		Level1,
		Level2,
		Level3,
		Level4,
		Level5,
		Count
	};

	/**
	 * @brief Shadow and tonal tint of one elevation level.
	 */
	struct ElevationToken {
		float tintOpacity { 0.0f };	   ///< SurfaceTint opacity over Surface.
		float shadowSpread { 0.0f };	///< Shadow expansion, in dp.
		std::uint8_t shadowAlpha { 0 };	   ///< Peak shadow opacity (0-255).
	};

	/**
	 * @brief Material 3 elevation scale (tonal tint + shadow per level).
	 *
	 * Tint opacities follow Paper's MD3 `elevation.level0-5` colors.
	 */
	class ElevationScale
	{
		public:
		/**
		 * @brief Construct the M3 baseline elevation scale.
		 */
		ElevationScale(void);

		/**
		 * @brief Get the token of a level.
		 * @param level Elevation level.
		 * @return Immutable token reference.
		 * @throws std::out_of_range If @p level is `ElevationLevel::Count`.
		 */
		const ElevationToken &get(ElevationLevel level) const;

		/**
		 * @brief Replace the token of a level.
		 * @param level Elevation level.
		 * @param token New token.
		 * @throws std::out_of_range If @p level is `ElevationLevel::Count`.
		 */
		void set(ElevationLevel level, const ElevationToken &token);

		/**
		 * @brief Apply the level tonal tint to a surface color.
		 * @param surface Surface color.
		 * @param tint SurfaceTint color.
		 * @param level Elevation level.
		 * @return The tinted surface color.
		 */
		utility::graphic::Color32Bit
			tint(const utility::graphic::Color32Bit &surface,
				 const utility::graphic::Color32Bit &tint,
				 ElevationLevel level) const;

		private:
		std::array<ElevationToken,
				   static_cast<std::size_t>(ElevationLevel::Count)>
			_tokens;	///< Tokens indexed by level.
	};

	/**
	 * @brief Material 3 state layers.
	 */
	enum class StateLayer : std::size_t {
		Hovered,			  ///< 0.08 overlay of the content color.
		Focused,			  ///< 0.12 overlay of the content color.
		Pressed,			  ///< 0.12 overlay of the content color.
		Dragged,			  ///< 0.16 overlay of the content color.
		DisabledContent,	  ///< 0.38 opacity of onSurface content.
		DisabledContainer,	  ///< 0.12 opacity of onSurface containers.
		Count
	};

	/**
	 * @brief Material 3 state-layer opacities (Paper `ref.opacity`).
	 */
	class StateLayerScale
	{
		public:
		/**
		 * @brief Construct the M3 baseline state-layer scale.
		 */
		StateLayerScale(void);

		/**
		 * @brief Get the opacity of a state layer.
		 * @param layer State layer.
		 * @return The opacity in [0, 1].
		 * @throws std::out_of_range If @p layer is `StateLayer::Count`.
		 */
		float getOpacity(StateLayer layer) const;

		/**
		 * @brief Get the opacity of a state layer as an 8-bit alpha.
		 * @param layer State layer.
		 * @return The alpha in [0, 255].
		 */
		std::uint8_t getAlpha(StateLayer layer) const;

		/**
		 * @brief Replace the opacity of a state layer.
		 * @param layer State layer.
		 * @param opacity New opacity in [0, 1].
		 * @throws std::out_of_range If @p layer is `StateLayer::Count`.
		 * @throws std::invalid_argument If @p opacity is outside [0, 1].
		 */
		void setOpacity(StateLayer layer, float opacity);

		private:
		std::array<float, static_cast<std::size_t>(StateLayer::Count)>
			_opacities;	   ///< Opacities indexed by layer.
	};

	/**
	 * @brief Material 3 motion duration tokens.
	 */
	enum class MotionDuration : std::size_t {
		Short1,		   ///< 50 ms.
		Short2,		   ///< 100 ms.
		Short3,		   ///< 150 ms.
		Short4,		   ///< 200 ms.
		Medium1,	   ///< 250 ms.
		Medium2,	   ///< 300 ms.
		Medium3,	   ///< 350 ms.
		Medium4,	   ///< 400 ms.
		Long1,		   ///< 450 ms.
		Long2,		   ///< 500 ms.
		Long3,		   ///< 550 ms.
		Long4,		   ///< 600 ms.
		ExtraLong1,	   ///< 700 ms.
		ExtraLong2,	   ///< 800 ms.
		ExtraLong3,	   ///< 900 ms.
		ExtraLong4,	   ///< 1000 ms.
		Count
	};

	/**
	 * @brief Material 3 motion scale: durations and role easings.
	 */
	class MotionScale
	{
		public:
		/**
		 * @brief Longest frame step the animation system integrates, in
		 * seconds, so a stalled frame does not skip a whole transition.
		 */
		static constexpr float MaxFrameStep = 0.1f;

		/**
		 * @brief Duration of one indeterminate progress/loading cycle, in
		 * seconds (Paper ActivityIndicator/ProgressBar use 1.5 s).
		 */
		static constexpr float IndeterminateCycle = 1.5f;

		/**
		 * @brief Default snackbar display time, in seconds (Paper 7 s).
		 */
		static constexpr float SnackbarDuration = 7.0f;

		/**
		 * @brief Construct the M3 baseline motion scale.
		 */
		MotionScale(void);

		/**
		 * @brief Get a duration token in seconds.
		 * @param duration Duration token.
		 * @return The duration, in seconds.
		 * @throws std::out_of_range If @p duration is `MotionDuration::Count`.
		 */
		float getDuration(MotionDuration duration) const;

		/**
		 * @brief Replace a duration token.
		 * @param duration Duration token.
		 * @param seconds New duration, must be positive.
		 * @throws std::out_of_range If @p duration is `MotionDuration::Count`.
		 * @throws std::invalid_argument If @p seconds is not positive.
		 */
		void setDuration(MotionDuration duration, float seconds);

		/**
		 * @brief Easing for elements that stay on screen.
		 * @return The standard easing.
		 */
		static constexpr motion::Easing standard(void)
		{
			return motion::Easing::Standard;
		}

		/**
		 * @brief Easing for elements entering the screen.
		 * @return The emphasized-decelerate easing.
		 */
		static constexpr motion::Easing enter(void)
		{
			return motion::Easing::EmphasizedDecelerate;
		}

		/**
		 * @brief Easing for elements leaving the screen.
		 * @return The emphasized-accelerate easing.
		 */
		static constexpr motion::Easing exit(void)
		{
			return motion::Easing::EmphasizedAccelerate;
		}

		private:
		std::array<float, static_cast<std::size_t>(MotionDuration::Count)>
			_durations;	   ///< Durations in seconds, indexed by token.
	};

	/**
	 * @brief Material 3 spacing on the 4 dp grid, plus the touch target.
	 */
	class SpacingScale
	{
		public:
		/**
		 * @brief Base grid unit, in dp.
		 */
		static constexpr float Unit = 4.0f;

		/**
		 * @brief Minimum interactive touch target, in dp.
		 */
		static constexpr float MinTouchTarget = 48.0f;

		/**
		 * @brief Get a multiple of the grid unit.
		 * @param steps Number of 4 dp steps.
		 * @return The spacing, in dp.
		 */
		static constexpr float get(unsigned int steps)
		{
			return Unit * static_cast<float>(steps);
		}

		/**
		 * @brief Get the hit margin that grows a control to the touch target.
		 * @param extent Visual extent of the control along one axis.
		 * @return The margin to add on each side (0 when already large
		 * enough).
		 */
		static constexpr float touchMargin(float extent)
		{
			return extent >= MinTouchTarget ? 0.0f
											: (MinTouchTarget - extent) / 2.0f;
		}
	};

	/**
	 * @brief Material 3 focus indicator token.
	 *
	 * The ring is painted outside the focused entity, `offset` away from its
	 * bound, `width` thick, in the scheme's secondary color.
	 */
	struct FocusRing {
		/**
		 * @brief Ring thickness, in dp.
		 */
		static constexpr float Width = 3.0f;

		/**
		 * @brief Gap between the bound and the ring, in dp.
		 */
		static constexpr float Offset = 2.0f;
	};

}	 // namespace guillaume::theme
