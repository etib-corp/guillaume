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

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "guillaume/entities/card.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/image.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/systems/card.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief Layout-only section of a composed card (title, actions).
		 */
		class CardSection: public SurfaceBase<>
		{
			public:
			/**
			 * @brief Construct a transparent section.
			 * @param registry Component registry.
			 * @param config Section configuration.
			 */
			CardSection(ecs::ComponentRegistry &registry,
						const SurfaceConfig &config)
				: SurfaceBase<>(registry, config)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~CardSection(void) override = default;
		};

		constexpr float DefaultWidth = 240.0f;
	}	 // namespace

	Card::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Card>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Card> Card::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Card>(this->getComponentRegistry(),
											 _config, _variant);
		if (_onPress) {
			entity->setOnPress(_onPress);
		}
		entity->setDisabled(_disabled);
		return entity;
	}

	void Card::Builder::reset(void)
	{
		_config				 = SurfaceConfig();
		_variant			 = Variant::Elevated;
		_onPress			 = {};
		_disabled			 = false;
		_config.borderRadius = getShapeRadius(theme::ShapeRole::Medium);
		_config.padding		 = 16.0f;
		_config.spacing		 = 8.0f;
		_config.axis		 = components::Layout::Axis::Vertical;
		_config.setFixedWidth(DefaultWidth);
	}

	Card::Builder &Card::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Card::Builder &Card::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Card::Builder &Card::Builder::withWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	Card::Builder &Card::Builder::withHeight(float height)
	{
		_config.hasFixedHeight = true;
		_config.fixedHeight	   = height;
		return *this;
	}

	Card::Builder &Card::Builder::withBorderRadius(float radius)
	{
		_config.borderRadius = radius;
		return *this;
	}

	Card::Builder &Card::Builder::withOnPress(std::function<void(void)> onPress)
	{
		_onPress = std::move(onPress);
		return *this;
	}

	Card::Builder &Card::Builder::withDisabled(bool disabled)
	{
		_disabled = disabled;
		return *this;
	}

	std::shared_ptr<Card>
		Card::Builder::makeCard(std::shared_ptr<ecs::Entity> parent,
								Variant variant)
	{
		return withVariant(variant).registerEntity(parent);
	}

	Card::Card(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant)
		: SurfaceBase<components::Elevation, components::Animation,
					  components::Card>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.setVariant(variant);
		systems::Card::install(getComponentRegistry(), getIdentifier());
		systems::Card::apply(getComponentRegistry(), getIdentifier());
	}

	Card::~Card(void)
	{
	}

	Card &Card::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.setVariant(variant);
		systems::Card::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Card::Variant Card::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.getVariant();
	}

	Card &Card::setOnPress(std::function<void(void)> onPress)
	{
		systems::Card::setOnPress(getComponentRegistry(), getIdentifier(),
								  std::move(onPress));
		return *this;
	}

	bool Card::isInteractive(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.isInteractive();
	}

	Card &Card::setDisabled(bool disabled)
	{
		systems::Card::setDisabled(getComponentRegistry(), getIdentifier(),
								   disabled);
		return *this;
	}

	bool Card::isDisabled(void) const
	{
		return systems::Card::isDisabled(getComponentRegistry(),
										 getIdentifier());
	}

	Card &Card::setCover(const std::string &texturePath)
	{
		getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.setCoverPath(texturePath);
		// A first cover needs its image child.
		buildComposition();
		systems::Card::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Card &Card::setTitle(const std::string &title, const std::string &subtitle)
	{
		getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.setTitle(title)
			.setSubtitle(subtitle);
		// A first title needs its section and text children.
		buildComposition();
		systems::Card::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Card &Card::setContent(const std::string &content)
	{
		getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.setContentText(content);
		// A first content needs its text child.
		buildComposition();
		systems::Card::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	Card &Card::addAction(const std::string &label,
						  std::function<void(void)> onPress)
	{
		getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.addAction(components::Card::Action { label, std::move(onPress) });
		// Every action needs its button child.
		buildComposition();
		systems::Card::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	std::shared_ptr<Image> Card::getCover(void) const
	{
		// The lookup is const because this getter is; the child itself is
		// shared and mutable, as it always was.
		return std::const_pointer_cast<Image>(getEntity<Image>(
			getComponentRegistry()
				.getComponent<components::Card>(getIdentifier())
				.getCover()));
	}

	std::shared_ptr<ecs::Entity> Card::getTitleSection(void) const
	{
		return std::const_pointer_cast<ecs::Entity>(getEntity<ecs::Entity>(
			getComponentRegistry()
				.getComponent<components::Card>(getIdentifier())
				.getTitleSection()));
	}

	std::shared_ptr<Text> Card::getContent(void) const
	{
		return std::const_pointer_cast<Text>(
			getEntity<Text>(getComponentRegistry()
								.getComponent<components::Card>(getIdentifier())
								.getContentEntity()));
	}

	std::shared_ptr<ecs::Entity> Card::getActionsSection(void) const
	{
		return std::const_pointer_cast<ecs::Entity>(getEntity<ecs::Entity>(
			getComponentRegistry()
				.getComponent<components::Card>(getIdentifier())
				.getActionsSection()));
	}

	std::vector<std::shared_ptr<Button>> Card::getActions(void) const
	{
		const auto &card =
			getComponentRegistry().getComponent<components::Card>(
				getIdentifier());
		const auto row = getEntity<CardSection>(card.getActionsSection());
		std::vector<std::shared_ptr<Button>> actions;

		if (row == nullptr) {
			return actions;
		}
		// The buttons are owned by the actions row.
		for (const auto identifier: card.getActionButtons()) {
			if (auto button = std::const_pointer_cast<Button>(
					row->getEntity<Button>(identifier))) {
				actions.push_back(std::move(button));
			}
		}
		return actions;
	}

	float Card::getPressProgress(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Card>(getIdentifier())
			.getPressProgress();
	}

	void Card::buildComposition(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry	= getComponentRegistry();
		const auto self = shared_from_this();
		const float inner =
			systems::Card::contentWidth(registry, getIdentifier());
		auto &card = registry.getComponent<components::Card>(getIdentifier());

		if (card.getCover() == ecs::Entity::InvalidIdentifier
			&& !card.getCoverPath().empty()) {
			Image::Builder builder(registry, *this);
			builder.withTexturePath(card.getCoverPath());
			if (inner > 0.0f) {
				builder.withSize(inner, CoverHeight);
			}
			const auto cover = builder.registerEntity(self);
			if (cover != nullptr) {
				card.setCover(cover->getIdentifier());
			}
		}

		if (card.getTitleSection() == ecs::Entity::InvalidIdentifier
			&& !card.getTitle().empty()) {
			SurfaceConfig sectionConfig;
			sectionConfig.axis = components::Layout::Axis::Vertical;
			sectionConfig.mainAxisAlignment =
				components::Layout::MainAxisAlignment::Center;
			sectionConfig.crossAxisAlignment =
				components::Layout::CrossAxisAlignment::Start;
			sectionConfig.setFixedHeight(TitleMinHeight);
			if (inner > 0.0f) {
				sectionConfig.setFixedWidth(inner);
			}
			auto section =
				std::make_shared<CardSection>(registry, sectionConfig);
			section->setParent(self);
			this->addEntity(section);
			card.setTitleSection(section->getIdentifier());

			const auto titleText =
				buildText(registry, *section, section, card.getTitle(),
						  theme::TypeRole::BodyLarge,
						  schemeColor(SchemeColorRole::OnSurface));
			const auto subtitleText =
				buildText(registry, *section, section, card.getSubtitle(),
						  theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));

			std::vector<std::shared_ptr<ecs::Entity>> texts;
			if (titleText != nullptr) {
				card.setTitleText(titleText->getIdentifier());
				texts.push_back(titleText);
			}
			if (subtitleText != nullptr) {
				card.setSubtitleText(subtitleText->getIdentifier());
				texts.push_back(subtitleText);
			}
			section->setChildren(texts);
			registry.addComponent<components::Accessibility>(
				section->getIdentifier());
			registry
				.getComponent<components::Accessibility>(
					section->getIdentifier())
				.setRole(components::AccessibilityRole::Header)
				.setLabel(card.getTitle());
		}

		if (card.getContentEntity() == ecs::Entity::InvalidIdentifier
			&& !card.getContentText().empty()) {
			const auto content =
				buildText(registry, *this, self, card.getContentText(),
						  theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (content != nullptr) {
				card.setContentEntity(content->getIdentifier());
			}
		}

		if (!card.getActions().empty()) {
			if (card.getActionsSection() == ecs::Entity::InvalidIdentifier) {
				SurfaceConfig rowConfig;
				rowConfig.axis = components::Layout::Axis::Horizontal;
				rowConfig.mainAxisAlignment =
					components::Layout::MainAxisAlignment::End;
				rowConfig.crossAxisAlignment =
					components::Layout::CrossAxisAlignment::Center;
				rowConfig.spacing = 8.0f;
				if (inner > 0.0f) {
					rowConfig.setFixedWidth(inner);
				}
				auto section =
					std::make_shared<CardSection>(registry, rowConfig);
				section->setParent(self);
				this->addEntity(section);
				card.setActionsSection(section->getIdentifier());
			}

			const auto row	 = getEntity<CardSection>(card.getActionsSection());
			auto identifiers = card.getActionButtons();
			while (row != nullptr
				   && identifiers.size() < card.getActions().size()) {
				const auto &action = card.getActions()[identifiers.size()];
				Button::Builder builder(registry, *row);
				const auto button = builder.withLabel(action.label)
										.withOnClick(action.onPress)
										.withColorStyle(Button::Color::Text)
										.registerEntity(row);
				identifiers.push_back(button != nullptr
										  ? button->getIdentifier()
										  : ecs::Entity::InvalidIdentifier);
			}
			card.setActionButtons(identifiers);

			if (row != nullptr) {
				std::vector<std::shared_ptr<ecs::Entity>> buttons;
				for (const auto identifier: identifiers) {
					if (auto button = row->getEntity<Button>(identifier)) {
						buttons.push_back(std::move(button));
					}
				}
				row->setChildren(buttons);
			}
		}

		// Hand the layout children to the surface.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		for (const auto identifier:
			 { card.getCover(), card.getTitleSection(), card.getContentEntity(),
			   card.getActionsSection() }) {
			if (auto child = getEntity<ecs::Entity>(identifier)) {
				children.push_back(std::move(child));
			}
		}
		setChildren(children);
	}

	void Card::initialize(void)
	{
		buildComposition();
		systems::Card::apply(getComponentRegistry(), getIdentifier());
	}

}	 // namespace guillaume::entities
