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

#include "guillaume/components/menu.hpp"

#include <algorithm>
#include <utility>

namespace guillaume::components
{
	bool Menu::Row::operator==(const Row &other) const noexcept
	{
		return view == other.view && content == other.content
			&& label == other.label && icons == other.icons;
	}

	Menu::Variant Menu::getVariant(void) const noexcept
	{
		return _variant;
	}

	Menu &Menu::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	Menu::Mode Menu::getMode(void) const noexcept
	{
		return _mode;
	}

	Menu &Menu::setMode(Mode mode)
	{
		if (_mode != mode) {
			_mode = mode;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<Menu::Item> &Menu::getItems(void) const noexcept
	{
		return _items;
	}

	Menu &Menu::setItems(std::vector<Item> items)
	{
		const bool same =
			std::equal(_items.begin(), _items.end(), items.begin(), items.end(),
					   [](const Item &left, const Item &right) {
						   return left.label == right.label
							   && left.leadingIcon == right.leadingIcon
							   && left.trailingIcon == right.trailingIcon
							   && left.disabled == right.disabled
							   && left.dense == right.dense;
					   });

		_items = std::move(items);
		if (!same) {
			setHasChanged(true);
		}
		return *this;
	}

	Menu &Menu::setItemDisabled(std::size_t index, bool disabled)
	{
		if (index < _items.size() && _items[index].disabled != disabled) {
			_items[index].disabled = disabled;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<Menu::Row> &Menu::getRows(void) const noexcept
	{
		return _rows;
	}

	Menu &Menu::setRows(const std::vector<Row> &rows)
	{
		if (_rows != rows) {
			_rows = rows;
			setHasChanged(true);
		}
		return *this;
	}

	bool Menu::isRowsDirty(void) const noexcept
	{
		return _rowsDirty;
	}

	Menu &Menu::setRowsDirty(bool dirty)
	{
		if (_rowsDirty != dirty) {
			_rowsDirty = dirty;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier Menu::getAnchor(void) const noexcept
	{
		return _anchor;
	}

	Menu &Menu::setAnchor(ecs::Entity::Identifier anchor)
	{
		if (_anchor != anchor) {
			_anchor = anchor;
			setHasChanged(true);
		}
		return *this;
	}

	Menu::AnchorPosition Menu::getAnchorPosition(void) const noexcept
	{
		return _anchorPosition;
	}

	Menu &Menu::setAnchorPosition(AnchorPosition position)
	{
		if (_anchorPosition != position) {
			_anchorPosition = position;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
