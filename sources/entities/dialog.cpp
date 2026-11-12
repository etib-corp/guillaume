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

#include "guillaume/entities/dialog.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/dialog.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/// M3 dialog width, in dp.
		constexpr float DialogWidth = components::Dialog::Width;

		/// M3 dialog padding, in dp.
		constexpr float DialogPadding = systems::Dialog::Padding;

		/// Gap between dialog sections, in dp.
		constexpr float DialogSpacing = systems::Dialog::Spacing;

		/// Size of the hero icon, in dp.
		constexpr float HeroIconSize = 24.0f;

		/**
		 * @brief Turn action labels into actions without a press handler.
		 * @param labels The action labels.
		 * @return One action per label.
		 */
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
		: SurfaceBase<components::Overlay, components::Elevation,
					  components::Accessibility, components::Dialog>(registry,
																	 config)
	{
		getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.setVariant(variant)
			.setTitle(title)
			.setMessage(message)
			.setActions(actions)
			.setIcon(icon);
		getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.setOnDismissHandler(std::move(onDismiss));
		systems::Dialog::install(getComponentRegistry(), getIdentifier());
		systems::Dialog::apply(getComponentRegistry(), getIdentifier());
	}

	Dialog &Dialog::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.setVariant(variant);
		// A Confirmation dialog may need its default hero icon.
		buildContent();
		systems::Dialog::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Dialog::Variant Dialog::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.getVariant();
	}

	Dialog &Dialog::setTitle(const std::string &title)
	{
		getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.setTitle(title);
		buildContent();
		systems::Dialog::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Dialog &Dialog::setMessage(const std::string &message)
	{
		getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.setMessage(message);
		buildContent();
		systems::Dialog::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Dialog &Dialog::show(void)
	{
		systems::Dialog::show(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Dialog &Dialog::hide(void)
	{
		systems::Dialog::hide(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool Dialog::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.isVisible();
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
		systems::Dialog::dismiss(getComponentRegistry(), getIdentifier());
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
		return getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.getActionEntities()
			.size();
	}

	ecs::Entity::Identifier Dialog::getActionIdentifier(std::size_t index) const
	{
		const auto &actions =
			getComponentRegistry()
				.getComponent<components::Dialog>(getIdentifier())
				.getActionEntities();

		return index < actions.size() ? actions[index]
									  : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier Dialog::getActionRowIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.getActionRow();
	}

	ecs::Entity::Identifier Dialog::getBackdropIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.getBackdrop();
	}

	ecs::Entity::Identifier Dialog::getIconIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Dialog>(getIdentifier())
			.getIconEntity();
	}

	void Dialog::initialize(void)
	{
		buildContent();
		systems::Dialog::apply(getComponentRegistry(), getIdentifier());
	}

	void Dialog::buildContent(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		auto &dialog =
			registry.getComponent<components::Dialog>(getIdentifier());

		if (dialog.getBackdrop() == ecs::Entity::InvalidIdentifier) {
			const auto backdrop = std::make_shared<Backdrop>(registry);
			backdrop->setParent(shared_from_this());
			addEntity(backdrop);
			dialog.setBackdrop(backdrop->getIdentifier());
		}

		const std::string glyph = dialog.getEffectiveIcon();
		if (dialog.getIconEntity() == ecs::Entity::InvalidIdentifier
			&& !glyph.empty()) {
			const auto icon = buildIcon(
				registry, *this, shared_from_this(), glyph, HeroIconSize,
				schemeColor(SchemeColorRole::Secondary));
			if (icon != nullptr) {
				dialog.setIconEntity(icon->getIdentifier());
			}
		}

		if (dialog.getTitleEntity() == ecs::Entity::InvalidIdentifier
			&& !dialog.getTitle().empty()) {
			const auto title =
				buildText(registry, *this, shared_from_this(),
						  dialog.getTitle(), theme::TypeRole::HeadlineSmall,
						  schemeColor(SchemeColorRole::OnSurface));
			if (title != nullptr) {
				dialog.setTitleEntity(title->getIdentifier());
				systems::Dialog::installTitle(registry, getIdentifier());
			}
		}

		if (dialog.getMessageEntity() == ecs::Entity::InvalidIdentifier
			&& !dialog.getMessage().empty()) {
			const auto message =
				buildText(registry, *this, shared_from_this(),
						  dialog.getMessage(), theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (message != nullptr) {
				dialog.setMessageEntity(message->getIdentifier());
			}
		}

		if (dialog.getActionRow() == ecs::Entity::InvalidIdentifier
			&& !dialog.getActions().empty()) {
			buildActionRow();
		}

		// Hand the layout children to the surface.
		Children children;
		if (auto icon = getEntity<Icon>(dialog.getIconEntity())) {
			children.push_back(std::move(icon));
		}
		if (auto title = getEntity<Text>(dialog.getTitleEntity())) {
			children.push_back(std::move(title));
		}
		if (auto message = getEntity<Text>(dialog.getMessageEntity())) {
			children.push_back(std::move(message));
		}
		if (auto row = getEntity<View>(dialog.getActionRow())) {
			children.push_back(std::move(row));
		}
		setChildren(children);
	}

	void Dialog::buildActionRow(void)
	{
		auto &registry = getComponentRegistry();
		auto &dialog =
			registry.getComponent<components::Dialog>(getIdentifier());

		View::Builder rowBuilder(registry, *this);
		const auto row = rowBuilder.makeDefaultView(
			shared_from_this(), utility::graphic::PoseF(), {});
		if (row == nullptr) {
			return;
		}
		row->setColor(transparentColor());
		row->setAxis(components::Layout::Axis::Horizontal);
		row->setMainAxisAlignment(components::Layout::MainAxisAlignment::End);
		row->setCrossAxisAlignment(
			components::Layout::CrossAxisAlignment::Center);
		row->setSpacing(theme::SpacingScale::get(2));
		row->setPadding(0.0f);
		row->setFixedWidth(DialogWidth - 2.0f * DialogPadding);

		// The press handlers outlive this call: capture the registry and
		// the identifier, never the entity object.
		ecs::ComponentRegistry *registryPointer	 = &registry;
		const ecs::Entity::Identifier identifier = getIdentifier();
		const auto &actions						 = dialog.getActions();
		std::vector<std::shared_ptr<ecs::Entity>> buttons;
		std::vector<ecs::Entity::Identifier> identifiers;

		for (std::size_t index = 0; index < actions.size(); ++index) {
			if (actions[index].label.empty()) {
				continue;
			}
			Button::Builder buttonBuilder(registry, *row);
			const auto button =
				buttonBuilder.withLabel(actions[index].label)
					.withColorStyle(Button::Color::Text)
					.withShape(Button::Shape::Round)
					.withSize(Button::Size::Small)
					.withOnClick([registryPointer, identifier, index]() {
						systems::Dialog::pressAction(*registryPointer,
													 identifier, index);
					})
					.registerEntity(row);
			if (button != nullptr) {
				buttons.push_back(button);
				identifiers.push_back(button->getIdentifier());
			}
		}
		row->setEntities(buttons);

		dialog.setActionEntities(identifiers)
			.setActionRow(row->getIdentifier());
	}

}	 // namespace guillaume::entities
