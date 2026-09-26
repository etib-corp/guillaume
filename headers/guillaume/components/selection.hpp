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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component marking an entity as selectable.
	 *
	 * Used together with `components::SelectionGroup` on the parent to provide
	 * mutually exclusive selection (RadioButton groups, Tabs, navigation
	 * destinations).
	 *
	 * @see components::SelectionGroup
	 * @see systems::Selection
	 */
	class Selectable: public ecs::Component
	{
		public:
		using Handler = std::function<void(bool)>;	  ///< Selection handler.

		private:
		bool _selected { false };		   ///< Whether the entity is selected.
		Handler _onSelectionChanged {};	   ///< Selection change handler.

		public:
		/**
		 * @brief Default constructor for the Selectable component.
		 */
		Selectable(void) = default;

		/**
		 * @brief Default destructor for the Selectable component.
		 */
		~Selectable(void) = default;

		/**
		 * @brief Check whether the entity is selected.
		 * @return True when selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Set the selected state and fire the handler on change.
		 * @param selected The new selected state.
		 * @return Reference to this Selectable for chaining.
		 */
		Selectable &setSelected(bool selected);

		/**
		 * @brief Set the selection change handler.
		 * @param handler The handler to call when the selection changes.
		 * @return Reference to this Selectable for chaining.
		 */
		Selectable &setOnSelectionChangedHandler(const Handler &handler);

		/**
		 * @brief Get the selection change handler.
		 * @return The selection change handler.
		 */
		Handler getOnSelectionChangedHandler(void) const;
	};

	/**
	 * @brief Component holding the single selected child of a group.
	 *
	 * @see components::Selectable
	 * @see systems::Selection
	 */
	class SelectionGroup: public ecs::Component
	{
		private:
		ecs::Entity::Identifier _selected {
			ecs::Entity::InvalidIdentifier
		};	  ///< Currently selected child.
		bool _allowEmpty {
			true
		};	  ///< Whether the group may have no selection.

		public:
		/**
		 * @brief Default constructor for the SelectionGroup component.
		 */
		SelectionGroup(void) = default;

		/**
		 * @brief Default destructor for the SelectionGroup component.
		 */
		~SelectionGroup(void) = default;

		/**
		 * @brief Check whether a child is selected.
		 * @return True when a child is selected.
		 */
		bool hasSelection(void) const;

		/**
		 * @brief Get the selected child identifier.
		 * @return The selected child, or InvalidIdentifier when none.
		 */
		ecs::Entity::Identifier getSelected(void) const;

		/**
		 * @brief Set the selected child identifier.
		 * @param selected The selected child identifier.
		 * @return Reference to this SelectionGroup for chaining.
		 */
		SelectionGroup &setSelected(ecs::Entity::Identifier selected);

		/**
		 * @brief Clear the current selection.
		 * @return Reference to this SelectionGroup for chaining.
		 */
		SelectionGroup &clearSelection(void);

		/**
		 * @brief Check whether the group may have no selection.
		 * @return True when empty selection is allowed.
		 */
		bool isEmptyAllowed(void) const;

		/**
		 * @brief Set whether the group may have no selection.
		 * @param allowEmpty Whether empty selection is allowed.
		 * @return Reference to this SelectionGroup for chaining.
		 */
		SelectionGroup &setAllowEmpty(bool allowEmpty);
	};

}	 // namespace guillaume::components
