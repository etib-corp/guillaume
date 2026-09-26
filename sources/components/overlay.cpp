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

#include "guillaume/components/overlay.hpp"

namespace guillaume::components
{
	bool Overlay::isVisible(void) const
	{
		return _visible;
	}

	Overlay &Overlay::setVisible(bool visible)
	{
		if (_visible == visible) {
			return *this;
		}
		_visible = visible;
		if (!_visible) {
			_elapsed = 0.0f;
		}
		setHasChanged(true);
		return *this;
	}

	bool Overlay::isModal(void) const
	{
		return _modal;
	}

	Overlay &Overlay::setModal(bool modal)
	{
		if (_modal == modal) {
			return *this;
		}
		_modal = modal;
		setHasChanged(true);
		return *this;
	}

	bool Overlay::dismissesOnOutsideClick(void) const
	{
		return _dismissOnOutsideClick;
	}

	Overlay &Overlay::setDismissOnOutsideClick(bool dismiss)
	{
		if (_dismissOnOutsideClick == dismiss) {
			return *this;
		}
		_dismissOnOutsideClick = dismiss;
		setHasChanged(true);
		return *this;
	}

	bool Overlay::dismissesOnEscape(void) const
	{
		return _dismissOnEscape;
	}

	Overlay &Overlay::setDismissOnEscape(bool dismiss)
	{
		if (_dismissOnEscape == dismiss) {
			return *this;
		}
		_dismissOnEscape = dismiss;
		setHasChanged(true);
		return *this;
	}

	float Overlay::getAutoDismiss(void) const
	{
		return _autoDismiss;
	}

	Overlay &Overlay::setAutoDismiss(float delay)
	{
		if (_autoDismiss == delay) {
			return *this;
		}
		_autoDismiss = delay;
		setHasChanged(true);
		return *this;
	}

	bool Overlay::hasAutoDismiss(void) const
	{
		return _autoDismiss > 0.0f;
	}

	float Overlay::getElapsed(void) const
	{
		return _elapsed;
	}

	Overlay &Overlay::setElapsed(float elapsed)
	{
		_elapsed = elapsed;
		return *this;
	}

	Overlay &Overlay::setOnDismissHandler(const DismissHandler &handler)
	{
		_onDismiss = handler;
		return *this;
	}

	Overlay::DismissHandler Overlay::getOnDismissHandler(void) const
	{
		return _onDismiss;
	}
}	 // namespace guillaume::components
