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

#include "guillaume/entities/dialog.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief M3 dialog width (basic dialogs are 280-560 dp wide).
		 */
		constexpr float DialogWidth = 320.0f;

		/**
		 * @brief M3 dialog padding.
		 */
		constexpr float DialogPadding = theme::SpacingScale::get(6);

		/**
		 * @brief Gap between dialog sections.
		 */
		constexpr float DialogSpacing = theme::SpacingScale::get(4);

		std::vector<Dialog::Action>
			toActions(const std::vector<std::string> &labels)
		{
			std::vector<Dialog::Action> actions;
			actions.reserve(labels.size());
			for (const auto &label: labels) {
				actions.push_back(Dialog::Action { label, {} });
			}
			return actions;
		}
	}	 // namespace

	Dialog::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Dialog>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Dialog> Dialog::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Dialog>(
			this->getComponentRegistry(), _config, _variant, _title, _message,
			_actions, _icon, _onDismiss);
		return entity;
	}

	void Dialog::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Alert;
		_title.clear();
		_message.clear();
		_actions.clear();
		_icon.clear();
		_onDismiss = {};

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding		 = DialogPadding;
		_config.spacing		 = DialogSpacing;
		_config.borderRadius = getShapeRadius(theme::ShapeRole::ExtraLarge);
		_config.setFixedWidth(DialogWidth);
	}

	Dialog::Builder &
		Dialog::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Dialog::Builder &Dialog::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Dialog::Builder &Dialog::Builder::withTitle(const std::string &title)
	{
		_title = title;
		return *this;
	}

	Dialog::Builder &Dialog::Builder::withMessage(const std::string &message)
	{
		_message = message;
		return *this;
	}

	Dialog::Builder &
		Dialog::Builder::addAction(const std::string &label,
								   std::function<void(void)> onPress)
	{
		_actions.push_back(Action { label, std::move(onPress) });
		return *this;
	}

	Dialog::Builder &Dialog::Builder::withIcon(const std::string &iconGlyph)
	{
		_icon = iconGlyph;
		return *this;
	}

	Dialog::Builder &
		Dialog::Builder::withOnDismiss(std::function<void(void)> onDismiss)
	{
		_onDismiss = std::move(onDismiss);
		return *this;
	}

	std::shared_ptr<Dialog>
		Dialog::Builder::makeDialog(std::shared_ptr<ecs::Entity> parent,
									Variant variant, const std::string &title,
									const std::string &message,
									const std::vector<std::string> &actions)
	{
		withVariant(variant).withTitle(title).withMessage(message);

		for (const auto &action: actions) {
			addAction(action);
		}

		return registerEntity(parent);
	}

	Dialog::Dialog(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &title, const std::string &message,
				   const std::vector<std::string> &actions)
		: Dialog(registry, config, variant, title, message, toActions(actions),
				 "", {})
	{
	}

	Dialog::Dialog(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &title, const std::string &message,
				   const std::vector<Action> &actions, const std::string &icon,
				   std::function<void(void)> onDismiss)
		: SurfaceBase<components::Overlay, components::Elevation>(registry,
																  config)
		, _variant(variant)
		, _title(title)
		, _message(message)
		, _actions(actions)
		, _icon(icon)
		, _iconEntity()
		, _titleEntity()
		, _messageEntity()
		, _actionEntities()
		, _actionRow()
		, _backdrop()
	{
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(theme::ElevationLevel::Level3);

		if (!getComponentRegistry().hasComponent<components::Accessibility>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Accessibility>(
				getIdentifier());
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::Dialog)
			.setLabel(title)
			.setFocusable(false);

		getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.setOnDismissHandler(std::move(onDismiss));

		applyVariant();
		applyOverlay();
	}

	Dialog::~Dialog(void)
	{
	}

	void Dialog::applyVariant(void)
	{
		auto config = SurfaceConfig::panel(
			DialogWidth, components::Layout::Axis::Vertical, DialogSpacing);
		config.padding = DialogPadding;
		// M3: with a hero icon, the icon and the headline are centred.
		config.crossAxisAlignment = _iconEntity != nullptr
			? components::Layout::CrossAxisAlignment::Center
			: components::Layout::CrossAxisAlignment::Start;

		switch (_variant) {
			case Variant::Alert:
			case Variant::Simple:
			case Variant::Confirmation: {
				config.hasFixedHeight = false;
				break;
			}
			case Variant::FullScreen:
			default: {
				config.borderRadius	  = getShapeRadius(theme::ShapeRole::None);
				config.hasFixedHeight = true;
				config.fixedHeight	  = 640.0f;
				break;
			}
		}

		setSurfaceConfig(config);
		setColor(getElevatedSurfaceColor(theme::ElevationLevel::Level3));
	}

	void Dialog::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(true);
		overlay.setDismissOnOutsideClick(_variant != Variant::FullScreen);
		overlay.setDismissOnEscape(true);

		if (_backdrop != nullptr) {
			// A full-screen dialog covers the viewport and needs no backdrop.
			_backdrop->setVisible(overlay.isVisible()
								  && _variant != Variant::FullScreen);
			const auto &bound =
				getComponentRegistry().getComponent<components::Bound>(
					getIdentifier());
			_backdrop->follow(
				getComponentRegistry()
					.getComponent<components::Transform>(getIdentifier())
					.getPose(),
				bound.getWidth(), bound.getHeight());
		}
	}

	void Dialog::buildContent(void)
	{
		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_backdrop == nullptr) {
			_backdrop = std::make_shared<Backdrop>(getComponentRegistry());
			_backdrop->setParent(this->shared_from_this());
			this->addEntity(_backdrop);
		}

		const std::string icon = !_icon.empty()
			? _icon
			: (_variant == Variant::Confirmation ? std::string("info")
												 : std::string());
		if (_iconEntity == nullptr && !icon.empty()) {
			_iconEntity = buildIcon(this->getComponentRegistry(), *this,
									this->shared_from_this(), icon, 24.0f,
									schemeColor(SchemeColorRole::Secondary));
		}

		if (_iconEntity != nullptr) {
			_iconEntity->setColor(schemeColor(SchemeColorRole::Secondary));
		}

		if (_titleEntity == nullptr && !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title,
									 theme::TypeRole::HeadlineSmall,
									 schemeColor(SchemeColorRole::OnSurface));
			if (_titleEntity != nullptr) {
				getComponentRegistry().addComponent<components::Accessibility>(
					_titleEntity->getIdentifier());
				getComponentRegistry()
					.getComponent<components::Accessibility>(
						_titleEntity->getIdentifier())
					.setRole(components::AccessibilityRole::Header)
					.setLabel(_title)
					.setFocusable(false);
			}
		}

		if (_messageEntity == nullptr && !_message.empty()) {
			_messageEntity = buildText(
				this->getComponentRegistry(), *this, this->shared_from_this(),
				_message, theme::TypeRole::BodyMedium,
				schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		if (_actionRow == nullptr && !_actions.empty()) {
			View::Builder rowBuilder(this->getComponentRegistry(), *this);
			_actionRow = rowBuilder.makeDefaultView(
				this->shared_from_this(), utility::graphic::PoseF(), {});
			_actionRow->setColor(transparentColor());
			_actionRow->setAxis(components::Layout::Axis::Horizontal);
			_actionRow->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::End);
			_actionRow->setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center);
			_actionRow->setSpacing(theme::SpacingScale::get(2));
			_actionRow->setPadding(0.0f);
			_actionRow->setFixedWidth(DialogWidth - 2.0f * DialogPadding);

			std::vector<std::shared_ptr<ecs::Entity>> buttons;
			for (const auto &action: _actions) {
				if (action.label.empty()) {
					continue;
				}
				Button::Builder buttonBuilder(this->getComponentRegistry(),
											  *_actionRow);
				auto button = buttonBuilder.withLabel(action.label)
								  .withColorStyle(Button::Color::Text)
								  .withShape(Button::Shape::Round)
								  .withSize(Button::Size::Small)
								  .withOnClick(action.onPress)
								  .registerEntity(_actionRow);
				if (button != nullptr) {
					_actionEntities.push_back(button);
					buttons.push_back(button);
				}
			}
			_actionRow->setEntities(buttons);
		}

		if (_iconEntity != nullptr) {
			children.push_back(_iconEntity);
		}

		if (_titleEntity != nullptr) {
			children.push_back(_titleEntity);
		}

		if (_messageEntity != nullptr) {
			children.push_back(_messageEntity);
		}

		if (_actionRow != nullptr) {
			children.push_back(_actionRow);
		}

		setChildren(children);
	}

	Dialog &Dialog::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		applyOverlay();
		return *this;
	}

	Dialog::Variant Dialog::getVariant(void) const
	{
		return _variant;
	}

	Dialog &Dialog::setTitle(const std::string &title)
	{
		_title = title;

		if (_titleEntity != nullptr) {
			_titleEntity->setContent(title);
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(title);

		return *this;
	}

	Dialog &Dialog::setMessage(const std::string &message)
	{
		_message = message;

		if (_messageEntity != nullptr) {
			_messageEntity->setContent(message);
		}

		return *this;
	}

	Dialog &Dialog::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		applyOverlay();
		return *this;
	}

	Dialog &Dialog::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		applyOverlay();
		return *this;
	}

	bool Dialog::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
	}

	Dialog &Dialog::open(void)
	{
		return show();
	}

	Dialog &Dialog::close(void)
	{
		return hide();
	}

	bool Dialog::isOpen(void) const
	{
		return isVisible();
	}

	Dialog &Dialog::dismiss(void)
	{
		if (!isVisible()) {
			return *this;
		}
		hide();
		if (const auto handler =
				getComponentRegistry()
					.getComponent<components::Overlay>(getIdentifier())
					.getOnDismissHandler()) {
			handler();
		}
		return *this;
	}

	Dialog &Dialog::setOnDismiss(std::function<void(void)> onDismiss)
	{
		getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.setOnDismissHandler(std::move(onDismiss));
		return *this;
	}

	std::size_t Dialog::getActionCount(void) const
	{
		return _actionEntities.size();
	}

	ecs::Entity::Identifier Dialog::getActionIdentifier(std::size_t index) const
	{
		if (index >= _actionEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _actionEntities[index]->getIdentifier();
	}

	ecs::Entity::Identifier Dialog::getActionRowIdentifier(void) const
	{
		return _actionRow != nullptr ? _actionRow->getIdentifier()
									 : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier Dialog::getBackdropIdentifier(void) const
	{
		return _backdrop != nullptr ? _backdrop->getIdentifier()
									: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier Dialog::getIconIdentifier(void) const
	{
		return _iconEntity != nullptr ? _iconEntity->getIdentifier()
									  : ecs::Entity::InvalidIdentifier;
	}

	void Dialog::initialize(void)
	{
		SurfaceBase::initialize();
		buildContent();
		applyVariant();
		applyOverlay();
	}

	void Dialog::update(void)
	{
		SurfaceBase::update();
		buildContent();
		applyVariant();
		applyOverlay();
	}

}	 // namespace guillaume::entities
