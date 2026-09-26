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
	 * @brief Easing curves covering the Material Design 3 motion tokens.
	 */
	enum class Easing {
		Linear,		   ///< Constant speed.
		Standard,	   ///< M3 standard curve, cubic-bezier(0.2, 0, 0, 1).
		Decelerate,	   ///< M3 decelerate curve, cubic-bezier(0, 0, 0, 1).
		Accelerate,	   ///< M3 accelerate curve, cubic-bezier(0.3, 0, 1, 1).
		Emphasized	  ///< M3 emphasized curve, cubic-bezier(0.05, 0.7, 0.1, 1).
	};

	/**
	 * @brief Evaluate an easing curve at a normalized time.
	 * @param easing The easing curve to apply.
	 * @param t The normalized time, clamped to `[0, 1]`.
	 * @return The eased value, in `[0, 1]`.
	 */
	float applyEasing(Easing easing, float t);

	/**
	 * @brief Evaluate a cubic bezier curve at a normalized x.
	 * @param x The normalized x position, clamped to `[0, 1]`.
	 * @param x1 First control point x.
	 * @param y1 First control point y.
	 * @param x2 Second control point x.
	 * @param y2 Second control point y.
	 * @return The curve value at `x`.
	 */
	float cubicBezier(float x, float x1, float y1, float x2, float y2);

}	 // namespace guillaume::motion
