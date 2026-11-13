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

#include <utility>
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
	 * @brief One destination of a NavigationBar: a pressable column with
	 * an active-indicator pill around the icon, a label under it and an
	 * optional badge on the icon.
	 *
	 * Internal to NavigationBar, which creates one per destination with
	 * `BarDestination::create(registry, spec)` and adds it to itself;
	 * applications use the NavigationBar methods instead. Its state lives
	 * in components::NavigationBarDestination (icon, focused icon, label,
	 * badge, child identifiers, press handler) and its selection in
	 * components::Selectable; systems::NavigationBarDestination styles it
	 * for its selection and places its children each frame, in the Layout
	 * phase. The class itself holds no field.
	 *
	 * The destination is the entity registry that owns the pill and the
	 * label; the pill owns the icon and the badge. systems::NavigationBar
	 * overwrites the size of the destination, whether it shows its label,
	 * its press handler and its selection-change handler.
	 *
	 * @see NavigationBar, components::NavigationBarDestination,
	 * systems::NavigationBarDestination
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
		 * @brief Create a destination and its children, without registering
		 * it in an entity registry or setting its parent.
		 *
		 * The constructor is private because the children need the
		 * destination to be owned by a shared pointer before they can take
		 * it as parent; create() constructs it, then builds the pill, the
		 * icon, the label and the badge.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to the private constructor:
		 * (registry, spec).
		 * @return The new destination.
		 */
		template<typename... Arguments>
		static std::shared_ptr<BarDestination> create(Arguments &&...arguments)
		{
			std::shared_ptr<BarDestination> entity(
				new BarDestination(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~BarDestination(void) override = default;

		/**
		 * @brief Set the handler called when the user presses the
		 * destination.
		 *
		 * systems::NavigationBar::install sets this handler on every
		 * destination of a bar; setting another one replaces it.
		 * @param onPress The handler; an empty function for none.
		 */
		void setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Change the badge label.
		 *
		 * Builds the badge child the first time the label is not empty,
		 * then restyles at once.
		 * @param badge The badge label. An empty label builds no badge, and
		 * turns an existing badge into a dot.
		 */
		void setBadge(const std::string &badge);

		/**
		 * @brief Show or hide the label. Restyles at once.
		 *
		 * A hidden label keeps its place in the column.
		 * @param show True to show the label.
		 */
		void setShowLabel(bool show);

		/**
		 * @brief Set the handler of the destination's
		 * components::Selectable.
		 *
		 * systems::NavigationBar::install sets this handler on every
		 * destination of a bar; setting another one replaces it.
		 * @param handler Called with the new state when the destination
		 * becomes selected or deselected.
		 */
		void setOnSelectionChanged(
			const components::Selectable::Handler &handler);

		/**
		 * @brief Whether the destination is the selected one of its bar.
		 * @return True when its components::Selectable is selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Get the active-indicator pill: the rounded shape behind
		 * the icon.
		 * @return The identifier of the pill, or
		 * ecs::Entity::InvalidIdentifier before the children are built.
		 */
		ecs::Entity::Identifier getIndicatorIdentifier(void) const;

		/**
		 * @brief Get the icon entity.
		 * @return The identifier of the icon, or
		 * ecs::Entity::InvalidIdentifier when the destination has no icon
		 * glyph.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Get the label entity.
		 * @return The identifier of the label text, or
		 * ecs::Entity::InvalidIdentifier when the destination has no label.
		 */
		ecs::Entity::Identifier getLabelIdentifier(void) const;

		/**
		 * @brief Get the badge entity.
		 * @return The identifier of the badge, or
		 * ecs::Entity::InvalidIdentifier when the destination never had a
		 * badge label.
		 */
		ecs::Entity::Identifier getBadgeIdentifier(void) const;

		private:
		/**
		 * @brief Build the badge child once the destination has a badge
		 * label and an indicator to carry it.
		 *
		 * Does nothing when the badge child exists already or when the
		 * destination is not owned by a shared pointer yet.
		 */
		void buildBadge(void);

		private:
		/**
		 * @brief Construct a navigation destination.
		 *
		 * Writes `spec` to components::NavigationBarDestination and makes
		 * the entity pressable. The children are built by build().
		 * @param registry The registry that stores the components.
		 * @param spec The icon, focused icon, label and badge of the
		 * destination.
		 */
		BarDestination(ecs::ComponentRegistry &registry,
					   const NavigationBar::Destination &spec);

		/**
		 * @brief Build the pill, the icon, the label and the badge, then
		 * style the destination.
		 *
		 * Called once by create(), when the destination is owned by a
		 * shared pointer and can be given as parent to its children.
		 */
		void build(void);
	};
}	 // namespace guillaume::entities::detail
