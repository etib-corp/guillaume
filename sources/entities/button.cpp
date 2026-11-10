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
					return getShapeRadius(theme::ShapeRole::Small);
				case Button::Size::Medium:
					return getShapeRadius(theme::ShapeRole::Medium);
				case Button::Size::Large:
				case Button::Size::ExtraLarge:
					return getShapeRadius(theme::ShapeRole::Large);
				default:
					throw std::runtime_error("Invalid button size");
			}
		}

		if (shape == Button::Shape::Round) {
			return getShapeRadius(theme::ShapeRole::Full);
		}

		switch (size) {
			case Button::Size::ExtraSmall:
			case Button::Size::Small:
				return getShapeRadius(theme::ShapeRole::Medium);
			case Button::Size::Medium:
				return getShapeRadius(theme::ShapeRole::Large);
			case Button::Size::Large:
			case Button::Size::ExtraLarge:
				return getShapeRadius(theme::ShapeRole::ExtraLarge);
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
				return 18.0f;
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

	static float getBaseWidthPadding(Button::Size size)
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

	static float getWidthPadding(Button::Size size, bool compact = false)
	{
		// Paper compact buttons halve the horizontal label margin.
		return compact ? getBaseWidthPadding(size) / 2.0f
					   : getBaseWidthPadding(size);
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
			getContentColor(style, isToggle, isSelected),
			getStateLayerAlpha(theme::StateLayer::Pressed));
	}

	static utility::graphic::Color32Bit
		getHoverContainerColor(Button::Color style, bool isToggle,
							   bool isSelected)
	{
		return ButtonBase::applyStateLayer(
			getBaseContainerColor(style, isToggle, isSelected),
			getContentColor(style, isToggle, isSelected),
			getStateLayerAlpha(theme::StateLayer::Hovered));
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

		// Segmented buttons keep their outline when selected (Paper
		// SegmentedButtons); outlined toggles drop it.
		if (style == Button::Color::Segmented || !isToggle || !isSelected) {
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
			.setColor(resolveContainerColor(
				getContainerColor(_colorStyle, _isToggle, _isSelected,
								  isHovered(), isPressed())));

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(getBorderColor(_colorStyle, _isToggle, _isSelected))
			.setWidth(1.0f);

		if (_icon) {
			_icon->setColor(resolveContentColor(
				getContentColor(_colorStyle, _isToggle, _isSelected)));
		}

		if (_label) {
			_label->setColor(resolveContentColor(
				getContentColor(_colorStyle, _isToggle, _isSelected)));
		}
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
			? getSpaceBetweenIconAndLabel(_size)
			: 0.0f;
		const float contentWidth  = iconWidth + gap + labelWidth;
		const float contentHeight = std::max(iconHeight, labelHeight);

		// The label typography (labelLarge) is independent of the icon size;
		// the container grows to the larger child and to the M3 minimums.
		const float minHeight = (_size == Size::ExtraSmall) ? 0.0f : MinHeight;
		const float buttonWidth =
			std::max({ contentWidth + getWidthPadding(_size, _isCompact) * 2.0f,
					   MinWidth, _minWidth });
		const float buttonHeight =
			std::max(contentHeight + getHeightPadding(_size) * 2.0f, minHeight);

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
