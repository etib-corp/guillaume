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

#include "guillaume/components/loading_indicator.hpp"

namespace guillaume::components
{
	LoadingIndicator::Variant LoadingIndicator::getVariant(void) const noexcept
	{
		return _variant;
	}

	LoadingIndicator &LoadingIndicator::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	float LoadingIndicator::getSize(void) const noexcept
	{
		return _size;
	}

	LoadingIndicator &LoadingIndicator::setSize(float size)
	{
		if (_size != size) {
			_size = size;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier LoadingIndicator::getIndicator(void) const noexcept
	{
		return _indicator;
	}

	LoadingIndicator &
		LoadingIndicator::setIndicator(ecs::Entity::Identifier identifier)
	{
		if (_indicator != identifier) {
			_indicator = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	bool LoadingIndicator::isBuilt(void) const noexcept
	{
		return _built;
	}

	LoadingIndicator &LoadingIndicator::setBuilt(bool built)
	{
		if (_built != built) {
			_built = built;
			setHasChanged(true);
		}
		return *this;
	}

	float LoadingIndicator::getProgress(void) const noexcept
	{
		return _progress;
	}

	LoadingIndicator &LoadingIndicator::setProgress(float progress)
	{
		if (_progress != progress) {
			_progress = progress;
			setHasChanged(true);
		}
		return *this;
	}

	bool LoadingIndicator::hidesWhenStopped(void) const noexcept
	{
		return _hidesWhenStopped;
	}

	LoadingIndicator &LoadingIndicator::setHidesWhenStopped(bool hides)
	{
		if (_hidesWhenStopped != hides) {
			_hidesWhenStopped = hides;
			setHasChanged(true);
		}
		return *this;
	}

	float LoadingIndicator::getOpacity(void) const noexcept
	{
		return _opacity;
	}

	LoadingIndicator &LoadingIndicator::setOpacity(float opacity)
	{
		if (_opacity != opacity) {
			_opacity = opacity;
			setHasChanged(true);
		}
		return *this;
	}

	float LoadingIndicator::getOpacityFrom(void) const noexcept
	{
		return _opacityFrom;
	}

	LoadingIndicator &LoadingIndicator::setOpacityFrom(float opacity)
	{
		if (_opacityFrom != opacity) {
			_opacityFrom = opacity;
			setHasChanged(true);
		}
		return *this;
	}

	float LoadingIndicator::getOpacityTarget(void) const noexcept
	{
		return _opacityTarget;
	}

	LoadingIndicator &LoadingIndicator::setOpacityTarget(float opacity)
	{
		if (_opacityTarget != opacity) {
			_opacityTarget = opacity;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<utility::graphic::Color32Bit> &
		LoadingIndicator::getIndicatorColor(void) const noexcept
	{
		return _indicatorColor;
	}

	LoadingIndicator &LoadingIndicator::setIndicatorColor(
		const std::optional<utility::graphic::Color32Bit> &color)
	{
		if (_indicatorColor != color) {
			_indicatorColor = color;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
