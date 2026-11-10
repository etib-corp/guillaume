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
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
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
		makeInteractive(
			getComponentRegistry(), getIdentifier(),
			components::AccessibilityRole::Checkbox,
			[this]() {
				pressHandler();
			},
			[this]() {
				applyVariant();
			});

		// Grow the 18 dp box to the 40 dp target without growing it visually.
		const auto margin =
			static_cast<unsigned int>((TouchTarget - config.fixedWidth) / 2.0f);
		getComponentRegistry()
			.getComponent<components::MouseHoverInteraction>(getIdentifier())
			.setAccessibilityMargin(utility::graphic::SizeUI(margin, margin));
		getComponentRegistry()
			.getComponent<components::HandHoverInteraction>(getIdentifier())
			.setAccessibilityMargin(utility::graphic::SizeUI(margin, margin));

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

		setEnabled(getComponentRegistry(), getIdentifier(),
				   _variant != Variant::Disabled);

		const bool disabled = isDisabled();
		const bool errored	= (_variant == Variant::Error);

		auto &accessibility =
			getComponentRegistry().getComponent<components::Accessibility>(
				getIdentifier());
		accessibility.setLabel(_label).setChecked(
			_state == State::Checked
				? components::CheckedState::Checked
				: (_state == State::Indeterminate
					   ? components::CheckedState::Mixed
					   : components::CheckedState::Unchecked));

		line.setThickness(_state == State::Indeterminate ? 2.0f : 0.0f);

		const auto onColorRole =
			errored ? SchemeColorRole::OnError : SchemeColorRole::OnPrimary;
		// The indeterminate dash shares the box entity, so it must carry its
		// own color instead of the container fill.
		line.setColor(schemeColor(onColorRole));
		const auto containerRole =
			errored ? SchemeColorRole::Error : SchemeColorRole::Primary;

		switch (_state) {
			case State::Checked:
			case State::Indeterminate: {
				if (disabled) {
					setColor(disabledContentColor());
				} else {
					setColor(stateLayerColor(
						getComponentRegistry(), getIdentifier(),
						errored ? schemeColor(containerRole)
								: _checkedColor.value_or(
									  schemeColor(containerRole)),
						schemeColor(onColorRole)));
				}
				borders.setColor(transparentColor()).setWidth(0.0f);
				break;
			}
			case State::Unchecked:
			default: {
				const auto outline = errored
					? schemeColor(SchemeColorRole::Error)
					: _uncheckedColor.value_or(
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
				setColor(disabled ? transparentColor()
								  : stateLayerColor(
										getComponentRegistry(), getIdentifier(),
										transparentColor(), outline));
				borders.setWidth(2.0f);
				borders.setColor(disabled ? disabledContentColor() : outline);
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
				_labelEntity->setColor(disabledContentColor());
			} else if (errored) {
				_labelEntity->setColor(schemeColor(SchemeColorRole::Error));
			} else {
				_labelEntity->setColor(schemeColor(SchemeColorRole::OnSurface));
			}
		}

		placeLabel();
	}

	void Checkbox::placeLabel(void)
	{
		if (_labelEntity == nullptr) {
			return;
		}

		const auto &box =
			getComponentRegistry().getComponent<components::Bound>(
				getIdentifier());
		const auto &label =
			getComponentRegistry().getComponent<components::Bound>(
				_labelEntity->getIdentifier());

		// The label starts after the 40 dp target, vertically centered on the
		// box, so it never overlaps the 18 dp surface.
		const float x = (box.getWidth() + TouchTarget) / 2.0f + LabelGap;
		const float y = (box.getHeight() - label.getHeight()) / 2.0f;
		placeChild(getComponentRegistry(), getIdentifier(),
				   _labelEntity->getIdentifier(),
				   utility::graphic::PositionF(x, y, 0.0f),
				   static_cast<std::uint32_t>(getLayer()));
	}

	void Checkbox::pressHandler(void)
	{
		toggle();
	}

	Checkbox &Checkbox::toggle(void)
	{
		if (isDisabled()) {
			return *this;
		}

		setState(_state == State::Checked ? State::Unchecked : State::Checked);
		if (_onValueChange) {
			_onValueChange(_state);
		}
		return *this;
	}

	bool Checkbox::isDisabled(void) const
	{
		return _variant == Variant::Disabled
			|| !isEnabled(getComponentRegistry(), getIdentifier());
	}

	Checkbox &
		Checkbox::setOnValueChange(std::function<void(State)> onValueChange)
	{
		_onValueChange = std::move(onValueChange);
		return *this;
	}

	Checkbox &Checkbox::setCheckedColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		_checkedColor = color;
		applyVariant();
		return *this;
	}

	Checkbox &Checkbox::setUncheckedColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		_uncheckedColor = color;
		applyVariant();
		return *this;
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
			_labelEntity =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  _label, theme::TypeRole::BodyLarge,
						  schemeColor(SchemeColorRole::OnSurface));
		}

		// Only the check glyph is laid out inside the box; the label is placed
		// beside it by placeLabel().
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (_checkIcon != nullptr) {
			children.push_back(_checkIcon);
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
