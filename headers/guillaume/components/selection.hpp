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
	 * @brief Selected/unselected flag of an entity, with a callback run when
	 * it changes.
	 *
	 * Widget systems write it and read it as the selection of their widget:
	 * radio buttons, checkboxes, chips and navigation destinations all have
	 * one. When the parent entity has a components::SelectionGroup,
	 * systems::Selection (Event phase) keeps at most one Selectable child
	 * selected.
	 *
	 * @code
	 * registry.getComponent<components::Selectable>(identifier)
	 * 	.setOnSelectionChangedHandler([](bool selected) { ... })
	 * 	.setSelected(true);	   // runs the handler with true
	 * @endcode
	 *
	 * The handler runs inside setSelected(), synchronously. The widget
	 * systems install their own handler here to restyle the widget, so
	 * replacing it on a widget's entity removes that behaviour.
	 *
	 * @see components::SelectionGroup
	 * @see systems::Selection
	 */
	class Selectable: public ecs::Component
	{
		public:
		/// Callback receiving the new state: true when just selected.
		using Handler = std::function<void(bool)>;

		private:
		bool _selected { false };		   ///< Whether the entity is selected.
		Handler _onSelectionChanged {};	   ///< Selection change handler.

		public:
		/**
		 * @brief Create an unselected flag with no handler.
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
		 * @brief Select or deselect the entity.
		 *
		 * When the state differs from the stored one, stores it, marks the
		 * component changed and calls the handler with the new state. Does
		 * nothing otherwise.
		 * @param selected True to select the entity.
		 * @return This component, for chaining.
		 */
		Selectable &setSelected(bool selected);

		/**
		 * @brief Set the callback run when the selection changes.
		 *
		 * Replaces the previous handler. Does not mark the component
		 * changed.
		 * @param handler The callback, or an empty one to remove it.
		 * @return This component, for chaining.
		 */
		Selectable &setOnSelectionChangedHandler(const Handler &handler);

		/**
		 * @brief Get the callback run when the selection changes.
		 * @return A copy of the callback, empty when none is set.
		 */
		Handler getOnSelectionChangedHandler(void) const;
	};

	/**
	 * @brief Marks an entity whose Selectable children are mutually
	 * exclusive, and remembers which one is selected.
	 *
	 * systems::Selection (Event phase) reads the children from the entity's
	 * components::Hierarchy each frame: when several are selected it keeps
	 * the first one, and it writes the result here. The navigation bar and
	 * the navigation drawer use it for their destinations.
	 *
	 * To change the selection, call systems::Selection::select() or select
	 * the child's Selectable; setSelected() here only records the result.
	 *
	 * Watch out: when no child is selected but this component still names
	 * one, the system selects that child again. Deselecting the selected
	 * child alone is therefore undone on the next frame; call
	 * clearSelection() as well to empty the group.
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
		 * @brief Create a group with no selection that may stay empty.
		 */
		SelectionGroup(void) = default;

		/**
		 * @brief Default destructor for the SelectionGroup component.
		 */
		~SelectionGroup(void) = default;

		/**
		 * @brief Check whether the group names a selected child.
		 * @return True when getSelected() is a valid identifier.
		 */
		bool hasSelection(void) const;

		/**
		 * @brief Get the selected child.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when
		 * none is selected.
		 */
		ecs::Entity::Identifier getSelected(void) const;

		/**
		 * @brief Record which child is selected.
		 *
		 * Marks the component changed when the identifier differs. It does
		 * not touch the Selectable of any child.
		 * @param selected The selected child, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		SelectionGroup &setSelected(ecs::Entity::Identifier selected);

		/**
		 * @brief Record that no child is selected.
		 *
		 * Same as `setSelected(ecs::Entity::InvalidIdentifier)`.
		 * @return This component, for chaining.
		 */
		SelectionGroup &clearSelection(void);

		/**
		 * @brief Check whether the group may have no selection.
		 * @return True when an empty selection is allowed (the default).
		 */
		bool isEmptyAllowed(void) const;

		/**
		 * @brief Choose whether the group may have no selection.
		 *
		 * When it may not, systems::Selection selects the first Selectable
		 * child whenever the group is empty. Marks the component changed
		 * when the flag differs.
		 * @param allowEmpty False to always keep one child selected.
		 * @return This component, for chaining.
		 */
		SelectionGroup &setAllowEmpty(bool allowEmpty);
	};

}	 // namespace guillaume::components
