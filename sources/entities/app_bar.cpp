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

#include "guillaume/entities/app_bar.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/app_bar.hpp"
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
		auto entity = std::make_shared<AppBar>(
			this->getComponentRegistry(), _config, _variant, _title,
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
		_actions.push_back(Action {
			iconGlyphName, std::move(onPress),
			accessibilityLabel.empty() ? iconGlyphName : accessibilityLabel,
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

	std::shared_ptr<AppBar>
		AppBar::Builder::makeAppBar(std::shared_ptr<ecs::Entity> parent,
									Variant variant)
	{
		return withVariant(variant).registerEntity(parent);
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
		: SurfaceBase<components::Elevation, components::AppBar>(registry,
																 config)
	{
		getComponentRegistry()
			.getComponent<components::AppBar>(getIdentifier())
			.setVariant(variant)
			.setTitle(title)
			.setNavigation(navigation)
			.setActions(actions)
			.setElevated(elevated);
		systems::AppBar::apply(getComponentRegistry(), getIdentifier());
	}

	AppBar::~AppBar(void)
	{
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
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		auto &appBar =
			registry.getComponent<components::AppBar>(getIdentifier());

		// Which children are missing is decided by systems::AppBar; they are
		// only built here because that needs shared_from_this().
		if (systems::AppBar::needsNavigationButton(registry, getIdentifier())) {
			const auto navigation = buildAction(appBar.getNavigation());
			if (navigation != nullptr) {
				appBar.setNavigationButton(navigation->getIdentifier());
			}
		}

		if (systems::AppBar::needsTitle(registry, getIdentifier())) {
			const auto title = buildText(
				registry, *this, shared_from_this(), appBar.getTitle(),
				systems::AppBar::titleRole(appBar.getVariant()),
				schemeColor(SchemeColorRole::OnSurface));
			if (title != nullptr) {
				appBar.setTitleEntity(title->getIdentifier());
				systems::AppBar::installTitle(registry, getIdentifier());
			}
		}

		if (systems::AppBar::needsActionButtons(registry, getIdentifier())) {
			components::AppBar::Identifiers identifiers;
			for (const auto &action: appBar.getActions()) {
				const auto button = buildAction(action);
				identifiers.push_back(button != nullptr
										  ? button->getIdentifier()
										  : ecs::Entity::InvalidIdentifier);
			}
			appBar.setActionButtons(identifiers);
		}

		// Hand the layout children to the surface.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (auto navigation =
				getEntity<IconButton>(appBar.getNavigationButton())) {
			children.push_back(std::move(navigation));
		}
		if (auto title = getEntity<Text>(appBar.getTitleEntity())) {
			children.push_back(std::move(title));
		}
		for (const auto identifier: appBar.getActionButtons()) {
			if (auto action = getEntity<IconButton>(identifier)) {
				children.push_back(std::move(action));
			}
		}
		setChildren(children);
	}

	AppBar &AppBar::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::AppBar>(getIdentifier())
			.setVariant(variant);
		systems::AppBar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	AppBar &AppBar::setElevated(bool elevated)
	{
		getComponentRegistry()
			.getComponent<components::AppBar>(getIdentifier())
			.setElevated(elevated);
		systems::AppBar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool AppBar::isElevated(void) const
	{
		return getComponentRegistry()
			.getComponent<components::AppBar>(getIdentifier())
			.isElevated();
	}

	std::shared_ptr<IconButton> AppBar::getNavigationButton(void) const
	{
		// The lookup is const because this getter is; the child itself is
		// shared and mutable, as it always was.
		return std::const_pointer_cast<IconButton>(getEntity<IconButton>(
			getComponentRegistry()
				.getComponent<components::AppBar>(getIdentifier())
				.getNavigationButton()));
	}

	std::shared_ptr<IconButton> AppBar::getActionButton(std::size_t index) const
	{
		const auto &actions =
			getComponentRegistry()
				.getComponent<components::AppBar>(getIdentifier())
				.getActionButtons();

		if (index >= actions.size()) {
			return nullptr;
		}
		return std::const_pointer_cast<IconButton>(
			getEntity<IconButton>(actions[index]));
	}

	std::shared_ptr<Text> AppBar::getTitle(void) const
	{
		return std::const_pointer_cast<Text>(getEntity<Text>(
			getComponentRegistry()
				.getComponent<components::AppBar>(getIdentifier())
				.getTitleEntity()));
	}

	AppBar::Variant AppBar::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::AppBar>(getIdentifier())
			.getVariant();
	}

	AppBar &AppBar::setTitle(const std::string &title)
	{
		getComponentRegistry()
			.getComponent<components::AppBar>(getIdentifier())
			.setTitle(title);
		// A first title needs its text child.
		buildContent();
		systems::AppBar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	void AppBar::initialize(void)
	{
		buildContent();
		systems::AppBar::apply(getComponentRegistry(), getIdentifier());
	}

}	 // namespace guillaume::entities
