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
	 * @brief Component holding a scalar value within a range.
	 *
	 * Reusable by Slider, ProgressIndicator, pickers and Carousel. The value is
	 * kept within `[min, max]` and snapped to `step` when `step` is strictly
	 * positive. Changing the value triggers the on-changed handler when the
	 * stored value actually changes.
	 */
	class Value: public ecs::Component
	{
		public:
		using Handler =
			std::function<void(float)>;	   ///< Value change handler.

		private:
		float _min { 0.0f };	  ///< Minimum value.
		float _max { 1.0f };	  ///< Maximum value.
		float _value { 0.0f };	  ///< Current value.
		float _step { 0.0f };	  ///< Snapping step (0 disables snapping).
		Handler _onChanged {};	  ///< Value change handler.

		public:
		/**
		 * @brief Default constructor for the Value component.
		 */
		Value(void) = default;

		/**
		 * @brief Default destructor for the Value component.
		 */
		~Value(void) = default;

		/**
		 * @brief Get the minimum value.
		 * @return The minimum value.
		 */
		float getMin(void) const;

		/**
		 * @brief Set the minimum value, clamping the current value.
		 * @param min The new minimum value.
		 * @return Reference to this Value for chaining.
		 */
		Value &setMin(float min);

		/**
		 * @brief Get the maximum value.
		 * @return The maximum value.
		 */
		float getMax(void) const;

		/**
		 * @brief Set the maximum value, clamping the current value.
		 * @param max The new maximum value.
		 * @return Reference to this Value for chaining.
		 */
		Value &setMax(float max);

		/**
		 * @brief Get the current value.
		 * @return The current value.
		 */
		float getValue(void) const;

		/**
		 * @brief Set the current value, clamped and snapped.
		 * @param value The new value.
		 * @return Reference to this Value for chaining.
		 */
		Value &setValue(float value);

		/**
		 * @brief Get the snapping step.
		 * @return The snapping step (0 disables snapping).
		 */
		float getStep(void) const;

		/**
		 * @brief Set the snapping step.
		 * @param step The new snapping step (0 disables snapping).
		 * @return Reference to this Value for chaining.
		 */
		Value &setStep(float step);

		/**
		 * @brief Get the value normalized to `[0, 1]`.
		 * @return The normalized value, or 0 when the range is empty.
		 */
		float getNormalizedValue(void) const;

		/**
		 * @brief Set the value from a normalized `[0, 1]` position.
		 * @param normalized The normalized value.
		 * @return Reference to this Value for chaining.
		 */
		Value &setNormalizedValue(float normalized);

		/**
		 * @brief Set the value change handler.
		 * @param handler The handler to call when the value changes.
		 * @return Reference to this Value for chaining.
		 */
		Value &setOnChangedHandler(const Handler &handler);

		/**
		 * @brief Get the value change handler.
		 * @return The value change handler.
		 */
		Handler getOnChangedHandler(void) const;

		private:
		/**
		 * @brief Clamp and snap a raw value to the current range and step.
		 * @param value The raw value.
		 * @return The clamped and snapped value.
		 */
		float sanitize(float value) const;
	};

}	 // namespace guillaume::components
