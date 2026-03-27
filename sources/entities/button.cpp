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
		: ecs::NodeEntityBuilder<Button>(componentRegistry, entityRegistry)
	{
		_iconBuilder =
			std::make_unique<Icon::Builder>(componentRegistry, entityRegistry);
		_textBuilder =
			std::make_unique<Text::Builder>(componentRegistry, entityRegistry);
		reset();
	}

	Button::Builder::~Builder(void)
	{
	}

	void Button::Builder::registerEntity(void)
	{
		if (!_iconName.empty()) {
			_iconDirector.makeDefaultIcon(*_iconBuilder, _iconName);
		}
		if (!_label.empty()) {
			_textDirector.makeDefaultText(*_textBuilder, _label, 24,
										  Text::Color { 255, 255, 255, 255 });
		}
		_button = std::make_unique<Button>(
			this->getComponentRegistry(), _toggleState, _colorStyle, _shape,
			_size, _morphState, nullptr, nullptr, _onClick);
		this->getEntityRegistry().addEntity(std::move(_button));
	}

	void Button::Builder::reset(void)
	{
		ecs::NodeEntityBuilder<Button>::reset();
		_button.reset();
		_toggleState = ToggleState::Default;
		_colorStyle	 = ColorStyle::Filled;
		_shape		 = Shape::Round;
		_size		 = Size::Small;
		_morphState	 = MorphState::Default;
		_icon.reset();
		_textLabel.reset();
		_iconName.clear();
		_label.clear();
		_onClick = {};
	}

	Button::Builder &Button::Builder::withIcon(const std::string &iconName)
	{
		_iconName = iconName;
		return *this;
	}

	Button::Builder &Button::Builder::withLabel(const std::string &label)
	{
		_label = label;
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

	void Button::Director::makeTextButton(Builder &builder,
										  const std::string &label,
										  std::function<void(void)> onClick)
	{
		builder.withLabel(label).withOnClick(onClick).registerEntity();
	}

	void Button::Director::makeIconButton(Builder &builder,
										  const std::string &iconName,
										  std::function<void(void)> onClick)
	{
		builder.withIcon(iconName).withOnClick(onClick).registerEntity();
	}

	void Button::Director::makeIconTextButton(Builder &builder,
											  const std::string &iconName,
											  const std::string &label,
											  std::function<void(void)> onClick)
	{
		builder.withIcon(iconName)
			.withLabel(label)
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
	}

	Button::Button(ecs::ComponentRegistry &registry, ToggleState toggleState,
				   ColorStyle colorStyle, Shape shape, Size size,
				   MorphState morphState, std::unique_ptr<ecs::Entity> icon,
				   std::unique_ptr<ecs::Entity> label,
				   std::function<void(void)> onClick)
		: ecs::NodeEntityFiller<components::Transform, components::Bound,
								components::Hover, components::Click,
								components::Color, components::Borders>(
			  registry)
		, _toggleState(toggleState)
		, _colorStyle(colorStyle)
		, _shape(shape)
		, _size(size)
		, _morphState(morphState)
		, _icon(std::move(icon))
		, _label(std::move(label))
		, _onClick(std::move(onClick))
	{
		// Initialize button position, rotation, and scale
		registry.getComponent<components::Transform>(getIdentifier())
			.setPosition({ 300.0f, 300.0f, 0.0f })
			.setRotation(components::Transform::Rotation())
			.setScale({ 1.0f, 1.0f, 1.0f });

		registry.getComponent<components::Bound>(getIdentifier())
			.setSize({ 136, 136 });

		// Set up hover event handlers
		registry.getComponent<components::Hover>(getIdentifier())
			.setOnHoverHandler([this]() {
				this->hoverHandler();
			})
			.setOnUnhoverHandler([this]() {
				this->unHoverHandler();
			});

		// Set up click event handlers
		registry.getComponent<components::Click>(getIdentifier())
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

}	 // namespace guillaume::entities