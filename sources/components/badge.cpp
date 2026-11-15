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

#include "guillaume/components/badge.hpp"

#include <utility>

namespace guillaume::components
{
	Badge::Variant Badge::getVariant(void) const noexcept
	{
		return _variant;
	}

	Badge &Badge::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::string &Badge::getLabel(void) const noexcept
	{
		return _label;
	}

	Badge &Badge::setLabel(const std::string &label)
	{
		if (_label != label) {
			_label = label;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Badge::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	Badge &Badge::setLabelEntity(ecs::Entity::Identifier identifier)
	{
		if (_labelEntity != identifier) {
			_labelEntity = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<float> &Badge::getSize(void) const noexcept
	{
		return _size;
	}

	Badge &Badge::setSize(std::optional<float> size)
	{
		if (_size != size) {
			_size = size;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<utility::graphic::Color32Bit> &
		Badge::getContainerColor(void) const noexcept
	{
		return _containerColor;
	}

	Badge &Badge::setContainerColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		if (_containerColor != color) {
			_containerColor = color;
			setHasChanged(true);
		}
		return *this;
	}

	const std::optional<utility::graphic::Color32Bit> &
		Badge::getContentColor(void) const noexcept
	{
		return _contentColor;
	}

	Badge &Badge::setContentColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		if (_contentColor != color) {
			_contentColor = color;
			setHasChanged(true);
		}
		return *this;
	}

	bool Badge::isVisible(void) const noexcept
	{
		return _visible;
	}

	Badge &Badge::setVisible(bool visible)
	{
		if (_visible != visible) {
			_visible = visible;
			setHasChanged(true);
		}
		return *this;
	}

	float Badge::getOpacity(void) const noexcept
	{
		return _opacity;
	}

	Badge &Badge::setOpacity(float opacity)
	{
		if (_opacity != opacity) {
			_opacity = opacity;
			setHasChanged(true);
		}
		return *this;
	}

	float Badge::getFadeFrom(void) const noexcept
	{
		return _fadeFrom;
	}

	Badge &Badge::setFadeFrom(float opacity)
	{
		if (_fadeFrom != opacity) {
			_fadeFrom = opacity;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
