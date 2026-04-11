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
		ecs::Entity::Identifier _identifier = ecs::Entity::InvalidIdentifier;

		_button = std::make_unique<Button>(
			this->getComponentRegistry(), _iconIdendifier, _labelIdentifier,
			_isToggle, _colorStyle, _shape, _size, _isMorph, _onClick);

		this->getEntityRegistry().addEntity(std::move(_button));
		return _identifier;
	}

	void Button::Builder::reset(void)
	{
		_button.reset();
		_iconIdendifier	 = ecs::Entity::InvalidIdentifier;
		_labelIdentifier = ecs::Entity::InvalidIdentifier;
		_isToggle		 = false;
		_colorStyle		 = Color::Filled;
		_shape			 = Shape::Round;
		_size			 = Size::Small;
		_isMorph		 = false;
		_onClick		 = {};
	}

	Button::Builder &
		Button::Builder::withIcon(ecs::Entity::Identifier iconIdentifier)
	{
		_iconIdendifier = iconIdentifier;
		return *this;
	}

	Button::Builder &
		Button::Builder::withLabel(ecs::Entity::Identifier labelIdentifier)
	{
		_labelIdentifier = labelIdentifier;
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

	ecs::Entity::Identifier Button::Director::makeTextButton(
		Builder &builder, ecs::Entity::Identifier labelIdentifier,
		std::function<void(void)> onClick)
	{
		return builder.withLabel(labelIdentifier)
			.withOnClick(onClick)
			.registerEntity();
	}

	ecs::Entity::Identifier
		Button::Director::makeIconButton(Builder &builder,
										 ecs::Entity::Identifier iconIdentifier,
										 std::function<void(void)> onClick)
	{
		return builder.withIcon(iconIdentifier)
			.withOnClick(onClick)
			.registerEntity();
	}

	ecs::Entity::Identifier Button::Director::makeIconTextButton(
		Builder &builder, ecs::Entity::Identifier iconIdentifier,
		ecs::Entity::Identifier labelIdentifier,
		std::function<void(void)> onClick)
	{
		return builder.withIcon(iconIdentifier)
			.withLabel(labelIdentifier)
			.withOnClick(onClick)
			.registerEntity();
	}

	static std::size_t getButtonHeight(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 32;
				break;
			case Button::Size::Small:
				return 40;
				break;
			case Button::Size::Medium:
				return 56;
				break;
			case Button::Size::Large:
				return 96;
				break;
			case Button::Size::ExtraLarge:
				return 136;
				break;
		}
	}

	static float getBorderRadius(Button::Size size, Button::Shape shape,
								 bool isPressed)
	{
		if (shape == Button::Shape::Round) {
			return 100.0f;
		}
		if (shape == Button::Shape::Square) {
			switch (size) {
				case Button::Size::ExtraSmall:
					return (isPressed) ? 8.0f : 12.0f;
				case Button::Size::Small:
					return (isPressed) ? 8.0f : 12.0f;
				case Button::Size::Medium:
					return (isPressed) ? 12.0f : 16.0f;
				case Button::Size::Large:
					return (isPressed) ? 16.0f : 28.0f;
				case Button::Size::ExtraLarge:
					return (isPressed) ? 16.0f : 28.0f;
			}
		}
		return 0.0f;
	}

	static std::size_t getFontSize(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 20U;
				break;
			case Button::Size::Small:
				return 20U;
				break;
			case Button::Size::Medium:
				return 24U;
				break;
			case Button::Size::Large:
				return 32U;
				break;
			case Button::Size::ExtraLarge:
				return 40U;
				break;
		}
	}

	static std::size_t getWidthPadding(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 12U;
				break;
			case Button::Size::Small:
				return 16U;
				break;
			case Button::Size::Medium:
				return 24U;
				break;
			case Button::Size::Large:
				return 48U;
				break;
			case Button::Size::ExtraLarge:
				return 64U;
				break;
		}
	}

	static std::size_t getSpaceBetweenIconAndLabel(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 4U;
				break;
			case Button::Size::Small:
				return 8U;
				break;
			case Button::Size::Medium:
				return 8U;
				break;
			case Button::Size::Large:
				return 12U;
				break;
			case Button::Size::ExtraLarge:
				return 16U;
				break;
		}
	}

	static std::size_t getTargetAreaHeightMargin(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 16U;
				break;
			case Button::Size::Small:
				return 8U;
				break;
			case Button::Size::Medium:
				return 0U;
				break;
			case Button::Size::Large:
				return 0U;
				break;
			case Button::Size::ExtraLarge:
				return 0U;
				break;
		}
	}

	void Button::hoverHandler(void)
	{
	}

	void Button::unHoverHandler(void)
	{
	}

	void Button::leftClickPressHandler(
		utility::event::MouseMotionEvent::MousePosition mousePosition)
	{
		if (_onClick) {
			_onClick();
		}
	}

	void Button::leftClickReleaseHandler(
		utility::event::MouseMotionEvent::MousePosition mousePosition)
	{
	}

	utility::graphic::PoseF Button::calculTextPoseWithoutIcon(void)
	{
		auto buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		auto textPosition = utility::graphic::PositionF();
		textPosition.setX(buttonPose.getPosition().getX()
						  + getWidthPadding(_size));
		textPosition.setY(buttonPose.getPosition().getY()
						  + getButtonHeight(_size) / 2.0f);
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

		auto textPosition = utility::graphic::PositionF();
		textPosition.setX(buttonPose.getPosition().getX()
						  + getWidthPadding(_size) + getFontSize(_size)
						  + getSpaceBetweenIconAndLabel(_size));
		textPosition.setY(buttonPose.getPosition().getY()
						  - getTargetAreaHeightMargin(_size) / 2.0f);
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
				   ecs::Entity::Identifier iconIdentifier,
				   ecs::Entity::Identifier labelIdentifier, bool isToggle,
				   Color colorStyle, Shape shape, Size size, bool isMorph,
				   std::function<void(void)> onClick)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Hover, components::Click,
							components::Color, components::Borders>(registry)
	{
		// For testing purposes, we set the pose of the button to a fixed value.
		// In a real application, this would likely be set by a layout system or
		// by the user of the Button class.
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(300.0f, 300.0f, 300.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)));

		setIconIdentifier(iconIdentifier);
		setLabelIdentifier(labelIdentifier);
		setIsToggle(isToggle);
		setColorStyle(colorStyle);
		setShape(shape);
		setSize(size);
		setMorph(isMorph);
		setOnClick(onClick);

		getComponentRegistry()	
			.getComponent<components::Hover>(getIdentifier())
			.setOnHoverHandler(std::bind(&Button::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&Button::unHoverHandler, this));

		getComponentRegistry()
			.getComponent<components::Click>(getIdentifier())
			.setOnClickHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				std::bind(&Button::leftClickPressHandler, this,
						  std::placeholders::_1))
			.setOnReleaseHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				std::bind(&Button::leftClickReleaseHandler, this,
						  std::placeholders::_1));
	}

	Button::~Button()
	{
	}

	Button &Button::setIconIdentifier(ecs::Entity::Identifier iconIdentifier)
	{
		_iconIdentifier = iconIdentifier;
		if (_iconIdentifier == ecs::Entity::InvalidIdentifier) {
			return *this;
		}

		auto buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		auto iconPosition = utility::graphic::PositionF();
		iconPosition.setX(buttonPose.getPosition().getX()
						  - getWidthPadding(_size));
		iconPosition.setY(buttonPose.getPosition().getY()
						  - getTargetAreaHeightMargin(_size) / 2.0f);
		iconPosition.setZ(buttonPose.getPosition().getZ());

		getComponentRegistry()
			.getComponent<components::Transform>(iconIdentifier)
			.setPose(utility::graphic::PoseF(iconPosition,
											 buttonPose.getOrientation()));

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(calculWidth());

		return *this;
	}

	Button &Button::setLabelIdentifier(ecs::Entity::Identifier labelIdentifier)
	{
		_labelIdentifier = labelIdentifier;
		if (_labelIdentifier == ecs::Entity::InvalidIdentifier) {
			return *this;
		}

		auto labelPose = utility::graphic::PoseF();

		if (_iconIdentifier == ecs::Entity::InvalidIdentifier) {
			labelPose = calculTextPoseWithoutIcon();
		} else {
			labelPose = calculTextPoseWithIcon();
		}

		getComponentRegistry()
			.getComponent<components::Transform>(labelIdentifier)
			.setPose(labelPose);

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(calculWidth());

		return *this;
	}

	Button &Button::setIsToggle(const bool &isToggle)
	{
		_isToggle = isToggle;
		return *this;
	}

	Button &Button::setColorStyle(const Color &colorStyle)
	{
		_colorStyle = colorStyle;
		return *this;
	}

	Button &Button::setShape(const Shape &shape)
	{
		_shape = shape;

		return *this;
	}

	Button &Button::setSize(const Size &size)
	{
		_size = size;

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(calculWidth())
			.setHeight(getButtonHeight(size));

		return *this;
	}

	Button &Button::setMorph(const bool &isMorph)
	{
		_isMorph = isMorph;
		return *this;
	}

	Button &Button::setOnClick(const std::function<void(void)> &onClick)
	{
		_onClick = onClick;
		return *this;
	}

}	 // namespace guillaume::entities