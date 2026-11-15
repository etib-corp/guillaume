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

#include "guillaume/components/text_field_configuration.hpp"

#include <algorithm>
#include <utility>

namespace guillaume::components
{
	TextFieldConfiguration::Variant
		TextFieldConfiguration::getVariant(void) const noexcept
	{
		return _variant;
	}

	TextFieldConfiguration &TextFieldConfiguration::setVariant(Variant variant)
	{
		if (_variant == variant) {
			return *this;
		}
		_variant = variant;
		setHasChanged(true);
		return *this;
	}

	const std::string &
		TextFieldConfiguration::getLeadingIcon(void) const noexcept
	{
		return _leadingIcon;
	}

	TextFieldConfiguration &
		TextFieldConfiguration::setLeadingIcon(const std::string &glyph)
	{
		if (_leadingIcon == glyph) {
			return *this;
		}
		_leadingIcon = glyph;
		setHasChanged(true);
		return *this;
	}

	const std::string &
		TextFieldConfiguration::getTrailingIcon(void) const noexcept
	{
		return _trailingIcon;
	}

	TextFieldConfiguration &
		TextFieldConfiguration::setTrailingIcon(const std::string &glyph)
	{
		if (_trailingIcon == glyph) {
			return *this;
		}
		_trailingIcon = glyph;
		setHasChanged(true);
		return *this;
	}

	const std::string &TextFieldConfiguration::getText(void) const noexcept
	{
		return _text;
	}

	TextFieldConfiguration &
		TextFieldConfiguration::setText(const std::string &text)
	{
		if (_text == text) {
			return *this;
		}
		_text = text;
		setHasChanged(true);
		return *this;
	}

	bool TextFieldConfiguration::isEditable(void) const noexcept
	{
		return _editable;
	}

	TextFieldConfiguration &TextFieldConfiguration::setEditable(bool editable)
	{
		if (_editable == editable) {
			return *this;
		}
		_editable = editable;
		setHasChanged(true);
		return *this;
	}

	bool TextFieldConfiguration::isDense(void) const noexcept
	{
		return _dense;
	}

	TextFieldConfiguration &TextFieldConfiguration::setDense(bool dense)
	{
		if (_dense == dense) {
			return *this;
		}
		_dense = dense;
		setHasChanged(true);
		return *this;
	}

	bool TextFieldConfiguration::isMultiline(void) const noexcept
	{
		return _multiline;
	}

	TextFieldConfiguration &TextFieldConfiguration::setMultiline(bool multiline)
	{
		if (_multiline == multiline) {
			return *this;
		}
		_multiline = multiline;
		setHasChanged(true);
		return *this;
	}

	const std::optional<float> &
		TextFieldConfiguration::getWidth(void) const noexcept
	{
		return _width;
	}

	TextFieldConfiguration &
		TextFieldConfiguration::setWidth(std::optional<float> width)
	{
		if (width.has_value()) {
			width = std::max(MinWidth, *width);
		}
		if (_width == width) {
			return *this;
		}
		_width = width;
		setHasChanged(true);
		return *this;
	}

	const TextFieldConfiguration::ChangeHandler &
		TextFieldConfiguration::getOnChange(void) const noexcept
	{
		return _onChange;
	}

	TextFieldConfiguration &
		TextFieldConfiguration::setOnChange(ChangeHandler handler)
	{
		_onChange = std::move(handler);
		return *this;
	}

	const TextFieldConfiguration::Handler &
		TextFieldConfiguration::getOnFocus(void) const noexcept
	{
		return _onFocus;
	}

	TextFieldConfiguration &TextFieldConfiguration::setOnFocus(Handler handler)
	{
		_onFocus = std::move(handler);
		return *this;
	}

	const TextFieldConfiguration::Handler &
		TextFieldConfiguration::getOnBlur(void) const noexcept
	{
		return _onBlur;
	}

	TextFieldConfiguration &TextFieldConfiguration::setOnBlur(Handler handler)
	{
		_onBlur = std::move(handler);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getLeadingIconEntity(void) const noexcept
	{
		return _leadingIconEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setLeadingIconEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_leadingIconEntity == identifier) {
			return *this;
		}
		_leadingIconEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getTrailingIconEntity(void) const noexcept
	{
		return _trailingIconEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setTrailingIconEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_trailingIconEntity == identifier) {
			return *this;
		}
		_trailingIconEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getErrorIconEntity(void) const noexcept
	{
		return _errorIconEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setErrorIconEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_errorIconEntity == identifier) {
			return *this;
		}
		_errorIconEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getInputColumn(void) const noexcept
	{
		return _inputColumn;
	}

	TextFieldConfiguration &TextFieldConfiguration::setInputColumn(
		ecs::Entity::Identifier identifier)
	{
		if (_inputColumn == identifier) {
			return *this;
		}
		_inputColumn = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getLabelEntity(void) const noexcept
	{
		return _labelEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setLabelEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_labelEntity == identifier) {
			return *this;
		}
		_labelEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getInputEntity(void) const noexcept
	{
		return _inputEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setInputEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_inputEntity == identifier) {
			return *this;
		}
		_inputEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getPrefixEntity(void) const noexcept
	{
		return _prefixEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setPrefixEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_prefixEntity == identifier) {
			return *this;
		}
		_prefixEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getSuffixEntity(void) const noexcept
	{
		return _suffixEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setSuffixEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_suffixEntity == identifier) {
			return *this;
		}
		_suffixEntity = identifier;
		setHasChanged(true);
		return *this;
	}

	ecs::Entity::Identifier
		TextFieldConfiguration::getSupportingEntity(void) const noexcept
	{
		return _supportingEntity;
	}

	TextFieldConfiguration &TextFieldConfiguration::setSupportingEntity(
		ecs::Entity::Identifier identifier)
	{
		if (_supportingEntity == identifier) {
			return *this;
		}
		_supportingEntity = identifier;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
