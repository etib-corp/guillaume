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

#include "guillaume/entities/standard_button_group.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::entities
{
	StandardButtonGroup::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<StandardButtonGroup>(componentRegistry,
												 entityRegistry)
	{
		reset();
	}

	std::shared_ptr<StandardButtonGroup>
		StandardButtonGroup::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<StandardButtonGroup>(
			this->getComponentRegistry(), _labels, _gap);
		return entity;
	}

	void StandardButtonGroup::Builder::reset(void)
	{
		_labels.clear();
		_gap = 8.0f;
	}

	StandardButtonGroup::Builder &StandardButtonGroup::Builder::withLabels(
		const std::vector<std::string> &labels)
	{
		_labels = labels;
		return *this;
	}

	StandardButtonGroup::Builder &
		StandardButtonGroup::Builder::withGap(const float &gap)
	{
		_gap = gap;
		return *this;
	}

	std::shared_ptr<StandardButtonGroup>
		StandardButtonGroup::Director::makeStandardButtonGroup(
			Builder &builder, std::shared_ptr<Entity> parent,
			const std::vector<std::string> &labels)
	{
		return builder.withLabels(labels).registerEntity(parent);
	}

	StandardButtonGroup::StandardButtonGroup(
		ecs::ComponentRegistry &registry,
		const std::vector<std::string> &labels, const float &gap)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Layout>(registry)
		, _labels(labels)
		, _gap(gap)
		, _buttons()
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(components::Layout::Axis::Horizontal)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(_gap)
			.setPadding(0.0f);
	}

	StandardButtonGroup::~StandardButtonGroup(void)
	{
	}

	std::size_t StandardButtonGroup::getButtonCount(void) const
	{
		return _buttons.size();
	}

	ecs::Entity::Identifier
		StandardButtonGroup::getButtonIdentifier(std::size_t index) const
	{
		if (index >= _buttons.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _buttons[index]->getIdentifier();
	}

	void StandardButtonGroup::layoutButtons(void)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_buttons.size());

		for (const auto &button: _buttons) {
			childIdentifiers.push_back(button->getIdentifier());
		}

		systems::Layout::apply(getComponentRegistry(), getIdentifier(),
							   childIdentifiers,
							   static_cast<std::uint32_t>(getLayer()));
	}

	void StandardButtonGroup::initialize(void)
	{
		Button::Builder buttonBuilder(getComponentRegistry(), *this);
		Button::Director buttonDirector;

		for (const auto &label: _labels) {
			auto button = buttonDirector.makeButton(
				buttonBuilder, shared_from_this(), label,
				std::function<void(void)>(), Button::Color::Outlined,
				Button::Shape::Round, Button::Size::Medium, false);

			_buttons.push_back(button);
		}
	}

	void StandardButtonGroup::update(void)
	{
		for (auto &button: _buttons) {
			button->update();
		}

		layoutButtons();
	}
}	 // namespace guillaume::entities
