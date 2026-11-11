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

#include "guillaume/entities/radio_group.hpp"

#include <stdexcept>
#include <utility>

#include "guillaume/components/accessibility.hpp"

namespace guillaume::entities
{
	RadioGroup::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<RadioGroup>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<RadioGroup> RadioGroup::Builder::buildEntity(void)
	{
		return std::make_shared<RadioGroup>(this->getComponentRegistry(),
											_config, _options, _value,
											_onValueChange);
	}

	void RadioGroup::Builder::reset(void)
	{
		_config = SurfaceConfig();
		_options.clear();
		_value.reset();
		_onValueChange = {};

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.spacing = RowSpacing;
		_config.padding = (RadioButton::TouchTarget - 20.0f) / 2.0f;
	}

	RadioGroup::Builder &
		RadioGroup::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	RadioGroup::Builder &
		RadioGroup::Builder::addOption(const std::string &label)
	{
		_options.push_back(label);
		return *this;
	}

	RadioGroup::Builder &RadioGroup::Builder::withValue(std::size_t index)
	{
		_value = index;
		return *this;
	}

	RadioGroup::Builder &RadioGroup::Builder::withOnValueChange(
		std::function<void(std::size_t)> onValueChange)
	{
		_onValueChange = std::move(onValueChange);
		return *this;
	}

	std::shared_ptr<RadioGroup> RadioGroup::Builder::makeRadioGroup(
		std::shared_ptr<ecs::Entity> parent,
		const std::vector<std::string> &options)
	{
		for (const auto &option: options) {
			addOption(option);
		}
		return registerEntity(parent);
	}

	RadioGroup::RadioGroup(ecs::ComponentRegistry &registry,
						   const SurfaceConfig &config,
						   const std::vector<std::string> &options,
						   std::optional<std::size_t> value,
						   std::function<void(std::size_t)> onValueChange)
		: SurfaceBase<>(registry, config)
		, _options(options)
		, _value(value)
		, _onValueChange(std::move(onValueChange))
	{
		if (!getComponentRegistry().hasComponent<components::Accessibility>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Accessibility>(
				getIdentifier());
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::RadioGroup)
			.setFocusable(false);
	}

	RadioGroup &RadioGroup::setValue(std::size_t index)
	{
		if (index >= _options.size()) {
			throw std::out_of_range("RadioGroup option index out of range");
		}
		_value = index;
		applySelection();
		return *this;
	}

	std::optional<std::size_t> RadioGroup::getValue(void) const
	{
		return _value;
	}

	std::size_t RadioGroup::getOptionCount(void) const
	{
		return _options.size();
	}

	std::shared_ptr<RadioButton> RadioGroup::getOption(std::size_t index) const
	{
		return index < _buttons.size() ? _buttons[index] : nullptr;
	}

	RadioGroup &RadioGroup::setOnValueChange(
		std::function<void(std::size_t)> onValueChange)
	{
		_onValueChange = std::move(onValueChange);
		return *this;
	}

	void RadioGroup::applySelection(void)
	{
		for (std::size_t i = 0; i < _buttons.size(); ++i) {
			const bool selected = _value.has_value() && *_value == i;
			if (_buttons[i]->isSelected() != selected) {
				_buttons[i]->select(selected);
			}
		}
	}

	void RadioGroup::initialize(void)
	{
		SurfaceBase::initialize();

		_buttons.clear();
		RadioButton::Builder builder(getComponentRegistry(), *this);
		for (std::size_t i = 0; i < _options.size(); ++i) {
			auto button = builder.withLabel(_options[i])
							  .registerEntity(shared_from_this());
			button->setOnPress([this, i]() {
				const bool changed = !_value.has_value() || *_value != i;
				_value			   = i;
				applySelection();
				if (changed && _onValueChange) {
					_onValueChange(i);
				}
			});
			_buttons.push_back(button);
		}

		setChildren(Children(_buttons.begin(), _buttons.end()));
		applySelection();
	}

	void RadioGroup::update(void)
	{
		SurfaceBase::update();
		applySelection();
	}

}	 // namespace guillaume::entities
