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

#include "guillaume/motion/easing.hpp"

#include <algorithm>
#include <cmath>

namespace guillaume::motion
{
	namespace
	{
		/**
		 * @brief Evaluate one coordinate of a cubic bezier at parameter u.
		 * @param u Curve parameter.
		 * @param p1 First control point.
		 * @param p2 Second control point.
		 * @return The coordinate value.
		 */
		float bezierComponent(float u, float p1, float p2)
		{
			const float oneMinusU = 1.0f - u;
			return 3.0f * oneMinusU * oneMinusU * u * p1
				+ 3.0f * oneMinusU * u * u * p2 + u * u * u;
		}
	}	 // namespace

	float cubicBezier(float x, float x1, float y1, float x2, float y2)
	{
		const float target = std::clamp(x, 0.0f, 1.0f);

		// Find the parameter u whose x coordinate matches the target, using a
		// bisection that is stable and precise enough for animation.
		float low  = 0.0f;
		float high = 1.0f;
		float u	   = target;

		for (int i = 0; i < 32; ++i) {
			u					 = (low + high) * 0.5f;
			const float currentX = bezierComponent(u, x1, x2);
			if (currentX < target) {
				low = u;
			} else {
				high = u;
			}
		}

		return bezierComponent(u, y1, y2);
	}

	float applyEasing(Easing easing, float t)
	{
		const float clamped = std::clamp(t, 0.0f, 1.0f);

		switch (easing) {
			case Easing::Linear:
				return clamped;
			case Easing::Decelerate:
				return cubicBezier(clamped, 0.0f, 0.0f, 0.0f, 1.0f);
			case Easing::Accelerate:
				return cubicBezier(clamped, 0.3f, 0.0f, 1.0f, 1.0f);
			case Easing::Emphasized:
				return cubicBezier(clamped, 0.05f, 0.7f, 0.1f, 1.0f);
			case Easing::Standard:
			default:
				return cubicBezier(clamped, 0.2f, 0.0f, 0.0f, 1.0f);
		}
	}

}	 // namespace guillaume::motion
