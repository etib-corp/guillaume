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

#include "guillaume/components/drag_interaction.hpp"

namespace guillaume::components
{
	bool DragInteraction::isDragging(void) const
	{
		return _dragging;
	}

	DragInteraction &DragInteraction::setDragging(bool dragging)
	{
		if (_dragging == dragging) {
			return *this;
		}
		_dragging = dragging;
		setHasChanged(true);
		return *this;
	}

	utility::math::Vector2F DragInteraction::getLastPosition(void) const
	{
		return _lastPosition;
	}

	DragInteraction &DragInteraction::setLastPosition(
		const utility::math::Vector2F &position)
	{
		_lastPosition = position;
		return *this;
	}

	utility::math::Vector2F DragInteraction::getDelta(void) const
	{
		return _delta;
	}

	DragInteraction &
		DragInteraction::setDelta(const utility::math::Vector2F &delta)
	{
		_delta = delta;
		setHasChanged(true);
		return *this;
	}

	DragInteraction &
		DragInteraction::setOnDragStartHandler(const DragStartHandler &handler)
	{
		_onDragStart = handler;
		return *this;
	}

	DragInteraction::DragStartHandler
		DragInteraction::getOnDragStartHandler(void) const
	{
		return _onDragStart;
	}

	DragInteraction &
		DragInteraction::setOnDragHandler(const DragHandler &handler)
	{
		_onDrag = handler;
		return *this;
	}

	DragInteraction::DragHandler DragInteraction::getOnDragHandler(void) const
	{
		return _onDrag;
	}

	DragInteraction &
		DragInteraction::setOnDragEndHandler(const DragEndHandler &handler)
	{
		_onDragEnd = handler;
		return *this;
	}

	DragInteraction::DragEndHandler
		DragInteraction::getOnDragEndHandler(void) const
	{
		return _onDragEnd;
	}
}	 // namespace guillaume::components
