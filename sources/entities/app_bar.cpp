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

#include "guillaume/entities/app_bar.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	AppBar::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<AppBar>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<AppBar> AppBar::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<AppBar>(this->getComponentRegistry(),
											   _config, _variant, _title,
											   _navigationIcon, _actions);
		return entity;
	}

	void AppBar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Small;
		_title.clear();
		_navigationIcon.clear();
		_actions.clear();

		_config.axis = components::Layout::Axis::Horizontal;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 16.0f;
		_config.setFixedWidth(360.0f);
	}

	AppBar::Builder &
		AppBar::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	AppBar::Builder &AppBar::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	AppBar::Builder &AppBar::Builder::withTitle(const std::string &title)
	{
		_title = title;
		return *this;
	}

	AppBar::Builder &
		AppBar::Builder::withNavigationIcon(const std::string &iconGlyphName)
	{
		_navigationIcon = iconGlyphName;
		return *this;
	}

	AppBar::Builder &
		AppBar::Builder::addAction(const std::string &iconGlyphName)
	{
		_actions.push_back(iconGlyphName);
		return *this;
	}

	AppBar::Builder &AppBar::Builder::withFixedWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	std::shared_ptr<AppBar> AppBar::Director::makeAppBar(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant)
	{
		return builder.withVariant(variant).registerEntity(parent);
	}

	AppBar::AppBar(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &title, const std::string &navigationIcon,
				   const std::vector<std::string> &actions)
		: SurfaceBase<>(registry, config)
		, _variant(variant)
		, _title(title)
		, _navigationIcon(navigationIcon)
		, _actions(actions)
		, _navigationIconEntity()
		, _titleEntity()
		, _actionEntities()
	{
		applyVariant();
	}

	AppBar::~AppBar(void)
	{
	}

	float AppBar::getVariantHeight(void) const
	{
		switch (_variant) {
			case Variant::Small:
				return 56.0f;
			case Variant::Medium:
				return 104.0f;
			case Variant::Large:
				return 152.0f;
			case Variant::Bottom:
				return 80.0f;
			default:
				return 56.0f;
		}
	}

	void AppBar::buildContent(void)
	{
		const auto foregroundColor = schemeColor(SchemeColorRole::OnSurface);

		if (_navigationIconEntity == nullptr && !_navigationIcon.empty()) {
			_navigationIconEntity = buildIcon(
				this->getComponentRegistry(), *this, this->shared_from_this(),
				_navigationIcon, 24.0f, foregroundColor);
		}

		if (_titleEntity == nullptr && !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title, 22.0f,
									 foregroundColor);
		}

		if (_actionEntities.size() != _actions.size()) {
			_actionEntities.clear();

			for (const auto &action: _actions) {
				auto icon = buildIcon(this->getComponentRegistry(), *this,
									  this->shared_from_this(), action, 24.0f,
									  foregroundColor);

				if (icon != nullptr) {
					_actionEntities.push_back(icon);
				}
			}
		}
	}

	void AppBar::attachContent(void)
	{
		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_navigationIconEntity != nullptr) {
			children.push_back(_navigationIconEntity);
		}

		if (_titleEntity != nullptr) {
			children.push_back(_titleEntity);
		}

		for (const auto &action: _actionEntities) {
			children.push_back(action);
		}

		SurfaceBase::setChildren(children);
	}

	void AppBar::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis = components::Layout::Axis::Horizontal;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		config.padding		  = 16.0f;
		config.spacing		  = 8.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = getVariantHeight();

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::Surface));
	}

	AppBar &AppBar::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	AppBar::Variant AppBar::getVariant(void) const
	{
		return _variant;
	}

	AppBar &AppBar::setTitle(const std::string &title)
	{
		_title = title;

		if (_titleEntity != nullptr) {
			_titleEntity->setContent(title);
		}

		return *this;
	}

	void AppBar::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		buildContent();
		attachContent();
	}

	void AppBar::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		buildContent();
		attachContent();
	}

}	 // namespace guillaume::entities
