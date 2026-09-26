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

#include "guillaume/components/selection.hpp"

namespace guillaume::components
{
	bool Selectable::isSelected(void) const
	{
		return _selected;
	}

	Selectable &Selectable::setSelected(bool selected)
	{
		if (_selected == selected) {
			return *this;
		}
		_selected = selected;
		setHasChanged(true);
		if (_onSelectionChanged) {
			_onSelectionChanged(_selected);
		}
		return *this;
	}

	Selectable &Selectable::setOnSelectionChangedHandler(const Handler &handler)
	{
		_onSelectionChanged = handler;
		return *this;
	}

	Selectable::Handler Selectable::getOnSelectionChangedHandler(void) const
	{
		return _onSelectionChanged;
	}

	bool SelectionGroup::hasSelection(void) const
	{
		return _selected != ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier SelectionGroup::getSelected(void) const
	{
		return _selected;
	}

	SelectionGroup &
		SelectionGroup::setSelected(ecs::Entity::Identifier selected)
	{
		if (_selected == selected) {
			return *this;
		}
		_selected = selected;
		setHasChanged(true);
		return *this;
	}

	SelectionGroup &SelectionGroup::clearSelection(void)
	{
		return setSelected(ecs::Entity::InvalidIdentifier);
	}

	bool SelectionGroup::isEmptyAllowed(void) const
	{
		return _allowEmpty;
	}

	SelectionGroup &SelectionGroup::setAllowEmpty(bool allowEmpty)
	{
		if (_allowEmpty == allowEmpty) {
			return *this;
		}
		_allowEmpty = allowEmpty;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
