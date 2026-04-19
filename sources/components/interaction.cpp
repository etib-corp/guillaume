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
		_onMouseHover	= nullptr;
		_onMouseUnhover = nullptr;
		_isMouseHovered = false;

		for (const auto button:
			 { utility::event::MouseButtonEvent::Button::Left,
			   utility::event::MouseButtonEvent::Button::Middle,
			   utility::event::MouseButtonEvent::Button::Right,
			   utility::event::MouseButtonEvent::Button::X1,
			   utility::event::MouseButtonEvent::Button::X2 }) {
			_isMouseButtonClicked[button]			   = false;
			_onMouseButtonClickHandlers[button]		   = nullptr;
			_onMouseButtonClickReleaseHandlers[button] = nullptr;
		}

		for (const auto button:
			 { utility::event::HandButtonEvent::Button::X,
			   utility::event::HandButtonEvent::Button::Y,
			   utility::event::HandButtonEvent::Button::A,
			   utility::event::HandButtonEvent::Button::B,
			   utility::event::HandButtonEvent::Button::Menu,
			   utility::event::HandButtonEvent::Button::System }) {
			_isHandButtonClicked[button]				= false;
			_onHandButtonClickHandlers[button]		= nullptr;
			_onHandButtonClickReleaseHandlers[button] = nullptr;
		}

		_onHandPinchHandler = nullptr;
		_isHandPinched		= false;
		_onHandPokeHandler	= nullptr;
		_isHandPoked		= false;
	}

	Interaction &
		Interaction::setMouseOnHoverHandler(const MouseHoverHandler &handler)
	{
		_onMouseHover = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setMouseOnUnhoverHandler(
		const MouseUnhoverHandler &handler)
	{
		_onMouseUnhover = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction::MouseHoverHandler
		Interaction::getMouseOnHoverHandler(void) const
	{
		return _onMouseHover;
	}

	Interaction::MouseUnhoverHandler
		Interaction::getMouseOnUnhoverHandler(void) const
	{
		return _onMouseUnhover;
	}

	bool Interaction::isMouseHovered(void) const
	{
		return _isMouseHovered;
	}

	Interaction &Interaction::setMouseHovered(bool isHovered)
	{
		if (_isMouseHovered == isHovered) {
			return *this;
		}
		_isMouseHovered = isHovered;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setMouseButtonOnClickHandler(
		const utility::event::MouseButtonEvent::Button &button,
		const MouseButtonClickHandler &handler)
	{
		_onMouseButtonClickHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setMouseButtonOnClickReleaseHandler(
		const utility::event::MouseButtonEvent::Button &button,
		const MouseButtonClickReleaseHandler &handler)
	{
		_onMouseButtonClickReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::MouseButtonEvent::Button,
				   Interaction::MouseButtonClickHandler> &
		Interaction::getMouseButtonOnClickHandlers() const
	{
		return _onMouseButtonClickHandlers;
	}

	const std::map<utility::event::MouseButtonEvent::Button,
				   Interaction::MouseButtonClickReleaseHandler> &
		Interaction::getMouseButtonOnClickReleaseHandlers() const
	{
		return _onMouseButtonClickReleaseHandlers;
	}

	bool Interaction::isMouseButtonClicked(
		const utility::event::MouseButtonEvent::Button &button) const
	{
		return _isMouseButtonClicked.at(button);
	}

	Interaction &Interaction::setMouseButtonClicked(
		const utility::event::MouseButtonEvent::Button &button,
		bool clicked)
	{
		if (_isMouseButtonClicked[button] == clicked) {
			return *this;
		}
		_isMouseButtonClicked[button] = clicked;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setHandButtonOnClickHandler(
		const utility::event::HandButtonEvent::Button &button,
		const HandButtonClickHandler &handler)
	{
		_onHandButtonClickHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setHandButtonOnClickReleaseHandler(
		const utility::event::HandButtonEvent::Button &button,
		const HandButtonClickReleaseHandler &handler)
	{
		_onHandButtonClickReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::HandButtonEvent::Button,
				   Interaction::HandButtonClickHandler> &
		Interaction::getHandButtonOnClickHandlers() const
	{
		return _onHandButtonClickHandlers;
	}

	const std::map<utility::event::HandButtonEvent::Button,
				   Interaction::HandButtonClickReleaseHandler> &
		Interaction::getHandButtonOnClickReleaseHandlers() const
	{
		return _onHandButtonClickReleaseHandlers;
	}

	bool Interaction::isHandButtonClicked(
		const utility::event::HandButtonEvent::Button &button) const
	{
		return _isHandButtonClicked.at(button);
	}

	Interaction &Interaction::setHandButtonClicked(
		const utility::event::HandButtonEvent::Button &button,
		bool clicked)
	{
		if (_isHandButtonClicked[button] == clicked) {
			return *this;
		}
		_isHandButtonClicked[button] = clicked;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setHandOnHoverHandler(
		const HandHoverHandler &handler)
	{
		_onHandHover = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setHandOnUnhoverHandler(
		const HandUnhoverHandler &handler)
	{
		_onHandUnhover = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction::HandHoverHandler
		Interaction::getHandOnHoverHandler(void) const
	{
		return _onHandHover;
	}

	Interaction::HandUnhoverHandler
		Interaction::getHandOnUnhoverHandler(void) const
	{
		return _onHandUnhover;
	}

	bool Interaction::isHandHovered(void) const
	{
		return _isHandHovered;
	}

	Interaction &Interaction::setHandHovered(bool isHovered)
	{
		if (_isHandHovered == isHovered) {
			return *this;
		}
		_isHandHovered = isHovered;
		setHasChanged(true);
		return *this;
	}

	Interaction &
		Interaction::setHandPinchHandler(const HandPinchHandler &handler)
	{
		_onHandPinchHandler = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction::HandPinchHandler Interaction::getHandPinchHandler(void) const
	{
		return _onHandPinchHandler;
	}

	bool Interaction::isHandPinched(void) const
	{
		return _isHandPinched;
	}

	Interaction &Interaction::setHandPinched(bool isPinched)
	{
		if (_isHandPinched == isPinched) {
			return *this;
		}
		_isHandPinched = isPinched;
		setHasChanged(true);
		return *this;
	}

	Interaction &Interaction::setHandPokeHandler(const HandPokeHandler &handler)
	{
		_onHandPokeHandler = handler;
		setHasChanged(true);
		return *this;
	}

	Interaction::HandPokeHandler Interaction::getHandPokeHandler(void) const
	{
		return _onHandPokeHandler;
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
}	 // namespace guillaume::components
