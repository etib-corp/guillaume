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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component describing a clip region resolved from an entity bound.
	 *
	 * The `systems::Clip` system derives a clip rectangle from the entity
	 * `components::Transform` and `components::Bound` (shrunk by `margin`) and
	 * stores it here. Consumers (or an engine layer) can then apply it as a
	 * scissor rectangle.
	 *
	 * @see systems::Clip
	 */
	class Clip: public ecs::Component
	{
		public:
		/**
		 * @brief Resolved clip rectangle.
		 */
		struct Rect {
			float x { 0.0f };		///< Rectangle origin X.
			float y { 0.0f };		///< Rectangle origin Y.
			float width { 0.0f };	///< Rectangle width.
			float height { 0.0f };	///< Rectangle height.

			/**
			 * @brief Equality operator.
			 * @param other The other rectangle.
			 * @return True when both rectangles are equal.
			 */
			bool operator==(const Rect &other) const
			{
				return x == other.x && y == other.y && width == other.width
					&& height == other.height;
			}
		};

		private:
		float _margin { 0.0f };	   ///< Inset applied to the clip rectangle.
		bool _enabled { true };	   ///< Whether clipping is active.
		Rect _rect {};			   ///< Resolved clip rectangle.

		public:
		/**
		 * @brief Default constructor for the Clip component.
		 */
		Clip(void) = default;

		/**
		 * @brief Default destructor for the Clip component.
		 */
		~Clip(void) = default;

		/**
		 * @brief Get the clip rectangle margin.
		 * @return The margin.
		 */
		float getMargin(void) const;

		/**
		 * @brief Set the clip rectangle margin.
		 * @param margin The new margin.
		 * @return Reference to this Clip for chaining.
		 */
		Clip &setMargin(float margin);

		/**
		 * @brief Check whether clipping is enabled.
		 * @return True when enabled.
		 */
		bool isEnabled(void) const;

		/**
		 * @brief Set whether clipping is enabled.
		 * @param enabled Whether clipping is enabled.
		 * @return Reference to this Clip for chaining.
		 */
		Clip &setEnabled(bool enabled);

		/**
		 * @brief Get the resolved clip rectangle.
		 * @return The resolved rectangle.
		 */
		const Rect &getRect(void) const;

		/**
		 * @brief Set the resolved clip rectangle.
		 * @param rect The resolved rectangle.
		 * @return Reference to this Clip for chaining.
		 */
		Clip &setRect(const Rect &rect);
	};

}	 // namespace guillaume::components
