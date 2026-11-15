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

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/focus.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/line.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/text_field.hpp>
#include <guillaume/components/text_field_configuration.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/components/typography.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/text_field.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::TextField on bare entity identifiers,
	 * without any entity object.
	 */
	class TestTextFieldSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			   ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		   ///< Widget under test.
		ecs::Entity::Identifier _leading { 2 };		   ///< Its leading icon.
		ecs::Entity::Identifier _trailing { 3 };	   ///< Its trailing icon.
		ecs::Entity::Identifier _errorIcon { 4 };	   ///< Its error icon.
		ecs::Entity::Identifier _column { 5 };		   ///< Its input column.
		ecs::Entity::Identifier _label { 6 };		   ///< Its label.
		ecs::Entity::Identifier _input { 7 };		   ///< Its input text.
		ecs::Entity::Identifier _prefix { 8 };		   ///< Its prefix.
		ecs::Entity::Identifier _suffix { 9 };		   ///< Its suffix.
		ecs::Entity::Identifier _supporting { 10 };	   ///< Its supporting text.
		guillaume::systems::TextField _system;		   ///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::Layout>(_entity);
			_registry.addComponent<components::Transform>(_entity);
			_registry.addComponent<components::Bound>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Borders>(_entity);
			_registry.addComponent<components::Line>(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::TextField>(_entity)
				.setPlaceholder("Name")
				.setSupportingText("Support")
				.setPrefix("$")
				.setSuffix("kg");
			_registry.addComponent<components::TextFieldConfiguration>(_entity)
				.setLeadingIconEntity(_leading)
				.setTrailingIconEntity(_trailing)
				.setErrorIconEntity(_errorIcon)
				.setInputColumn(_column)
				.setLabelEntity(_label)
				.setInputEntity(_input)
				.setPrefixEntity(_prefix)
				.setSuffixEntity(_suffix)
				.setSupportingEntity(_supporting);

			_registry.addComponent<components::Layout>(_column)
				.setAxis(components::Layout::Axis::Vertical)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Start);
			_registry.addComponent<components::Transform>(_column);
			_registry.addComponent<components::Bound>(_column);

			for (const auto icon: { _leading, _trailing, _errorIcon }) {
				_registry.addComponent<components::Transform>(icon);
				_registry.addComponent<components::Bound>(icon)
					.setWidth(24.0f)
					.setHeight(24.0f);
				_registry.addComponent<components::Color>(icon);
			}
			for (const auto text:
				 { _label, _input, _prefix, _suffix, _supporting }) {
				_registry.addComponent<components::Transform>(text);
				_registry.addComponent<components::Bound>(text)
					.setWidth(40.0f)
					.setHeight(20.0f);
				_registry.addComponent<components::Color>(text);
				_registry.addComponent<components::Text>(text);
			}
			_registry.addComponent<components::Typography>(_label);

			guillaume::systems::TextField::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Get the configuration of the field under test.
		 * @return The configuration component.
		 */
		components::TextFieldConfiguration &configuration(void)
		{
			return _registry.getComponent<components::TextFieldConfiguration>(
				_entity);
		}

		/**
		 * @brief Get the label and error state of the field under test.
		 * @return The text field component.
		 */
		components::TextField &field(void)
		{
			return _registry.getComponent<components::TextField>(_entity);
		}

		/**
		 * @brief Get the color written on an entity.
		 * @param identifier The entity.
		 * @return Its color.
		 */
		utility::graphic::Color32Bit colorOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Color>(identifier)
				.getColor();
		}

		/**
		 * @brief Get the text written on an entity.
		 * @param identifier The entity.
		 * @return Its content.
		 */
		std::string contentOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Text>(identifier)
				.getContent();
		}

		/**
		 * @brief Get the children the Layout system arranges.
		 * @param identifier The container.
		 * @return Its layout children, in order.
		 */
		std::vector<ecs::Entity::Identifier>
			laidOut(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Layout>(identifier)
				.getChildren();
		}

		/**
		 * @brief Get the vertical position of an entity.
		 * @param identifier The entity.
		 * @return The Y coordinate of its pose.
		 */
		float yOf(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Transform>(identifier)
				.getPose()
				.getPosition()
				.getY();
		}

		/**
		 * @brief Give or take the input focus, as systems::Focus would.
		 * @param focused Whether the field is focused.
		 */
		void focus(bool focused)
		{
			_registry.getComponent<components::Focus>(_entity).setFocused(
				focused);
		}

		/**
		 * @brief Type into the input child, as systems::TextInput would.
		 * @param text The new content.
		 */
		void type(const std::string &text)
		{
			_registry.getComponent<components::Text>(_input).setContent(text);
		}
	};

}	 // namespace guillaume::systems::tests
