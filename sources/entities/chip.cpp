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

		_config.borderRadius = 16.0f;
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

	std::shared_ptr<Chip> Chip::Director::makeChip(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &label, const std::string &iconGlyph, bool selected)
	{
		return builder.withVariant(variant)
			.withLabel(label)
			.withIcon(iconGlyph)
			.withSelected(selected)
			.registerEntity(parent);
	}

	Chip::Chip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &label,
			   const std::string &iconGlyph, bool selected)
		: SurfaceBase<components::Selectable>(registry, config)
		, _variant(variant)
		, _label(label)
		, _iconGlyph(iconGlyph)
		, _leadingIcon()
		, _trailingIcon()
		, _labelEntity()
	{
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setSelected(selected)
			.setOnSelectionChangedHandler([this](bool) {
				applyVariant();
			});

		applyVariant();
	}

	Chip::~Chip(void)
	{
	}

	void Chip::applyVariant(void)
	{
		auto &borders =
			getComponentRegistry().getComponent<components::Borders>(
				getIdentifier());

		const bool selected =
			getComponentRegistry()
				.getComponent<components::Selectable>(getIdentifier())
				.isSelected();

		auto contentColor = schemeColor(SchemeColorRole::OnSurfaceVariant);

		if (_variant == Variant::Filter && selected) {
			setColor(schemeColor(SchemeColorRole::SecondaryContainer));
			borders.setColor(transparentColor());
			contentColor = schemeColor(SchemeColorRole::OnSecondaryContainer);
		} else {
			setColor(transparentColor());
			borders.setColor(schemeColor(SchemeColorRole::Outline));
			contentColor = schemeColor(SchemeColorRole::OnSurfaceVariant);
		}

		if (_leadingIcon != nullptr) {
			_leadingIcon->setColor(contentColor);
		}
		if (_trailingIcon != nullptr) {
			_trailingIcon->setColor(contentColor);
		}
		if (_labelEntity != nullptr) {
			_labelEntity->setColor(contentColor);
		}
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
		SurfaceBase::initialize();

		const auto contentColor =
			schemeColor(SchemeColorRole::OnSurfaceVariant);

		std::string leadingGlyph = _iconGlyph;

		switch (_variant) {
			case Variant::Assist:
				if (leadingGlyph.empty()) {
					leadingGlyph = "event";
				}
				break;
			case Variant::Filter:
				if (leadingGlyph.empty()) {
					leadingGlyph = "check";
				}
				break;
			case Variant::Input:
			case Variant::Suggestion:
			default:
				break;
		}

		if (!leadingGlyph.empty()) {
			_leadingIcon =
				buildIcon(getComponentRegistry(), *this, shared_from_this(),
						  leadingGlyph, 18.0f, contentColor);
		}

		if (!_label.empty()) {
			_labelEntity =
				buildText(getComponentRegistry(), *this, shared_from_this(),
						  _label, 14.0f, contentColor);
		}

		if (_variant == Variant::Input) {
			_trailingIcon =
				buildIcon(getComponentRegistry(), *this, shared_from_this(),
						  "cancel", 18.0f, contentColor);
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (_leadingIcon != nullptr) {
			children.push_back(_leadingIcon);
		}
		if (_labelEntity != nullptr) {
			children.push_back(_labelEntity);
		}
		if (_trailingIcon != nullptr) {
			children.push_back(_trailingIcon);
		}
		setChildren(children);

		applyVariant();
	}

	void Chip::update(void)
	{
		SurfaceBase::update();
		applyVariant();
	}

}	 // namespace guillaume::entities
