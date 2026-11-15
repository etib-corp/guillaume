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

#include "guillaume/components/list.hpp"

#include <algorithm>
#include <utility>

namespace guillaume::components
{
	namespace
	{
		/**
		 * @brief Compare two entries, ignoring their press handlers.
		 * @param left The first entry.
		 * @param right The second entry.
		 * @return True when both describe the same content.
		 */
		bool sameItem(const List::Item &left, const List::Item &right)
		{
			return left.headline == right.headline
				&& left.supporting == right.supporting
				&& left.leadingIcon == right.leadingIcon
				&& left.trailingIcon == right.trailingIcon
				&& left.leadingImage == right.leadingImage
				&& left.disabled == right.disabled && left.kind == right.kind
				&& left.expanded == right.expanded
				&& std::equal(left.children.begin(), left.children.end(),
							  right.children.begin(), right.children.end(),
							  sameItem);
		}
	}	 // namespace

	bool List::Row::operator==(const Row &other) const noexcept
	{
		return entity == other.entity && view == other.view
			&& lead == other.lead && spacer == other.spacer
			&& leadingImage == other.leadingImage
			&& leadingIcon == other.leadingIcon && content == other.content
			&& headline == other.headline && supporting == other.supporting
			&& trailingIcon == other.trailingIcon && children == other.children;
	}

	List::Variant List::getVariant(void) const noexcept
	{
		return _variant;
	}

	List &List::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<List::Item> &List::getItems(void) const noexcept
	{
		return _items;
	}

	List &List::setItems(std::vector<Item> items)
	{
		const bool same = std::equal(_items.begin(), _items.end(),
									 items.begin(), items.end(), sameItem);

		_items = std::move(items);
		if (!same) {
			setHasChanged(true);
		}
		return *this;
	}

	const List::Item *List::findItem(const Path &path) const noexcept
	{
		if (path.empty() || path[0] >= _items.size()) {
			return nullptr;
		}

		const Item *item = &_items[path[0]];
		for (std::size_t depth = 1; depth < path.size(); ++depth) {
			if (path[depth] >= item->children.size()) {
				return nullptr;
			}
			item = &item->children[path[depth]];
		}
		return item;
	}

	const List::Row *List::findRow(const Path &path) const noexcept
	{
		if (path.empty() || path[0] >= _rows.size()) {
			return nullptr;
		}

		const Row *row = &_rows[path[0]];
		for (std::size_t depth = 1; depth < path.size(); ++depth) {
			if (path[depth] >= row->children.size()) {
				return nullptr;
			}
			row = &row->children[path[depth]];
		}
		return row;
	}

	List &List::setItemDisabled(std::size_t index, bool disabled)
	{
		if (index < _items.size() && _items[index].disabled != disabled) {
			_items[index].disabled = disabled;
			setHasChanged(true);
		}
		return *this;
	}

	List &List::setItemExpanded(std::size_t index, bool expanded)
	{
		if (index < _items.size() && !_items[index].children.empty()
			&& _items[index].expanded != expanded) {
			_items[index].expanded = expanded;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<List::Row> &List::getRows(void) const noexcept
	{
		return _rows;
	}

	List &List::setRows(const std::vector<Row> &rows)
	{
		if (_rows != rows) {
			_rows = rows;
			setHasChanged(true);
		}
		return *this;
	}

	bool List::isRowsDirty(void) const noexcept
	{
		return _rowsDirty;
	}

	List &List::setRowsDirty(bool dirty)
	{
		if (_rowsDirty != dirty) {
			_rowsDirty = dirty;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
