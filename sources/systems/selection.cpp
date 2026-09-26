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

#include "guillaume/systems/selection.hpp"

#include "guillaume/ecs/entity.hpp"

namespace guillaume::systems
{
	Selection::Selection(void)
		: ecs::SystemFiller<components::SelectionGroup, components::Transform,
							components::Bound>(ecs::Phase::Event)
	{
	}

	void Selection::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::SelectionGroup>(entityIdentifier)) {
			return;
		}

		std::vector<ecs::Entity::Identifier> childIdentifiers;

		for (const auto &entity:
			 getEntityRegistry().getEntitiesBreadthFirst()) {
			const auto parent = entity->getParent();
			if (parent != nullptr
				&& parent->getIdentifier() == entityIdentifier) {
				childIdentifiers.push_back(entity->getIdentifier());
			}
		}

		reconcile(getComponentRegistry(), entityIdentifier, childIdentifiers);
	}

	ecs::Entity::Identifier Selection::reconcile(
		ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &groupIdentifier,
		const std::vector<ecs::Entity::Identifier> &childIdentifiers)
	{
		auto &group =
			registry.getComponent<components::SelectionGroup>(groupIdentifier);

		ecs::Entity::Identifier selected = ecs::Entity::InvalidIdentifier;

		for (const auto &childIdentifier: childIdentifiers) {
			if (!registry.hasComponent<components::Selectable>(
					childIdentifier)) {
				continue;
			}

			auto &selectable =
				registry.getComponent<components::Selectable>(childIdentifier);

			if (!selectable.isSelected()) {
				continue;
			}

			if (selected == ecs::Entity::InvalidIdentifier) {
				selected = childIdentifier;
			} else {
				selectable.setSelected(false);
			}
		}

		if (selected != ecs::Entity::InvalidIdentifier) {
			group.setSelected(selected);
			return group.getSelected();
		}

		// No child is selected. Restore the previous selection when it is still
		// part of the group, otherwise select the first selectable child when
		// the group does not allow an empty selection.
		const auto previous = group.getSelected();
		if (previous != ecs::Entity::InvalidIdentifier
			&& registry.hasComponent<components::Selectable>(previous)) {
			registry.getComponent<components::Selectable>(previous).setSelected(
				true);
			return previous;
		}

		if (!group.isEmptyAllowed()) {
			for (const auto &childIdentifier: childIdentifiers) {
				if (registry.hasComponent<components::Selectable>(
						childIdentifier)) {
					registry
						.getComponent<components::Selectable>(childIdentifier)
						.setSelected(true);
					group.setSelected(childIdentifier);
					return childIdentifier;
				}
			}
		}

		group.clearSelection();
		return ecs::Entity::InvalidIdentifier;
	}

}	 // namespace guillaume::systems
