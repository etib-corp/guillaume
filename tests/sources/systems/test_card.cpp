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

#include "systems/test_card.hpp"

#include <guillaume/components/mouse_hover_interaction.hpp>
#include <guillaume/components/style_state.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	namespace
	{
		using Variant = components::Card::Variant;
		using Ids	  = std::vector<ecs::Entity::Identifier>;
	}	 // namespace

	TEST_F(TestCardSystem, RunsInTheLayoutPhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Layout);
	}

	TEST_F(TestCardSystem, ElevatedCardIsATonalLevelOneSurface)
	{
		_system.update(_entity);

		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);
		EXPECT_EQ(color(),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level1));
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level1);
		EXPECT_TRUE(elevation.hasShadow());
		EXPECT_FALSE(
			_registry.getComponent<components::Borders>(_entity).hasStroke());
	}

	TEST_F(TestCardSystem, FilledCardSitsOnSurfaceVariantWithoutShadow)
	{
		card().setVariant(Variant::Filled);
		_system.update(_entity);

		EXPECT_EQ(color(), getActiveColor(SchemeColorRole::SurfaceVariant));
		EXPECT_FALSE(
			_registry.getComponent<components::Elevation>(_entity).hasShadow());
		EXPECT_FALSE(
			_registry.getComponent<components::Borders>(_entity).hasStroke());
	}

	TEST_F(TestCardSystem, OutlinedCardHasAnOutlineVariantStroke)
	{
		card().setVariant(Variant::Outlined);
		_system.update(_entity);

		const auto &borders =
			_registry.getComponent<components::Borders>(_entity);
		EXPECT_EQ(color(), getActiveColor(SchemeColorRole::Surface));
		EXPECT_EQ(borders.getColor(),
				  getActiveColor(SchemeColorRole::OutlineVariant));
		EXPECT_FLOAT_EQ(borders.getWidth(), 1.0f);
		EXPECT_FALSE(
			_registry.getComponent<components::Elevation>(_entity).hasShadow());
	}

	TEST_F(TestCardSystem, APressHandlerMakesTheCardInteractive)
	{
		int presses = 0;
		EXPECT_FALSE(card().isInteractive());

		guillaume::systems::Card::setOnPress(_registry, _entity, [&presses]() {
			++presses;
		});

		EXPECT_TRUE(card().isInteractive());
		EXPECT_EQ(_registry.getComponent<components::Accessibility>(_entity)
					  .getRole(),
				  components::AccessibilityRole::Button);

		guillaume::systems::Card::press(_registry, _entity);
		EXPECT_EQ(presses, 1);
		mouse(true);
		mouse(false);
		EXPECT_EQ(presses, 2);

		// Replacing the handler keeps the wiring.
		guillaume::systems::Card::setOnPress(_registry, _entity, {});
		EXPECT_TRUE(card().isInteractive());
		mouse(true);
		mouse(false);
		EXPECT_EQ(presses, 2);
	}

	TEST_F(TestCardSystem, HoverAddsAStateLayerOnlyWhenInteractive)
	{
		_system.update(_entity);
		const auto rest = color();

		guillaume::systems::Card::setOnPress(_registry, _entity, {});
		EXPECT_EQ(color(), rest);

		_registry.getComponent<components::MouseHoverInteraction>(_entity)
			.setHovered(true);
		_system.update(_entity);
		EXPECT_NE(color(), rest);
	}

	TEST_F(TestCardSystem, PressingAnElevatedCardRaisesItToLevelTwo)
	{
		guillaume::systems::Card::setOnPress(_registry, _entity, {});
		auto &animation =
			_registry.getComponent<components::Animation>(_entity);
		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);
		EXPECT_FLOAT_EQ(animation.getDuration(),
						getMotionDuration(theme::MotionDuration::Short3));

		mouse(true);
		EXPECT_TRUE(card().isPressed());
		EXPECT_TRUE(animation.isPlaying());

		animation.advance(animation.getDuration() / 2.0f);
		EXPECT_GT(card().getPressProgress(), 0.0f);
		EXPECT_LT(card().getPressProgress(), 1.0f);

		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(card().getPressProgress(), 1.0f);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level2);
		EXPECT_FLOAT_EQ(
			elevation.getSpread(),
			getElevationToken(theme::ElevationLevel::Level2).shadowSpread);

		mouse(false);
		EXPECT_FALSE(card().isPressed());
		EXPECT_FLOAT_EQ(card().getPressFrom(), 1.0f);
		animation.advance(1.0f);
		EXPECT_FLOAT_EQ(card().getPressProgress(), 0.0f);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level1);
	}

	TEST_F(TestCardSystem, OtherVariantsDoNotRunThePressMotion)
	{
		card().setVariant(Variant::Filled);
		guillaume::systems::Card::setOnPress(_registry, _entity, {});

		mouse(true);

		EXPECT_FALSE(card().isPressed());
		EXPECT_FALSE(
			_registry.getComponent<components::Animation>(_entity).isPlaying());
	}

	TEST_F(TestCardSystem, DisabledCardIgnoresPressesAndDropsToRest)
	{
		int presses = 0;
		guillaume::systems::Card::setOnPress(_registry, _entity, [&presses]() {
			++presses;
		});
		mouse(true);
		_registry.getComponent<components::Animation>(_entity).advance(1.0f);
		EXPECT_FLOAT_EQ(card().getPressProgress(), 1.0f);

		guillaume::systems::Card::setDisabled(_registry, _entity, true);

		EXPECT_TRUE(guillaume::systems::Card::isDisabled(_registry, _entity));
		EXPECT_FALSE(_registry.getComponent<components::StyleState>(_entity)
						 .isEnabled());
		EXPECT_FALSE(card().isPressed());
		EXPECT_FLOAT_EQ(card().getPressProgress(), 0.0f);
		EXPECT_EQ(_registry.getComponent<components::Elevation>(_entity)
					  .getElevationLevel(),
				  theme::ElevationLevel::Level1);

		mouse(false);
		guillaume::systems::Card::press(_registry, _entity);
		EXPECT_EQ(presses, 0);

		guillaume::systems::Card::setDisabled(_registry, _entity, false);
		guillaume::systems::Card::press(_registry, _entity);
		EXPECT_EQ(presses, 1);
	}

	TEST_F(TestCardSystem, SectionsAreLaidOutCoverTitleContentActions)
	{
		compose();
		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			(Ids { _cover, _titleSection, _content, _actionsSection }));
		EXPECT_EQ(_registry.getComponent<components::Layout>(_titleSection)
					  .getChildren(),
				  (Ids { _titleText, _subtitleText }));
		EXPECT_EQ(_registry.getComponent<components::Layout>(_actionsSection)
					  .getChildren(),
				  (Ids { _button }));

		// 16 dp padding, then each section followed by an 8 dp gap.
		const float titleY	 = 16.0f + components::Card::CoverHeight + 8.0f;
		const float contentY = titleY + components::Card::TitleMinHeight + 8.0f;
		const float actionsY = contentY + 20.0f + 8.0f;
		EXPECT_FLOAT_EQ(positionOf(_cover).getY(), 16.0f);
		EXPECT_FLOAT_EQ(positionOf(_titleSection).getY(), titleY);
		EXPECT_FLOAT_EQ(positionOf(_content).getY(), contentY);
		EXPECT_FLOAT_EQ(positionOf(_actionsSection).getY(), actionsY);
		EXPECT_FLOAT_EQ(
			_registry.getComponent<components::Bound>(_entity).getHeight(),
			actionsY + 40.0f + 16.0f);

		// The content of a section follows the section the card just moved.
		EXPECT_FLOAT_EQ(positionOf(_titleText).getY(), titleY);
		EXPECT_FLOAT_EQ(positionOf(_subtitleText).getY(), titleY + 20.0f);
		EXPECT_FLOAT_EQ(positionOf(_button).getY(), actionsY);
	}

	TEST_F(TestCardSystem, MissingSectionsAreLeftOutOfTheLayout)
	{
		compose();
		card()
			.setCover(ecs::Entity::InvalidIdentifier)
			.setContentEntity(ecs::Entity::InvalidIdentifier);
		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			(Ids { _titleSection, _actionsSection }));
	}

	TEST_F(TestCardSystem, ACardWithoutSectionsKeepsItsOwnChildren)
	{
		addChild(_content, 80.0f, 20.0f);
		_registry.getComponent<components::Layout>(_entity).setChildren(
			{ _content });

		_system.update(_entity);

		EXPECT_EQ(
			_registry.getComponent<components::Layout>(_entity).getChildren(),
			(Ids { _content }));
		EXPECT_FLOAT_EQ(positionOf(_content).getY(), 16.0f);
	}

	TEST_F(TestCardSystem, ChildrenFollowTheConfiguredContent)
	{
		compose();
		_system.update(_entity);

		const auto textOf = [this](ecs::Entity::Identifier identifier) {
			return _registry.getComponent<components::Text>(identifier)
				.getContent();
		};
		EXPECT_EQ(textOf(_titleText), "Title");
		EXPECT_EQ(textOf(_subtitleText), "Subtitle");
		EXPECT_EQ(textOf(_content), "Body");
		EXPECT_EQ(
			_registry.getComponent<components::Image>(_cover).getTexturePath(),
			"images/cover.png");
		EXPECT_EQ(
			_registry.getComponent<components::Accessibility>(_titleSection)
				.getLabel(),
			"Title");

		card()
			.setTitle("Other")
			.setSubtitle("")
			.setContentText("Text")
			.setCoverPath("images/other.png");
		_system.update(_entity);
		EXPECT_EQ(textOf(_titleText), "Other");
		EXPECT_EQ(textOf(_subtitleText), "");
		EXPECT_EQ(textOf(_content), "Text");
		EXPECT_EQ(
			_registry.getComponent<components::Image>(_cover).getTexturePath(),
			"images/other.png");
		EXPECT_EQ(
			_registry.getComponent<components::Accessibility>(_titleSection)
				.getLabel(),
			"Other");
	}

	TEST_F(TestCardSystem, ContentWidthIsTheFixedWidthMinusThePadding)
	{
		EXPECT_FLOAT_EQ(
			guillaume::systems::Card::contentWidth(_registry, _entity),
			240.0f - 2.0f * 16.0f);

		_registry.getComponent<components::Layout>(_entity).clearFixedWidth();
		EXPECT_FLOAT_EQ(
			guillaume::systems::Card::contentWidth(_registry, _entity), 0.0f);
	}

	TEST_F(TestCardSystem, AnUnchangedCardIsNotMarkedChanged)
	{
		compose();
		guillaume::systems::Card::setOnPress(_registry, _entity, {});
		// Stop half-way through the press motion, where the shadow is
		// interpolated between the two elevation tokens.
		mouse(true);
		auto &animation =
			_registry.getComponent<components::Animation>(_entity);
		animation.advance(animation.getDuration() * 0.75f);
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		for (const auto identifier:
			 { _entity, _cover, _titleSection, _titleText, _subtitleText,
			   _content, _actionsSection, _button }) {
			EXPECT_FALSE(_registry.hasChanged(identifier));
		}
	}

}	 // namespace guillaume::systems::tests
