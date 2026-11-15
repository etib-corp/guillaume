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

#include "guillaume/entities/tooltip.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/tooltip.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Tooltip::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Tooltip>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Tooltip> Tooltip::Builder::buildEntity(void)
	{
		auto entity = Tooltip::create(this->getComponentRegistry(), _config,
									  _variant, _text, _title);
		return entity;
	}

	void Tooltip::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Plain;
		_text.clear();
		_title.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = 8.0f;
		_config.spacing = 2.0f;
	}

	Tooltip::Builder &
		Tooltip::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withText(const std::string &text)
	{
		_text = text;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withTitle(const std::string &title)
	{
		_title = title;
		return *this;
	}

	Tooltip::Builder &Tooltip::Builder::withFixedWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	std::shared_ptr<Tooltip>
		Tooltip::Builder::makeTooltip(std::shared_ptr<ecs::Entity> parent,
									  Variant variant, const std::string &text,
									  const std::string &title)
	{
		return withVariant(variant)
			.withText(text)
			.withTitle(title)
			.registerEntity(parent);
	}

	Tooltip::Tooltip(ecs::ComponentRegistry &registry,
					 const SurfaceConfig &config, Variant variant,
					 const std::string &text, const std::string &title)
		: SurfaceBase<components::Overlay, components::Elevation,
					  components::Animation, components::Accessibility,
					  components::Tooltip>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.setVariant(variant)
			.setText(text)
			.setTitle(title);
		systems::Tooltip::install(getComponentRegistry(), getIdentifier());
		systems::Tooltip::apply(getComponentRegistry(), getIdentifier());
	}

	Tooltip &Tooltip::setAnchor(std::shared_ptr<ecs::Entity> anchor)
	{
		systems::Tooltip::setAnchor(getComponentRegistry(), getIdentifier(),
									anchor != nullptr
										? anchor->getIdentifier()
										: ecs::Entity::InvalidIdentifier);
		return *this;
	}

	Tooltip &Tooltip::setEnterDelay(float seconds)
	{
		getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.setEnterDelay(seconds);
		return *this;
	}

	Tooltip &Tooltip::setLeaveDelay(float seconds)
	{
		getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.setLeaveDelay(seconds);
		return *this;
	}

	float Tooltip::getEnterDelay(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.getEnterDelay();
	}

	float Tooltip::getLeaveDelay(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.getLeaveDelay();
	}

	Tooltip &Tooltip::setViewport(float width, float height)
	{
		systems::Tooltip::setViewport(getComponentRegistry(), getIdentifier(),
									  width, height);
		return *this;
	}

	Tooltip &Tooltip::show(void)
	{
		systems::Tooltip::show(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Tooltip &Tooltip::hide(void)
	{
		systems::Tooltip::hide(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool Tooltip::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.isVisible();
	}

	void Tooltip::advance(float deltaTime)
	{
		systems::Tooltip::advance(getComponentRegistry(), getIdentifier(),
								  deltaTime);
	}

	void Tooltip::place(void)
	{
		systems::Tooltip::place(getComponentRegistry(), getIdentifier());
	}

	Tooltip &Tooltip::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.setVariant(variant);
		buildContent();
		systems::Tooltip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Tooltip::Variant Tooltip::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.getVariant();
	}

	Tooltip &Tooltip::setText(const std::string &text)
	{
		getComponentRegistry()
			.getComponent<components::Tooltip>(getIdentifier())
			.setText(text);
		buildContent();
		systems::Tooltip::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	void Tooltip::build(void)
	{
		buildContent();
		systems::Tooltip::apply(getComponentRegistry(), getIdentifier());
	}

	void Tooltip::buildContent(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		auto &tooltip =
			registry.getComponent<components::Tooltip>(getIdentifier());

		// Children are built transparent: systems::Tooltip colors them once
		// the tooltip is shown.
		if (tooltip.getVariant() == Variant::Rich
			&& tooltip.getTitleEntity() == ecs::Entity::InvalidIdentifier) {
			const auto title = buildText(
				registry, *this, shared_from_this(), tooltip.getTitle(),
				theme::TypeRole::TitleSmall, transparentColor());
			if (title != nullptr) {
				tooltip.setTitleEntity(title->getIdentifier());
			}
		}

		if (tooltip.getTextEntity() == ecs::Entity::InvalidIdentifier) {
			const auto text = buildText(
				registry, *this, shared_from_this(), tooltip.getText(),
				theme::TypeRole::BodySmall, transparentColor());
			if (text != nullptr) {
				tooltip.setTextEntity(text->getIdentifier());
			}
		}

		// Hand the layout children to the surface: the title only takes part
		// in the layout of a rich tooltip.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (tooltip.getVariant() == Variant::Rich) {
			if (auto title = getEntity<Text>(tooltip.getTitleEntity())) {
				children.push_back(std::move(title));
			}
		}
		if (auto text = getEntity<Text>(tooltip.getTextEntity())) {
			children.push_back(std::move(text));
		}
		setChildren(children);
	}

}	 // namespace guillaume::entities
