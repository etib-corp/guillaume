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

#include <memory>
#include <utility>

#include "guillaume/entities/split_button.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon_button.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::entities
{
	SplitButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<SplitButton>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<SplitButton> SplitButton::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<SplitButton>(
			this->getComponentRegistry(), _labelContent, _onClick, _onMenuOpen);
		return entity;
	}

	void SplitButton::Builder::reset(void)
	{
		_labelContent.clear();
		_onClick	= {};
		_onMenuOpen = {};
	}

	SplitButton::Builder &
		SplitButton::Builder::withLabel(const std::string &labelContent)
	{
		_labelContent = labelContent;
		return *this;
	}

	SplitButton::Builder &
		SplitButton::Builder::withOnClick(std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	SplitButton::Builder &SplitButton::Builder::withOnMenuOpen(
		std::function<void(void)> onMenuOpen)
	{
		_onMenuOpen = std::move(onMenuOpen);
		return *this;
	}

	std::shared_ptr<SplitButton> SplitButton::Director::makeSplitButton(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &labelContent, std::function<void(void)> onClick,
		std::function<void(void)> onMenuOpen)
	{
		return builder.withLabel(labelContent)
			.withOnClick(onClick)
			.withOnMenuOpen(onMenuOpen)
			.registerEntity(parent);
	}

	SplitButton::SplitButton(ecs::ComponentRegistry &registry,
							 const std::string &labelContent,
							 std::function<void(void)> onClick,
							 std::function<void(void)> onMenuOpen)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Layout>(registry)
		, _labelContent(labelContent)
		, _onClick(std::move(onClick))
		, _onMenuOpen(std::move(onMenuOpen))
		, _action()
		, _chevron()
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(components::Layout::Axis::Horizontal)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(0.0f)
			.setPadding(0.0f);
	}

	SplitButton::~SplitButton(void)
	{
	}

	ecs::Entity::Identifier SplitButton::getActionIdentifier(void) const
	{
		return _action != nullptr ? _action->getIdentifier()
								  : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier SplitButton::getChevronIdentifier(void) const
	{
		return _chevron != nullptr ? _chevron->getIdentifier()
								   : ecs::Entity::InvalidIdentifier;
	}

	void SplitButton::layoutParts(void)
	{
		systems::Layout::apply(
			getComponentRegistry(), getIdentifier(),
			{ _action->getIdentifier(), _chevron->getIdentifier() },
			static_cast<std::uint32_t>(getLayer()));

		const auto actionBound =
			getComponentRegistry().getComponent<components::Bound>(
				_action->getIdentifier());

		auto &actionBorders =
			getComponentRegistry().getComponent<components::Borders>(
				_action->getIdentifier());
		auto &chevronBorders =
			getComponentRegistry().getComponent<components::Borders>(
				_chevron->getIdentifier());

		const float radius = actionBound.getHeight() * 0.5f;

		actionBorders.setTopLeftRadius(radius)
			.setBottomLeftRadius(radius)
			.setTopRightRadius(0.0f)
			.setBottomRightRadius(0.0f);

		chevronBorders.setTopLeftRadius(0.0f)
			.setBottomLeftRadius(0.0f)
			.setTopRightRadius(radius)
			.setBottomRightRadius(radius);
	}

	void SplitButton::initialize(void)
	{
		Button::Builder actionBuilder(getComponentRegistry(), *this);
		Button::Director actionDirector;
		IconButton::Builder chevronBuilder(getComponentRegistry(), *this);
		IconButton::Director chevronDirector;

		_action = actionDirector.makeButton(
			actionBuilder, shared_from_this(), _labelContent, _onClick,
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false);

		_chevron = chevronDirector.makeIconButton(
			chevronBuilder, shared_from_this(), "arrow_drop_down", _onMenuOpen,
			IconButton::Variant::Filled, IconButton::Size::Medium);
	}

	void SplitButton::update(void)
	{
		_action->update();
		_chevron->update();

		layoutParts();
	}
}	 // namespace guillaume::entities
