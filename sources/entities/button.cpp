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
		auto entity = std::make_shared<Button>(
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

	std::shared_ptr<Button> Button::Director::makeButton(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &labelContent, std::function<void(void)> onClick,
		Color colorStyle, Shape shape, Size size, bool isMorph)
	{
		return builder.withLabel(labelContent)
			.withOnClick(onClick)
			.withColorStyle(colorStyle)
			.withShape(shape)
			.withSize(size)
			.withMorph(isMorph)
			.registerEntity(parent);
	}

	std::shared_ptr<Button> Button::Director::makeIconButton(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &labelContent, const std::string &iconGlyphName,
		const components::Glyph::Style &iconStyle,
		std::function<void(void)> onClick, Color colorStyle, Shape shape,
		Size size, bool isMorph)
	{
		return builder.withIcon(iconGlyphName)
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
		, _colorStyle(colorStyle)
		, _shape(shape)
		, _size(size)
	{
		if (!getComponentRegistry().hasComponent<components::Elevation>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Elevation>(
				getIdentifier());
		}
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(theme::ElevationLevel::Level0);
	}

	Button::~Button(void)
	{
	}

	float Button::getIconFontSize(void) const
	{
		return button_style::getIconFontSize(_size);
	}

	utility::graphic::Color32Bit Button::getInitialContentColor(void) const
	{
		return button_style::getContentColor(_colorStyle, _isToggle,
											 _isSelected);
	}

	void Button::applyStyleState(void)
	{
		// M3 elevated buttons rest at level 1 and lift to level 2 on hover;
		// every other style, and any disabled button, is flat.
		auto level = theme::ElevationLevel::Level0;
		if (_colorStyle == Color::Elevated && !isDisabled()) {
			level = (isHovered() && !isPressed())
				? theme::ElevationLevel::Level2
				: theme::ElevationLevel::Level1;
		}
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(level);

		if (isDisabled()) {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(getDisabledContainerColor());

			getComponentRegistry()
				.getComponent<components::Borders>(getIdentifier())
				.setColor(getDisabledBorderColor())
				.setWidth(1.0f);

			if (_icon) {
				_icon->setColor(getDisabledContentColor());
			}

			if (_label) {
				_label->setColor(getDisabledContentColor());
			}

			return;
		}

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(resolveContainerColor(button_style::getContainerColor(
				_colorStyle, _isToggle, _isSelected, isHovered(),
				isPressed())));

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(button_style::getBorderColor(_colorStyle, _isToggle,
												   _isSelected))
			.setWidth(1.0f);

		if (_icon) {
			_icon->setColor(resolveContentColor(button_style::getContentColor(
				_colorStyle, _isToggle, _isSelected)));
		}

		if (_label) {
			_label->setColor(resolveContentColor(button_style::getContentColor(
				_colorStyle, _isToggle, _isSelected)));
		}
	}

	void Button::applyGeometry(void)
	{
		const float radius = button_style::getBorderRadius(
			_size,
			button_style::getRestingShape(_shape, _isToggle, _isSelected,
										  _isMorph),
			isPressed());

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(radius);

		if (!_icon && !_label) {
			return;
		}

		const float fontHeight = getIconFontSize();

		if (_icon) {
			_icon->setFontSize(fontHeight);
		}

		if (_label) {
			_label->setTypeRole(theme::TypeRole::LabelLarge);
			std::string content = _labelContent;
			if (_isUppercase) {
				std::transform(content.begin(), content.end(), content.begin(),
							   [](unsigned char character) {
								   return static_cast<char>(
									   std::toupper(character));
							   });
			}
			_label->setContent(content);
		}

		const auto &buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		const bool hasIcon	= _icon != nullptr;
		const bool hasLabel = _label != nullptr;

		const float iconWidth	= hasIcon
			  ? getComponentRegistry()
					.getComponent<components::Bound>(_icon->getIdentifier())
					.getWidth()
			  : 0.0f;
		const float iconHeight	= hasIcon
			 ? getComponentRegistry()
				   .getComponent<components::Bound>(_icon->getIdentifier())
				   .getHeight()
			 : 0.0f;
		const float labelWidth	= hasLabel
			 ? getComponentRegistry()
				   .getComponent<components::Bound>(_label->getIdentifier())
				   .getWidth()
			 : 0.0f;
		const float labelHeight = hasLabel
			? getComponentRegistry()
				  .getComponent<components::Bound>(_label->getIdentifier())
				  .getHeight()
			: 0.0f;

		const float gap = (!_iconGlyphName.empty() && !_labelContent.empty())
			? button_style::getSpaceBetweenIconAndLabel(_size)
			: 0.0f;
		const float contentWidth  = iconWidth + gap + labelWidth;
		const float contentHeight = std::max(iconHeight, labelHeight);

		// The label typography (labelLarge) is independent of the icon size;
		// the container grows to the larger child and to the M3 minimums.
		const float minHeight = (_size == Size::ExtraSmall) ? 0.0f : MinHeight;
		const float buttonWidth = std::max(
			{ contentWidth
				  + button_style::getWidthPadding(_size, _isCompact) * 2.0f,
			  MinWidth, _minWidth });
		const float buttonHeight = std::max(
			contentHeight + button_style::getHeightPadding(_size) * 2.0f,
			minHeight);

		const auto place = [&](const std::shared_ptr<ecs::Entity> &child,
							   float x, float childHeight) {
			const float y = (buttonHeight - childHeight) / 2.0f;
			getComponentRegistry()
				.getComponent<components::Transform>(child->getIdentifier())
				.setPose(ButtonBase::applyLayerToPosition(
					utility::graphic::PositionF(buttonPose.getPosition().x + x,
												buttonPose.getPosition().y + y,
												buttonPose.getPosition().z),
					buttonPose.getOrientation(),
					static_cast<std::uint32_t>(getLayer())));
		};

		float cursor = (buttonWidth - contentWidth) / 2.0f;
		if (_isTrailingIcon) {
			if (hasLabel) {
				place(_label, cursor, labelHeight);
				cursor += labelWidth + gap;
			}
			if (hasIcon) {
				place(_icon, cursor, iconHeight);
			}
		} else {
			if (hasIcon) {
				place(_icon, cursor, iconHeight);
				cursor += iconWidth + gap;
			}
			if (hasLabel) {
				place(_label, cursor, labelHeight);
			}
		}

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(buttonWidth)
			.setHeight(buttonHeight);
		applyTouchTarget(buttonWidth, buttonHeight);
	}

	Button &Button::setMinWidth(float minWidth)
	{
		_minWidth = minWidth;
		applyGeometry();
		return *this;
	}

	Button &Button::setCompact(bool compact)
	{
		_isCompact = compact;
		applyGeometry();
		return *this;
	}

	bool Button::isCompact(void) const
	{
		return _isCompact;
	}

	Button &Button::setUppercase(bool uppercase)
	{
		_isUppercase = uppercase;
		applyGeometry();
		return *this;
	}

	bool Button::isUppercase(void) const
	{
		return _isUppercase;
	}

	Button &Button::setColorStyle(const Color &colorStyle)
	{
		_colorStyle = colorStyle;

		applyStyleState();

		return *this;
	}

	Button &Button::setShape(const Shape &shape)
	{
		_shape = shape;

		applyStyleState();

		return *this;
	}

	Button &Button::setSize(const Size &size)
	{
		_size = size;

		applyGeometry();

		return *this;
	}

	Button::Color Button::getColorStyle(void) const
	{
		return _colorStyle;
	}

	Button::Shape Button::getShape(void) const
	{
		return _shape;
	}

	Button::Size Button::getSize(void) const
	{
		return _size;
	}
}	 // namespace guillaume::entities
