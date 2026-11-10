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
#include <vector>

#include "guillaume/entities/search.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/focus.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief Default search width.
		 */
		constexpr float SearchWidth = 360.0f;

		/**
		 * @brief Paper `bar` height.
		 */
		constexpr float BarHeight = 56.0f;

		/**
		 * @brief Paper `view` height.
		 */
		constexpr float ViewHeight = 72.0f;

		/**
		 * @brief Search icon size.
		 */
		constexpr float SearchIconSize = 24.0f;

		/**
		 * @brief Material Symbols glyph used while loading.
		 */
		constexpr const char *LoadingGlyph = "progress_activity";
	}	 // namespace

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
		_config.setFixedWidth(SearchWidth);
		_config.setFixedHeight(BarHeight);
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
					  components::Elevation, components::Line>(registry, config)
		, _variant(variant)
		, _placeholder(placeholder)
		, _text(text)
		, _searchIcon()
		, _textEntity()
		, _placeholderEntity()
		, _clearIcon()
		, _loadingIcon()
		, _trailingIcon()
	{
		if (!getComponentRegistry().hasComponent<components::Focus>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Focus>(
				getIdentifier());
		}
		if (!getComponentRegistry().hasComponent<components::Accessibility>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Accessibility>(
				getIdentifier());
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::Search)
			.setLabel(placeholder);

		// Paper's Searchbar defaults to elevation 0.
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(theme::ElevationLevel::Level0);

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
		config.spacing		 = theme::SpacingScale::get(4);
		config.padding		 = theme::SpacingScale::get(4);
		config.hasFixedWidth = true;
		if (config.fixedWidth <= 0.0f) {
			config.fixedWidth = SearchWidth;
		}
		config.hasFixedHeight = true;

		auto &line = getComponentRegistry().getComponent<components::Line>(
			getIdentifier());

		if (_variant == Variant::FullScreen) {
			config.borderRadius = getShapeRadius(theme::ShapeRole::None);
			config.fixedHeight	= ViewHeight;
			line.setThickness(1.0f)
				.setPlacement(components::Line::Placement::Bottom)
				.setColor(schemeColor(SchemeColorRole::OutlineVariant));
		} else {
			config.borderRadius = getShapeRadius(theme::ShapeRole::ExtraLarge);
			config.fixedHeight	= BarHeight;
			line.setThickness(0.0f).clearColor();
		}

		setSurfaceConfig(config);

		const auto level =
			getComponentRegistry()
				.getComponent<components::Elevation>(getIdentifier())
				.getElevationLevel();
		setColor(level == theme::ElevationLevel::Level0
					 ? schemeColor(SchemeColorRole::SurfaceContainerHigh)
					 : getElevatedSurfaceColor(level));
	}

	void Search::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		// Both Paper modes are inline fields; neither is modal.
		overlay.setModal(false);
		overlay.setDismissOnOutsideClick(false);
		overlay.setDismissOnEscape(true);
	}

	void Search::applyTextField(void)
	{
		auto &textField =
			this->getComponentRegistry().getComponent<components::TextField>(
				this->getIdentifier());

		textField.setPlaceholder(_placeholder);
		textField.setLabel(_placeholder);
		textField.setLabelFloating(!getText().empty());
	}

	std::shared_ptr<Icon>
		Search::buildPressableIcon(const std::string &glyph,
								   const std::string &label,
								   std::function<void(void)> onPress)
	{
		auto icon = buildIcon(this->getComponentRegistry(), *this,
							  this->shared_from_this(), glyph, SearchIconSize,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));
		if (icon == nullptr) {
			return nullptr;
		}
		makeInteractive(this->getComponentRegistry(), icon->getIdentifier(),
						components::AccessibilityRole::Button,
						std::move(onPress));
		this->getComponentRegistry()
			.getComponent<components::Accessibility>(icon->getIdentifier())
			.setLabel(label);
		return icon;
	}

	void Search::buildContent(void)
	{
		if (_searchIcon == nullptr) {
			_searchIcon = buildPressableIcon(_iconGlyph, "search", [this]() {
				if (_onIconPress) {
					_onIconPress();
				}
			});
		}

		if (_textEntity == nullptr) {
			// Built even when empty so the text-input systems can edit it.
			Text::Builder builder(this->getComponentRegistry(), *this);
			_textEntity = builder.makeText(
				this->shared_from_this(), _text, theme::TypeRole::BodyLarge,
				schemeColor(SchemeColorRole::OnSurface));
			_textEntity->setContent(_text);
			this->getComponentRegistry().addComponent<components::Focus>(
				_textEntity->getIdentifier());
		}

		if (_placeholderEntity == nullptr && !_placeholder.empty()) {
			_placeholderEntity = buildText(
				this->getComponentRegistry(), *this, this->shared_from_this(),
				_placeholder, theme::TypeRole::BodyLarge,
				schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		if (_clearIcon == nullptr) {
			_clearIcon = buildPressableIcon("close", "clear search", [this]() {
				clear();
			});
		}

		if (_loadingIcon == nullptr) {
			_loadingIcon = buildIcon(this->getComponentRegistry(), *this,
									 this->shared_from_this(), LoadingGlyph,
									 SearchIconSize,
									 schemeColor(SchemeColorRole::Primary));
		}

		if (_trailingIcon == nullptr && !_trailingGlyph.empty()) {
			_trailingIcon =
				buildPressableIcon(_trailingGlyph, _trailingGlyph, [this]() {
					if (_onTrailingIconPress) {
						_onTrailingIconPress();
					}
				});
		}

		const bool hasQuery	 = !getText().empty();
		const auto iconColor = schemeColor(SchemeColorRole::OnSurfaceVariant);

		if (_searchIcon != nullptr) {
			_searchIcon->setGlyphName(_iconGlyph);
			_searchIcon->setColor(iconColor);
		}
		if (_placeholderEntity != nullptr) {
			_placeholderEntity->setColor(hasQuery ? transparentColor()
												  : iconColor);
		}
		if (_clearIcon != nullptr) {
			const bool showClear = hasQuery && !_loading;
			_clearIcon->setColor(showClear ? iconColor : transparentColor());
			setEnabled(this->getComponentRegistry(),
					   _clearIcon->getIdentifier(), showClear);
		}
		if (_loadingIcon != nullptr) {
			_loadingIcon->setColor(_loading
									   ? schemeColor(SchemeColorRole::Primary)
									   : transparentColor());
		}
		if (_trailingIcon != nullptr) {
			_trailingIcon->setColor(iconColor);
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_searchIcon != nullptr) {
			children.push_back(_searchIcon);
		}

		if (hasQuery || _placeholderEntity == nullptr) {
			children.push_back(_textEntity);
		} else {
			children.push_back(_placeholderEntity);
		}

		children.push_back(_loading ? std::shared_ptr<ecs::Entity>(_loadingIcon)
									: std::shared_ptr<ecs::Entity>(_clearIcon));

		if (_trailingIcon != nullptr) {
			children.push_back(_trailingIcon);
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
		if (_placeholderEntity != nullptr) {
			_placeholderEntity->setContent(placeholder);
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(placeholder);
		applyTextField();
		return *this;
	}

	Search &Search::setText(const std::string &text)
	{
		_text = text;

		if (_textEntity != nullptr) {
			_textEntity->setContent(text);
		}

		applyTextField();
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

	Search &Search::clear(void)
	{
		setText("");
		if (_onClearIconPress) {
			_onClearIconPress();
		}
		return *this;
	}

	Search &Search::setIcon(const std::string &iconGlyph)
	{
		_iconGlyph = iconGlyph;
		if (_searchIcon != nullptr) {
			_searchIcon->setGlyphName(iconGlyph);
		}
		return *this;
	}

	Search &Search::setOnIconPress(std::function<void(void)> onIconPress)
	{
		_onIconPress = std::move(onIconPress);
		return *this;
	}

	Search &
		Search::setOnClearIconPress(std::function<void(void)> onClearIconPress)
	{
		_onClearIconPress = std::move(onClearIconPress);
		return *this;
	}

	Search &Search::setTrailingIcon(const std::string &iconGlyph,
									std::function<void(void)> onPress)
	{
		_trailingGlyph		 = iconGlyph;
		_onTrailingIconPress = std::move(onPress);
		if (_trailingIcon != nullptr) {
			_trailingIcon->setGlyphName(iconGlyph);
		}
		return *this;
	}

	Search &Search::setLoading(bool loading)
	{
		_loading = loading;
		return *this;
	}

	bool Search::isLoading(void) const
	{
		return _loading;
	}

	Search &Search::setElevation(theme::ElevationLevel level)
	{
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(level);
		applyVariant();
		return *this;
	}

	ecs::Entity::Identifier Search::getIconIdentifier(void) const
	{
		return _searchIcon != nullptr ? _searchIcon->getIdentifier()
									  : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier Search::getClearIdentifier(void) const
	{
		return _clearIcon != nullptr ? _clearIcon->getIdentifier()
									 : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier Search::getPlaceholderIdentifier(void) const
	{
		return _placeholderEntity != nullptr
			? _placeholderEntity->getIdentifier()
			: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier Search::getTextIdentifier(void) const
	{
		return _textEntity != nullptr ? _textEntity->getIdentifier()
									  : ecs::Entity::InvalidIdentifier;
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
		buildContent();
		applyTextField();
	}

	void Search::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildContent();
		applyTextField();
	}

}	 // namespace guillaume::entities
