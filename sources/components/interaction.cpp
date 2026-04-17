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

#include "guillaume/components/interaction.hpp"

namespace guillaume::components
{
	Interaction::Interaction(void)
	{
		for (const auto button:
			 { utility::event::MouseButtonEvent::MouseButton::Left,
			   utility::event::MouseButtonEvent::MouseButton::Middle,
			   utility::event::MouseButtonEvent::MouseButton::Right,
			   utility::event::MouseButtonEvent::MouseButton::X1,
			   utility::event::MouseButtonEvent::MouseButton::X2 }) {
			_isClicked[button] = false;
			_pressedInside[button] = false;
			_onClickHandlers[button] = nullptr;
			_onReleaseHandlers[button] = nullptr;
		}
	}

	Interaction &Interaction::setOnClickHandler(
		const utility::event::MouseButtonEvent::MouseButton &button,
		const ClickHandler &handler)
	{
		_onClickHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setOnReleaseHandler(
		const utility::event::MouseButtonEvent::MouseButton &button,
		const ClickHandler &handler)
	{
		_onReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::MouseButtonEvent::MouseButton,
				   Interaction::ClickHandler> &
		Interaction::getOnClickHandlers() const
	{
		return _onClickHandlers;
	}

	const std::map<utility::event::MouseButtonEvent::MouseButton,
				   Interaction::ClickHandler> &
		Interaction::getOnReleaseHandlers() const
	{
		return _onReleaseHandlers;
	}

	bool Interaction::isClicked(
		const utility::event::MouseButtonEvent::MouseButton &button) const
	{
		return _isClicked.at(button);
	}

	Interaction &Interaction::setClicked(
		const utility::event::MouseButtonEvent::MouseButton &button,
		bool clicked)
	{
		if (_isEntityClicked == clicked && _isClicked[button] == clicked) {
			return *this;
		}
		_isEntityClicked = clicked;
		_isClicked[button] = clicked;
		setHasChanged(true);
		return *this;
	}

	bool Interaction::isEntityClicked() const
	{
		return _isEntityClicked;
	}

	Interaction &Interaction::setPressedInside(
		const utility::event::MouseButtonEvent::MouseButton &button,
		bool pressedInside)
	{
		if (_pressedInside[button] == pressedInside) {
			return *this;
		}
		_pressedInside[button] = pressedInside;
		setHasChanged(true);
		return *this;
	}

	bool Interaction::isPressedInside(
		const utility::event::MouseButtonEvent::MouseButton &button) const
	{
		return _pressedInside.at(button);
	}

	Interaction &Interaction::setOnHoverHandler(const HoverHandler &handler)
	{
		_onHover = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setOnUnhoverHandler(const HoverHandler &handler)
	{
		_onUnhover = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction::HoverHandler Interaction::getOnHoverHandler(void) const
	{
		return _onHover;
	}

	Interaction::HoverHandler Interaction::getOnUnhoverHandler(void) const
	{
		return _onUnhover;
	}

	bool Interaction::isHovered(void) const
	{
		return _isHovered;
	}

	Interaction &Interaction::setHovered(bool isHovered)
	{
		if (_isHovered == isHovered) {
			return *this;
		}
		_isHovered = isHovered;
		setHasChanged(true);
		return *this;
	}

	const utility::math::Vector2F &Interaction::getAccessibilityMargin() const
	{
		return _accessibilityMargin;
	}

	void Interaction::setAccessibilityMargin(
		const utility::math::Vector2F &margin)
	{
		if (_accessibilityMargin == margin) {
			return;
		}
		_accessibilityMargin = margin;
		setHasChanged(true);
	}
} // namespace guillaume::components
