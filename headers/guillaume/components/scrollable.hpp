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

#include <utility/graphic/size.hpp>
#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component holding a scrollable content offset.
	 *
	 * The viewport is the entity `components::Bound`; `contentSize` is the
	 * total size of the scrollable content. The offset is clamped to
	 * `[0, contentSize - viewportSize]` by `systems::Scroll`.
	 *
	 * @see systems::Scroll
	 */
	class Scrollable: public ecs::Component
	{
		private:
		utility::math::Vector2F _contentOffset { 0.0f,
												 0.0f };	///< Scroll offset.
		utility::math::Vector2F _contentSize { 0.0f,
											   0.0f };	  ///< Content size.
		utility::math::Vector2F _scrollStep { 1.0f,
											  1.0f };	 ///< Wheel step scale.

		public:
		/**
		 * @brief Default constructor for the Scrollable component.
		 */
		Scrollable(void) = default;

		/**
		 * @brief Default destructor for the Scrollable component.
		 */
		~Scrollable(void) = default;

		/**
		 * @brief Get the content offset.
		 * @return The content offset.
		 */
		utility::math::Vector2F getContentOffset(void) const;

		/**
		 * @brief Set the content offset.
		 * @param offset The new content offset.
		 * @return Reference to this Scrollable for chaining.
		 */
		Scrollable &setContentOffset(const utility::math::Vector2F &offset);

		/**
		 * @brief Get the total content size.
		 * @return The content size.
		 */
		utility::math::Vector2F getContentSize(void) const;

		/**
		 * @brief Set the total content size.
		 * @param size The new content size.
		 * @return Reference to this Scrollable for chaining.
		 */
		Scrollable &setContentSize(const utility::math::Vector2F &size);

		/**
		 * @brief Get the wheel step scale.
		 * @return The wheel step scale.
		 */
		utility::math::Vector2F getScrollStep(void) const;

		/**
		 * @brief Set the wheel step scale.
		 * @param step The new wheel step scale.
		 * @return Reference to this Scrollable for chaining.
		 */
		Scrollable &setScrollStep(const utility::math::Vector2F &step);

		/**
		 * @brief Get the maximum reachable offset for a viewport size.
		 * @param viewport The viewport size.
		 * @return The maximum offset, never negative.
		 */
		utility::math::Vector2F
			getMaxOffset(const utility::graphic::SizeF &viewport) const;

		/**
		 * @brief Clamp the content offset to the valid range.
		 * @param viewport The viewport size.
		 * @return Reference to this Scrollable for chaining.
		 */
		Scrollable &clampOffset(const utility::graphic::SizeF &viewport);
	};

}	 // namespace guillaume::components
