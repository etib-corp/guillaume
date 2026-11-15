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
#include "guillaume/systems/checkbox.hpp"
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
		auto entity = Checkbox::create(this->getComponentRegistry(), _config,
									   _state, _variant, _label);
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

	std::shared_ptr<Checkbox>
		Checkbox::Builder::makeCheckbox(std::shared_ptr<ecs::Entity> parent,
										State state, Variant variant,
										const std::string &label)
	{
		return withState(state)
			.withVariant(variant)
			.withLabel(label)
			.registerEntity(parent);
	}

	Checkbox::Checkbox(ecs::ComponentRegistry &registry,
					   const SurfaceConfig &config, State state,
					   Variant variant, const std::string &label)
		: SurfaceBase<components::Selectable, components::Line,
					  components::Checkbox>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.setState(state)
			.setVariant(variant)
			.setLabel(label);
		systems::Checkbox::install(getComponentRegistry(), getIdentifier());
		systems::Checkbox::apply(getComponentRegistry(), getIdentifier());
	}

	Checkbox &Checkbox::toggle(void)
	{
		systems::Checkbox::toggle(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool Checkbox::isDisabled(void) const
	{
		return systems::Checkbox::isDisabled(getComponentRegistry(),
											 getIdentifier());
	}

	Checkbox &
		Checkbox::setOnValueChange(std::function<void(State)> onValueChange)
	{
		getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.setOnValueChange(std::move(onValueChange));
		return *this;
	}

	Checkbox &Checkbox::setCheckedColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.setCheckedColor(color);
		systems::Checkbox::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Checkbox &Checkbox::setUncheckedColor(
		std::optional<utility::graphic::Color32Bit> color)
	{
		getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.setUncheckedColor(color);
		systems::Checkbox::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Checkbox &Checkbox::setState(State state)
	{
		systems::Checkbox::setState(getComponentRegistry(), getIdentifier(),
									state);
		return *this;
	}

	Checkbox::State Checkbox::getState(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.getState();
	}

	Checkbox &Checkbox::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.setVariant(variant);
		systems::Checkbox::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Checkbox::Variant Checkbox::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Checkbox>(getIdentifier())
			.getVariant();
	}

	Checkbox &Checkbox::setLabel(const std::string &label)
	{
		auto &registry = getComponentRegistry();
		auto &checkbox =
			registry.getComponent<components::Checkbox>(getIdentifier());

		checkbox.setLabel(label);
		if (registry.hasComponent<components::Text>(
				checkbox.getLabelEntity())) {
			registry.getComponent<components::Text>(checkbox.getLabelEntity())
				.setContent(label);
		}
		systems::Checkbox::apply(registry, getIdentifier());
		return *this;
	}

	bool Checkbox::isChecked(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Selectable>(getIdentifier())
			.isSelected();
	}

	void Checkbox::build(void)
	{
		auto &registry = getComponentRegistry();
		auto &checkbox =
			registry.getComponent<components::Checkbox>(getIdentifier());

		// Only the check glyph is laid out inside the box; the label is placed
		// beside it by systems::Checkbox.
		const auto checkIcon =
			buildIcon(registry, *this, shared_from_this(), "check", 14.0f,
					  schemeColor(SchemeColorRole::OnPrimary));
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (checkIcon != nullptr) {
			checkbox.setCheckIcon(checkIcon->getIdentifier());
			children.push_back(checkIcon);
		}

		if (!checkbox.getLabel().empty()) {
			const auto label =
				buildText(registry, *this, shared_from_this(),
						  checkbox.getLabel(), theme::TypeRole::BodyLarge,
						  schemeColor(SchemeColorRole::OnSurface));
			if (label != nullptr) {
				checkbox.setLabelEntity(label->getIdentifier());
			}
		}

		setChildren(children);
		systems::Checkbox::apply(registry, getIdentifier());
	}

}	 // namespace guillaume::entities
