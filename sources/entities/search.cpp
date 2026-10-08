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

#include "guillaume/entities/search.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

#include <vector>

#include "guillaume/components/focus.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
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
		_config.spacing		 = 8.0f;
		_config.padding		 = 12.0f;
		_config.borderRadius = 28.0f;
		_config.setFixedWidth(360.0f);
		_config.setFixedHeight(56.0f);
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

	std::shared_ptr<Search> Search::Director::makeSearch(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &placeholder, const std::string &text)
	{
		return builder.withVariant(variant)
			.withPlaceholder(placeholder)
			.withText(text)
			.registerEntity(parent);
	}

	Search::Search(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &placeholder, const std::string &text)
		: SurfaceBase<components::Overlay, components::TextField>(registry,
																  config)
		, _variant(variant)
		, _placeholder(placeholder)
		, _text(text)
		, _searchIcon()
		, _textEntity()
	{
		if (!getComponentRegistry().hasComponent<components::Focus>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Focus>(
				getIdentifier());
		}

		applyVariant();
		applyOverlay();
		applyTextField();
	}

	Search::~Search(void)
	{
	}

	void Search::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Horizontal;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		config.spacing		 = 8.0f;
		config.padding		 = 12.0f;
		config.hasFixedWidth = true;
		config.fixedWidth	 = 360.0f;

		if (_variant == Variant::FullScreen) {
			config.borderRadius	  = 0.0f;
			config.hasFixedHeight = true;
			config.fixedHeight	  = 640.0f;
		} else {
			config.borderRadius	  = 28.0f;
			config.hasFixedHeight = true;
			config.fixedHeight	  = 56.0f;
		}

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::SurfaceContainerHigh));
	}

	void Search::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(_variant == Variant::FullScreen);
		overlay.setDismissOnOutsideClick(_variant == Variant::FullScreen);
		overlay.setDismissOnEscape(true);
	}

	void Search::applyTextField(void)
	{
		auto &textField =
			this->getComponentRegistry().getComponent<components::TextField>(
				this->getIdentifier());

		textField.setPlaceholder(_placeholder);
		textField.setLabel(_placeholder);
	}

	void Search::buildContent(void)
	{
		if (_searchIcon == nullptr) {
			_searchIcon =
				buildIcon(this->getComponentRegistry(), *this,
						  this->shared_from_this(), "search", 24.0f,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		if (_textEntity == nullptr && !_text.empty()) {
			_textEntity = buildText(this->getComponentRegistry(), *this,
									this->shared_from_this(), _text, 16.0f,
									schemeColor(SchemeColorRole::OnSurface));
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_searchIcon != nullptr) {
			children.push_back(_searchIcon);
		}

		if (_textEntity != nullptr) {
			children.push_back(_textEntity);
		}

		setChildren(children);
	}

	Search &Search::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		applyOverlay();
		return *this;
	}

	Search::Variant Search::getVariant(void) const
	{
		return _variant;
	}

	Search &Search::setPlaceholder(const std::string &placeholder)
	{
		_placeholder = placeholder;
		applyTextField();
		return *this;
	}

	Search &Search::setText(const std::string &text)
	{
		_text = text;

		if (_textEntity != nullptr) {
			_textEntity->setContent(text);
		}

		return *this;
	}

	std::string Search::getText(void) const
	{
		if (_textEntity == nullptr) {
			return _text;
		}

		return this->getComponentRegistry()
			.getComponent<components::Text>(_textEntity->getIdentifier())
			.getContent();
	}

	Search &Search::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	Search &Search::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		return *this;
	}

	bool Search::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
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
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		applyTextField();
		buildContent();
	}

	void Search::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		applyTextField();
		buildContent();
	}

}	 // namespace guillaume::entities
