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

#include "guillaume/entities/checkbox.hpp"

#include <memory>
#include <vector>

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Checkbox::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Checkbox>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Checkbox> Checkbox::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Checkbox>(
			this->getComponentRegistry(), _config, _state, _variant, _label);
		return entity;
	}

	void Checkbox::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_state	 = State::Unchecked;
		_variant = Variant::Enabled;
		_label.clear();

		_config.borderRadius = 2.0f;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 0.0f;
		_config.setFixedWidth(18.0f);
		_config.setFixedHeight(18.0f);
	}

	Checkbox::Builder &
		Checkbox::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Checkbox::Builder &Checkbox::Builder::withState(State state)
	{
		_state = state;
		return *this;
	}

	Checkbox::Builder &Checkbox::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Checkbox::Builder &Checkbox::Builder::withLabel(const std::string &label)
	{
		_label = label;
		return *this;
	}

	std::shared_ptr<Checkbox> Checkbox::Director::makeCheckbox(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, State state,
		Variant variant, const std::string &label)
	{
		return builder.withState(state)
			.withVariant(variant)
			.withLabel(label)
			.registerEntity(parent);
	}

	Checkbox::Checkbox(ecs::ComponentRegistry &registry,
					   const SurfaceConfig &config, State state,
					   Variant variant, const std::string &label)
		: SurfaceBase<components::Selectable, components::Line>(registry,
																config)
		, _state(state)
		, _variant(variant)
		, _label(label)
		, _checkIcon()
		, _labelEntity()
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setOnSelectionChangedHandler([this](bool selected) {
				if (_state != State::Indeterminate) {
					_state = selected ? State::Checked : State::Unchecked;
				}
				applyVariant();
			});

		applyVariant();
	}

	Checkbox::~Checkbox(void)
	{
	}

	void Checkbox::applyVariant(void)
	{
		auto &borders =
			getComponentRegistry().getComponent<components::Borders>(
				getIdentifier());
		auto &line = getComponentRegistry().getComponent<components::Line>(
			getIdentifier());

		const bool disabled = (_variant == Variant::Disabled);
		const bool errored	= (_variant == Variant::Error);

		line.setThickness(_state == State::Indeterminate ? 2.0f : 0.0f);

		const auto onColorRole =
			errored ? SchemeColorRole::OnError : SchemeColorRole::OnPrimary;
		const auto containerRole =
			errored ? SchemeColorRole::Error : SchemeColorRole::Primary;

		switch (_state) {
			case State::Checked:
			case State::Indeterminate: {
				if (disabled) {
					setColor(withAlpha(schemeColor(containerRole), 97U));
				} else {
					setColor(schemeColor(containerRole));
				}
				borders.setColor(transparentColor());
				break;
			}
			case State::Unchecked:
			default: {
				setColor(transparentColor());
				if (disabled) {
					borders.setColor(withAlpha(
						schemeColor(SchemeColorRole::OnSurface), 97U));
				} else {
					borders.setColor(
						schemeColor(SchemeColorRole::OnSurfaceVariant));
				}
				break;
			}
		}

		if (_checkIcon != nullptr) {
			const bool visible = (_state == State::Checked);
			_checkIcon->setColor(visible ? schemeColor(onColorRole)
										 : transparentColor());
		}

		if (_labelEntity != nullptr) {
			if (disabled) {
				_labelEntity->setColor(
					withAlpha(schemeColor(SchemeColorRole::OnSurface), 97U));
			} else if (errored) {
				_labelEntity->setColor(schemeColor(SchemeColorRole::Error));
			} else {
				_labelEntity->setColor(schemeColor(SchemeColorRole::OnSurface));
			}
		}
	}

	Checkbox &Checkbox::setState(State state)
	{
		_state = state;

		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setSelected(state != State::Unchecked);

		applyVariant();

		return *this;
	}

	Checkbox::State Checkbox::getState(void) const
	{
		return _state;
	}

	Checkbox &Checkbox::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Checkbox::Variant Checkbox::getVariant(void) const
	{
		return _variant;
	}

	Checkbox &Checkbox::setLabel(const std::string &label)
	{
		_label = label;

		if (_labelEntity != nullptr) {
			_labelEntity->setContent(label);
		}

		applyVariant();

		return *this;
	}

	bool Checkbox::isChecked(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	void Checkbox::initialize(void)
	{
		SurfaceBase::initialize();

		_checkIcon =
			buildIcon(getComponentRegistry(), *this, shared_from_this(),
					  "check", 14.0f, schemeColor(SchemeColorRole::OnPrimary));

		if (!_label.empty()) {
			_labelEntity = buildText(getComponentRegistry(), *this,
									 shared_from_this(), _label, 14.0f,
									 schemeColor(SchemeColorRole::OnSurface));
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (_checkIcon != nullptr) {
			children.push_back(_checkIcon);
		}
		if (_labelEntity != nullptr) {
			children.push_back(_labelEntity);
		}
		setChildren(children);

		applyVariant();
	}

	void Checkbox::update(void)
	{
		SurfaceBase::update();
		applyVariant();
	}

}	 // namespace guillaume::entities
