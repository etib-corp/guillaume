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
#include "guillaume/components/navigation_bar_destination.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/entities/badge.hpp"
#include "guillaume/entities/navigation_bar.hpp"

namespace guillaume::entities::detail
{
	/**
	 * @brief Internal navigation destination: a pressable vertical layout
	 * holding the active-indicator pill (wrapping the icon) and a label.
	 *
	 * The class is a facade without fields: its state is
	 * components::NavigationBarDestination and
	 * systems::NavigationBarDestination styles and arranges it.
	 * `initialize()` only builds the child entities.
	 */
	class BarDestination:
		public std::enable_shared_from_this<BarDestination>,
		public ecs::ParentEntityFiller<
			components::Layout, components::Transform, components::Bound,
			components::Color, components::Borders, components::Selectable,
			components::NavigationBarDestination>
	{
		public:
		/**
		 * @brief Construct a navigation destination.
		 * @param registry The component registry.
		 * @param spec The destination data.
		 */
		BarDestination(ecs::ComponentRegistry &registry,
					   const NavigationBar::Destination &spec);

		/**
		 * @brief Default destructor.
		 */
		~BarDestination(void) override = default;

		/**
		 * @brief Build the indicator, icon, label and badge children.
		 */
		void initialize(void) override;

		/**
		 * @brief Set the press handler.
		 * @param onPress Press handler.
		 */
		void setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Set or update the badge label.
		 * @param badge Badge label ("" keeps no badge, or shows a dot on
		 * an existing badge).
		 */
		void setBadge(const std::string &badge);

		/**
		 * @brief Set whether the label is shown.
		 * @param show Whether to show the label.
		 */
		void setShowLabel(bool show);

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
		 * @brief Get the indicator identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getIndicatorIdentifier(void) const;

		/**
		 * @brief Get the icon identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Get the label identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getLabelIdentifier(void) const;

		/**
		 * @brief Get the badge identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getBadgeIdentifier(void) const;

		private:
		/**
		 * @brief Build the badge child once the destination has a badge
		 * label and an indicator to carry it.
		 */
		void buildBadge(void);
	};
}	 // namespace guillaume::entities::detail
