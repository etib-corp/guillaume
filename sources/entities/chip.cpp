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

#include "guillaume/entities/chip.hpp"

#include <memory>
#include <vector>

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Chip::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Chip>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Chip> Chip::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Chip>(this->getComponentRegistry(), _config,
								   _variant, _label, _iconGlyph, _selected);
		return entity;
	}

	void Chip::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Assist;
		_label.clear();
		_iconGlyph.clear();
		_selected = false;

		_config.borderRadius = getShapeRadius(theme::ShapeRole::Small);
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 12.0f;
		_config.setFixedHeight(32.0f);
	}

	Chip::Builder &Chip::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Chip::Builder &Chip::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Chip::Builder &Chip::Builder::withLabel(const std::string &label)
	{
		_label = label;
		return *this;
	}

	Chip::Builder &Chip::Builder::withIcon(const std::string &iconGlyph)
	{
		_iconGlyph = iconGlyph;
		return *this;
	}

	Chip::Builder &Chip::Builder::withSelected(bool selected)
	{
		_selected = selected;
		return *this;
	}

	std::shared_ptr<Chip>
		Chip::Builder::makeChip(std::shared_ptr<ecs::Entity> parent,
								Variant variant, const std::string &label,
								const std::string &iconGlyph, bool selected)
	{
		return withVariant(variant)
			.withLabel(label)
			.withIcon(iconGlyph)
			.withSelected(selected)
			.registerEntity(parent);
	}

	Chip::Chip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &label,
			   const std::string &iconGlyph, bool selected)
		: SurfaceBase<components::Selectable, components::Elevation>(registry,
																	 config)
		, _variant(variant)
		, _label(label)
		, _iconGlyph(iconGlyph)
		, _leadingIcon()
		, _trailingIcon()
		, _labelEntity()
		, _checkIcon()
	{
		makeInteractive(
			getComponentRegistry(), getIdentifier(),
			components::AccessibilityRole::Button,
			[this]() {
				pressHandler();
			},
			[this]() {
				applyVariant();
			});
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(label);

		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setSelected(selected)
			.setOnSelectionChangedHandler([this](bool) {
				applyVariant();
			});

		applyVariant();
	}

	void Chip::pressHandler(void)
	{
		if (isDisabled()) {
			return;
		}
		if (_variant == Variant::Filter) {
			setSelected(!isSelected());
		}
		if (_onPress) {
			_onPress();
		}
	}

	Chip::~Chip(void)
	{
	}

	void Chip::applyVariant(void)
	{
		auto &borders =
			getComponentRegistry().getComponent<components::Borders>(
				getIdentifier());

		const bool selected = isSelected();
		const bool disabled = isDisabled();
		const bool outlined = (_mode == Mode::Outlined);

		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setSelected(selected);

		// Paper v5 Chip/helpers.tsx: flat chips sit on secondaryContainer;
		// outlined chips are transparent with an outline until selected.
		utility::graphic::Color32Bit container = transparentColor();
		if (!outlined || selected) {
			container = schemeColor(SchemeColorRole::SecondaryContainer);
		}
		auto content   = (outlined && !selected)
			  ? schemeColor(SchemeColorRole::OnSurfaceVariant)
			  : schemeColor(SchemeColorRole::OnSecondaryContainer);
		auto iconColor = selected
			? schemeColor(SchemeColorRole::OnSecondaryContainer)
			: schemeColor(SchemeColorRole::Primary);

		if (disabled) {
			container =
				outlined ? transparentColor() : disabledContainerColor();
			content	  = disabledContentColor();
			iconColor = disabledContentColor();
		} else {
			if (selected && _showSelectedOverlay) {
				container =
					stateLayer(container, content, theme::StateLayer::Pressed);
			}
			container = stateLayerColor(getComponentRegistry(), getIdentifier(),
										container, content);
		}

		setColor(container);

		if (outlined && !selected) {
			borders
				.setColor(disabled ? disabledContainerColor()
								   : schemeColor(SchemeColorRole::Outline))
				.setWidth(1.0f);
		} else {
			borders.setColor(transparentColor()).setWidth(0.0f);
		}

		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel((_elevated && !disabled)
								   ? theme::ElevationLevel::Level1
								   : theme::ElevationLevel::Level0);

		auto config	   = getSurfaceConfig();
		config.padding = _compact ? 8.0f : 12.0f;
		setSurfaceConfig(config);

		if (_leadingIcon != nullptr) {
			_leadingIcon->setColor(iconColor);
		}
		if (_checkIcon != nullptr) {
			_checkIcon->setColor((selected && _showSelectedCheck)
									 ? iconColor
									 : transparentColor());
		}
		if (_trailingIcon != nullptr) {
			_trailingIcon->setColor(content);
		}
		if (_labelEntity != nullptr) {
			_labelEntity->setColor(content);
		}

		attachChildren();
	}

	void Chip::attachChildren(void)
	{
		if (_labelEntity == nullptr && _leadingIcon == nullptr
			&& _checkIcon == nullptr && _trailingIcon == nullptr) {
			return;
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		// The selected check replaces the leading icon (Paper
		// showSelectedCheck); it is not laid out while unselected.
		if (_checkIcon != nullptr && isSelected() && _showSelectedCheck) {
			children.push_back(_checkIcon);
		} else if (_leadingIcon != nullptr) {
			children.push_back(_leadingIcon);
		}
		if (_labelEntity != nullptr) {
			children.push_back(_labelEntity);
		}
		if (_trailingIcon != nullptr) {
			children.push_back(_trailingIcon);
		}
		setChildren(children);
	}

	Chip &Chip::setMode(Mode mode)
	{
		_mode = mode;
		applyVariant();
		return *this;
	}

	Chip::Mode Chip::getMode(void) const
	{
		return _mode;
	}

	Chip &Chip::setDisabled(bool disabled)
	{
		setEnabled(getComponentRegistry(), getIdentifier(), !disabled);
		applyVariant();
		return *this;
	}

	bool Chip::isDisabled(void) const
	{
		return !isEnabled(getComponentRegistry(), getIdentifier());
	}

	Chip &Chip::setCompact(bool compact)
	{
		_compact = compact;
		applyVariant();
		return *this;
	}

	Chip &Chip::setElevated(bool elevated)
	{
		_elevated = elevated;
		applyVariant();
		return *this;
	}

	Chip &Chip::setShowSelectedOverlay(bool show)
	{
		_showSelectedOverlay = show;
		applyVariant();
		return *this;
	}

	Chip &Chip::setShowSelectedCheck(bool show)
	{
		_showSelectedCheck = show;
		applyVariant();
		return *this;
	}

	Chip &Chip::setOnPress(std::function<void(void)> onPress)
	{
		_onPress = std::move(onPress);
		return *this;
	}

	Chip &Chip::setOnClose(std::function<void(void)> onClose)
	{
		_onClose = std::move(onClose);
		return *this;
	}

	ecs::Entity::Identifier Chip::getCloseIconIdentifier(void) const
	{
		return _trailingIcon != nullptr ? _trailingIcon->getIdentifier()
										: ecs::Entity::InvalidIdentifier;
	}

	Chip &Chip::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Chip::Variant Chip::getVariant(void) const
	{
		return _variant;
	}

	Chip &Chip::setLabel(const std::string &label)
	{
		_label = label;

		if (_labelEntity != nullptr) {
			_labelEntity->setContent(label);
		}

		applyVariant();

		return *this;
	}

	Chip &Chip::setSelected(bool selected)
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setSelected(selected);

		applyVariant();

		return *this;
	}

	bool Chip::isSelected(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	void Chip::initialize(void)
	{
		update();

		const auto contentColor =
			schemeColor(SchemeColorRole::OnSecondaryContainer);

		std::string leadingGlyph = _iconGlyph;
		if (_variant == Variant::Assist && leadingGlyph.empty()) {
			leadingGlyph = "event";
		}

		if (!leadingGlyph.empty()) {
			_leadingIcon =
				buildIcon(getComponentRegistry(), *this, shared_from_this(),
						  leadingGlyph, 18.0f, contentColor);
		}

		if (_variant == Variant::Filter) {
			_checkIcon =
				buildIcon(getComponentRegistry(), *this, shared_from_this(),
						  "check", 18.0f, contentColor);
		}

		if (!_label.empty()) {
			_labelEntity =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  _label, theme::TypeRole::LabelLarge, contentColor);
		}

		if (_variant == Variant::Input || _onClose) {
			_trailingIcon =
				buildIcon(getComponentRegistry(), *this, shared_from_this(),
						  "close", 18.0f, contentColor);
			if (_trailingIcon != nullptr) {
				makeInteractive(
					getComponentRegistry(), _trailingIcon->getIdentifier(),
					components::AccessibilityRole::Button, [this]() {
						if (!isDisabled() && _onClose) {
							_onClose();
						}
					});
				getComponentRegistry()
					.getComponent<components::Accessibility>(
						_trailingIcon->getIdentifier())
					.setLabel("Close");
			}
		}

		applyVariant();
	}

	void Chip::update(void)
	{
		applyGeometry();
		applyVariant();
	}

}	 // namespace guillaume::entities
