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

#include "guillaume/entities/button.hpp"

#include "guillaume/theme.hpp"

#include <utility>

namespace guillaume::entities
{

	Button::Button::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									 ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Button::Builder::~Builder(void)
	{
	}

	ecs::Entity::Identifier Button::Builder::registerEntity(void)
	{
		_button = std::make_unique<Button>(
			this->getComponentRegistry(), _iconGlyphName, _labelContent,
			_isToggle, _colorStyle, _shape, _size, _isMorph, _onClick);

		ecs::Entity::Identifier identifier = _button->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_button));
		return identifier;
	}

	void Button::Builder::reset(void)
	{
		_button.reset();
		_iconGlyphName.clear();
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

	Button::Builder &Button::Builder::withLabel(const std::string &labelContent)
	{
		_labelContent = labelContent;
		return *this;
	}

	Button::Builder &
		Button::Builder::withOnClick(const std::function<void(void)> &onClick)
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

	ecs::Entity::Identifier
		Button::Director::makeTextButton(Builder &builder,
										 const std::string &labelContent,
										 std::function<void(void)> onClick)
	{
		return builder.withLabel(labelContent)
			.withOnClick(onClick)
			.registerEntity();
	}

	ecs::Entity::Identifier
		Button::Director::makeIconButton(Builder &builder,
										 const std::string &iconGlyphName,
										 std::function<void(void)> onClick)
	{
		return builder.withIcon(iconGlyphName)
			.withOnClick(onClick)
			.registerEntity();
	}

	ecs::Entity::Identifier Button::Director::makeIconTextButton(
		Builder &builder, const std::string &iconGlyphName,
		const std::string &labelContent, std::function<void(void)> onClick)
	{
		return builder.withIcon(iconGlyphName)
			.withLabel(labelContent)
			.withOnClick(onClick)
			.registerEntity();
	}

	static std::size_t getButtonHeight(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 32;
			case Button::Size::Small:
				return 40;
			case Button::Size::Medium:
				return 56;
			case Button::Size::Large:
				return 96;
			case Button::Size::ExtraLarge:
				return 136;
		}

		return 40;
	}

	static float getBorderRadius(Button::Size size, Button::Shape shape,
								 bool isPressed)
	{
		if (isPressed) {
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

	static std::size_t getFontSize(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 20U;
			case Button::Size::Small:
				return 20U;
			case Button::Size::Medium:
				return 24U;
			case Button::Size::Large:
				return 32U;
			case Button::Size::ExtraLarge:
				return 40U;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static std::size_t getWidthPadding(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 12U;
			case Button::Size::Small:
				return 16U;
			case Button::Size::Medium:
				return 24U;
			case Button::Size::Large:
				return 48U;
			case Button::Size::ExtraLarge:
				return 64U;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static std::size_t getSpaceBetweenIconAndLabel(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 4U;
			case Button::Size::Small:
				return 8U;
			case Button::Size::Medium:
				return 8U;
			case Button::Size::Large:
				return 12U;
			case Button::Size::ExtraLarge:
				return 16U;
		}

		return 8U;
	}

	static utility::graphic::Color32Bit
		getContainerColor(Button::Color style, bool isHovered, bool isPressed)
	{
		const auto &scheme = guillaume::defaultTheme.getLightScheme();

		auto applyStateAlpha = [](const utility::graphic::Color32Bit &color,
								  std::uint8_t alpha) {
			return utility::graphic::Color32Bit(
				color.getRed(), color.getGreen(), color.getBlue(), alpha);
		};

		switch (style) {
			case Button::Color::Elevated:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SurfaceContainerHigh)
							.getColor(),
						220U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SurfaceContainer)
							.getColor(),
						235U);
				}
				return scheme.getColor(SchemeColorRole::SurfaceContainerLow)
					.getColor();
			case Button::Color::Filled:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						220U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						235U);
				}
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case Button::Color::Tonal:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SecondaryContainer)
							.getColor(),
						220U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SecondaryContainer)
							.getColor(),
						235U);
				}
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case Button::Color::Outlined:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::OnSurfaceVariant)
							.getColor(),
						64U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::OnSurfaceVariant)
							.getColor(),
						32U);
				}
				return utility::graphic::Color32Bit(0, 0, 0, 0);
			case Button::Color::Text:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						48U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						24U);
				}
				return utility::graphic::Color32Bit(0, 0, 0, 0);
		}
		return utility::graphic::Color32Bit(255, 241, 237, 255);
	}

	static utility::graphic::Color32Bit getContentColor(Button::Color style)
	{
		const auto &scheme = guillaume::defaultTheme.getLightScheme();

		switch (style) {
			case Button::Color::Elevated:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::OnPrimary).getColor();
			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::OnSecondaryContainer)
					.getColor();
			case Button::Color::Outlined:
				return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
					.getColor();
			case Button::Color::Text:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
		}
		return scheme.getColor(SchemeColorRole::Primary).getColor();
	}

	void Button::hoverHandler(void)
	{
		applyMaterialState();
	}

	void Button::unHoverHandler(void)
	{
		applyMaterialState();
	}

	void Button::leftClickPressHandler()
	{
		applyMaterialState();
	}

	void Button::leftClickReleaseHandler()
	{
		if (_isToggle) {
			_isMorph = !_isMorph;
		}
		applyMaterialState();
		if (_onClick) {
			_onClick();
		}
	}

	void Button::applyMaterialState(void)
	{
		auto &interaction =
			getComponentRegistry().getComponent<components::Interaction>(
				getIdentifier());

		auto &buttonColor =
			getComponentRegistry().getComponent<components::Color>(
				getIdentifier());
		buttonColor.setColor(getContainerColor(
			_colorStyle, interaction.isMouseHovered(),
			interaction.isMouseButtonClicked(
				utility::event::MouseButtonEvent::MouseButton::Left)));

		auto &buttonBorders =
			getComponentRegistry().getComponent<components::Borders>(
				getIdentifier());
		const auto restingShape = getRestingShape(_shape, _isToggle, _isMorph);
		buttonBorders.setBorderRadius(getBorderRadius(
			_size, restingShape,
			interaction.isMouseButtonClicked(
				utility::event::MouseButtonEvent::MouseButton::Left)));

		if (_iconIdentifier != ecs::Entity::InvalidIdentifier) {
			getComponentRegistry()
				.getComponent<components::Color>(_iconIdentifier)
				.setColor(getContentColor(_colorStyle));
		}

		if (_labelIdentifier != ecs::Entity::InvalidIdentifier) {
			getComponentRegistry()
				.getComponent<components::Color>(_labelIdentifier)
				.setColor(getContentColor(_colorStyle));
		}
	}

	utility::graphic::PoseF Button::calculTextPoseWithoutIcon(void)
	{
		auto buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		const auto buttonWidth = static_cast<float>(calculWidth());
		const auto labelWidth  = static_cast<float>(
			getComponentRegistry()
				.getComponent<components::Bound>(_labelIdentifier)
				.getWidth());
		const float buttonLeft =
			buttonPose.getPosition().getX() - (buttonWidth / 2.0f);
		const float buttonCenterY = buttonPose.getPosition().getY()
			- (static_cast<float>(getButtonHeight(_size)) / 2.0f);

		auto textPosition = utility::graphic::PositionF();
		textPosition.setX(buttonLeft + getWidthPadding(_size)
						  + (labelWidth / 2.0f));
		textPosition.setY(buttonCenterY);
		textPosition.setZ(buttonPose.getPosition().getZ());
		return utility::graphic::PoseF(textPosition,
									   buttonPose.getOrientation());
	}

	utility::graphic::PoseF Button::calculTextPoseWithIcon(void)
	{
		auto buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		const auto buttonWidth = static_cast<float>(calculWidth());
		const auto labelWidth  = static_cast<float>(
			getComponentRegistry()
				.getComponent<components::Bound>(_labelIdentifier)
				.getWidth());
		const float buttonLeft =
			buttonPose.getPosition().getX() - (buttonWidth / 2.0f);
		const float buttonCenterY = buttonPose.getPosition().getY()
			- (static_cast<float>(getButtonHeight(_size)) / 2.0f);

		auto textPosition = utility::graphic::PositionF();
		textPosition.setX(
			buttonLeft + getWidthPadding(_size) + getFontSize(_size)
			+ getSpaceBetweenIconAndLabel(_size) + (labelWidth / 2.0f));
		textPosition.setY(buttonCenterY);
		textPosition.setZ(buttonPose.getPosition().getZ());
		return utility::graphic::PoseF(textPosition,
									   buttonPose.getOrientation());
	}

	std::size_t Button::calculWidth(void)
	{
		std::size_t width = getWidthPadding(_size) * 2;
		if (_iconIdentifier != ecs::Entity::InvalidIdentifier) {
			width += getFontSize(_size) + getSpaceBetweenIconAndLabel(_size);
		}
		if (_labelIdentifier != ecs::Entity::InvalidIdentifier) {
			auto labelBound =
				getComponentRegistry().getComponent<components::Bound>(
					_labelIdentifier);
			width += labelBound.getWidth();
		}
		return width;
	}

	Button::Button(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const std::string &labelContent, bool isToggle,
				   Color colorStyle, Shape shape, Size size, bool isMorph,
				   std::function<void(void)> onClick)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Interaction, components::Color,
								  components::Borders>(registry)
		, _iconGlyphName(iconGlyphName)
		, _labelContent(labelContent)
		, _isToggle(isToggle)
		, _colorStyle(colorStyle)
		, _shape(shape)
		, _size(size)
		, _isMorph(isMorph)
		, _onClick(std::move(onClick))
	{
		update();
	}

	Button::~Button()
	{
	}

	Button &Button::setIconGlyphName(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;

		if (_iconGlyphName.empty()) {
			_iconIdentifier = ecs::Entity::InvalidIdentifier;
		} else if (_iconIdentifier == ecs::Entity::InvalidIdentifier) {
			auto icon = std::make_unique<Icon>(
				getComponentRegistry(), _iconGlyphName,
				static_cast<float>(getFontSize(_size)),
				getContentColor(_colorStyle), Icon::Style::Outlined);
			_iconIdentifier = icon->getIdentifier();
			addEntity(std::move(icon));
		} else {
			getComponentRegistry()
				.getComponent<components::Glyph>(_iconIdentifier)
				.setName(_iconGlyphName);
			getComponentRegistry()
				.getComponent<components::Bound>(_iconIdentifier)
				.setWidth(getFontSize(_size))
				.setHeight(getFontSize(_size));
		}

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(calculWidth());

		if (_iconIdentifier == ecs::Entity::InvalidIdentifier) {
			if (_labelIdentifier != ecs::Entity::InvalidIdentifier) {
				getComponentRegistry()
					.getComponent<components::Transform>(_labelIdentifier)
					.setPose(calculTextPoseWithoutIcon());
			}

			applyMaterialState();
			return *this;
		}

		auto buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		const auto buttonWidth = static_cast<float>(calculWidth());
		const float buttonLeft =
			buttonPose.getPosition().getX() - (buttonWidth / 2.0f);
		const float buttonCenterY = buttonPose.getPosition().getY()
			- (static_cast<float>(getButtonHeight(_size)) / 2.0f);

		auto iconPosition = utility::graphic::PositionF();
		iconPosition.setX(buttonLeft + getWidthPadding(_size)
						  + (getFontSize(_size) / 2.0f));
		iconPosition.setY(buttonCenterY);
		iconPosition.setZ(buttonPose.getPosition().getZ());

		getComponentRegistry()
			.getComponent<components::Transform>(_iconIdentifier)
			.setPose(utility::graphic::PoseF(iconPosition,
											 buttonPose.getOrientation()));

		if (_labelIdentifier != ecs::Entity::InvalidIdentifier) {
			getComponentRegistry()
				.getComponent<components::Transform>(_labelIdentifier)
				.setPose(calculTextPoseWithIcon());
		}

		applyMaterialState();

		return *this;
	}

	Button &Button::setLabelContent(const std::string &labelContent)
	{
		_labelContent = labelContent;

		if (_labelContent.empty()) {
			_labelIdentifier = ecs::Entity::InvalidIdentifier;
		} else if (_labelIdentifier == ecs::Entity::InvalidIdentifier) {
			auto label = std::make_unique<Text>(
				getComponentRegistry(), _labelContent, getFontSize(_size),
				getContentColor(_colorStyle));
			_labelIdentifier = label->getIdentifier();
			addEntity(std::move(label));
		} else {
			getComponentRegistry()
				.getComponent<components::Text>(_labelIdentifier)
				.setContent(_labelContent)
				.setFontSize(getFontSize(_size));
		}

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(calculWidth());

		if (_labelIdentifier == ecs::Entity::InvalidIdentifier) {
			applyMaterialState();
			return *this;
		}

		auto labelPose = utility::graphic::PoseF();

		if (_iconIdentifier == ecs::Entity::InvalidIdentifier) {
			labelPose = calculTextPoseWithoutIcon();
		} else {
			labelPose = calculTextPoseWithIcon();
		}

		getComponentRegistry()
			.getComponent<components::Transform>(_labelIdentifier)
			.setPose(labelPose);

		applyMaterialState();

		return *this;
	}

	Button &Button::setIsToggle(const bool &isToggle)
	{
		_isToggle = isToggle;
		applyMaterialState();
		return *this;
	}

	Button &Button::setColorStyle(const Color &colorStyle)
	{
		_colorStyle = colorStyle;
		applyMaterialState();
		return *this;
	}

	Button &Button::setShape(const Shape &shape)
	{
		_shape = shape;
		applyMaterialState();

		return *this;
	}

	Button &Button::setSize(const Size &size)
	{
		_size = size;

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(calculWidth())
			.setHeight(getButtonHeight(size));

		if (_iconIdentifier != ecs::Entity::InvalidIdentifier) {
			setIconGlyphName(_iconGlyphName);
		}

		if (_labelIdentifier != ecs::Entity::InvalidIdentifier) {
			setLabelContent(_labelContent);
		}

		applyMaterialState();

		return *this;
	}

	Button &Button::setMorph(const bool &isMorph)
	{
		_isMorph = isMorph;
		applyMaterialState();
		return *this;
	}

	Button &Button::setOnClick(const std::function<void(void)> &onClick)
	{
		_onClick = onClick;
		return *this;
	}

	void Button::update(void)
	{
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(300.0f, 300.0f, 300.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)));

		setIconGlyphName(_iconGlyphName);
		setLabelContent(_labelContent);
		setIsToggle(_isToggle);
		setColorStyle(_colorStyle);
		setShape(_shape);
		setSize(_size);
		setMorph(_isMorph);
		setOnClick(_onClick);

		getComponentRegistry()
			.getComponent<components::Interaction>(getIdentifier())
			.setMouseOnHoverHandler(std::bind(&Button::hoverHandler, this))
			.setMouseOnUnhoverHandler(std::bind(&Button::unHoverHandler, this))
			.setMouseButtonOnClickHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				std::bind(&Button::leftClickPressHandler, this))
			.setMouseButtonOnClickReleaseHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				std::bind(&Button::leftClickReleaseHandler, this));
	}

}	 // namespace guillaume::entities