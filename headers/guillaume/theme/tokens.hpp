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
	 * @brief The corner radii a widget can ask for, by name.
	 *
	 * The values are those of Material Design 3 with the default roundness.
	 * Resolve a role with guillaume::getShapeRadius.
	 */
	enum class ShapeRole : std::size_t {
		None,		   ///< 0 dp: square corners.
		ExtraSmall,	   ///< 4 dp (text fields, menus, snackbars).
		Small,		   ///< 8 dp (chips).
		Medium,		   ///< 12 dp (cards).
		Large,		   ///< 16 dp (FABs, drawers).
		ExtraLarge,	   ///< 28 dp (dialogs, search bars, sheets).
		Full,		   ///< Fully rounded (buttons, icon buttons, badges).
		Count		   ///< Number of roles; not a role.
	};

	/**
	 * @brief Turns a ShapeRole into a corner radius in dp, scaled by one
	 * "roundness" value.
	 *
	 * The roundness is the single knob of the scale (the name comes from
	 * react-native-paper, whose Material 3 themes use 4). Every role scales
	 * linearly with it: `radius = base * roundness / 4`, where `base` is
	 * the value listed in ShapeRole. `None` stays 0 and `Full` stays
	 * FullRadius whatever the roundness.
	 *
	 * Read it through guillaume::getShapeRadius. To make the whole UI
	 * squarer or rounder, change the roundness in a copy of the theme:
	 * @code
	 * guillaume::Theme theme = guillaume::getActiveTheme();
	 * theme.getShapes().setRoundness(8.0f);	// Medium becomes 24 dp
	 * guillaume::setActiveTheme(theme);
	 * @endcode
	 *
	 * @see guillaume::Theme::getShapes
	 */
	class ShapeScale
	{
		public:
		/**
		 * @brief Radius returned for ShapeRole::Full, in dp. Renderers clamp
		 * radii to half the smallest extent, which yields a pill or a circle.
		 */
		static constexpr float FullRadius = 10000.0f;

		/**
		 * @brief The roundness for which each role has the radius listed
		 * in ShapeRole.
		 */
		static constexpr float DefaultRoundness = 4.0f;

		/**
		 * @brief Create a scale with DefaultRoundness.
		 */
		ShapeScale(void) = default;

		/**
		 * @brief Get the roundness of the scale.
		 * @return The roundness; 4 by default, 0 for square corners
		 * everywhere except `Full`.
		 */
		float getRoundness(void) const;

		/**
		 * @brief Change the roundness, which rescales every role except
		 * `None` and `Full`.
		 * @param roundness The new roundness. 4 gives the Material 3 radii,
		 * 8 doubles them, 0 makes corners square.
		 * @throws std::invalid_argument When `roundness` is negative.
		 */
		void setRoundness(float roundness);

		/**
		 * @brief Get the corner radius of a role for the current roundness.
		 * @param role The shape role to resolve.
		 * @return The corner radius, in dp.
		 * @throws std::out_of_range When `role` is `ShapeRole::Count`.
		 */
		float getRadius(ShapeRole role) const;

		private:
		float _roundness {
			DefaultRoundness
		};	  ///< Scale factor of the radii; 4 is the baseline.
	};

	/**
	 * @brief How far a surface is raised above the background, in six
	 * steps.
	 *
	 * A higher level gets a larger shadow and a stronger tint of its
	 * surface color. See ElevationToken for the values.
	 */
	enum class ElevationLevel : std::size_t {
		Level0,	   ///< Flat: no shadow, no tint.
		Level1,	   ///< Lowest raised level.
		Level2,	   ///< Second raised level.
		Level3,	   ///< Third raised level.
		Level4,	   ///< Fourth raised level.
		Level5,	   ///< Highest raised level.
		Count	   ///< Number of levels; not a level.
	};

	/**
	 * @brief What one elevation level looks like: how much the surface is
	 * tinted and how large and dark its shadow is.
	 */
	struct ElevationToken {
		float tintOpacity {
			0.0f
		};	  ///< Opacity of the SurfaceTint color blended over the Surface
			  ///< color, 0 to 1.
		float shadowSpread { 0.0f };	///< How far the shadow extends beyond
										///< the surface, in dp.
		std::uint8_t shadowAlpha {
			0
		};	  ///< Opacity of the shadow where it is darkest, 0 to 255.
	};

	/**
	 * @brief The six elevation levels of a theme: one ElevationToken per
	 * ElevationLevel.
	 *
	 * The default values grow with the level, from no shadow and no tint at
	 * `Level0` to a 12 dp shadow and a 14 % tint at `Level5`. The tint
	 * opacities are those of react-native-paper's Material 3 themes.
	 *
	 * Read it through guillaume::getElevationToken and
	 * guillaume::getElevatedSurfaceColor.
	 *
	 * @see guillaume::Theme::getElevations
	 */
	class ElevationScale
	{
		public:
		/**
		 * @brief Create the scale with the Material 3 baseline values.
		 */
		ElevationScale(void);

		/**
		 * @brief Get the shadow and tint of a level.
		 * @param level The elevation level to look up.
		 * @return The token of that level, read-only.
		 * @throws std::out_of_range When `level` is `ElevationLevel::Count`.
		 */
		const ElevationToken &get(ElevationLevel level) const;

		/**
		 * @brief Replace the shadow and tint of a level.
		 * @param level The elevation level to change.
		 * @param token The new values. They are stored as given, without
		 * range check.
		 * @throws std::out_of_range When `level` is `ElevationLevel::Count`.
		 */
		void set(ElevationLevel level, const ElevationToken &token);

		/**
		 * @brief Compute the color of a surface raised to a level: the tint
		 * color blended over the surface color at the level's tint opacity.
		 * @param surface The color of the surface when it is flat.
		 * @param tint The color to blend over it (the scheme's
		 * SurfaceTint). Its own alpha is ignored.
		 * @param level The elevation level of the surface.
		 * @return The tinted surface color.
		 * @throws std::out_of_range When `level` is `ElevationLevel::Count`.
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
	 * @brief The interaction states that change how a widget is drawn.
	 *
	 * For the first four, the widget draws a translucent overlay (a "state
	 * layer") of its content color on top of itself; the last two are the
	 * opacities applied to a disabled widget. The values given are the
	 * defaults.
	 */
	enum class StateLayer : std::size_t {
		Hovered,			  ///< 0.08 overlay of the content color.
		Focused,			  ///< 0.12 overlay of the content color.
		Pressed,			  ///< 0.12 overlay of the content color.
		Dragged,			  ///< 0.16 overlay of the content color.
		DisabledContent,	  ///< 0.38 opacity of onSurface content.
		DisabledContainer,	  ///< 0.12 opacity of onSurface containers.
		Count				  ///< Number of states; not a state.
	};

	/**
	 * @brief The opacity of each StateLayer, 0 to 1.
	 *
	 * The defaults are the Material 3 values listed in StateLayer. Read
	 * them through guillaume::getStateLayerAlpha, which returns the opacity
	 * as an alpha channel value ready to put in a color.
	 *
	 * @see guillaume::Theme::getStateLayers
	 */
	class StateLayerScale
	{
		public:
		/**
		 * @brief Create the scale with the Material 3 baseline opacities.
		 */
		StateLayerScale(void);

		/**
		 * @brief Get the opacity of a state layer.
		 * @param layer The state layer to look up.
		 * @return The opacity, 0 (invisible) to 1 (opaque).
		 * @throws std::out_of_range When `layer` is `StateLayer::Count`.
		 */
		float getOpacity(StateLayer layer) const;

		/**
		 * @brief Get the opacity of a state layer as an alpha channel
		 * value.
		 * @param layer The state layer to look up.
		 * @return The opacity scaled and rounded to 0 (transparent) to 255
		 * (opaque); 0.12 gives 31.
		 * @throws std::out_of_range When `layer` is `StateLayer::Count`.
		 */
		std::uint8_t getAlpha(StateLayer layer) const;

		/**
		 * @brief Replace the opacity of a state layer.
		 * @param layer The state layer to change.
		 * @param opacity The new opacity, 0 to 1.
		 * @throws std::out_of_range When `layer` is `StateLayer::Count`.
		 * @throws std::invalid_argument When `opacity` is outside 0 to 1.
		 */
		void setOpacity(StateLayer layer, float opacity);

		private:
		std::array<float, static_cast<std::size_t>(StateLayer::Count)>
			_opacities;	   ///< Opacities, 0 to 1, indexed by layer.
	};

	/**
	 * @brief The named animation lengths of Material Design 3. The values
	 * given are the defaults.
	 *
	 * Short durations suit small changes (a state layer fading in), long
	 * ones suit large surfaces moving across the screen. Resolve a token
	 * with guillaume::getMotionDuration, which returns seconds.
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
		Count		   ///< Number of tokens; not a token.
	};

	/**
	 * @brief The animation timings of a theme: one length per
	 * MotionDuration, plus which easing curve to use for which kind of
	 * movement.
	 *
	 * The durations can be changed per theme; the constants and the three
	 * easing choices are the same for every theme.
	 *
	 * @code
	 * const float seconds =
	 * 	guillaume::getMotionDuration(theme::MotionDuration::Medium2);
	 * const float progress = motion::applyEasing(
	 * 	theme::MotionScale::enter(), elapsed / seconds);
	 * @endcode
	 *
	 * @see guillaume::Theme::getMotion
	 * @see guillaume::motion::applyEasing
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
		 * @brief Duration of one cycle of an indeterminate progress or
		 * loading indicator, in seconds (1.5 s, as in react-native-paper).
		 */
		static constexpr float IndeterminateCycle = 1.5f;

		/**
		 * @brief How long a snackbar stays visible by default, in seconds
		 * (7 s, as in react-native-paper).
		 */
		static constexpr float SnackbarDuration = 7.0f;

		/**
		 * @brief Create the scale with the Material 3 baseline durations
		 * listed in MotionDuration.
		 */
		MotionScale(void);

		/**
		 * @brief Get the length of a duration token.
		 * @param duration The duration token to look up.
		 * @return The duration, in seconds.
		 * @throws std::out_of_range When `duration` is
		 * `MotionDuration::Count`.
		 */
		float getDuration(MotionDuration duration) const;

		/**
		 * @brief Replace the length of a duration token.
		 * @param duration The duration token to change.
		 * @param seconds The new duration, in seconds; must be greater than
		 * 0.
		 * @throws std::out_of_range When `duration` is
		 * `MotionDuration::Count`.
		 * @throws std::invalid_argument When `seconds` is zero or negative.
		 */
		void setDuration(MotionDuration duration, float seconds);

		/**
		 * @brief Get the easing for elements that move or change while
		 * staying on screen.
		 * @return motion::Easing::Standard.
		 */
		static constexpr motion::Easing standard(void)
		{
			return motion::Easing::Standard;
		}

		/**
		 * @brief Get the easing for elements entering the screen: fast at
		 * first, then settling.
		 * @return motion::Easing::EmphasizedDecelerate.
		 */
		static constexpr motion::Easing enter(void)
		{
			return motion::Easing::EmphasizedDecelerate;
		}

		/**
		 * @brief Get the easing for elements leaving the screen: slow at
		 * first, then speeding away.
		 * @return motion::Easing::EmphasizedAccelerate.
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
	 * @brief Spacing constants: the 4 dp grid every margin and gap is a
	 * multiple of, and the minimum size of something a user can hit.
	 *
	 * Everything is static and the same for every theme; the class is
	 * never instantiated.
	 *
	 * @code
	 * const float padding = theme::SpacingScale::get(4);			// 16 dp
	 * const float margin  = theme::SpacingScale::touchMargin(40);	// 4 dp
	 * @endcode
	 */
	class SpacingScale
	{
		public:
		/**
		 * @brief Base grid unit, in dp.
		 */
		static constexpr float Unit = 4.0f;

		/**
		 * @brief Smallest width and height an interactive control should
		 * offer to a pointer or a finger, in dp.
		 */
		static constexpr float MinTouchTarget = 48.0f;

		/**
		 * @brief Get a spacing as a number of grid units.
		 * @param steps The number of 4 dp units.
		 * @return The spacing, in dp (`steps` times 4).
		 */
		static constexpr float get(unsigned int steps)
		{
			return Unit * static_cast<float>(steps);
		}

		/**
		 * @brief Get the invisible margin that makes a small control as
		 * easy to hit as MinTouchTarget.
		 * @param extent The visible width or height of the control, in dp.
		 * @return The margin to add on each side along that axis, in dp;
		 * 0 when the control is already at least MinTouchTarget.
		 */
		static constexpr float touchMargin(float extent)
		{
			return extent >= MinTouchTarget ? 0.0f
											: (MinTouchTarget - extent) / 2.0f;
		}
	};

	/**
	 * @brief Dimensions of the ring drawn around the entity that has the
	 * keyboard focus.
	 *
	 * The ring is painted outside the focused entity, `Offset` away from its
	 * bound, `Width` thick, in the scheme's secondary color. The values are
	 * the same for every theme.
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
