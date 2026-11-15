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
	 * @brief How far the content of an entity is scrolled, how large that
	 * content is, and how fast the mouse wheel scrolls it.
	 *
	 * The entity's Bound is the viewport: the visible window onto content
	 * that may be larger. The owner of the content sets the content size.
	 * systems::Scroll writes the offset in the Event phase: on each mouse
	 * wheel event it adds the wheel offset times the scroll step, then clamps
	 * the result between 0 and `contentSize - viewportSize` on each axis.
	 *
	 * @code
	 * registry.addComponent<components::Scrollable>(identifier)
	 * 	.setContentSize(utility::math::Vector2F({ 320.0f, 1200.0f }))
	 * 	.setScrollStep(utility::math::Vector2F({ 0.0f, 24.0f }));
	 * @endcode
	 *
	 * This component only holds the offset: no system of the library moves
	 * the children by it, the code that places the content reads
	 * getContentOffset(). Sizes and offsets are in dp, like Bound. The
	 * setters store what they are given and always mark the component
	 * changed; only clampOffset() keeps the offset in range.
	 *
	 * @see systems::Scroll, Bound
	 */
	class Scrollable: public ecs::Component
	{
		private:
		utility::math::Vector2F _contentOffset {
			0.0f, 0.0f
		};	  ///< Distance scrolled from the top-left of the content, in dp.
		utility::math::Vector2F _contentSize {
			0.0f, 0.0f
		};	  ///< Full size of the content, in dp.
		utility::math::Vector2F _scrollStep {
			1.0f, 1.0f
		};	  ///< Offset added per unit of wheel movement, per axis.

		public:
		/**
		 * @brief Construct a component scrolled to the origin, with an empty
		 * content and a scroll step of 1 on both axes.
		 */
		Scrollable(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Scrollable(void) = default;

		/**
		 * @brief Get how far the content is scrolled.
		 * @return The offset from the top-left of the content, in dp; (0, 0)
		 * when not scrolled.
		 */
		utility::math::Vector2F getContentOffset(void) const;

		/**
		 * @brief Scroll the content to an offset.
		 *
		 * The value is stored as given: call clampOffset() afterwards to keep
		 * it in range. Always marks the component changed.
		 *
		 * @param offset The offset from the top-left of the content, in dp.
		 * @return This component, to chain calls.
		 */
		Scrollable &setContentOffset(const utility::math::Vector2F &offset);

		/**
		 * @brief Get the full size of the scrollable content.
		 * @return The content size, in dp.
		 */
		utility::math::Vector2F getContentSize(void) const;

		/**
		 * @brief Set the full size of the scrollable content.
		 *
		 * The offset is not clamped again. Always marks the component
		 * changed.
		 *
		 * @param size The content size, in dp. A content smaller than the
		 * viewport cannot scroll.
		 * @return This component, to chain calls.
		 */
		Scrollable &setContentSize(const utility::math::Vector2F &size);

		/**
		 * @brief Get how much one unit of wheel movement scrolls.
		 * @return The factor applied to the wheel offset, per axis.
		 */
		utility::math::Vector2F getScrollStep(void) const;

		/**
		 * @brief Set how much one unit of wheel movement scrolls.
		 *
		 * Always marks the component changed.
		 *
		 * @param step The factor applied to the wheel offset, per axis: 0
		 * locks an axis, a negative value reverses it.
		 * @return This component, to chain calls.
		 */
		Scrollable &setScrollStep(const utility::math::Vector2F &step);

		/**
		 * @brief Get the largest offset that still shows content, for a
		 * viewport size.
		 * @param viewport The size of the visible window, in dp; usually the
		 * entity's Bound.
		 * @return `contentSize - viewport` on each axis, or 0 on an axis where
		 * the content fits in the viewport.
		 */
		utility::math::Vector2F
			getMaxOffset(const utility::graphic::SizeF &viewport) const;

		/**
		 * @brief Bring the offset back between 0 and getMaxOffset().
		 *
		 * Always marks the component changed, even when the offset was
		 * already in range.
		 *
		 * @param viewport The size of the visible window, in dp; usually the
		 * entity's Bound.
		 * @return This component, to chain calls.
		 */
		Scrollable &clampOffset(const utility::graphic::SizeF &viewport);
	};

}	 // namespace guillaume::components
