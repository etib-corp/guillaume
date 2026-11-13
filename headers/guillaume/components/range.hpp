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

#include <functional>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief An interval `[low, high]` kept inside a range, with a callback
	 * run when it changes.
	 *
	 * It is the two-ended counterpart of components::Value, meant for
	 * controls with two handles such as a range slider. Both ends always lie
	 * within `[min, max]` (0 to 1 by default), are snapped to a multiple of
	 * `step` above `min` when `step` is strictly positive, and are kept
	 * ordered: if a new low end passes the high end, the two are swapped.
	 * No system of the library reads it; it is a building block for
	 * application widgets.
	 *
	 * @code
	 * components::Range range;
	 * range.setMax(100.0f);	 // the interval is 0 to 1
	 * range.setHigh(80.0f).setLow(90.0f);	  // the interval is 80 to 90
	 * @endcode
	 *
	 * Watch out:
	 * - The handler runs inside the setter, synchronously, each time the
	 *   stored interval really changes, including when a new bound clamps
	 *   it.
	 * - Keep `min <= max`: when raising both bounds, set the maximum first.
	 *
	 * @see components::Value
	 */
	class Range: public ecs::Component
	{
		public:
		/// Callback receiving the new low and high ends, in that order.
		using Handler = std::function<void(float, float)>;

		private:
		float _min { 0.0f };	  ///< Minimum bound.
		float _max { 1.0f };	  ///< Maximum bound.
		float _low { 0.0f };	  ///< Low end of the interval.
		float _high { 1.0f };	  ///< High end of the interval.
		float _step { 0.0f };	  ///< Snapping step (0 disables snapping).
		Handler _onChanged {};	  ///< Change handler.

		public:
		/**
		 * @brief Create the interval 0 to 1 in the range 0 to 1, without
		 * snapping.
		 */
		Range(void) = default;

		/**
		 * @brief Default destructor for the Range component.
		 */
		~Range(void) = default;

		/**
		 * @brief Get the lower bound of the range.
		 * @return The minimum (0 by default).
		 */
		float getMin(void) const;

		/**
		 * @brief Set the lower bound of the range.
		 *
		 * When the bound differs, marks the component changed, then clamps
		 * and snaps both ends again, which runs the handler if one moves.
		 * @param min The new minimum; it must not exceed the maximum.
		 * @return This component, for chaining.
		 */
		Range &setMin(float min);

		/**
		 * @brief Get the upper bound of the range.
		 * @return The maximum (1 by default).
		 */
		float getMax(void) const;

		/**
		 * @brief Set the upper bound of the range.
		 *
		 * When the bound differs, marks the component changed, then clamps
		 * and snaps both ends again, which runs the handler if one moves.
		 * @param max The new maximum; it must not be below the minimum.
		 * @return This component, for chaining.
		 */
		Range &setMax(float max);

		/**
		 * @brief Get the low end of the interval.
		 * @return The low end, within `[min, high]`.
		 */
		float getLow(void) const;

		/**
		 * @brief Set the low end of the interval.
		 *
		 * The value is clamped and snapped first. If it ends up above the
		 * high end, the two ends are swapped. When the interval changes,
		 * marks the component changed and calls the handler.
		 * @param low The requested low end, in the unit of the range.
		 * @return This component, for chaining.
		 */
		Range &setLow(float low);

		/**
		 * @brief Get the high end of the interval.
		 * @return The high end, within `[low, max]`.
		 */
		float getHigh(void) const;

		/**
		 * @brief Set the high end of the interval.
		 *
		 * The value is clamped and snapped first. If it ends up below the
		 * low end, the two ends are swapped. When the interval changes,
		 * marks the component changed and calls the handler.
		 * @param high The requested high end, in the unit of the range.
		 * @return This component, for chaining.
		 */
		Range &setHigh(float high);

		/**
		 * @brief Get the snapping step.
		 * @return The step; 0 (the default) or less disables snapping.
		 */
		float getStep(void) const;

		/**
		 * @brief Set the snapping step.
		 *
		 * Marks the component changed when the step differs. The interval
		 * is not snapped again until one of its ends or a bound is set.
		 * @param step The distance between two allowed values, counted from
		 * the minimum; 0 or less disables snapping.
		 * @return This component, for chaining.
		 */
		Range &setStep(float step);

		/**
		 * @brief Set the callback run when the interval changes.
		 *
		 * Replaces the previous handler. Does not mark the component
		 * changed.
		 * @param handler The callback, or an empty one to remove it; it
		 * receives the new low and high ends.
		 * @return This component, for chaining.
		 */
		Range &setOnChangedHandler(const Handler &handler);

		/**
		 * @brief Get the callback run when the interval changes.
		 * @return A copy of the callback, empty when none is set.
		 */
		Handler getOnChangedHandler(void) const;

		private:
		/**
		 * @brief Clamp and snap a raw value to the current bounds and step.
		 * @param value The raw value.
		 * @return The value clamped to `[min, max]`, then moved to the
		 * nearest multiple of the step above the minimum when the step is
		 * strictly positive.
		 */
		float sanitize(float value) const;

		/**
		 * @brief Commit a new interval.
		 *
		 * Clamps and snaps both ends, swaps them when out of order, and,
		 * only when the result differs from the stored interval, stores it,
		 * marks the component changed and calls the handler.
		 * @param low The raw low end.
		 * @param high The raw high end.
		 */
		void apply(float low, float high);
	};

}	 // namespace guillaume::components
