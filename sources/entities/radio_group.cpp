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

#include <utility>

#include "guillaume/systems/radio_group.hpp"

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
		return RadioGroup::create(this->getComponentRegistry(), _config,
								  _options, _value, _onValueChange);
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
		: SurfaceBase<components::RadioGroup>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::RadioGroup>(getIdentifier())
			.setOptions(options)
			.setValue(value)
			.setOnValueChange(std::move(onValueChange));
		systems::RadioGroup::install(getComponentRegistry(), getIdentifier());
	}

	RadioGroup &RadioGroup::setValue(std::size_t index)
	{
		systems::RadioGroup::select(getComponentRegistry(), getIdentifier(),
									index);
		return *this;
	}

	std::optional<std::size_t> RadioGroup::getValue(void) const
	{
		return getComponentRegistry()
			.getComponent<components::RadioGroup>(getIdentifier())
			.getValue();
	}

	std::size_t RadioGroup::getOptionCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::RadioGroup>(getIdentifier())
			.getOptions()
			.size();
	}

	std::shared_ptr<RadioButton> RadioGroup::getOption(std::size_t index) const
	{
		const auto &buttons =
			getComponentRegistry()
				.getComponent<components::RadioGroup>(getIdentifier())
				.getButtons();

		if (index >= buttons.size()) {
			return nullptr;
		}
		// The lookup returns a pointer to const because this getter is
		// const; the child is shared and callers may modify it, hence the
		// cast.
		return std::const_pointer_cast<RadioButton>(
			getEntity<RadioButton>(buttons[index]));
	}

	RadioGroup &RadioGroup::setOnValueChange(
		std::function<void(std::size_t)> onValueChange)
	{
		getComponentRegistry()
			.getComponent<components::RadioGroup>(getIdentifier())
			.setOnValueChange(std::move(onValueChange));
		return *this;
	}

	void RadioGroup::build(void)
	{
		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const std::vector<std::string> options =
			registry.getComponent<components::RadioGroup>(getIdentifier())
				.getOptions();

		std::vector<ecs::Entity::Identifier> identifiers;
		Children children;
		RadioButton::Builder builder(registry, *this);
		for (const auto &option: options) {
			const auto button =
				builder.withLabel(option).registerEntity(shared_from_this());
			identifiers.push_back(button->getIdentifier());
			children.push_back(button);
		}

		registry.getComponent<components::RadioGroup>(getIdentifier())
			.setButtons(identifiers);
		setChildren(children);
		systems::RadioGroup::install(registry, getIdentifier());
		systems::RadioGroup::apply(registry, getIdentifier());
	}

}	 // namespace guillaume::entities
