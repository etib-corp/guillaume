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

#include "guillaume/components/mouse_hover_interaction.hpp"

namespace guillaume::components
{

	MouseHoverInteraction::MouseHoverInteraction(void)
		: ecs::Component()
	{
		_onHover   = nullptr;
		_onUnhover = nullptr;
		_isHovered = false;
	}

	MouseHoverInteraction &
		MouseHoverInteraction::setOnHoverHandler(const HoverHandler &handler)
	{
		_onHover = handler;
		setHasChanged(true);
		return *this;
	}

	MouseHoverInteraction &MouseHoverInteraction::setOnUnhoverHandler(
		const UnhoverHandler &handler)
	{
		_onUnhover = handler;
		setHasChanged(true);
		return *this;
	}

	MouseHoverInteraction::HoverHandler
		MouseHoverInteraction::getOnHoverHandler(void) const
	{
		return _onHover;
	}

	MouseHoverInteraction::UnhoverHandler
		MouseHoverInteraction::getOnUnhoverHandler(void) const
	{
		return _onUnhover;
	}

	bool MouseHoverInteraction::isHovered(void) const
	{
		return _isHovered;
	}

	MouseHoverInteraction &MouseHoverInteraction::setHovered(bool isHovered)
	{
		if (_isHovered == isHovered) {
			return *this;
		}
		_isHovered = isHovered;
		setHasChanged(true);
		return *this;
	}

	utility::graphic::SizeUI
		MouseHoverInteraction::getAccessibilityMargin(void) const
	{
		return _accessibilityMargin;
	}

	MouseHoverInteraction &MouseHoverInteraction::setAccessibilityMargin(
		const utility::graphic::SizeUI &margin)
	{
		if (_accessibilityMargin == margin) {
			return *this;
		}
		_accessibilityMargin = margin;
		setHasChanged(true);
		return *this;
	}

}	 // namespace guillaume::components
