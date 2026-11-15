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

#include "guillaume/components/tooltip.hpp"

#include <stdexcept>

namespace guillaume::components
{
	Tooltip::Variant Tooltip::getVariant(void) const noexcept
	{
		return _variant;
	}

	Tooltip &Tooltip::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Tooltip::getText(void) const noexcept
	{
		return _text;
	}

	Tooltip &Tooltip::setText(const std::string &text)
	{
		if (_text != text) {
			_text = text;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Tooltip::getTitle(void) const noexcept
	{
		return _title;
	}

	Tooltip &Tooltip::setTitle(const std::string &title)
	{
		if (_title != title) {
			_title = title;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Tooltip::getTitleEntity(void) const noexcept
	{
		return _titleEntity;
	}

	Tooltip &Tooltip::setTitleEntity(ecs::Entity::Identifier identifier)
	{
		if (_titleEntity != identifier) {
			_titleEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Tooltip::getTextEntity(void) const noexcept
	{
		return _textEntity;
	}

	Tooltip &Tooltip::setTextEntity(ecs::Entity::Identifier identifier)
	{
		if (_textEntity != identifier) {
			_textEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Tooltip::getAnchor(void) const noexcept
	{
		return _anchor;
	}

	Tooltip &Tooltip::setAnchor(ecs::Entity::Identifier identifier)
	{
		if (_anchor != identifier) {
			_anchor = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	float Tooltip::getEnterDelay(void) const noexcept
	{
		return _enterDelay;
	}

	Tooltip &Tooltip::setEnterDelay(float seconds)
	{
		if (seconds < 0.0f) {
			throw std::invalid_argument("Tooltip delay must be >= 0");
		}
		if (_enterDelay != seconds) {
			_enterDelay = seconds;
			setHasChanged(true);
		}
		return *this;
	}

	float Tooltip::getLeaveDelay(void) const noexcept
	{
		return _leaveDelay;
	}

	Tooltip &Tooltip::setLeaveDelay(float seconds)
	{
		if (seconds < 0.0f) {
			throw std::invalid_argument("Tooltip delay must be >= 0");
		}
		if (_leaveDelay != seconds) {
			_leaveDelay = seconds;
			setHasChanged(true);
		}
		return *this;
	}

	float Tooltip::getHoverTime(void) const noexcept
	{
		return _hoverTime;
	}

	Tooltip &Tooltip::setHoverTime(float seconds)
	{
		if (_hoverTime != seconds) {
			_hoverTime = seconds;
			setHasChanged(true);
		}
		return *this;
	}

	float Tooltip::getLeaveTime(void) const noexcept
	{
		return _leaveTime;
	}

	Tooltip &Tooltip::setLeaveTime(float seconds)
	{
		if (_leaveTime != seconds) {
			_leaveTime = seconds;
			setHasChanged(true);
		}
		return *this;
	}

	float Tooltip::getLastProgress(void) const noexcept
	{
		return _lastProgress;
	}

	Tooltip &Tooltip::setLastProgress(float progress)
	{
		if (_lastProgress != progress) {
			_lastProgress = progress;
			setHasChanged(true);
		}
		return *this;
	}

	bool Tooltip::isVisible(void) const noexcept
	{
		return _visible;
	}

	Tooltip &Tooltip::setVisible(bool visible)
	{
		if (_visible != visible) {
			_visible = visible;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<Tooltip::Viewport> &
		Tooltip::getViewport(void) const noexcept
	{
		return _viewport;
	}

	Tooltip &Tooltip::setViewport(std::optional<Viewport> viewport)
	{
		if (_viewport != viewport) {
			_viewport = viewport;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
