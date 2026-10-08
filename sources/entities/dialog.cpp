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
#include "guillaume/entities/overlay_helpers.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Dialog::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Dialog>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Dialog> Dialog::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Dialog>(this->getComponentRegistry(), _config,
									 _variant, _title, _message, _actions);
		return entity;
	}

	void Dialog::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Alert;
		_title.clear();
		_message.clear();
		_actions.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding		 = 24.0f;
		_config.spacing		 = 16.0f;
		_config.borderRadius = 28.0f;
		_config.setFixedWidth(320.0f);
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

	Dialog::Builder &Dialog::Builder::addAction(const std::string &label)
	{
		_actions.push_back(label);
		return *this;
	}

	std::shared_ptr<Dialog> Dialog::Director::makeDialog(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &title, const std::string &message,
		const std::vector<std::string> &actions)
	{
		builder.withVariant(variant).withTitle(title).withMessage(message);

		for (const auto &action: actions) {
			builder.addAction(action);
		}

		return builder.registerEntity(parent);
	}

	Dialog::Dialog(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant,
				   const std::string &title, const std::string &message,
				   const std::vector<std::string> &actions)
		: SurfaceBase<components::Overlay>(registry, config)
		, _variant(variant)
		, _title(title)
		, _message(message)
		, _actions(actions)
		, _iconEntity()
		, _titleEntity()
		, _messageEntity()
		, _actionEntities()
	{
		applyVariant();
		applyOverlay();
	}

	Dialog::~Dialog(void)
	{
	}

	void Dialog::applyVariant(void)
	{
		auto config = SurfaceConfig::panel(
			320.0f, components::Layout::Axis::Vertical, 16.0f);
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;

		switch (_variant) {
			case Variant::Alert:
			case Variant::Simple:
			case Variant::Confirmation: {
				config.hasFixedHeight = false;
				break;
			}
			case Variant::FullScreen:
			default: {
				config.borderRadius	  = 0.0f;
				config.hasFixedWidth  = false;
				config.fixedWidth	  = 0.0f;
				config.hasFixedHeight = true;
				config.fixedHeight	  = 640.0f;
				break;
			}
		}

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::SurfaceContainerHigh));
	}

	void Dialog::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(true);
		overlay.setDismissOnOutsideClick(_variant != Variant::FullScreen);
		overlay.setDismissOnEscape(true);
	}

	void Dialog::buildContent(void)
	{
		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_variant == Variant::Confirmation && _iconEntity == nullptr) {
			_iconEntity = buildIcon(this->getComponentRegistry(), *this,
									this->shared_from_this(), "info", 24.0f,
									schemeColor(SchemeColorRole::Primary));
		}

		if (_titleEntity == nullptr && !_title.empty()) {
			_titleEntity = buildText(this->getComponentRegistry(), *this,
									 this->shared_from_this(), _title, 22.0f,
									 schemeColor(SchemeColorRole::OnSurface));
		}

		if (_messageEntity == nullptr && !_message.empty()) {
			_messageEntity =
				buildText(this->getComponentRegistry(), *this,
						  this->shared_from_this(), _message, 14.0f,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		std::size_t expectedActions = 0;
		for (const auto &action: _actions) {
			if (!action.empty()) {
				++expectedActions;
			}
		}

		if (_actionEntities.size() != expectedActions) {
			_actionEntities.clear();

			for (const auto &action: _actions) {
				auto actionEntity =
					buildText(this->getComponentRegistry(), *this,
							  this->shared_from_this(), action, 14.0f,
							  schemeColor(SchemeColorRole::Primary));

				if (actionEntity != nullptr) {
					_actionEntities.push_back(actionEntity);
				}
			}
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

		for (const auto &actionEntity: _actionEntities) {
			children.push_back(actionEntity);
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
		return *this;
	}

	Dialog &Dialog::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
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

	void Dialog::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		buildContent();
	}

	void Dialog::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildContent();
	}

}	 // namespace guillaume::entities
