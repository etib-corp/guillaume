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

#include "guillaume/components/progress_indicator.hpp"

namespace guillaume::components
{
	ProgressIndicator::Variant
		ProgressIndicator::getVariant(void) const noexcept
	{
		return _variant;
	}

	ProgressIndicator &ProgressIndicator::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	bool ProgressIndicator::isCircular(void) const noexcept
	{
		return _variant == Variant::CircularDeterminate
			|| _variant == Variant::CircularIndeterminate;
	}

	bool ProgressIndicator::isIndeterminate(void) const noexcept
	{
		return _variant == Variant::LinearIndeterminate
			|| _variant == Variant::CircularIndeterminate;
	}

	float ProgressIndicator::getTrackLength(void) const noexcept
	{
		return _trackLength;
	}

	ProgressIndicator &ProgressIndicator::setTrackLength(float length)
	{
		if (_trackLength != length) {
			_trackLength = length;
			setHasChanged(true);
		}
		return *this;
	}

	float ProgressIndicator::getDiameter(void) const noexcept
	{
		return _diameter;
	}

	ProgressIndicator &ProgressIndicator::setDiameter(float diameter)
	{
		if (_diameter != diameter) {
			_diameter = diameter;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier ProgressIndicator::getTrack(void) const noexcept
	{
		return _track;
	}

	ProgressIndicator &
		ProgressIndicator::setTrack(ecs::Entity::Identifier identifier)
	{
		if (_track != identifier) {
			_track = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier ProgressIndicator::getFill(void) const noexcept
	{
		return _fill;
	}

	ProgressIndicator &
		ProgressIndicator::setFill(ecs::Entity::Identifier identifier)
	{
		if (_fill != identifier) {
			_fill = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	bool ProgressIndicator::isBuilt(void) const noexcept
	{
		return _built;
	}

	ProgressIndicator &ProgressIndicator::setBuilt(bool built)
	{
		if (_built != built) {
			_built = built;
			setHasChanged(true);
		}
		return *this;
	}

	float ProgressIndicator::getDisplayed(void) const noexcept
	{
		return _displayed;
	}

	ProgressIndicator &ProgressIndicator::setDisplayed(float displayed)
	{
		if (_displayed != displayed) {
			_displayed = displayed;
			setHasChanged(true);
		}
		return *this;
	}

	float ProgressIndicator::getValueFrom(void) const noexcept
	{
		return _valueFrom;
	}

	ProgressIndicator &ProgressIndicator::setValueFrom(float value)
	{
		if (_valueFrom != value) {
			_valueFrom = value;
			setHasChanged(true);
		}
		return *this;
	}

	bool ProgressIndicator::isVisible(void) const noexcept
	{
		return _visible;
	}

	ProgressIndicator &ProgressIndicator::setVisible(bool visible)
	{
		if (_visible != visible) {
			_visible = visible;
			setHasChanged(true);
		}
		return *this;
	}

	float ProgressIndicator::getOpacity(void) const noexcept
	{
		return _opacity;
	}

	ProgressIndicator &ProgressIndicator::setOpacity(float opacity)
	{
		if (_opacity != opacity) {
			_opacity = opacity;
			setHasChanged(true);
		}
		return *this;
	}

	float ProgressIndicator::getOpacityFrom(void) const noexcept
	{
		return _opacityFrom;
	}

	ProgressIndicator &ProgressIndicator::setOpacityFrom(float opacity)
	{
		if (_opacityFrom != opacity) {
			_opacityFrom = opacity;
			setHasChanged(true);
		}
		return *this;
	}

	float ProgressIndicator::getCycle(void) const noexcept
	{
		return _cycle;
	}

	ProgressIndicator &ProgressIndicator::setCycle(float cycle)
	{
		if (_cycle != cycle) {
			_cycle = cycle;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<utility::graphic::Color32Bit> &
		ProgressIndicator::getIndicatorColor(void) const noexcept
	{
		return _indicatorColor;
	}

	ProgressIndicator &ProgressIndicator::setIndicatorColor(
		const std::optional<utility::graphic::Color32Bit> &color)
	{
		if (_indicatorColor != color) {
			_indicatorColor = color;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
