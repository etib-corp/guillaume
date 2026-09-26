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
	 * @brief Component holding a low/high interval within a range.
	 *
	 * Reusable by Slider (range variant), TimePicker and Carousel. The interval
	 * is kept ordered and within `[min, max]`, and snapped to `step` when
	 * `step` is strictly positive.
	 */
	class Range: public ecs::Component
	{
		public:
		using Handler =
			std::function<void(float, float)>;	  ///< Change handler.

		private:
		float _min { 0.0f };	  ///< Minimum bound.
		float _max { 1.0f };	  ///< Maximum bound.
		float _low { 0.0f };	  ///< Low end of the interval.
		float _high { 1.0f };	  ///< High end of the interval.
		float _step { 0.0f };	  ///< Snapping step (0 disables snapping).
		Handler _onChanged {};	  ///< Change handler.

		public:
		/**
		 * @brief Default constructor for the Range component.
		 */
		Range(void) = default;

		/**
		 * @brief Default destructor for the Range component.
		 */
		~Range(void) = default;

		/**
		 * @brief Get the minimum bound.
		 * @return The minimum bound.
		 */
		float getMin(void) const;

		/**
		 * @brief Set the minimum bound, clamping the interval.
		 * @param min The new minimum bound.
		 * @return Reference to this Range for chaining.
		 */
		Range &setMin(float min);

		/**
		 * @brief Get the maximum bound.
		 * @return The maximum bound.
		 */
		float getMax(void) const;

		/**
		 * @brief Set the maximum bound, clamping the interval.
		 * @param max The new maximum bound.
		 * @return Reference to this Range for chaining.
		 */
		Range &setMax(float max);

		/**
		 * @brief Get the low end of the interval.
		 * @return The low end.
		 */
		float getLow(void) const;

		/**
		 * @brief Set the low end of the interval.
		 * @param low The new low end.
		 * @return Reference to this Range for chaining.
		 */
		Range &setLow(float low);

		/**
		 * @brief Get the high end of the interval.
		 * @return The high end.
		 */
		float getHigh(void) const;

		/**
		 * @brief Set the high end of the interval.
		 * @param high The new high end.
		 * @return Reference to this Range for chaining.
		 */
		Range &setHigh(float high);

		/**
		 * @brief Get the snapping step.
		 * @return The snapping step (0 disables snapping).
		 */
		float getStep(void) const;

		/**
		 * @brief Set the snapping step.
		 * @param step The new snapping step (0 disables snapping).
		 * @return Reference to this Range for chaining.
		 */
		Range &setStep(float step);

		/**
		 * @brief Set the change handler.
		 * @param handler The handler to call when the interval changes.
		 * @return Reference to this Range for chaining.
		 */
		Range &setOnChangedHandler(const Handler &handler);

		/**
		 * @brief Get the change handler.
		 * @return The change handler.
		 */
		Handler getOnChangedHandler(void) const;

		private:
		/**
		 * @brief Clamp and snap a raw value to the current bounds and step.
		 * @param value The raw value.
		 * @return The clamped and snapped value.
		 */
		float sanitize(float value) const;

		/**
		 * @brief Commit a new interval, firing the handler when it changes.
		 * @param low The raw low end.
		 * @param high The raw high end.
		 */
		void apply(float low, float high);
	};

}	 // namespace guillaume::components
