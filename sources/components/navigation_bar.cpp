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

#include "guillaume/components/navigation_bar.hpp"

#include <utility>

namespace guillaume::components
{
	NavigationBar::Variant NavigationBar::getVariant(void) const noexcept
	{
		return _variant;
	}

	NavigationBar &NavigationBar::setVariant(Variant variant)
	{
		if (_variant != variant) {
			_variant = variant;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<NavigationBar::Destination> &
		NavigationBar::getDestinations(void) const noexcept
	{
		return _destinations;
	}

	NavigationBar &NavigationBar::setDestinations(
		const std::vector<Destination> &destinations)
	{
		if (_destinations != destinations) {
			_destinations = destinations;
			setHasChanged(true);
		}
		return *this;
	}

	NavigationBar &NavigationBar::addDestination(const Destination &destination)
	{
		_destinations.push_back(destination);
		setHasChanged(true);
		return *this;
	}

	NavigationBar &NavigationBar::setDestinationBadge(std::size_t index,
													  const std::string &badge)
	{
		if (index < _destinations.size()
			&& _destinations[index].badge != badge) {
			_destinations[index].badge = badge;
			setHasChanged(true);
		}
		return *this;
	}

	const std::vector<ecs::Entity::Identifier> &
		NavigationBar::getDestinationEntities(void) const noexcept
	{
		return _destinationEntities;
	}

	NavigationBar &NavigationBar::setDestinationEntities(
		const std::vector<ecs::Entity::Identifier> &identifiers)
	{
		if (_destinationEntities != identifiers) {
			_destinationEntities = identifiers;
			setHasChanged(true);
		}
		return *this;
	}

	const NavigationBar::IndexHandler &
		NavigationBar::getOnIndexChange(void) const noexcept
	{
		return _onIndexChange;
	}

	NavigationBar &NavigationBar::setOnIndexChange(IndexHandler handler)
	{
		_onIndexChange = std::move(handler);
		return *this;
	}

	const NavigationBar::IndexHandler &
		NavigationBar::getOnTabPress(void) const noexcept
	{
		return _onTabPress;
	}

	NavigationBar &NavigationBar::setOnTabPress(IndexHandler handler)
	{
		_onTabPress = std::move(handler);
		return *this;
	}

	bool NavigationBar::isUpdatingSelection(void) const noexcept
	{
		return _updatingSelection;
	}

	NavigationBar &NavigationBar::setUpdatingSelection(bool updating)
	{
		if (_updatingSelection != updating) {
			_updatingSelection = updating;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
