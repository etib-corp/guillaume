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

#include "guillaume/entities/snackbar.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/snackbar.hpp"
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
		auto entity = Snackbar::create(this->getComponentRegistry(), _config,
									   _variant, _message, _action);
		entity->setDuration(_duration);
		entity->setOnDismiss(_onDismiss);
		entity->setOnAction(_onAction);
		entity->setVisible(_visible);
		return entity;
	}

	void Snackbar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::SingleLine;
		_message.clear();
		_action.clear();
		_visible   = true;
		_duration  = DurationMedium;
		_onDismiss = {};
		_onAction  = {};

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

	Snackbar::Builder &Snackbar::Builder::withVisible(bool visible)
	{
		_visible = visible;
		return *this;
	}

	Snackbar::Builder &Snackbar::Builder::withDuration(float seconds)
	{
		_duration = seconds;
		return *this;
	}

	Snackbar::Builder &
		Snackbar::Builder::withOnDismiss(std::function<void(void)> onDismiss)
	{
		_onDismiss = std::move(onDismiss);
		return *this;
	}

	Snackbar::Builder &
		Snackbar::Builder::withOnAction(std::function<void(void)> onAction)
	{
		_onAction = std::move(onAction);
		return *this;
	}

	std::shared_ptr<Snackbar> Snackbar::Builder::makeSnackbar(
		std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &message, const std::string &action)
	{
		return withVariant(variant)
			.withMessage(message)
			.withAction(action)
			.registerEntity(parent);
	}

	Snackbar::Snackbar(ecs::ComponentRegistry &registry,
					   const SurfaceConfig &config, Variant variant,
					   const std::string &message, const std::string &action)
		: SurfaceBase<components::Overlay, components::Elevation,
					  components::Accessibility, components::Snackbar>(registry,
																	   config)
	{
		getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.setVariant(variant)
			.setMessage(message)
			.setAction(action);
		systems::Snackbar::install(getComponentRegistry(), getIdentifier());
		systems::Snackbar::apply(getComponentRegistry(), getIdentifier());
	}

	Snackbar &Snackbar::setVisible(bool visible)
	{
		if (visible) {
			systems::Snackbar::show(getComponentRegistry(), getIdentifier());
		} else {
			systems::Snackbar::hide(getComponentRegistry(), getIdentifier());
		}
		return *this;
	}

	Snackbar &Snackbar::show(void)
	{
		return setVisible(true);
	}

	Snackbar &Snackbar::hide(void)
	{
		return setVisible(false);
	}

	bool Snackbar::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.isVisible();
	}

	Snackbar &Snackbar::setDuration(float seconds)
	{
		getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.setDuration(seconds);
		systems::Snackbar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	float Snackbar::getDuration(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.getDuration();
	}

	Snackbar &Snackbar::setOnDismiss(std::function<void(void)> onDismiss)
	{
		getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.setOnDismiss(std::move(onDismiss));
		return *this;
	}

	Snackbar &Snackbar::setOnAction(std::function<void(void)> onAction)
	{
		getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.setOnAction(std::move(onAction));
		return *this;
	}

	Snackbar &Snackbar::pressAction(void)
	{
		systems::Snackbar::pressAction(getComponentRegistry(), getIdentifier());
		return *this;
	}

	std::shared_ptr<Text> Snackbar::getActionEntity(void) const
	{
		// The lookup is const because this getter is; the child itself is
		// shared and mutable, as it always was.
		return std::const_pointer_cast<Text>(getEntity<Text>(
			getComponentRegistry()
				.getComponent<components::Snackbar>(getIdentifier())
				.getActionEntity()));
	}

	Snackbar &Snackbar::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.setVariant(variant);
		buildContent();
		systems::Snackbar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Snackbar::Variant Snackbar::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.getVariant();
	}

	Snackbar &Snackbar::setMessage(const std::string &message)
	{
		getComponentRegistry()
			.getComponent<components::Snackbar>(getIdentifier())
			.setMessage(message);
		buildContent();
		systems::Snackbar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	void Snackbar::build(void)
	{
		buildContent();
		systems::Snackbar::apply(getComponentRegistry(), getIdentifier());
	}

	void Snackbar::buildContent(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		auto &snackbar =
			registry.getComponent<components::Snackbar>(getIdentifier());

		if (snackbar.getMessageEntity() == ecs::Entity::InvalidIdentifier) {
			const auto message =
				buildText(registry, *this, shared_from_this(),
						  snackbar.getMessage(), theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::InverseOnSurface));
			if (message != nullptr) {
				snackbar.setMessageEntity(message->getIdentifier());
			}
		}

		if (snackbar.getVariant() == Variant::WithAction
			&& snackbar.getActionEntity() == ecs::Entity::InvalidIdentifier) {
			const auto action =
				buildText(registry, *this, shared_from_this(),
						  snackbar.getAction(), theme::TypeRole::LabelLarge,
						  schemeColor(SchemeColorRole::InversePrimary));
			if (action != nullptr) {
				snackbar.setActionEntity(action->getIdentifier());
				systems::Snackbar::installAction(registry, getIdentifier());
			}
		}

		// Hand the layout children to the surface.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (auto message = getEntity<Text>(snackbar.getMessageEntity())) {
			children.push_back(std::move(message));
		}
		if (auto action = getEntity<Text>(snackbar.getActionEntity())) {
			children.push_back(std::move(action));
		}
		setChildren(children);
	}

}	 // namespace guillaume::entities
