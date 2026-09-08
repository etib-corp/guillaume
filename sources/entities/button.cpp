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

#include <utility>

#include "guillaume/entities/button.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Button::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Button::Builder::~Builder(void)
	{
	}

	std::shared_ptr<Button>
		Button::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_button = std::make_shared<Button>(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _isToggle, _colorStyle, _shape, _size, _isMorph,
			_isDisabled, _isTrailingIcon, _accessibilityLabel, _onClick);
		_button->setParent(parent);

		this->getEntityRegistry().addEntity(_button);

		auto buttonCopy = _button;

		reset();

		return buttonCopy;
	}

	void Button::Builder::reset(void)
	{
		_button.reset();
		_iconGlyphName.clear();
		_iconStyle = components::Glyph::Style::Outlined;
		_labelContent.clear();
		_isToggle		= false;
		_colorStyle		= Color::Filled;
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

	Button::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Button::Director::~Director(void)
	{
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
	}

	Button::~Button(void)
	{
	}

	static float getHeightPadding(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 6.0f;
			case Button::Size::Small:
				return 8.0f;
			case Button::Size::Medium:
				return 12.0f;
			case Button::Size::Large:
				return 24.0f;
			case Button::Size::ExtraLarge:
				return 32.0f;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static float getBorderRadius(Button::Size size, Button::Shape shape,
								 bool isButtonPressed)
	{
		if (isButtonPressed) {
			switch (size) {
				case Button::Size::ExtraSmall:
				case Button::Size::Small:
					return 8.0f;
				case Button::Size::Medium:
					return 12.0f;
				case Button::Size::Large:
				case Button::Size::ExtraLarge:
					return 16.0f;
				default:
					throw std::runtime_error("Invalid button size");
			}
		}

		if (shape == Button::Shape::Round) {
			return 100.0f;
		}

		switch (size) {
			case Button::Size::ExtraSmall:
			case Button::Size::Small:
				return 12.0f;
			case Button::Size::Medium:
				return 16.0f;
			case Button::Size::Large:
			case Button::Size::ExtraLarge:
				return 28.0f;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static Button::Shape getRestingShape(Button::Shape baseShape, bool isToggle,
										 bool isSelected, bool isMorph)
	{
		if (!isMorph || !isToggle || !isSelected) {
			return baseShape;
		}

		return (baseShape == Button::Shape::Round) ? Button::Shape::Square
												   : Button::Shape::Round;
	}

	float Button::getIconFontSize(void) const
	{
		switch (_size) {
			case Button::Size::ExtraSmall:
				return 15.0f;
			case Button::Size::Small:
				return 15.0f;
			case Button::Size::Medium:
				return 18.0f;
			case Button::Size::Large:
				return 24.0f;
			case Button::Size::ExtraLarge:
				return 30.0f;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static float getWidthPadding(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 12.0f;
			case Button::Size::Small:
				return 16.0f;
			case Button::Size::Medium:
				return 24.0f;
			case Button::Size::Large:
				return 48.0f;
			case Button::Size::ExtraLarge:
				return 64.0f;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static float getSpaceBetweenIconAndLabel(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 4.0f;
			case Button::Size::Small:
				return 8.0f;
			case Button::Size::Medium:
				return 8.0f;
			case Button::Size::Large:
				return 12.0f;
			case Button::Size::ExtraLarge:
				return 16.0f;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static utility::graphic::Color32Bit
		getDefaultContainerColor(Button::Color style)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (style) {
			case Button::Color::Elevated:
				return scheme.getColor(SchemeColorRole::SurfaceContainerLow)
					.getColor();

			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::Primary).getColor();

			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();

			case Button::Color::Outlined:
			case Button::Color::Text:
			case Button::Color::Segmented:
				return utility::graphic::Color32Bit(0, 0, 0, 0);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getToggleUnselectedContainerColor(Button::Color style)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (style) {
			case Button::Color::Elevated:
				return scheme.getColor(SchemeColorRole::SurfaceContainerLow)
					.getColor();

			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::SurfaceContainer)
					.getColor();

			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();

			case Button::Color::Outlined:
			case Button::Color::Text:
			case Button::Color::Segmented:
				return utility::graphic::Color32Bit(0, 0, 0, 0);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getToggleSelectedContainerColor(Button::Color style)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (style) {
			case Button::Color::Elevated:
			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::Primary).getColor();

			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::Secondary).getColor();

			case Button::Color::Outlined:
			case Button::Color::Segmented:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();

			case Button::Color::Text:
				return utility::graphic::Color32Bit(0, 0, 0, 0);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getContentColor(Button::Color style, bool isToggle, bool isSelected);

	static utility::graphic::Color32Bit
		getBaseContainerColor(Button::Color style, bool isToggle,
							  bool isSelected)
	{
		if (!isToggle) {
			return getDefaultContainerColor(style);
		}

		if (isSelected) {
			return getToggleSelectedContainerColor(style);
		}

		return getToggleUnselectedContainerColor(style);
	}

	static utility::graphic::Color32Bit
		getPressedContainerColor(Button::Color style, bool isToggle,
								 bool isSelected)
	{
		return ButtonBase::applyStateLayer(
			getBaseContainerColor(style, isToggle, isSelected),
			getContentColor(style, isToggle, isSelected), 31U);
	}

	static utility::graphic::Color32Bit
		getHoverContainerColor(Button::Color style, bool isToggle,
							   bool isSelected)
	{
		return ButtonBase::applyStateLayer(
			getBaseContainerColor(style, isToggle, isSelected),
			getContentColor(style, isToggle, isSelected), 20U);
	}

	static utility::graphic::Color32Bit
		getContainerColor(Button::Color style, bool isToggle, bool isSelected,
						  bool isHovered, bool isButtonPressed)
	{
		if (isButtonPressed) {
			return getPressedContainerColor(style, isToggle, isSelected);
		}

		if (isHovered) {
			return getHoverContainerColor(style, isToggle, isSelected);
		}

		if (!isToggle) {
			return getDefaultContainerColor(style);
		}

		if (isSelected) {
			return getToggleSelectedContainerColor(style);
		}

		return getToggleUnselectedContainerColor(style);
	}

	static utility::graphic::Color32Bit
		getContentColor(Button::Color style, bool isToggle, bool isSelected)
	{
		const auto &scheme = guillaume::getActiveScheme();

		if (!isToggle) {
			switch (style) {
				case Button::Color::Elevated:
					return scheme.getColor(SchemeColorRole::Primary).getColor();
				case Button::Color::Filled:
					return scheme.getColor(SchemeColorRole::OnPrimary)
						.getColor();
				case Button::Color::Tonal:
					return scheme
						.getColor(SchemeColorRole::OnSecondaryContainer)
						.getColor();
				case Button::Color::Outlined:
					return scheme.getColor(SchemeColorRole::Primary).getColor();
				case Button::Color::Text:
					return scheme.getColor(SchemeColorRole::Primary).getColor();
				case Button::Color::Segmented:
					return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor();
				default:
					throw std::runtime_error("Invalid button color style");
			}
		}

		if (!isSelected) {
			switch (style) {
				case Button::Color::Elevated:
					return scheme.getColor(SchemeColorRole::Primary).getColor();

				case Button::Color::Filled:
					return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor();

				case Button::Color::Outlined:
					return scheme.getColor(SchemeColorRole::Primary).getColor();

				case Button::Color::Tonal:
					return scheme
						.getColor(SchemeColorRole::OnSecondaryContainer)
						.getColor();

				case Button::Color::Text:
					return scheme.getColor(SchemeColorRole::Primary).getColor();

				case Button::Color::Segmented:
					return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor();

				default:
					throw std::runtime_error("Invalid button color style");
			}
		}

		switch (style) {
			case Button::Color::Elevated:
			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::OnPrimary).getColor();

			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::OnSecondary).getColor();

			case Button::Color::Outlined:
			case Button::Color::Segmented:
				return scheme.getColor(SchemeColorRole::OnSecondaryContainer)
					.getColor();

			case Button::Color::Text:
				return scheme.getColor(SchemeColorRole::Primary).getColor();

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getBorderColor(Button::Color style, bool isToggle, bool isSelected)
	{
		const auto &scheme = guillaume::getActiveScheme();

		if (style != Button::Color::Outlined
			&& style != Button::Color::Segmented) {
			return utility::graphic::Color32Bit(0, 0, 0, 0);
		}

		if (!isToggle || !isSelected) {
			return scheme.getColor(SchemeColorRole::Outline).getColor();
		}

		return utility::graphic::Color32Bit(0, 0, 0, 0);
	}

	utility::graphic::Color32Bit Button::getInitialContentColor(void) const
	{
		return getContentColor(_colorStyle, _isToggle, _isSelected);
	}

	void Button::applyStyleState(void)
	{
		if (_isDisabled) {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(getDisabledContainerColor());

			getComponentRegistry()
				.getComponent<components::Borders>(getIdentifier())
				.setColor(getDisabledBorderColor());

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
			.setColor(getContainerColor(_colorStyle, _isToggle, _isSelected,
										isHovered(), isPressed()));

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(getBorderColor(_colorStyle, _isToggle, _isSelected));

		if (_icon) {
			_icon->setColor(
				getContentColor(_colorStyle, _isToggle, _isSelected));
		}

		if (_label) {
			_label->setColor(
				getContentColor(_colorStyle, _isToggle, _isSelected));
		}
	}

	static const utility::graphic::PoseF
		getIconPose(const utility::graphic::PoseF &buttonPose,
					const Button::Size &size, const std::uint32_t &buttonLayer)
	{
		const float widthPadding  = getWidthPadding(size);
		const float heightPadding = getHeightPadding(size);

		const float iconX = buttonPose.getPosition().x + widthPadding;
		const float iconY = buttonPose.getPosition().y + heightPadding;
		const float iconZ = buttonPose.getPosition().z;

		const utility::graphic::PositionF iconPosition(iconX, iconY, iconZ);

		const utility::graphic::OrientationF iconOrientation =
			buttonPose.getOrientation();

		return ButtonBase::applyLayerToPosition(iconPosition, iconOrientation,
												buttonLayer);
	}

	static const utility::graphic::PoseF
		getLabelPose(const utility::graphic::PoseF &buttonPose,
					 const Button::Size &size, const std::uint32_t &buttonLayer,
					 const float &iconWidth)
	{
		const float widthPadding = getWidthPadding(size);
		const float spaceBetweenIconAndLabel =
			getSpaceBetweenIconAndLabel(size);
		const float heightPadding = getHeightPadding(size);

		const float labelX = buttonPose.getPosition().x + widthPadding
			+ iconWidth + spaceBetweenIconAndLabel;
		const float labelY = buttonPose.getPosition().y + heightPadding;
		const float labelZ = buttonPose.getPosition().z;

		const utility::graphic::PositionF labelPosition(labelX, labelY, labelZ);

		const utility::graphic::OrientationF labelOrientation =
			buttonPose.getOrientation();

		return ButtonBase::applyLayerToPosition(labelPosition, labelOrientation,
												buttonLayer);
	}

	static const utility::graphic::PoseF getTrailingIconPose(
		const utility::graphic::PoseF &buttonPose, const Button::Size &size,
		const std::uint32_t &buttonLayer, const float &labelWidth)
	{
		const float widthPadding = getWidthPadding(size);
		const float spaceBetweenIconAndLabel =
			getSpaceBetweenIconAndLabel(size);
		const float heightPadding = getHeightPadding(size);

		const float iconX = buttonPose.getPosition().x + widthPadding
			+ labelWidth + spaceBetweenIconAndLabel;
		const float iconY = buttonPose.getPosition().y + heightPadding;
		const float iconZ = buttonPose.getPosition().z;

		const utility::graphic::PositionF iconPosition(iconX, iconY, iconZ);

		const utility::graphic::OrientationF iconOrientation =
			buttonPose.getOrientation();

		return ButtonBase::applyLayerToPosition(iconPosition, iconOrientation,
												buttonLayer);
	}

	void Button::applyGeometry(void)
	{
		const float radius = getBorderRadius(
			_size, getRestingShape(_shape, _isToggle, _isSelected, _isMorph),
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
			_label->setFontSize(fontHeight);
		}

		const auto &buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		const auto &iconBound =
			getComponentRegistry().getComponent<components::Bound>(
				_icon->getIdentifier());

		const auto &labelBound =
			getComponentRegistry().getComponent<components::Bound>(
				_label->getIdentifier());

		if (_isTrailingIcon) {
			auto labelPose = getLabelPose(buttonPose, _size, getLayer(), 0.0f);

			getComponentRegistry()
				.getComponent<components::Transform>(_label->getIdentifier())
				.setPose(labelPose);

			auto iconPose = getTrailingIconPose(buttonPose, _size, getLayer(),
												labelBound.getWidth());

			getComponentRegistry()
				.getComponent<components::Transform>(_icon->getIdentifier())
				.setPose(iconPose);
		} else {
			auto iconPose = getIconPose(buttonPose, _size, getLayer());

			getComponentRegistry()
				.getComponent<components::Transform>(_icon->getIdentifier())
				.setPose(iconPose);

			auto labelPose = getLabelPose(buttonPose, _size, getLayer(),
										  iconBound.getWidth());

			getComponentRegistry()
				.getComponent<components::Transform>(_label->getIdentifier())
				.setPose(labelPose);
		}

		const bool hasIcon	= !_iconGlyphName.empty();
		const bool hasLabel = !_labelContent.empty();

		auto buttonWidth = getWidthPadding(_size) * 2.0f;

		if (hasIcon) {
			buttonWidth += iconBound.getWidth();
		}

		if (hasIcon && hasLabel) {
			buttonWidth += getSpaceBetweenIconAndLabel(_size);
		}

		if (hasLabel) {
			buttonWidth += labelBound.getWidth();
		}

		auto buttonHeight = 0.0f;

		buttonHeight = iconBound.getHeight() > buttonHeight
			? iconBound.getHeight()
			: buttonHeight;
		buttonHeight = labelBound.getHeight() > buttonHeight
			? labelBound.getHeight()
			: buttonHeight;

		buttonHeight += getHeightPadding(_size) * 2.0f;

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(buttonWidth)
			.setHeight(buttonHeight);
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
