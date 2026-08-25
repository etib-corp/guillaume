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

#include <iostream>
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
			_onClick);
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
		_isToggle	= false;
		_colorStyle = Color::Filled;
		_shape		= Shape::Round;
		_size		= Size::Small;
		_isMorph	= false;
		_onClick	= {};
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
										 bool isSelected)
	{
		if (!isToggle || !isSelected) {
			return baseShape;
		}

		return (baseShape == Button::Shape::Round) ? Button::Shape::Square
												   : Button::Shape::Round;
	}

	static float getFontSize(Button::Size size)
	{
		switch (size) {
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
		applyStateAlpha(const utility::graphic::Color32Bit &color,
						std::uint8_t alpha)
	{
		return utility::graphic::Color32Bit(color.getRed(), color.getGreen(),
											color.getBlue(), alpha);
	}

	static utility::graphic::Color32Bit
		getDefaultContainerColor(Button::Color style)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

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
				return utility::graphic::Color32Bit(0, 0, 0, 0);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getToggleUnselectedContainerColor(Button::Color style)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

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
				return utility::graphic::Color32Bit(0, 0, 0, 0);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getToggleSelectedContainerColor(Button::Color style)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

		switch (style) {
			case Button::Color::Elevated:
			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::Primary).getColor();

			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::Secondary).getColor();

			case Button::Color::Outlined:
				return scheme.getColor(SchemeColorRole::InverseSurface)
					.getColor();

			case Button::Color::Text:
				return utility::graphic::Color32Bit(0, 0, 0, 0);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getPressedContainerColor(Button::Color style, bool isToggle,
								 bool isSelected)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

		if (isToggle) {
			switch (style) {
				case Button::Color::Elevated:
				case Button::Color::Filled:
					return applyStateAlpha(
						isSelected
							? scheme.getColor(SchemeColorRole::Primary)
								  .getColor()
							: (style == Button::Color::Filled
								   ? scheme
										 .getColor(
											 SchemeColorRole::SurfaceContainer)
										 .getColor()
								   : scheme
										 .getColor(SchemeColorRole::
													   SurfaceContainerLow)
										 .getColor()),
						220U);

				case Button::Color::Tonal:
					return applyStateAlpha(
						isSelected
							? scheme.getColor(SchemeColorRole::Secondary)
								  .getColor()
							: scheme
								  .getColor(SchemeColorRole::SecondaryContainer)
								  .getColor(),
						220U);

				case Button::Color::Outlined:
					return applyStateAlpha(
						isSelected
							? scheme.getColor(SchemeColorRole::InverseSurface)
								  .getColor()
							: scheme.getColor(SchemeColorRole::InverseOnSurface)
								  .getColor(),
						isSelected ? 220U : 64U);

				case Button::Color::Text:
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						48U);

				default:
					throw std::runtime_error("Invalid button color style");
			}
		}

		switch (style) {
			case Button::Color::Elevated:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::SurfaceContainerHigh)
						.getColor(),
					220U);

			case Button::Color::Filled:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::Primary).getColor(), 220U);

			case Button::Color::Tonal:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::SecondaryContainer)
						.getColor(),
					220U);

			case Button::Color::Outlined:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor(),
					64U);

			case Button::Color::Text:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::Primary).getColor(), 48U);

			default:
				throw std::runtime_error("Invalid button color style");
		}
	}

	static utility::graphic::Color32Bit
		getHoverContainerColor(Button::Color style, bool isToggle,
							   bool isSelected)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

		if (isToggle) {
			switch (style) {
				case Button::Color::Elevated:
				case Button::Color::Filled:
					return applyStateAlpha(
						isSelected
							? scheme.getColor(SchemeColorRole::Primary)
								  .getColor()
							: (style == Button::Color::Filled
								   ? scheme
										 .getColor(
											 SchemeColorRole::SurfaceContainer)
										 .getColor()
								   : scheme
										 .getColor(SchemeColorRole::
													   SurfaceContainerLow)
										 .getColor()),
						235U);

				case Button::Color::Tonal:
					return applyStateAlpha(
						isSelected
							? scheme.getColor(SchemeColorRole::Secondary)
								  .getColor()
							: scheme
								  .getColor(SchemeColorRole::SecondaryContainer)
								  .getColor(),
						235U);

				case Button::Color::Outlined:
					return applyStateAlpha(
						isSelected
							? scheme.getColor(SchemeColorRole::InverseSurface)
								  .getColor()
							: scheme.getColor(SchemeColorRole::OnSurfaceVariant)
								  .getColor(),
						isSelected ? 235U : 32U);

				case Button::Color::Text:
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						24U);

				default:
					throw std::runtime_error("Invalid button color style");
			}
		}

		switch (style) {
			case Button::Color::Elevated:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::SurfaceContainer)
						.getColor(),
					235U);

			case Button::Color::Filled:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::Primary).getColor(), 235U);

			case Button::Color::Tonal:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::SecondaryContainer)
						.getColor(),
					235U);

			case Button::Color::Outlined:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor(),
					32U);

			case Button::Color::Text:
				return applyStateAlpha(
					scheme.getColor(SchemeColorRole::Primary).getColor(), 24U);

			default:
				throw std::runtime_error("Invalid button color style");
		}
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
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

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
					return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor();
				case Button::Color::Text:
					return scheme.getColor(SchemeColorRole::Primary).getColor();
				default:
					throw std::runtime_error("Invalid button color style");
			}
		}

		if (!isSelected) {
			switch (style) {
				case Button::Color::Elevated:
					return scheme.getColor(SchemeColorRole::Primary).getColor();

				case Button::Color::Filled:
				case Button::Color::Outlined:
					return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
						.getColor();

				case Button::Color::Tonal:
					return scheme
						.getColor(SchemeColorRole::OnSecondaryContainer)
						.getColor();

				case Button::Color::Text:
					return scheme.getColor(SchemeColorRole::Primary).getColor();

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
				return scheme.getColor(SchemeColorRole::InverseOnSurface)
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
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

		if (style != Button::Color::Outlined) {
			return utility::graphic::Color32Bit(0, 0, 0, 0);
		}

		if (!isToggle || !isSelected) {
			return scheme.getColor(SchemeColorRole::OutlineVariant).getColor();
		}

		return utility::graphic::Color32Bit(0, 0, 0, 0);
	}

	void Button::hoverHandler(void)
	{
		setShape(_shape);
		setColorStyle(_colorStyle);
	}

	void Button::unHoverHandler(void)
	{
		setShape(_shape);
		setColorStyle(_colorStyle);
	}

	void Button::buttonPressHandler()
	{
		if (_isToggle) {
			_isSelected = !_isSelected;
		}

		setShape(_shape);
		setColorStyle(_colorStyle);

		if (_onClick) {
			_onClick();
		}
	}

	void Button::buttonReleaseHandler()
	{
		setShape(_shape);
		setColorStyle(_colorStyle);
	}

	Button::Button(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const components::Glyph::Style &iconStyle,
				   const std::string &labelContent, bool isToggle,
				   Color colorStyle, Shape shape, Size size, bool isMorph,
				   std::function<void(void)> onClick)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Color, components::Borders,
								  components::Focus,
								  components::HandButtonInteraction,
								  components::HandHoverInteraction,
								  components::MouseHoverInteraction,
								  components::MouseButtonInteraction>(registry)
		, _iconGlyphName(iconGlyphName)
		, _iconStyle(iconStyle)
		, _labelContent(labelContent)
		, _icon()
		, _label()
		, _isToggle(isToggle)
		, _isSelected(false)
		, _colorStyle(colorStyle)
		, _shape(shape)
		, _size(size)
		, _isMorph(isMorph)
		, _onClick(std::move(onClick))
	{
	}

	Button::~Button(void)
	{
	}

	static const utility::graphic::PoseF
		applyLayerToPosition(const utility::graphic::PositionF &position,
							 const utility::graphic::OrientationF &orientation,
							 const std::uint32_t &layer)
	{
		const auto forwardVector					= orientation.getForward();
		utility::graphic::PositionF forwardPosition = position;

		forwardPosition.translate(utility::graphic::PositionF(
			-forwardVector * static_cast<float>(layer + 1) * 1.0f));

		return utility::graphic::PoseF(forwardPosition, orientation);
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

		utility::graphic::PoseF iconPose(iconPosition, iconOrientation);
		auto iconPoseWithLayer = applyLayerToPosition(
			iconPose.getPosition(), iconPose.getOrientation(), buttonLayer);

		return iconPoseWithLayer;
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

		utility::graphic::PoseF labelPose(labelPosition, labelOrientation);
		auto labelPoseWithLayer = applyLayerToPosition(
			labelPose.getPosition(), labelPose.getOrientation(), buttonLayer);

		return labelPoseWithLayer;
	}

	Button &Button::setIconGlyphName(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;

		_icon->setGlyphName(_iconGlyphName);

		return *this;
	}

	Button &Button::setIconStyle(const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;

		_icon->setStyle(_iconStyle);

		return *this;
	}

	Button &Button::setLabelContent(const std::string &labelContent)
	{
		_labelContent = labelContent;

		_label->setContent(_labelContent);

		return *this;
	}

	Button &Button::setIsToggle(const bool &isToggle)
	{
		_isToggle = isToggle;
		return *this;
	}

	Button &Button::setColorStyle(const Color &colorStyle)
	{
		bool isHovered		 = false;
		bool isButtonPressed = false;

		_colorStyle = colorStyle;

		isHovered =
			getComponentRegistry()
				.getComponent<components::HandHoverInteraction>(getIdentifier())
				.isHovered()
			? true
			: isHovered;

		isHovered = getComponentRegistry()
						.getComponent<components::MouseHoverInteraction>(
							getIdentifier())
						.isHovered()
			? true
			: isHovered;

		isButtonPressed =
			getComponentRegistry()
				.getComponent<components::HandButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::HandButtonEvent::Button::A)
			? true
			: isButtonPressed;

		isButtonPressed =
			getComponentRegistry()
				.getComponent<components::MouseButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::MouseButtonEvent::Button::Left)
			? true
			: isButtonPressed;

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(getContainerColor(_colorStyle, _isToggle, _isSelected,
										isHovered, isButtonPressed));

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(getBorderColor(_colorStyle, _isToggle, _isSelected));

		_icon->setColor(getContentColor(_colorStyle, _isToggle, _isSelected));
		_label->setColor(getContentColor(_colorStyle, _isToggle, _isSelected));

		return *this;
	}

	Button &Button::setShape(const Shape &shape)
	{
		bool isHovered		 = false;
		bool isButtonPressed = false;

		_shape = getRestingShape(shape, _isToggle, _isSelected);

		isHovered =
			getComponentRegistry()
				.getComponent<components::HandHoverInteraction>(getIdentifier())
				.isHovered()
			? true
			: isHovered;

		isHovered = getComponentRegistry()
						.getComponent<components::MouseHoverInteraction>(
							getIdentifier())
						.isHovered()
			? true
			: isHovered;

		isButtonPressed =
			getComponentRegistry()
				.getComponent<components::HandButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::HandButtonEvent::Button::A)
			? true
			: isButtonPressed;

		isButtonPressed =
			getComponentRegistry()
				.getComponent<components::MouseButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::MouseButtonEvent::Button::Left)
			? true
			: isButtonPressed;

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getBorderRadius(_size, _shape, isButtonPressed));

		return *this;
	}

	Button &Button::setSize(const Size &size)
	{
		_size = size;

		_icon->setFontSize(getFontSize(_size));
		_label->setFontSize(getFontSize(_size));

		const auto &buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		auto iconPose = getIconPose(buttonPose, _size, getLayer());

		getComponentRegistry()
			.getComponent<components::Transform>(_icon->getIdentifier())
			.setPose(iconPose);

		const auto &iconBound =
			getComponentRegistry().getComponent<components::Bound>(
				_icon->getIdentifier());

		auto labelPose =
			getLabelPose(buttonPose, _size, getLayer(), iconBound.getWidth());

		getComponentRegistry()
			.getComponent<components::Transform>(_label->getIdentifier())
			.setPose(labelPose);

		const auto &labelBound =
			getComponentRegistry().getComponent<components::Bound>(
				_label->getIdentifier());

		auto buttonWidth = getWidthPadding(_size) * 2.0f + iconBound.getWidth()
			+ getSpaceBetweenIconAndLabel(_size) + labelBound.getWidth();

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

		return *this;
	}

	Button &Button::setMorph(const bool &isMorph)
	{
		_isMorph = isMorph;
		return *this;
	}

	Button &Button::setOnClick(std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	void Button::initialize(void)
	{
		Icon::Builder iconBuilder(getComponentRegistry(), *this);
		Icon::Director iconDirector;
		Text::Builder labelBuilder(getComponentRegistry(), *this);
		Text::Director labelDirector;

		_icon = iconDirector.makeIcon(
			iconBuilder, shared_from_this(), _iconGlyphName, getFontSize(_size),
			getContentColor(_colorStyle, _isToggle, _isSelected), _iconStyle);

		getLogger().info() << "Icon : " << _icon
						   << " created for button: " << shared_from_this();

		_label = labelDirector.makeText(
			labelBuilder, shared_from_this(), _labelContent, getFontSize(_size),
			getContentColor(_colorStyle, _isToggle, _isSelected));

		getLogger().info() << "Label : " << _label
						   << " created for button: " << shared_from_this();

		getComponentRegistry()
			.getComponent<components::HandButtonInteraction>(getIdentifier())
			.setOnButtonPressHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&Button::buttonPressHandler, this))
			.setOnButtonReleaseHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&Button::buttonReleaseHandler, this));

		getComponentRegistry()
			.getComponent<components::MouseButtonInteraction>(getIdentifier())
			.setOnButtonPressHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&Button::buttonPressHandler, this))
			.setOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&Button::buttonReleaseHandler, this));

		getComponentRegistry()
			.getComponent<components::HandHoverInteraction>(getIdentifier())
			.setOnHoverHandler(std::bind(&Button::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&Button::unHoverHandler, this));

		getComponentRegistry()
			.getComponent<components::MouseHoverInteraction>(getIdentifier())
			.setOnHoverHandler(std::bind(&Button::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&Button::unHoverHandler, this));
	}

	void Button::update(void)
	{
		setIconGlyphName(_iconGlyphName);
		setIconStyle(_iconStyle);
		setLabelContent(_labelContent);
		setIsToggle(_isToggle);
		setColorStyle(_colorStyle);
		setShape(_shape);
		setSize(_size);
		setMorph(_isMorph);
		setOnClick(_onClick);
	}
}	 // namespace guillaume::entities