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

#include "guillaume/components/snackbar.hpp"

#include <stdexcept>
#include <utility>

namespace guillaume::components
{
	Snackbar::Variant Snackbar::getVariant(void) const noexcept
	{
		return _variant;
	}

	Snackbar &Snackbar::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Snackbar::getMessage(void) const noexcept
	{
		return _message;
	}

	Snackbar &Snackbar::setMessage(const std::string &message)
	{
		if (_message != message) {
			_message = message;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Snackbar::getAction(void) const noexcept
	{
		return _action;
	}

	Snackbar &Snackbar::setAction(const std::string &action)
	{
		if (_action != action) {
			_action = action;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Snackbar::getMessageEntity(void) const noexcept
	{
		return _messageEntity;
	}

	Snackbar &Snackbar::setMessageEntity(ecs::Entity::Identifier identifier)
	{
		if (_messageEntity != identifier) {
			_messageEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Snackbar::getActionEntity(void) const noexcept
	{
		return _actionEntity;
	}

	Snackbar &Snackbar::setActionEntity(ecs::Entity::Identifier identifier)
	{
		if (_actionEntity != identifier) {
			_actionEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	bool Snackbar::isVisible(void) const noexcept
	{
		return _visible;
	}

	Snackbar &Snackbar::setVisible(bool visible)
	{
		if (_visible != visible) {
			_visible = visible;
			setHasChanged(true);
		}
		return *this;
	}

	float Snackbar::getDuration(void) const noexcept
	{
		return _duration;
	}

	Snackbar &Snackbar::setDuration(float seconds)
	{
		if (seconds < 0.0f) {
			throw std::invalid_argument("Snackbar duration must be >= 0");
		}
		if (_duration != seconds) {
			_duration = seconds;
			setHasChanged(true);
		}
		return *this;
	}

	const Snackbar::Handler &Snackbar::getOnDismiss(void) const noexcept
	{
		return _onDismiss;
	}

	Snackbar &Snackbar::setOnDismiss(Handler handler)
	{
		_onDismiss = std::move(handler);
		return *this;
	}

	const Snackbar::Handler &Snackbar::getOnAction(void) const noexcept
	{
		return _onAction;
	}

	Snackbar &Snackbar::setOnAction(Handler handler)
	{
		_onAction = std::move(handler);
		return *this;
	}
}	 // namespace guillaume::components
