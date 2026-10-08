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

#include "guillaume/entities/snackbar.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Snackbar::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Snackbar>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Snackbar> Snackbar::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Snackbar>(
			this->getComponentRegistry(), _config, _variant, _message, _action);
		return entity;
	}

	void Snackbar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::SingleLine;
		_message.clear();
		_action.clear();

		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::SpaceBetween;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 16.0f;
		_config.spacing = 8.0f;
		_config.setFixedWidth(344.0f);
	}

	Snackbar::Builder &
		Snackbar::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Snackbar::Builder &Snackbar::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Snackbar::Builder &
		Snackbar::Builder::withMessage(const std::string &message)
	{
		_message = message;
		return *this;
	}

	Snackbar::Builder &Snackbar::Builder::withAction(const std::string &action)
	{
		_action = action;
		return *this;
	}

	std::shared_ptr<Snackbar> Snackbar::Director::makeSnackbar(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &message, const std::string &action)
	{
		return builder.withVariant(variant)
			.withMessage(message)
			.withAction(action)
			.registerEntity(parent);
	}

	Snackbar::Snackbar(ecs::ComponentRegistry &registry,
					   const SurfaceConfig &config, Variant variant,
					   const std::string &message, const std::string &action)
		: SurfaceBase<components::Overlay>(registry, config)
		, _variant(variant)
		, _message(message)
		, _action(action)
		, _messageEntity()
		, _actionEntity()
	{
		applyVariant();
		applyOverlay();
	}

	Snackbar::~Snackbar(void)
	{
	}

	float Snackbar::getVariantHeight(void) const
	{
		switch (_variant) {
			case Variant::MultiLine:
				return 68.0f;
			case Variant::SingleLine:
			case Variant::WithAction:
			default:
				return 48.0f;
		}
	}

	void Snackbar::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis = components::Layout::Axis::Horizontal;
		config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::SpaceBetween;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		config.padding		  = 16.0f;
		config.spacing		  = 8.0f;
		config.borderRadius	  = 4.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = getVariantHeight();

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::InverseSurface));
	}

	void Snackbar::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(false);
		overlay.setDismissOnOutsideClick(false);
		overlay.setAutoDismiss(4.0f);
	}

	void Snackbar::buildContent(void)
	{
		const auto messageColor =
			schemeColor(SchemeColorRole::InverseOnSurface);

		if (_messageEntity == nullptr && !_message.empty()) {
			_messageEntity = buildText(this->getComponentRegistry(), *this,
									   this->shared_from_this(), _message,
									   14.0f, messageColor);
		}

		if (_variant == Variant::WithAction && _actionEntity == nullptr
			&& !_action.empty()) {
			_actionEntity = buildText(
				this->getComponentRegistry(), *this, this->shared_from_this(),
				_action, 14.0f, schemeColor(SchemeColorRole::InversePrimary));
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_messageEntity != nullptr) {
			children.push_back(_messageEntity);
		}

		if (_actionEntity != nullptr) {
			children.push_back(_actionEntity);
		}

		setChildren(children);
	}

	Snackbar &Snackbar::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Snackbar::Variant Snackbar::getVariant(void) const
	{
		return _variant;
	}

	Snackbar &Snackbar::setMessage(const std::string &message)
	{
		_message = message;

		if (_messageEntity != nullptr) {
			_messageEntity->setContent(message);
		}

		return *this;
	}

	void Snackbar::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		buildContent();
	}

	void Snackbar::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildContent();
	}

}	 // namespace guillaume::entities
