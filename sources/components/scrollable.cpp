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

#include "guillaume/components/scrollable.hpp"

#include <algorithm>

namespace guillaume::components
{
	utility::math::Vector2F Scrollable::getContentOffset(void) const
	{
		return _contentOffset;
	}

	Scrollable &
		Scrollable::setContentOffset(const utility::math::Vector2F &offset)
	{
		_contentOffset = offset;
		setHasChanged(true);
		return *this;
	}

	utility::math::Vector2F Scrollable::getContentSize(void) const
	{
		return _contentSize;
	}

	Scrollable &Scrollable::setContentSize(const utility::math::Vector2F &size)
	{
		_contentSize = size;
		setHasChanged(true);
		return *this;
	}

	utility::math::Vector2F Scrollable::getScrollStep(void) const
	{
		return _scrollStep;
	}

	Scrollable &Scrollable::setScrollStep(const utility::math::Vector2F &step)
	{
		_scrollStep = step;
		setHasChanged(true);
		return *this;
	}

	utility::math::Vector2F
		Scrollable::getMaxOffset(const utility::graphic::SizeF &viewport) const
	{
		return utility::math::Vector2F(
			{ std::max(0.0f, _contentSize.x - viewport.getWidth()),
			  std::max(0.0f, _contentSize.y - viewport.getHeight()) });
	}

	Scrollable &Scrollable::clampOffset(const utility::graphic::SizeF &viewport)
	{
		const auto maxOffset = getMaxOffset(viewport);
		_contentOffset.x	 = std::clamp(_contentOffset.x, 0.0f, maxOffset.x);
		_contentOffset.y	 = std::clamp(_contentOffset.y, 0.0f, maxOffset.y);
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
