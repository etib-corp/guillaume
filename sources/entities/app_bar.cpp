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
#include "guillaume/components/accessibility.hpp"
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
											   _navigation, _actions, _elevated);
		return entity;
	}

	void AppBar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Small;
		_title.clear();
		_navigation = Action {};
		_actions.clear();
		_elevated = false;

		_config.axis = components::Layout::Axis::Horizontal;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = HorizontalPadding;
		_config.padding = HorizontalPadding;
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
		_navigation.icon = iconGlyphName;
		if (_navigation.accessibilityLabel.empty()) {
			_navigation.accessibilityLabel = iconGlyphName;
		}
		return *this;
	}

	AppBar::Builder &
		AppBar::Builder::addAction(const std::string &iconGlyphName)
	{
		_actions.push_back(Action { iconGlyphName, {}, iconGlyphName, false });
		return *this;
	}

	AppBar::Builder &
		AppBar::Builder::addAction(const std::string &iconGlyphName,
								   std::function<void(void)> onPress,
								   const std::string &accessibilityLabel)
	{
		_actions.push_back(Action { iconGlyphName, std::move(onPress),
									accessibilityLabel.empty()
										? iconGlyphName
										: accessibilityLabel,
									false });
		return *this;
	}

	AppBar::Builder &
		AppBar::Builder::withOnNavigation(std::function<void(void)> onPress,
										  const std::string &accessibilityLabel)
	{
		_navigation.onPress			   = std::move(onPress);
		_navigation.accessibilityLabel = accessibilityLabel;
		return *this;
	}

	AppBar::Builder &AppBar::Builder::withElevated(bool elevated)
	{
		_elevated = elevated;
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

	namespace
	{
		std::vector<AppBar::Action>
			toActions(const std::vector<std::string> &icons)
		{
			std::vector<AppBar::Action> actions;
			for (const auto &icon: icons) {
				actions.push_back(AppBar::Action { icon, {}, icon, false });
			}
			return actions;
		}
	}	 // namespace

	AppBar::AppBar(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &title, const std::string &navigationIcon,
				   const std::vector<std::string> &actions)
		: AppBar(registry, config, variant, title,
				 Action { navigationIcon, {}, navigationIcon, false },
				 toActions(actions), false)
	{
	}

	AppBar::AppBar(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &title, const Action &navigation,
				   const std::vector<Action> &actions, bool elevated)
		: SurfaceBase<components::Elevation>(registry, config)
		, _variant(variant)
		, _title(title)
		, _navigation(navigation)
		, _actions(actions)
		, _elevated(elevated)
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
		// Paper Appbar/utils.ts: 64 small/center-aligned, 112 medium,
		// 152 large; the bottom app bar is 80.
		switch (_variant) {
			case Variant::Small:
			case Variant::CenterAligned:
				return 64.0f;
			case Variant::Medium:
				return 112.0f;
			case Variant::Large:
				return 152.0f;
			case Variant::Bottom:
				return 80.0f;
			default:
				return 64.0f;
		}
	}

	theme::TypeRole AppBar::getTitleRole(void) const
	{
		switch (_variant) {
			case Variant::Medium:
				return theme::TypeRole::HeadlineSmall;
			case Variant::Large:
				return theme::TypeRole::HeadlineMedium;
			case Variant::Small:
			case Variant::CenterAligned:
			case Variant::Bottom:
			default:
				return theme::TypeRole::TitleLarge;
		}
	}

	std::shared_ptr<IconButton> AppBar::buildAction(const Action &action)
	{
		IconButton::Builder builder(this->getComponentRegistry(), *this);
		return builder.withIcon(action.icon)
			.withOnClick(action.onPress)
			.withVariant(IconButton::Variant::Standard)
			.withSize(IconButton::Size::Medium)
			.withDisabled(action.disabled)
			.withAccessibilityLabel(action.accessibilityLabel)
			.registerEntity(this->shared_from_this());
	}

	void AppBar::buildContent(void)
	{
		if (_navigationIconEntity == nullptr && !_navigation.icon.empty()) {
			_navigationIconEntity = buildAction(_navigation);
			// Paper: the leading icon uses onSurface, actions onSurfaceVariant.
			_navigationIconEntity->setContentColorOverride(
				schemeColor(SchemeColorRole::OnSurface));
		}

		if (_titleEntity == nullptr && !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title,
									 getTitleRole(),
									 schemeColor(SchemeColorRole::OnSurface));
			if (_titleEntity != nullptr) {
				auto &registry = this->getComponentRegistry();
				if (!registry.hasComponent<components::Accessibility>(
						_titleEntity->getIdentifier())) {
					registry.addComponent<components::Accessibility>(
						_titleEntity->getIdentifier());
				}
				registry
					.getComponent<components::Accessibility>(
						_titleEntity->getIdentifier())
					.setRole(components::AccessibilityRole::Header)
					.setLabel(_title)
					.setFocusable(false);
			}
		}

		if (_titleEntity != nullptr) {
			_titleEntity->setTypeRole(getTitleRole());
		}

		if (_actionEntities.size() != _actions.size()) {
			_actionEntities.clear();

			for (const auto &action: _actions) {
				auto button = buildAction(action);
				if (button != nullptr) {
					_actionEntities.push_back(button);
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
		config.mainAxisAlignment = (_variant == Variant::CenterAligned)
			? components::Layout::MainAxisAlignment::SpaceBetween
			: components::Layout::MainAxisAlignment::Start;
		if (_variant == Variant::CenterAligned && _navigation.icon.empty()
			&& _actions.empty()) {
			config.mainAxisAlignment =
				components::Layout::MainAxisAlignment::Center;
		}
		config.padding		  = HorizontalPadding;
		config.spacing		  = HorizontalPadding;
		config.hasFixedHeight = true;
		config.fixedHeight	  = getVariantHeight();

		setSurfaceConfig(config);

		// Paper: elevated bars (and the bottom bar) use elevation.level2.
		const bool elevated = _elevated || _variant == Variant::Bottom;
		setColor(elevated
					 ? getElevatedSurfaceColor(theme::ElevationLevel::Level2)
					 : schemeColor(SchemeColorRole::Surface));
		this->getComponentRegistry()
			.getComponent<components::Elevation>(this->getIdentifier())
			.setElevationLevel(_elevated ? theme::ElevationLevel::Level2
										 : theme::ElevationLevel::Level0);
	}

	AppBar &AppBar::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		if (_titleEntity != nullptr) {
			_titleEntity->setTypeRole(getTitleRole());
		}
		return *this;
	}

	AppBar &AppBar::setElevated(bool elevated)
	{
		_elevated = elevated;
		applyVariant();
		return *this;
	}

	bool AppBar::isElevated(void) const
	{
		return _elevated;
	}

	std::shared_ptr<IconButton> AppBar::getNavigationButton(void) const
	{
		return _navigationIconEntity;
	}

	std::shared_ptr<IconButton>
		AppBar::getActionButton(std::size_t index) const
	{
		return index < _actionEntities.size() ? _actionEntities[index]
											  : nullptr;
	}

	std::shared_ptr<Text> AppBar::getTitle(void) const
	{
		return _titleEntity;
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
