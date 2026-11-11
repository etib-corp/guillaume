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
	 * @brief Position of an entity in the entity tree.
	 *
	 * Holds the parent identifier, the identifiers of the direct children and
	 * the depth layer, so that systems can walk the tree from identifiers
	 * alone. ecs::Entity::setParent keeps it up to date for every entity
	 * built on a component registry; systems should treat it as read-only.
	 */
	class Hierarchy: public ecs::Component
	{
		private:
		ecs::Entity::Identifier _parentIdentifier {
			ecs::Entity::InvalidIdentifier
		};	  ///< Parent entity identifier
		std::vector<ecs::Entity::Identifier>
			_childIdentifiers;		  ///< Direct children, in attach order
		std::int32_t _layer { 0 };	  ///< Depth in the tree (root is 0)

		public:
		/**
		 * @brief Default constructor: a root entity without children.
		 */
		Hierarchy(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Hierarchy(void) = default;

		/**
		 * @brief Get the parent entity identifier.
		 * @return The parent identifier, or ecs::Entity::InvalidIdentifier
		 * for a root entity.
		 */
		ecs::Entity::Identifier getParentIdentifier(void) const noexcept;

		/**
		 * @brief Set the parent entity identifier.
		 * @param identifier The new parent identifier.
		 * @return Reference to this component for chaining.
		 */
		Hierarchy &setParentIdentifier(ecs::Entity::Identifier identifier);

		/**
		 * @brief Whether the entity has a parent.
		 * @return True when a parent identifier is set.
		 */
		bool hasParent(void) const noexcept;

		/**
		 * @brief Get the direct children, in the order they were attached.
		 * @return The child identifiers.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getChildIdentifiers(void) const noexcept;

		/**
		 * @brief Whether an entity is a direct child.
		 * @param identifier The identifier to look for.
		 * @return True when the identifier is a direct child.
		 */
		bool hasChild(ecs::Entity::Identifier identifier) const noexcept;

		/**
		 * @brief Append a direct child.
		 * @param identifier The child identifier.
		 * @return True when added, false when it was already a child.
		 */
		bool addChild(ecs::Entity::Identifier identifier);

		/**
		 * @brief Remove a direct child.
		 * @param identifier The child identifier.
		 * @return True when removed, false when it was not a child.
		 */
		bool removeChild(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the depth layer.
		 * @return The layer; 0 for a root entity.
		 */
		std::int32_t getLayer(void) const noexcept;

		/**
		 * @brief Set the depth layer.
		 * @param layer The new layer.
		 * @return Reference to this component for chaining.
		 */
		Hierarchy &setLayer(std::int32_t layer);
	};

}	 // namespace guillaume::components
