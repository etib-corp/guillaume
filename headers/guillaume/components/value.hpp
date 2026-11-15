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
	 * @brief A number kept inside a range, with a callback run when it
	 * changes.
	 *
	 * The value always lies within `[min, max]` (0 to 1 by default) and is
	 * snapped to a multiple of `step` above `min` when `step` is strictly
	 * positive. The widget systems use it as the value of their widget: the
	 * fraction of a progress indicator (systems::ProgressIndicator), the
	 * on/off state of a switch (systems::Switch, 0 or 1 with a step of 1).
	 *
	 * @code
	 * auto &value = registry.getComponent<components::Value>(identifier);
	 * value.setMax(100.0f).setStep(5.0f);
	 * value.setValue(42.0f);	 // stores 40
	 * @endcode
	 *
	 * Watch out:
	 * - The handler runs inside the setter, synchronously, each time the
	 *   stored value really changes, including when a new bound clamps it.
	 * - Keep `min <= max`: when raising both bounds, set the maximum first.
	 *
	 * @see components::Range, systems::Switch, systems::ProgressIndicator
	 */
	class Value: public ecs::Component
	{
		public:
		/// Callback receiving the new value, already clamped and snapped.
		using Handler = std::function<void(float)>;

		private:
		float _min { 0.0f };	  ///< Minimum value.
		float _max { 1.0f };	  ///< Maximum value.
		float _value { 0.0f };	  ///< Current value.
		float _step { 0.0f };	  ///< Snapping step (0 disables snapping).
		Handler _onChanged {};	  ///< Value change handler.

		public:
		/**
		 * @brief Create a value of 0 in the range 0 to 1, without snapping.
		 */
		Value(void) = default;

		/**
		 * @brief Default destructor for the Value component.
		 */
		~Value(void) = default;

		/**
		 * @brief Get the lower bound of the range.
		 * @return The minimum (0 by default).
		 */
		float getMin(void) const;

		/**
		 * @brief Set the lower bound of the range.
		 *
		 * When the bound differs, marks the component changed, then clamps
		 * and snaps the current value again, which runs the handler if the
		 * value moves.
		 * @param min The new minimum; it must not exceed the maximum.
		 * @return This component, for chaining.
		 */
		Value &setMin(float min);

		/**
		 * @brief Get the upper bound of the range.
		 * @return The maximum (1 by default).
		 */
		float getMax(void) const;

		/**
		 * @brief Set the upper bound of the range.
		 *
		 * When the bound differs, marks the component changed, then clamps
		 * and snaps the current value again, which runs the handler if the
		 * value moves.
		 * @param max The new maximum; it must not be below the minimum.
		 * @return This component, for chaining.
		 */
		Value &setMax(float max);

		/**
		 * @brief Get the current value.
		 * @return The value, within `[min, max]`.
		 */
		float getValue(void) const;

		/**
		 * @brief Set the current value.
		 *
		 * The value is clamped to `[min, max]` and snapped to the step
		 * first. When the result differs from the stored value, stores it,
		 * marks the component changed and calls the handler with it.
		 * @param value The requested value, in the unit of the range.
		 * @return This component, for chaining.
		 */
		Value &setValue(float value);

		/**
		 * @brief Get the snapping step.
		 * @return The step; 0 (the default) or less disables snapping.
		 */
		float getStep(void) const;

		/**
		 * @brief Set the snapping step.
		 *
		 * Marks the component changed when the step differs. The current
		 * value is not snapped again until the next setValue() or bound
		 * change.
		 * @param step The distance between two allowed values, counted from
		 * the minimum; 0 or less disables snapping.
		 * @return This component, for chaining.
		 */
		Value &setStep(float step);

		/**
		 * @brief Get where the value sits in its range.
		 * @return 0 at the minimum to 1 at the maximum, or 0 when the range
		 * is empty (`max <= min`).
		 */
		float getNormalizedValue(void) const;

		/**
		 * @brief Set the value from its position in the range.
		 *
		 * Same as setValue() with `min + normalized * (max - min)`.
		 * @param normalized The position, 0 to 1; values outside are
		 * clamped.
		 * @return This component, for chaining.
		 */
		Value &setNormalizedValue(float normalized);

		/**
		 * @brief Set the callback run when the value changes.
		 *
		 * Replaces the previous handler. Does not mark the component
		 * changed. The widget systems install their own handler here to
		 * restyle the widget.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		Value &setOnChangedHandler(const Handler &handler);

		/**
		 * @brief Get the callback run when the value changes.
		 * @return A copy of the callback, empty when none is set.
		 */
		Handler getOnChangedHandler(void) const;

		private:
		/**
		 * @brief Clamp and snap a raw value to the current range and step.
		 * @param value The raw value.
		 * @return The value clamped to `[min, max]`, then moved to the
		 * nearest multiple of the step above the minimum when the step is
		 * strictly positive.
		 */
		float sanitize(float value) const;
	};

}	 // namespace guillaume::components
