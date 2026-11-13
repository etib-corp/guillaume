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

#include <functional>
#include <memory>
#include <string>

#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/navigation_drawer_destination.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/entities/navigation_drawer.hpp"

namespace guillaume::entities::detail
{
	/**
	 * @brief Internal drawer destination (Paper `Drawer.Item` or
	 * `Drawer.CollapsedItem`).
	 *
	 * The class is a facade without fields: its state is
	 * components::NavigationDrawerDestination and
	 * systems::NavigationDrawerDestination styles and arranges it.
	 * `initialize()` only builds the child entities.
	 */
	class DrawerDestination:
		public std::enable_shared_from_this<DrawerDestination>,
		public ecs::ParentEntityFiller<
			components::Layout, components::Transform, components::Bound,
			components::Color, components::Borders, components::Selectable,
			components::NavigationDrawerDestination>
	{
		public:
		/**
		 * @brief Construct a drawer destination.
		 * @param registry The component registry.
		 * @param entry The destination data.
		 * @param collapsed Whether to render a collapsed item.
		 * @param width Destination width.
		 */
		DrawerDestination(ecs::ComponentRegistry &registry,
						  const NavigationDrawer::Entry &entry, bool collapsed,
						  float width);

		/**
		 * @brief Default destructor.
		 */
		~DrawerDestination(void) override = default;

		/**
		 * @brief Build the icon, label and trailing children.
		 */
		void initialize(void) override;

		/**
		 * @brief Set the press handler.
		 * @param onPress Press handler.
		 */
		void setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Enable or disable the destination.
		 * @param disabled Whether the destination is disabled.
		 */
		void setDisabled(bool disabled);

		/**
		 * @brief Check whether the destination is disabled.
		 * @return True when disabled.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Set the selection change handler.
		 * @param handler The handler to invoke on selection change.
		 */
		void setOnSelectionChanged(
			const components::Selectable::Handler &handler);

		/**
		 * @brief Whether the destination is selected.
		 * @return True when selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Get the trailing text identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getRightIdentifier(void) const;
	};
}	 // namespace guillaume::entities::detail
