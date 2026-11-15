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

namespace guillaume::motion
{

	/**
	 * @brief The shapes an animation's progress can follow over time.
	 *
	 * An easing turns "how much time has passed" into "how far the
	 * animation is": linear moves at constant speed, the others start or
	 * end gently. The curves are those of Material Design 3, given as CSS
	 * cubic-bezier control points. theme::MotionScale says which one to use
	 * for which kind of movement.
	 *
	 * @note `Emphasized` and `EmphasizedDecelerate` are the same curve.
	 * @see applyEasing
	 */
	enum class Easing {
		Linear,		   ///< Constant speed.
		Standard,	   ///< M3 standard curve, cubic-bezier(0.2, 0, 0, 1).
		Decelerate,	   ///< M3 decelerate curve, cubic-bezier(0, 0, 0, 1).
		Accelerate,	   ///< M3 accelerate curve, cubic-bezier(0.3, 0, 1, 1).
		Emphasized,	   ///< M3 emphasized curve, cubic-bezier(0.05, 0.7, 0.1,
					   ///< 1).
		EmphasizedDecelerate,	 ///< M3 emphasized decelerate,
								 ///< cubic-bezier(0.05, 0.7, 0.1, 1).
		EmphasizedAccelerate	 ///< M3 emphasized accelerate,
								 ///< cubic-bezier(0.3, 0, 0.8, 0.15).
	};

	/**
	 * @brief Get how far an animation is, given how much of its time has
	 * passed.
	 *
	 * @code
	 * // elapsed and duration in seconds
	 * const float progress =
	 * 	motion::applyEasing(motion::Easing::Standard, elapsed / duration);
	 * const float width = from + (to - from) * progress;
	 * @endcode
	 * @param easing The curve to follow.
	 * @param t The fraction of the animation's duration that has passed, 0
	 * (start) to 1 (end). Values outside that range are clamped.
	 * @return The progress of the animation, 0 (start value) to 1 (end
	 * value). 0 gives 0 and 1 gives 1 for every curve.
	 */
	float applyEasing(Easing easing, float t);

	/**
	 * @brief Evaluate a CSS-style cubic bezier easing curve, which goes
	 * from (0, 0) to (1, 1) and is bent by two control points.
	 *
	 * Use it for a curve that Easing does not offer. The result is found by
	 * bisection, precise enough for animation.
	 * @param x The fraction of time that has passed, 0 to 1. Values outside
	 * that range are clamped.
	 * @param x1 The x of the first control point, 0 to 1.
	 * @param y1 The y of the first control point.
	 * @param x2 The x of the second control point, 0 to 1.
	 * @param y2 The y of the second control point.
	 * @return The y of the curve at `x`: the progress of the animation. It
	 * can leave the 0 to 1 range when `y1` or `y2` does.
	 */
	float cubicBezier(float x, float x1, float y1, float x2, float y2);

}	 // namespace guillaume::motion
