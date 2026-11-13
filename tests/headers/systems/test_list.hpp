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
#include <guillaume/components/borders.hpp>
#include <guillaume/components/bound.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/glyph.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/list.hpp>
#include <guillaume/components/mouse_button_interaction.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/list.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::List on bare entity identifiers,
	 * without any entity object.
	 *
	 * The list has three entries: "Inbox" with a description, a leading
	 * icon and a trailing icon; the accordion "Folders" holding "Work" and
	 * a disabled "Home"; and the subheader "More".
	 */
	class TestListSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;				 ///< Component storage.
		ecs::Entity::Identifier _entity { 1 };			 ///< List under test.
		ecs::Entity::Identifier _inbox { 10 };			 ///< First row.
		ecs::Entity::Identifier _inboxLead { 11 };		 ///< Its leading group.
		ecs::Entity::Identifier _inboxContent { 12 };	 ///< Its text column.
		ecs::Entity::Identifier _inboxTitle { 13 };		 ///< Its title.
		ecs::Entity::Identifier _inboxDetails { 14 };	 ///< Its description.
		ecs::Entity::Identifier _inboxIcon { 15 };		 ///< Its leading icon.
		ecs::Entity::Identifier _inboxAction { 16 };	 ///< Its trailing icon.
		ecs::Entity::Identifier _folders { 20 };		 ///< Accordion row.
		ecs::Entity::Identifier _foldersLead { 21 };	 ///< Its leading group.
		ecs::Entity::Identifier _foldersContent { 22 };	   ///< Its text column.
		ecs::Entity::Identifier _foldersTitle { 23 };	   ///< Its title.
		ecs::Entity::Identifier _chevron { 24 };		   ///< Its chevron.
		ecs::Entity::Identifier _work { 30 };			   ///< First child row.
		ecs::Entity::Identifier _workLead { 31 };		///< Its leading group.
		ecs::Entity::Identifier _workSpacer { 32 };		///< Its nesting inset.
		ecs::Entity::Identifier _workContent { 33 };	///< Its text column.
		ecs::Entity::Identifier _workTitle { 34 };		///< Its title.
		ecs::Entity::Identifier _home { 40 };	 ///< Second (disabled) child.
		ecs::Entity::Identifier _homeLead { 41 };		///< Its leading group.
		ecs::Entity::Identifier _homeSpacer { 42 };		///< Its nesting inset.
		ecs::Entity::Identifier _homeContent { 43 };	///< Its text column.
		ecs::Entity::Identifier _homeTitle { 44 };		///< Its title.
		ecs::Entity::Identifier _more { 50 };			///< Subheader row.
		guillaume::systems::List _system;				///< System under test.
		int _inboxPresses { 0 };	  ///< "Inbox" handler calls.
		int _foldersPresses { 0 };	  ///< "Folders" handler calls.
		int _workPresses { 0 };		  ///< "Work" handler calls.
		int _homePresses { 0 };		  ///< "Home" handler calls.

		void SetUp(void) override
		{
			addSurface(_entity);
			_registry.addComponent<components::Hierarchy>(_entity);
			_registry.addComponent<components::List>(_entity)
				.setItems(makeItems())
				.setRows(makeRows())
				.setRowsDirty(false);

			for (const auto surface:
				 { _inbox, _inboxLead, _inboxContent, _folders, _foldersLead,
				   _foldersContent, _work, _workLead, _workSpacer, _workContent,
				   _home, _homeLead, _homeSpacer, _homeContent }) {
				addSurface(surface);
			}
			for (const auto title: { _inboxTitle, _foldersTitle, _workTitle,
									 _homeTitle, _more }) {
				addContent(title, 100.0f, 24.0f);
			}
			addContent(_inboxDetails, 80.0f, 20.0f);
			for (const auto icon: { _inboxIcon, _inboxAction, _chevron }) {
				addContent(icon, 24.0f, 24.0f);
				_registry.addComponent<components::Glyph>(icon);
			}

			guillaume::systems::List::install(_registry, _entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}

		/**
		 * @brief Make the entries of the list under test.
		 * @return The entries.
		 */
		std::vector<components::List::Item> makeItems(void)
		{
			components::List::Item inbox;
			inbox.headline	   = "Inbox";
			inbox.supporting   = "3 new";
			inbox.leadingIcon  = "inbox";
			inbox.trailingIcon = "more_vert";
			inbox.onPress	   = [this]() {
				 ++_inboxPresses;
			};

			components::List::Item work;
			work.headline = "Work";
			work.onPress  = [this]() {
				 ++_workPresses;
			};
			components::List::Item home;
			home.headline = "Home";
			home.disabled = true;
			home.onPress  = [this]() {
				 ++_homePresses;
			};

			components::List::Item folders;
			folders.headline = "Folders";
			folders.children = { work, home };
			folders.onPress	 = [this]() {
				 ++_foldersPresses;
			};

			components::List::Item more;
			more.kind	  = components::List::ItemKind::Subheader;
			more.headline = "More";

			return { inbox, folders, more };
		}

		/**
		 * @brief Make the rows naming the entities of the fixture.
		 * @return The rows.
		 */
		std::vector<components::List::Row> makeRows(void)
		{
			components::List::Row inbox;
			inbox.entity	   = _inbox;
			inbox.view		   = _inbox;
			inbox.lead		   = _inboxLead;
			inbox.leadingIcon  = _inboxIcon;
			inbox.content	   = _inboxContent;
			inbox.headline	   = _inboxTitle;
			inbox.supporting   = _inboxDetails;
			inbox.trailingIcon = _inboxAction;

			components::List::Row work;
			work.entity	  = _work;
			work.view	  = _work;
			work.lead	  = _workLead;
			work.spacer	  = _workSpacer;
			work.content  = _workContent;
			work.headline = _workTitle;

			components::List::Row home;
			home.entity	  = _home;
			home.view	  = _home;
			home.lead	  = _homeLead;
			home.spacer	  = _homeSpacer;
			home.content  = _homeContent;
			home.headline = _homeTitle;

			components::List::Row folders;
			folders.entity		 = _folders;
			folders.view		 = _folders;
			folders.lead		 = _foldersLead;
			folders.content		 = _foldersContent;
			folders.headline	 = _foldersTitle;
			folders.trailingIcon = _chevron;
			folders.children	 = { work, home };

			components::List::Row more;
			more.entity = _more;

			return { inbox, folders, more };
		}

		/**
		 * @brief Give an entity the components of a layout surface.
		 * @param identifier The entity.
		 */
		void addSurface(ecs::Entity::Identifier identifier)
		{
			_registry.addComponent<components::Layout>(identifier);
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier);
			_registry.addComponent<components::Color>(identifier);
			_registry.addComponent<components::Borders>(identifier);
		}

		/**
		 * @brief Give an entity the components of measured content.
		 * @param identifier The entity.
		 * @param width Its measured width.
		 * @param height Its measured height.
		 */
		void addContent(ecs::Entity::Identifier identifier, float width,
						float height)
		{
			_registry.addComponent<components::Transform>(identifier);
			_registry.addComponent<components::Bound>(identifier)
				.setWidth(width)
				.setHeight(height);
			_registry.addComponent<components::Color>(identifier);
		}

		/**
		 * @brief Get the configuration of the list under test.
		 * @return The list component.
		 */
		components::List &list(void)
		{
			return _registry.getComponent<components::List>(_entity);
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
		 * @brief Get the layout children of an entity.
		 * @param identifier The entity.
		 * @return Its layout children, in order.
		 */
		std::vector<ecs::Entity::Identifier>
			laidOut(ecs::Entity::Identifier identifier)
		{
			return _registry.getComponent<components::Layout>(identifier)
				.getChildren();
		}

		/**
		 * @brief Release the left mouse button on an entity.
		 * @param identifier The entity.
		 */
		void click(ecs::Entity::Identifier identifier)
		{
			_registry
				.getComponent<components::MouseButtonInteraction>(identifier)
				.getOnButtonReleaseHandler(
					utility::event::MouseButtonEvent::Button::Left)();
		}
	};

}	 // namespace guillaume::systems::tests
