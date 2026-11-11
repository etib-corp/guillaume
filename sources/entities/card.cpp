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

#include "guillaume/entities/card.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/image.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
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

		utility::graphic::Color32Bit
			lerpColor(const utility::graphic::Color32Bit &from,
					  const utility::graphic::Color32Bit &to, float t)
		{
			const auto mix = [t](std::uint8_t a, std::uint8_t b) {
				return static_cast<std::uint8_t>(
					static_cast<float>(a)
					+ (static_cast<float>(b) - static_cast<float>(a)) * t
					+ 0.5f);
			};
			return utility::graphic::Color32Bit(
				mix(from.getRed(), to.getRed()),
				mix(from.getGreen(), to.getGreen()),
				mix(from.getBlue(), to.getBlue()),
				mix(from.getAlpha(), to.getAlpha()));
		}

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
		: SurfaceBase<components::Elevation, components::Animation>(registry,
																	config)
		, _variant(variant)
	{
		getComponentRegistry()
			.getComponent<components::Animation>(getIdentifier())
			.setDuration(getMotionDuration(theme::MotionDuration::Short3))
			.setEasing(theme::MotionScale::standard())
			.setOnSampleHandler([this](float progress) {
				const float target = _pressed ? 1.0f : 0.0f;
				_pressProgress = _pressFrom + (target - _pressFrom) * progress;
				applyVariant();
			});
		applyVariant();
	}

	Card::~Card(void)
	{
	}

	void Card::applyVariant(void)
	{
		auto &borders =
			getComponentRegistry().getComponent<components::Borders>(
				getIdentifier());
		auto &elevation =
			getComponentRegistry().getComponent<components::Elevation>(
				getIdentifier());

		utility::graphic::Color32Bit container;

		switch (_variant) {
			case Variant::Elevated: {
				// Paper raises an elevated card from level 1 to level 2 while
				// pressed; the shadow and the tonal tint follow the motion.
				const auto &rest =
					getElevationToken(theme::ElevationLevel::Level1);
				const auto &raised =
					getElevationToken(theme::ElevationLevel::Level2);
				const float t = _pressProgress;
				container	  = lerpColor(
					getElevatedSurfaceColor(theme::ElevationLevel::Level1),
					getElevatedSurfaceColor(theme::ElevationLevel::Level2), t);
				elevation.setElevationLevel(
					t >= 0.5f ? theme::ElevationLevel::Level2
							  : theme::ElevationLevel::Level1);
				elevation
					.setSpread(rest.shadowSpread
							   + (raised.shadowSpread - rest.shadowSpread) * t)
					.setColor(utility::graphic::Color32Bit(
						0, 0, 0,
						static_cast<std::uint8_t>(
							static_cast<float>(rest.shadowAlpha)
							+ (static_cast<float>(raised.shadowAlpha)
							   - static_cast<float>(rest.shadowAlpha))
								* t)));
				borders.setColor(transparentColor()).setWidth(0.0f);
				break;
			}
			case Variant::Filled: {
				container = schemeColor(SchemeColorRole::SurfaceVariant);
				borders.setColor(transparentColor()).setWidth(0.0f);
				elevation.setElevationLevel(theme::ElevationLevel::Level0);
				break;
			}
			case Variant::Outlined:
			default: {
				container = schemeColor(SchemeColorRole::Surface);
				borders.setColor(schemeColor(SchemeColorRole::OutlineVariant))
					.setWidth(1.0f);
				elevation.setElevationLevel(theme::ElevationLevel::Level0);
				break;
			}
		}

		if (_interactive) {
			container = stateLayerColor(
				getComponentRegistry(), getIdentifier(), container,
				schemeColor(SchemeColorRole::OnSurface));
		}
		setColor(container);
	}

	void Card::onInteractionChanged(void)
	{
		const bool pressed = _variant == Variant::Elevated
			&& entities::isPressed(getComponentRegistry(), getIdentifier());
		if (pressed != _pressed) {
			_pressed   = pressed;
			_pressFrom = _pressProgress;
			getComponentRegistry()
				.getComponent<components::Animation>(getIdentifier())
				.play();
		}
		applyVariant();
	}

	Card &Card::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Card::Variant Card::getVariant(void) const
	{
		return _variant;
	}

	Card &Card::setOnPress(std::function<void(void)> onPress)
	{
		_onPress = std::move(onPress);
		if (!_interactive) {
			_interactive = true;
			makeInteractive(
				getComponentRegistry(), getIdentifier(),
				components::AccessibilityRole::Button,
				[this]() {
					if (_onPress) {
						_onPress();
					}
				},
				[this]() {
					onInteractionChanged();
				});
		}
		applyVariant();
		return *this;
	}

	bool Card::isInteractive(void) const
	{
		return _interactive;
	}

	Card &Card::setDisabled(bool disabled)
	{
		setEnabled(getComponentRegistry(), getIdentifier(), !disabled);
		if (disabled && _pressed) {
			_pressed	   = false;
			_pressProgress = 0.0f;
		}
		applyVariant();
		return *this;
	}

	bool Card::isDisabled(void) const
	{
		return !isEnabled(getComponentRegistry(), getIdentifier());
	}

	Card &Card::setCover(const std::string &texturePath)
	{
		_coverPath = texturePath;
		if (_cover != nullptr) {
			_cover->setTexturePath(texturePath);
		}
		buildComposition();
		return *this;
	}

	Card &Card::setTitle(const std::string &title, const std::string &subtitle)
	{
		_title	  = title;
		_subtitle = subtitle;
		if (_titleText != nullptr) {
			_titleText->setContent(title);
		}
		if (_subtitleText != nullptr) {
			_subtitleText->setContent(subtitle);
		}
		buildComposition();
		return *this;
	}

	Card &Card::setContent(const std::string &content)
	{
		_contentText = content;
		if (_content != nullptr) {
			_content->setContent(content);
		}
		buildComposition();
		return *this;
	}

	Card &Card::addAction(const std::string &label,
						  std::function<void(void)> onPress)
	{
		_actionSpecs.push_back(ActionSpec { label, std::move(onPress) });
		buildComposition();
		return *this;
	}

	std::shared_ptr<Image> Card::getCover(void) const
	{
		return _cover;
	}

	std::shared_ptr<ecs::Entity> Card::getTitleSection(void) const
	{
		return _titleSection;
	}

	std::shared_ptr<Text> Card::getContent(void) const
	{
		return _content;
	}

	std::shared_ptr<ecs::Entity> Card::getActionsSection(void) const
	{
		return _actionsSection;
	}

	const std::vector<std::shared_ptr<Button>> &Card::getActions(void) const
	{
		return _actions;
	}

	float Card::getPressProgress(void) const
	{
		return _pressProgress;
	}

	void Card::buildComposition(void)
	{
		if (!_initialized) {
			return;
		}

		auto &registry	   = getComponentRegistry();
		const auto self	   = shared_from_this();
		const auto &config = getSurfaceConfig();
		const float width  = config.hasFixedWidth ? config.fixedWidth : 0.0f;
		const float inner  = std::max(0.0f, width - 2.0f * config.padding);

		if (_cover == nullptr && !_coverPath.empty()) {
			Image::Builder builder(registry, *this);
			builder.withTexturePath(_coverPath);
			if (inner > 0.0f) {
				builder.withSize(inner, CoverHeight);
			}
			_cover = builder.registerEntity(self);
		}

		if (_titleSection == nullptr && !_title.empty()) {
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
			_titleSection = section;

			_titleText = buildText(registry, *section, section, _title,
								   theme::TypeRole::BodyLarge,
								   schemeColor(SchemeColorRole::OnSurface));
			_subtitleText =
				buildText(registry, *section, section, _subtitle,
						  theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));

			std::vector<std::shared_ptr<ecs::Entity>> texts;
			if (_titleText != nullptr) {
				texts.push_back(_titleText);
			}
			if (_subtitleText != nullptr) {
				texts.push_back(_subtitleText);
			}
			section->setChildren(texts);
			registry.addComponent<components::Accessibility>(
				section->getIdentifier());
			registry
				.getComponent<components::Accessibility>(
					section->getIdentifier())
				.setRole(components::AccessibilityRole::Header)
				.setLabel(_title);
		}

		if (_content == nullptr && !_contentText.empty()) {
			_content =
				buildText(registry, *this, self, _contentText,
						  theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		if (!_actionSpecs.empty()) {
			if (_actionsSection == nullptr) {
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
				auto row = std::make_shared<CardSection>(registry, rowConfig);
				row->setParent(self);
				this->addEntity(row);
				_actionsSection = row;
			}

			auto row = std::static_pointer_cast<CardSection>(_actionsSection);
			while (_actions.size() < _actionSpecs.size()) {
				const auto &spec = _actionSpecs[_actions.size()];
				Button::Builder builder(registry, *row);
				_actions.push_back(builder.withLabel(spec.label)
									   .withOnClick(spec.onPress)
									   .withColorStyle(Button::Color::Text)
									   .registerEntity(row));
			}

			std::vector<std::shared_ptr<ecs::Entity>> buttons(_actions.begin(),
															  _actions.end());
			row->setChildren(buttons);
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (_cover != nullptr) {
			children.push_back(_cover);
		}
		if (_titleSection != nullptr) {
			children.push_back(_titleSection);
		}
		if (_content != nullptr) {
			children.push_back(_content);
		}
		if (_actionsSection != nullptr) {
			children.push_back(_actionsSection);
		}
		setChildren(children);
	}

	void Card::initialize(void)
	{
		update();
		_initialized = true;
		buildComposition();
		applyVariant();
	}

	void Card::update(void)
	{
		applyGeometry();
		applyVariant();
	}

}	 // namespace guillaume::entities
