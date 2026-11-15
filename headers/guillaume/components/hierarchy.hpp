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

#include <cstdint>
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Place of an entity in the entity tree: its parent, its direct
	 * children and its depth.
	 *
	 * It lets a system walk the tree from identifiers alone, without entity
	 * objects. Every entity built on a component registry has one.
	 * ecs::Entity::setParent writes it: it updates the parent identifier of
	 * the entity, the child lists of the previous and of the new parent, and
	 * the layer of the entity and of all its descendants. Removing an entity
	 * unlinks it from its parent and resets the parent of its children.
	 * Systems read it: systems::Layout::layerOf returns the layer, which
	 * stacks content in front of its container, and systems::Selection takes
	 * the members of a selection group from the child list.
	 *
	 * @code
	 * const auto &hierarchy =
	 * 	registry.getComponent<components::Hierarchy>(identifier);
	 * for (const auto child: hierarchy.getChildIdentifiers()) { ... }
	 * @endcode
	 *
	 * Treat it as read-only outside ecs::Entity. Each setter changes this
	 * component only: setParentIdentifier() does not touch the child list of
	 * the parent, and addChild() does not set the parent or the layer of the
	 * child.
	 *
	 * @see ecs::Entity::setParent, systems::Layout::layerOf
	 */
	class Hierarchy: public ecs::Component
	{
		private:
		ecs::Entity::Identifier _parentIdentifier {
			ecs::Entity::InvalidIdentifier
		};	  ///< Parent entity identifier; InvalidIdentifier for a root

		std::vector<ecs::Entity::Identifier>
			_childIdentifiers;		  ///< Direct children, in attach order
		std::int32_t _layer { 0 };	  ///< Depth in the tree (root is 0)

		public:
		/**
		 * @brief Construct the hierarchy of a root entity without children.
		 */
		Hierarchy(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Hierarchy(void) = default;

		/**
		 * @brief Get the identifier of the parent entity.
		 * @return The parent identifier, or ecs::Entity::InvalidIdentifier
		 * for a root entity.
		 */
		ecs::Entity::Identifier getParentIdentifier(void) const noexcept;

		/**
		 * @brief Store the identifier of the parent entity.
		 *
		 * Called by ecs::Entity::setParent. Marks the component changed only
		 * when the identifier differs.
		 *
		 * @param identifier The parent identifier, or
		 * ecs::Entity::InvalidIdentifier to make the entity a root.
		 * @return This component, to chain calls.
		 */
		Hierarchy &setParentIdentifier(ecs::Entity::Identifier identifier);

		/**
		 * @brief Check whether the entity has a parent.
		 * @return False for a root entity.
		 */
		bool hasParent(void) const noexcept;

		/**
		 * @brief Get the direct children, in the order they were attached.
		 * @return The child identifiers; empty for a leaf entity.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getChildIdentifiers(void) const noexcept;

		/**
		 * @brief Check whether an entity is a direct child of this one.
		 * @param identifier The identifier to look for.
		 * @return True for a direct child; false for a deeper descendant.
		 */
		bool hasChild(ecs::Entity::Identifier identifier) const noexcept;

		/**
		 * @brief Append an entity to the direct children.
		 *
		 * Called by ecs::Entity::setParent. Marks the component changed when
		 * the child is added.
		 *
		 * @param identifier The child identifier.
		 * @return True when added, false when it was already a child.
		 */
		bool addChild(ecs::Entity::Identifier identifier);

		/**
		 * @brief Remove an entity from the direct children.
		 *
		 * The order of the remaining children is kept. Marks the component
		 * changed when the child is removed.
		 *
		 * @param identifier The child identifier.
		 * @return True when removed, false when it was not a child.
		 */
		bool removeChild(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the depth of the entity in the tree.
		 * @return The layer: 0 for a root entity, the layer of the parent
		 * plus 1 otherwise.
		 */
		std::int32_t getLayer(void) const noexcept;

		/**
		 * @brief Store the depth of the entity in the tree.
		 *
		 * Called by ecs::Entity::setParent for the entity and each of its
		 * descendants. Marks the component changed only when the layer
		 * differs.
		 *
		 * @param layer The depth: 0 for a root entity.
		 * @return This component, to chain calls.
		 */
		Hierarchy &setLayer(std::int32_t layer);
	};

}	 // namespace guillaume::components
