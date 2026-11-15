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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{

	/**
	 * @brief Keeps at most one child selected in each selection group.
	 *
	 * Runs in the Event phase on entities that have a
	 * components::SelectionGroup, a Transform and a Bound. The members of a
	 * group are its direct children in the tree, read from
	 * components::Hierarchy (a group without one is skipped); only the
	 * children with a components::Selectable take part. The system writes
	 * the `selected` flag of those children and the selected identifier of
	 * the group.
	 *
	 * The rules are in the static functions, so a widget system can apply
	 * them at once instead of waiting for the frame:
	 * @code
	 * // Select `second` among the children of `group`.
	 * systems::Selection::select(registry, group, children, second);
	 * @endcode
	 *
	 * @see components::Selectable
	 * @see components::SelectionGroup
	 * @see systems::RadioGroup
	 */
	class Selection:
		public ecs::SystemFiller<components::SelectionGroup,
								 components::Transform, components::Bound>
	{
		public:
		/**
		 * @brief Create the system, in the Event phase.
		 */
		Selection(void);

		/**
		 * @brief Default destructor.
		 */
		~Selection(void) override = default;

		/**
		 * @brief Reconcile one group with the children listed in its
		 * components::Hierarchy.
		 * @param entityIdentifier The entity holding the
		 * components::SelectionGroup.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Make a group and its children agree on which child is
		 * selected.
		 *
		 * - When one or more children are selected, the first one in
		 *   `childIdentifiers` wins: the others are deselected and the group
		 *   records the winner.
		 * - When none is selected, the child the group last recorded is
		 *   selected again if it still has a components::Selectable (it is
		 *   not checked against `childIdentifiers`).
		 * - Otherwise, a group that does not allow an empty selection
		 *   selects its first selectable child; a group that allows it has
		 *   its selection cleared.
		 *
		 * Because of the second rule, deselecting the selected child does
		 * not empty the group: the child is selected again on the next
		 * call. To empty a group, call
		 * components::SelectionGroup::clearSelection as well.
		 *
		 * @param registry The registry holding the components.
		 * @param groupIdentifier The entity holding the
		 * components::SelectionGroup.
		 * @param childIdentifiers The children of the group, in order.
		 * Those without a components::Selectable are ignored.
		 * @return The selected child, or `ecs::Entity::InvalidIdentifier`
		 * when the group ends up with no selection.
		 */
		static ecs::Entity::Identifier reconcile(
			ecs::ComponentRegistry &registry,
			const ecs::Entity::Identifier &groupIdentifier,
			const std::vector<ecs::Entity::Identifier> &childIdentifiers);

		/**
		 * @brief Select one child of a group, at once.
		 *
		 * Selects `selectedIdentifier`, deselects every other selectable
		 * child, then calls reconcile() so the group records the result.
		 * systems::NavigationBar and systems::NavigationDrawer use it to
		 * change the active destination.
		 *
		 * @param registry The registry holding the components.
		 * @param groupIdentifier The entity holding the
		 * components::SelectionGroup.
		 * @param childIdentifiers The children of the group, in order.
		 * @param selectedIdentifier The child to select. With
		 * `ecs::Entity::InvalidIdentifier`, or an identifier that is not a
		 * selectable child, every child is deselected and reconcile() then
		 * applies its "none selected" rules, which select the previously
		 * recorded child again when it still exists.
		 * @return The child selected after reconciliation, or
		 * `ecs::Entity::InvalidIdentifier` when there is none.
		 */
		static ecs::Entity::Identifier
			select(ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &groupIdentifier,
				   const std::vector<ecs::Entity::Identifier> &childIdentifiers,
				   ecs::Entity::Identifier selectedIdentifier);
	};

}	 // namespace guillaume::systems
