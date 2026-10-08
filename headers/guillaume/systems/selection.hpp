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
	 * @brief System enforcing mutually exclusive selection within a group.
	 *
	 * Walks the children of every `components::SelectionGroup` and makes sure
	 * at most one `components::Selectable` child is selected, keeping the group
	 * selection identifier in sync.
	 *
	 * @see components::Selectable
	 * @see components::SelectionGroup
	 */
	class Selection:
		public ecs::SystemFiller<components::SelectionGroup,
								 components::Transform, components::Bound>
	{
		public:
		/**
		 * @brief Construct a selection system running in the Event phase.
		 */
		Selection(void);

		/**
		 * @brief Default destructor.
		 */
		~Selection(void) override = default;

		/**
		 * @brief Update the selection of one group entity.
		 * @param entityIdentifier The group entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Reconcile the selection of a group with its children.
		 *
		 * Keeps the first selected child, deselects the other selected
		 * children, and updates the group identifier to match. When no child is
		 * selected, either the previous selection is restored (when the child
		 * is still part of the group) or, if empty selection is not allowed,
		 * the first selectable child is selected.
		 *
		 * @param registry The component registry holding the entities.
		 * @param groupIdentifier The group entity identifier.
		 * @param childIdentifiers The group children, in order.
		 * @return The selected child identifier, or InvalidIdentifier.
		 */
		static ecs::Entity::Identifier reconcile(
			ecs::ComponentRegistry &registry,
			const ecs::Entity::Identifier &groupIdentifier,
			const std::vector<ecs::Entity::Identifier> &childIdentifiers);

		/**
		 * @brief Select one child of a group and reconcile the group.
		 *
		 * Marks `selectedIdentifier` as selected, deselects the other
		 * selectable children, then reconciles the group so its selection
		 * identifier and the children stay consistent.
		 *
		 * @param registry The component registry holding the entities.
		 * @param groupIdentifier The group entity identifier.
		 * @param childIdentifiers The group children, in order.
		 * @param selectedIdentifier The child to select, or InvalidIdentifier
		 * to clear the selection.
		 * @return The resulting selected child identifier.
		 */
		static ecs::Entity::Identifier
			select(ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &groupIdentifier,
				   const std::vector<ecs::Entity::Identifier> &childIdentifiers,
				   ecs::Entity::Identifier selectedIdentifier);
	};

}	 // namespace guillaume::systems
