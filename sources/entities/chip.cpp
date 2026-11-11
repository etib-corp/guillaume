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
#include "guillaume/systems/chip.hpp"
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
		auto entity = Chip::create(this->getComponentRegistry(), _config,
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
		: SurfaceBase<components::Selectable, components::Elevation,
					  components::Chip>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setVariant(variant)
			.setLabel(label)
			.setIconGlyph(iconGlyph);
		getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.setSelected(selected);
		systems::Chip::install(getComponentRegistry(), getIdentifier());
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
	}

	Chip::~Chip(void)
	{
	}

	Chip &Chip::setMode(Mode mode)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setMode(mode);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip::Mode Chip::getMode(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.getMode();
	}

	Chip &Chip::setDisabled(bool disabled)
	{
		systems::Chip::setDisabled(getComponentRegistry(), getIdentifier(),
								   disabled);
		return *this;
	}

	bool Chip::isDisabled(void) const
	{
		return systems::Chip::isDisabled(getComponentRegistry(),
										 getIdentifier());
	}

	Chip &Chip::setCompact(bool compact)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setCompact(compact);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip &Chip::setElevated(bool elevated)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setElevated(elevated);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip &Chip::setShowSelectedOverlay(bool show)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setShowSelectedOverlay(show);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip &Chip::setShowSelectedCheck(bool show)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setShowSelectedCheck(show);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip &Chip::setOnPress(std::function<void(void)> onPress)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setOnPress(std::move(onPress));
		return *this;
	}

	Chip &Chip::setOnClose(std::function<void(void)> onClose)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setOnClose(std::move(onClose));
		return *this;
	}

	ecs::Entity::Identifier Chip::getCloseIconIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.getTrailingIcon();
	}

	Chip &Chip::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setVariant(variant);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip::Variant Chip::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.getVariant();
	}

	Chip &Chip::setLabel(const std::string &label)
	{
		getComponentRegistry()
			.getComponent<components::Chip>(getIdentifier())
			.setLabel(label);
		systems::Chip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Chip &Chip::setSelected(bool selected)
	{
		systems::Chip::setSelected(getComponentRegistry(), getIdentifier(),
								   selected);
		return *this;
	}

	bool Chip::isSelected(void) const
	{
		return systems::Chip::isSelected(getComponentRegistry(),
										 getIdentifier());
	}

	void Chip::build(void)
	{
		auto &registry = getComponentRegistry();
		auto &chip = registry.getComponent<components::Chip>(getIdentifier());
		const auto contentColor =
			schemeColor(SchemeColorRole::OnSecondaryContainer);
		std::vector<std::shared_ptr<ecs::Entity>> children;

		// Which children exist is decided by systems::Chip; they are only
		// built here because that needs shared_from_this().
		const auto leadingIcon =
			buildIcon(registry, *this, shared_from_this(),
					  systems::Chip::leadingGlyph(registry, getIdentifier()),
					  components::Chip::IconSize, contentColor);
		if (leadingIcon != nullptr) {
			chip.setLeadingIcon(leadingIcon->getIdentifier());
			children.push_back(leadingIcon);
		}

		if (systems::Chip::needsCheckIcon(registry, getIdentifier())) {
			const auto checkIcon =
				buildIcon(registry, *this, shared_from_this(),
						  components::Chip::CheckGlyph,
						  components::Chip::IconSize, contentColor);
			if (checkIcon != nullptr) {
				chip.setCheckIcon(checkIcon->getIdentifier());
				children.push_back(checkIcon);
			}
		}

		const auto label =
			buildText(registry, *this, shared_from_this(), chip.getLabel(),
					  theme::TypeRole::LabelLarge, contentColor);
		if (label != nullptr) {
			chip.setLabelEntity(label->getIdentifier());
			children.push_back(label);
		}

		if (systems::Chip::needsCloseIcon(registry, getIdentifier())) {
			const auto closeIcon =
				buildIcon(registry, *this, shared_from_this(),
						  components::Chip::CloseGlyph,
						  components::Chip::IconSize, contentColor);
			if (closeIcon != nullptr) {
				chip.setTrailingIcon(closeIcon->getIdentifier());
				children.push_back(closeIcon);
				systems::Chip::installCloseIcon(registry, getIdentifier());
			}
		}

		// Every child is attached; systems::Chip then narrows the layout
		// children to the ones currently shown.
		if (!children.empty()) {
			setChildren(children);
		}
		systems::Chip::apply(registry, getIdentifier());
	}

}	 // namespace guillaume::entities
