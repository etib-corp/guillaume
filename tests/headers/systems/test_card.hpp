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

#pragma once

#include <vector>

#include <gtest/gtest.h>

#include <guillaume/components/accessibility.hpp>
#include <guillaume/components/animation.hpp>
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/card.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/image.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/card.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::Card on bare entity identifiers,
	 * without any entity object.
	 */
	class TestCardSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;				///< Component storage.
		ecs::Entity::Identifier _entity { 1 };			///< Card under test.
		ecs::Entity::Identifier _cover { 2 };			///< Its cover image.
		ecs::Entity::Identifier _titleSection { 3 };	///< Its title section.
		ecs::Entity::Identifier _titleText { 4 };		///< Its title text.
		ecs::Entity::Identifier _subtitleText { 5 };	///< Its subtitle text.
		ecs::Entity::Identifier _content { 6 };			///< Its content text.
		ecs::Entity::Identifier _actionsSection { 7 };	  ///< Its actions row.
		ecs::Entity::Identifier _button { 8 };	  ///< Its action button.
		guillaume::systems::Card _system;		  ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity)
				.setAxis(components::Layout::Axis::Vertical)
				.setPadding(16.0f)
				.setSpacing(8.0f)
				.setFixedWidth(240.0f);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_registry.addComponent<components::Animation>(_entity);
			_registry.addComponent<components::Card>(_entity);

			guillaume::systems::Card::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Give the card its composed children: a cover, a title
		 * section with two texts, a content text and a row with one button.
		 */
		void compose(void)
		{
			addChild(_cover, 208.0f, components::Card::CoverHeight);
			_registry.addComponent<components::Image>(_cover);

			addChild(_titleSection, 0.0f, 0.0f);
			_registry.addComponent<components::Layout>(_titleSection)
				.setAxis(components::Layout::Axis::Vertical)
				.setFixedWidth(208.0f)
				.setFixedHeight(components::Card::TitleMinHeight);
			_registry.addComponent<components::Accessibility>(_titleSection);

			for (const auto text: { _titleText, _subtitleText, _content }) {
				addChild(text, 80.0f, 20.0f);
				_registry.addComponent<components::Text>(text);
			}

			addChild(_actionsSection, 0.0f, 0.0f);
			_registry.addComponent<components::Layout>(_actionsSection)
				.setAxis(components::Layout::Axis::Horizontal)
				.setFixedWidth(208.0f);
			addChild(_button, 60.0f, 40.0f);

			card()
				.setCoverPath("images/cover.png")
				.setTitle("Title")
				.setSubtitle("Subtitle")
				.setContentText("Body")
				.addAction({ "Ok", {} })
				.setCover(_cover)
				.setTitleSection(_titleSection)
				.setTitleText(_titleText)
				.setSubtitleText(_subtitleText)
				.setContentEntity(_content)
				.setActionsSection(_actionsSection)
				.setActionButtons({ _button });
		}

		/**
		 * @brief Add the Transform and Bound of a child.
		 * @param identifier The child.
		 * @param width Its measured width.
		 * @param height Its measured height.
		 */
		void addChild(ecs::Entity::Identifier identifier, float width,
					  float height)
		{
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier)
				.setWidth(width)
				.setHeight(height);
		}

		/**
		 * @brief Get the configuration of the card under test.
		 * @return The card component.
		 */
		components::Card &card(void)
		{
			return _registry.getComponent<components::Card>(_entity);
		}

		/**
		 * @brief Get the color written on the card.
		 * @return Its container color.
		 */
		utility::graphic::Color32Bit color(void)
		{
			return _registry.getComponent<components::Color>(_entity)
				.getColor();
		}

		/**
		 * @brief Get the position written on an entity.
		 * @param identifier The entity.
		 * @return The position of its transform.
		 */
		utility::graphic::PositionF
			positionOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition();
		}

		/**
		 * @brief Hold or release the left mouse button on the card, as the
		 * MouseButton system would.
		 * @param pressed True to press, false to release.
		 */
		void mouse(bool pressed)
		{
			const auto left = utility::event::MouseButtonEvent::Button::Left;
			auto &interaction =
				_registry.getComponent<components::MouseButtonInteraction>(
					_entity);

			interaction.setButtonPressed(left, pressed);
			if (pressed) {
				interaction.getOnButtonPressHandler(left)();
			} else {
				interaction.getOnButtonReleaseHandler(left)();
			}
		}
	};

}	 // namespace guillaume::systems::tests
