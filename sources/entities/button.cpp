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

#include <algorithm>
#include <cctype>
#include <memory>
#include <utility>

#include "guillaume/entities/button.hpp"
#include "guillaume/entities/button_style.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/theme.hpp"
#include "guillaume/systems/common_button.hpp"

namespace guillaume::entities
{
	Button::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Button>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Button> Button::Builder::buildEntity(void)
	{
		auto entity = Button::create(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _isToggle, _colorStyle, _shape, _size, _isMorph,
			_isDisabled, _isTrailingIcon, _accessibilityLabel, _onClick);
		return entity;
	}

	void Button::Builder::reset(void)
	{
		_iconGlyphName.clear();
		_iconStyle = components::Glyph::Style::Outlined;
		_labelContent.clear();
		_isToggle		= false;
		_colorStyle		= Color::Text;
		_shape			= Shape::Round;
		_size			= Size::Small;
		_isMorph		= false;
		_isDisabled		= false;
		_isTrailingIcon = false;
		_accessibilityLabel.clear();
		_onClick = {};
	}

	Button::Builder &Button::Builder::withIcon(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;
		return *this;
	}

	Button::Builder &Button::Builder::withIconStyle(
		const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;
		return *this;
	}

	Button::Builder &Button::Builder::withLabel(const std::string &labelContent)
	{
		_labelContent = labelContent;
		return *this;
	}

	Button::Builder &
		Button::Builder::withOnClick(std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	Button::Builder &Button::Builder::withToggle(const bool &isToggle)
	{
		_isToggle = isToggle;
		return *this;
	}

	Button::Builder &Button::Builder::withColorStyle(const Color &colorStyle)
	{
		_colorStyle = colorStyle;
		return *this;
	}

	Button::Builder &Button::Builder::withShape(const Shape &shape)
	{
		_shape = shape;
		return *this;
	}

	Button::Builder &Button::Builder::withSize(const Size &size)
	{
		_size = size;
		return *this;
	}

	Button::Builder &Button::Builder::withMorph(const bool &isMorph)
	{
		_isMorph = isMorph;
		return *this;
	}

	Button::Builder &Button::Builder::withDisabled(const bool &isDisabled)
	{
		_isDisabled = isDisabled;
		return *this;
	}

	Button::Builder &
		Button::Builder::withTrailingIcon(const bool &isTrailingIcon)
	{
		_isTrailingIcon = isTrailingIcon;
		return *this;
	}

	Button::Builder &Button::Builder::withAccessibilityLabel(
		const std::string &accessibilityLabel)
	{
		_accessibilityLabel = accessibilityLabel;
		return *this;
	}

	std::shared_ptr<Button> Button::Builder::makeButton(
		std::shared_ptr<Entity> parent, const std::string &labelContent,
		std::function<void(void)> onClick, Color colorStyle, Shape shape,
		Size size, bool isMorph)
	{
		return withLabel(labelContent)
			.withOnClick(onClick)
			.withColorStyle(colorStyle)
			.withShape(shape)
			.withSize(size)
			.withMorph(isMorph)
			.registerEntity(parent);
	}

	std::shared_ptr<Button> Button::Builder::makeIconButton(
		std::shared_ptr<Entity> parent, const std::string &labelContent,
		const std::string &iconGlyphName,
		const components::Glyph::Style &iconStyle,
		std::function<void(void)> onClick, Color colorStyle, Shape shape,
		Size size, bool isMorph)
	{
		return withIcon(iconGlyphName)
			.withIconStyle(iconStyle)
			.withLabel(labelContent)
			.withOnClick(onClick)
			.withColorStyle(colorStyle)
			.withShape(shape)
			.withSize(size)
			.withMorph(isMorph)
			.registerEntity(parent);
	}

	Button::Button(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const components::Glyph::Style &iconStyle,
				   const std::string &labelContent, bool isToggle,
				   Color colorStyle, Shape shape, Size size, bool isMorph,
				   bool isDisabled, bool isTrailingIcon,
				   const std::string &accessibilityLabel,
				   std::function<void(void)> onClick)
		: ButtonBase(registry, iconGlyphName, iconStyle, labelContent, isToggle,
					 isMorph, isDisabled, isTrailingIcon, accessibilityLabel,
					 std::move(onClick))
	{
		getComponentRegistry()
			.addComponent<components::CommonButton>(getIdentifier())
			.setColorStyle(colorStyle)
			.setShape(shape)
			.setSize(size);
		setSignature(getSignature()
					 | getSignatureFromTypes<components::CommonButton>());
		systems::CommonButton::install(getComponentRegistry(), getIdentifier());
	}

	float Button::getIconFontSize(void) const
	{
		return systems::CommonButton::iconFontSize(getComponentRegistry(),
												   getIdentifier());
	}

	utility::graphic::Color32Bit Button::getInitialContentColor(void) const
	{
		return systems::CommonButton::contentColor(getComponentRegistry(),
												   getIdentifier());
	}

	Button &Button::setMinWidth(float minWidth)
	{
		getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.setMinWidth(minWidth);
		restyle();
		return *this;
	}

	Button &Button::setCompact(bool compact)
	{
		getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.setCompact(compact);
		restyle();
		return *this;
	}

	Button &Button::setUppercase(bool uppercase)
	{
		getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.setUppercase(uppercase);
		restyle();
		return *this;
	}

	Button &Button::setColorStyle(const Color &colorStyle)
	{
		getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.setColorStyle(colorStyle);
		restyle();
		return *this;
	}

	Button &Button::setShape(const Shape &shape)
	{
		getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.setShape(shape);
		restyle();
		return *this;
	}

	Button &Button::setSize(const Size &size)
	{
		getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.setSize(size);
		restyle();
		return *this;
	}

	bool Button::isCompact(void) const
	{
		return getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.isCompact();
	}

	bool Button::isUppercase(void) const
	{
		return getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.isUppercase();
	}

	Button::Color Button::getColorStyle(void) const
	{
		return getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.getColorStyle();
	}

	Button::Shape Button::getShape(void) const
	{
		return getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.getShape();
	}

	Button::Size Button::getSize(void) const
	{
		return getComponentRegistry()
			.getComponent<components::CommonButton>(getIdentifier())
			.getSize();
	}

}	 // namespace guillaume::entities
