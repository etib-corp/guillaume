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
#include <utility>
#include <vector>

#include "guillaume/entities/search.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/search.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Search::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Search>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Search> Search::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Search>(this->getComponentRegistry(), _config,
									 _variant, _placeholder, _text);
		return entity;
	}

	void Search::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::SearchBar;
		_placeholder.clear();
		_text.clear();

		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing		 = theme::SpacingScale::get(4);
		_config.padding		 = theme::SpacingScale::get(4);
		_config.borderRadius = getShapeRadius(theme::ShapeRole::ExtraLarge);
		_config.setFixedWidth(components::Search::DefaultWidth);
		_config.setFixedHeight(components::Search::BarHeight);
	}

	Search::Builder &
		Search::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Search::Builder &Search::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Search::Builder &
		Search::Builder::withPlaceholder(const std::string &placeholder)
	{
		_placeholder = placeholder;
		return *this;
	}

	Search::Builder &Search::Builder::withText(const std::string &text)
	{
		_text = text;
		return *this;
	}

	std::shared_ptr<Search> Search::Builder::makeSearch(
		std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &placeholder, const std::string &text)
	{
		return withVariant(variant)
			.withPlaceholder(placeholder)
			.withText(text)
			.registerEntity(parent);
	}

	Search::Search(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &placeholder, const std::string &text)
		: SurfaceBase<components::Overlay, components::TextField,
					  components::Elevation, components::Line,
					  components::Focus, components::Accessibility,
					  components::Search>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setVariant(variant)
			.setPlaceholder(placeholder)
			.setText(text);
		systems::Search::install(getComponentRegistry(), getIdentifier());
		systems::Search::apply(getComponentRegistry(), getIdentifier());
	}

	Search::~Search(void)
	{
	}

	Search &Search::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setVariant(variant);
		systems::Search::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Search::Variant Search::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.getVariant();
	}

	Search &Search::setPlaceholder(const std::string &placeholder)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setPlaceholder(placeholder);
		buildChildren();
		systems::Search::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Search &Search::setText(const std::string &text)
	{
		systems::Search::setText(getComponentRegistry(), getIdentifier(), text);
		return *this;
	}

	std::string Search::getText(void) const
	{
		return systems::Search::text(getComponentRegistry(), getIdentifier());
	}

	Search &Search::clear(void)
	{
		systems::Search::clear(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Search &Search::setIcon(const std::string &iconGlyph)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setIconGlyph(iconGlyph);
		buildChildren();
		systems::Search::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Search &Search::setOnIconPress(std::function<void(void)> onIconPress)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setOnIconPress(std::move(onIconPress));
		return *this;
	}

	Search &
		Search::setOnClearIconPress(std::function<void(void)> onClearIconPress)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setOnClearIconPress(std::move(onClearIconPress));
		return *this;
	}

	Search &Search::setTrailingIcon(const std::string &iconGlyph,
									std::function<void(void)> onPress)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setTrailingGlyph(iconGlyph)
			.setOnTrailingIconPress(std::move(onPress));
		buildChildren();
		systems::Search::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Search &Search::setLoading(bool loading)
	{
		getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.setLoading(loading);
		systems::Search::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool Search::isLoading(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.isLoading();
	}

	Search &Search::setElevation(theme::ElevationLevel level)
	{
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(level);
		systems::Search::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	ecs::Entity::Identifier Search::getIconIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.getSearchIcon();
	}

	ecs::Entity::Identifier Search::getClearIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.getClearIcon();
	}

	ecs::Entity::Identifier Search::getPlaceholderIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.getPlaceholderEntity();
	}

	ecs::Entity::Identifier Search::getTextIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Search>(getIdentifier())
			.getTextEntity();
	}

	Search &Search::show(void)
	{
		systems::Search::show(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Search &Search::hide(void)
	{
		systems::Search::hide(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool Search::isVisible(void) const
	{
		return systems::Search::isVisible(getComponentRegistry(),
										  getIdentifier());
	}

	Search &Search::open(void)
	{
		return show();
	}

	Search &Search::close(void)
	{
		return hide();
	}

	bool Search::isOpen(void) const
	{
		return isVisible();
	}

	void Search::initialize(void)
	{
		buildChildren();
		systems::Search::apply(getComponentRegistry(), getIdentifier());
	}

	void Search::buildChildren(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry		  = getComponentRegistry();
		const auto identifier = getIdentifier();
		const auto self		  = shared_from_this();
		const auto iconColor  = schemeColor(SchemeColorRole::OnSurfaceVariant);
		// Copy: the configuration is read while the children are created.
		const components::Search config =
			registry.getComponent<components::Search>(identifier);
		const auto search = [&registry, identifier]() -> components::Search & {
			return registry.getComponent<components::Search>(identifier);
		};
		bool built = false;

		if (config.getSearchIcon() == ecs::Entity::InvalidIdentifier) {
			if (const auto icon =
					buildIcon(registry, *this, self, config.getIconGlyph(),
							  components::Search::IconSize, iconColor)) {
				search().setSearchIcon(icon->getIdentifier());
				systems::Search::installIcon(registry, identifier);
				built = true;
			}
		}

		if (config.getTextEntity() == ecs::Entity::InvalidIdentifier) {
			// Built even when empty so the text-input systems can edit it.
			Text::Builder builder(registry, *this);
			const auto text = builder.makeText(
				self, config.getText(), theme::TypeRole::BodyLarge,
				schemeColor(SchemeColorRole::OnSurface));
			text->setContent(config.getText());
			registry.addComponent<components::Focus>(text->getIdentifier());
			search().setTextEntity(text->getIdentifier());
			built = true;
		}

		if (config.getPlaceholderEntity() == ecs::Entity::InvalidIdentifier) {
			if (const auto placeholder =
					buildText(registry, *this, self, config.getPlaceholder(),
							  theme::TypeRole::BodyLarge, iconColor)) {
				search().setPlaceholderEntity(placeholder->getIdentifier());
				built = true;
			}
		}

		if (config.getClearIcon() == ecs::Entity::InvalidIdentifier) {
			if (const auto icon = buildIcon(
					registry, *this, self, components::Search::ClearGlyph,
					components::Search::IconSize, iconColor)) {
				search().setClearIcon(icon->getIdentifier());
				systems::Search::installClearIcon(registry, identifier);
				built = true;
			}
		}

		if (config.getLoadingIcon() == ecs::Entity::InvalidIdentifier) {
			if (const auto icon = buildIcon(
					registry, *this, self, components::Search::LoadingGlyph,
					components::Search::IconSize,
					schemeColor(SchemeColorRole::Primary))) {
				search().setLoadingIcon(icon->getIdentifier());
				built = true;
			}
		}

		if (config.getTrailingIcon() == ecs::Entity::InvalidIdentifier) {
			if (const auto icon =
					buildIcon(registry, *this, self, config.getTrailingGlyph(),
							  components::Search::IconSize, iconColor)) {
				search().setTrailingIcon(icon->getIdentifier());
				systems::Search::installTrailingIcon(registry, identifier);
				built = true;
			}
		}

		if (!built) {
			return;
		}

		// Hand every child to the surface; systems::Search then narrows the
		// layout children to the ones currently shown.
		const auto &current = search();
		Children children;

		for (const auto child:
			 { current.getSearchIcon(), current.getTextEntity(),
			   current.getPlaceholderEntity(), current.getClearIcon(),
			   current.getLoadingIcon(), current.getTrailingIcon() }) {
			if (const auto entity = getEntity<ecs::Entity>(child)) {
				children.push_back(entity);
			}
		}
		setChildren(children);
	}

}	 // namespace guillaume::entities
