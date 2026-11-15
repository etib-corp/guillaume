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

#include "guillaume/components/navigation_drawer.hpp"

#include <utility>

namespace guillaume::components
{
	NavigationDrawer::Variant NavigationDrawer::getVariant(void) const noexcept
	{
		return _variant;
	}

	NavigationDrawer &NavigationDrawer::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<NavigationDrawer::Entry> &
		NavigationDrawer::getEntries(void) const noexcept
	{
		return _entries;
	}

	NavigationDrawer &
		NavigationDrawer::setEntries(const std::vector<Entry> &entries)
	{
		if (_entries != entries) {
			_entries = entries;
			setHasChanged(true);
		}
		return *this;
	}

	NavigationDrawer &
		NavigationDrawer::setDestinationDisabled(std::size_t index,
												 bool disabled)
	{
		std::size_t destinationIndex = 0;

		for (auto &entry: _entries) {
			if (entry.kind != EntryKind::Destination) {
				continue;
			}
			if (destinationIndex++ == index) {
				if (entry.disabled != disabled) {
					entry.disabled = disabled;
					setHasChanged(true);
				}
				break;
			}
		}
		return *this;
	}

	std::size_t NavigationDrawer::getDestinationCount(void) const noexcept
	{
		std::size_t count = 0;

		for (const auto &entry: _entries) {
			if (entry.kind == EntryKind::Destination) {
				++count;
			}
		}
		return count;
	}

	const std::vector<ecs::Entity::Identifier> &
		NavigationDrawer::getEntryEntities(void) const noexcept
	{
		return _entryEntities;
	}

	NavigationDrawer &NavigationDrawer::setEntryEntities(
		const std::vector<ecs::Entity::Identifier> &identifiers)
	{
		if (_entryEntities != identifiers) {
			_entryEntities = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		NavigationDrawer::getDestinationEntities(void) const noexcept
	{
		return _destinationEntities;
	}

	NavigationDrawer &NavigationDrawer::setDestinationEntities(
		const std::vector<ecs::Entity::Identifier> &identifiers)
	{
		if (_destinationEntities != identifiers) {
			_destinationEntities = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		NavigationDrawer::getSectionEntities(void) const noexcept
	{
		return _sectionEntities;
	}

	NavigationDrawer &NavigationDrawer::setSectionEntities(
		const std::vector<ecs::Entity::Identifier> &identifiers)
	{
		if (_sectionEntities != identifiers) {
			_sectionEntities = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		NavigationDrawer::getDividerEntities(void) const noexcept
	{
		return _dividerEntities;
	}

	NavigationDrawer &NavigationDrawer::setDividerEntities(
		const std::vector<ecs::Entity::Identifier> &identifiers)
	{
		if (_dividerEntities != identifiers) {
			_dividerEntities = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	ecs::Entity::Identifier NavigationDrawer::getBackdrop(void) const noexcept
	{
		return _backdrop;
	}

	NavigationDrawer &
		NavigationDrawer::setBackdrop(ecs::Entity::Identifier identifier)
	{
		if (_backdrop != identifier) {
			_backdrop = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	const NavigationDrawer::IndexHandler &
		NavigationDrawer::getOnIndexChange(void) const noexcept
	{
		return _onIndexChange;
	}

	NavigationDrawer &NavigationDrawer::setOnIndexChange(IndexHandler handler)
	{
		_onIndexChange = std::move(handler);
		return *this;
	}

	bool NavigationDrawer::isUpdatingSelection(void) const noexcept
	{
		return _updatingSelection;
	}

	NavigationDrawer &NavigationDrawer::setUpdatingSelection(bool updating)
	{
		if (_updatingSelection != updating) {
			_updatingSelection = updating;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
