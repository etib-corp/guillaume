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

#include "guillaume/components/style_state.hpp"

namespace guillaume::components
{
	void StyleState::setFlag(bool &flag, bool value)
	{
		if (flag == value) {
			return;
		}
		flag = value;
		setHasChanged(true);
	}

	bool StyleState::isEnabled(void) const
	{
		return _enabled;
	}

	StyleState &StyleState::setEnabled(bool enabled)
	{
		setFlag(_enabled, enabled);
		if (!enabled) {
			setFlag(_hovered, false);
			setFlag(_pressed, false);
			setFlag(_focused, false);
			setFlag(_dragged, false);
		}
		return *this;
	}

	bool StyleState::isHovered(void) const
	{
		return _hovered;
	}

	StyleState &StyleState::setHovered(bool hovered)
	{
		setFlag(_hovered, hovered && _enabled);
		return *this;
	}

	bool StyleState::isPressed(void) const
	{
		return _pressed;
	}

	StyleState &StyleState::setPressed(bool pressed)
	{
		setFlag(_pressed, pressed && _enabled);
		return *this;
	}

	bool StyleState::isFocused(void) const
	{
		return _focused;
	}

	StyleState &StyleState::setFocused(bool focused)
	{
		setFlag(_focused, focused && _enabled);
		return *this;
	}

	bool StyleState::isDragged(void) const
	{
		return _dragged;
	}

	StyleState &StyleState::setDragged(bool dragged)
	{
		setFlag(_dragged, dragged && _enabled);
		return *this;
	}

	std::optional<theme::StateLayer> StyleState::getActiveLayer(void) const
	{
		if (!_enabled) {
			return std::nullopt;
		}
		if (_dragged) {
			return theme::StateLayer::Dragged;
		}
		if (_pressed) {
			return theme::StateLayer::Pressed;
		}
		if (_focused) {
			return theme::StateLayer::Focused;
		}
		if (_hovered) {
			return theme::StateLayer::Hovered;
		}
		return std::nullopt;
	}

}	 // namespace guillaume::components
