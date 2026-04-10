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
			_toggleState, _colorStyle, _shape, _size, _morphState, _onClick);

		_identifier = _button->getIdentifier();

		this->getEntityRegistry().addEntity(std::move(_button));
		return _identifier;
	}

	void Button::Builder::reset(void)
	{
		_button.reset();
		_iconIdendifier	 = ecs::Entity::InvalidIdentifier;
		_labelIdentifier = ecs::Entity::InvalidIdentifier;
		_toggleState	 = ToggleState::Default;
		_colorStyle		 = ColorStyle::Filled;
		_shape			 = Shape::Round;
		_size			 = Size::Small;
		_morphState		 = MorphState::Default;
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

	Button::Builder &
		Button::Builder::withToggleState(const ToggleState &toggleState)
	{
		_toggleState = toggleState;
		return *this;
	}

	Button::Builder &
		Button::Builder::withColorStyle(const ColorStyle &colorStyle)
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

	Button::Builder &
		Button::Builder::withMorphState(const MorphState &morphState)
	{
		_morphState = morphState;
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

	void Button::hoverHandler(void)
	{
	}

	void Button::unHoverHandler(void)
	{
	}

	void Button::leftClickPressHandler(
		utility::event::MouseMotionEvent::MousePosition mousePosition)
	{
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setTopLeftRadius(44.0f)
			.setTopRightRadius(44.0f)
			.setBottomRightRadius(44.0f)
			.setBottomLeftRadius(44.0f);

		auto color = getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.getColor();
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color.withAlpha(192));

		if (_onClick) {
			_onClick();
		}
	}

	void Button::leftClickReleaseHandler(
		utility::event::MouseMotionEvent::MousePosition mousePosition)
	{
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setTopLeftRadius(28.0f)
			.setTopRightRadius(28.0f)
			.setBottomRightRadius(28.0f)
			.setBottomLeftRadius(28.0f);

		auto color = getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.getColor();
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color.withAlpha(255));
	}

	Button::Button(ecs::ComponentRegistry &registry,
				   ecs::Entity::Identifier iconIdentifier,
				   ecs::Entity::Identifier labelIdentifier,
				   ToggleState toggleState, ColorStyle colorStyle, Shape shape,
				   Size size, MorphState morphState,
				   std::function<void(void)> onClick)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Interaction,
							components::Color, components::Borders>(registry)
		, _iconIdentifier(iconIdentifier)
		, _labelIdentifier(labelIdentifier)
		, _toggleState(toggleState)
		, _colorStyle(colorStyle)
		, _shape(shape)
		, _size(size)
		, _morphState(morphState)
		, _onClick(std::move(onClick))
	{
		// Initialize button pose
		registry.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(
				utility::graphic::PositionF(200.0f, 200.0f, 0.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)));

		registry.getComponent<components::Bound>(getIdentifier())
			.setSize({ 136, 136 });

		// Set up hover event handlers
		registry.getComponent<components::Interaction>(getIdentifier())
			.setOnHoverHandler([this]() {
				this->hoverHandler();
			})
			.setOnUnhoverHandler([this]() {
				this->unHoverHandler();
			});

		// Set up click event handlers
		registry.getComponent<components::Interaction>(getIdentifier())
			.setOnClickHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				[this](utility::event::MouseMotionEvent::MousePosition
						   mousePosition) {
					this->leftClickPressHandler(mousePosition);
				})
			.setOnReleaseHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				[this](utility::event::MouseMotionEvent::MousePosition
						   mousePosition) {
					this->leftClickReleaseHandler(mousePosition);
				});

		registry.getComponent<components::Color>(getIdentifier())
			.setColor({ 255, 0, 0, 255 });	  // Default color (red)

		registry.getComponent<components::Borders>(getIdentifier())
			.setTopLeftRadius(28.0f)
			.setTopRightRadius(28.0f)
			.setBottomRightRadius(28.0f)
			.setBottomLeftRadius(28.0f);
	}

	Button::~Button()
	{
	}

	Button &Button::setIconIdentifier(ecs::Entity::Identifier iconIdentifier)
	{
		_iconIdentifier = iconIdentifier;
		return *this;
	}

	Button &Button::setLabelIdentifier(ecs::Entity::Identifier labelIdentifier)
	{
		_labelIdentifier = labelIdentifier;
		return *this;
	}

	Button &Button::setToggleState(const ToggleState &toggleState)
	{
		_toggleState = toggleState;
		return *this;
	}

	Button &Button::setColorStyle(const ColorStyle &colorStyle)
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
		return *this;
	}

	Button &Button::setMorphState(const MorphState &morphState)
	{
		_morphState = morphState;
		return *this;
	}

	Button &Button::setOnClick(const std::function<void(void)> &onClick)
	{
		_onClick = onClick;
		return *this;
	}

	Button &Button::setOnClick(std::function<void(void)> &&onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

}	 // namespace guillaume::entities