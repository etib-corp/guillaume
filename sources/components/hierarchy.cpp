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
#include "guillaume/components/hierarchy.hpp"

#include <algorithm>

namespace guillaume::components
{
	ecs::Entity::Identifier Hierarchy::getParentIdentifier(void) const noexcept
	{
		return _parentIdentifier;
	}

	Hierarchy &
		Hierarchy::setParentIdentifier(ecs::Entity::Identifier identifier)
	{
		if (_parentIdentifier != identifier) {
			_parentIdentifier = identifier;
			setHasChanged(true);
		}
		return *this;
	}

	bool Hierarchy::hasParent(void) const noexcept
	{
		return _parentIdentifier != ecs::Entity::InvalidIdentifier;
	}

	const std::vector<ecs::Entity::Identifier> &
		Hierarchy::getChildIdentifiers(void) const noexcept
	{
		return _childIdentifiers;
	}

	bool Hierarchy::hasChild(ecs::Entity::Identifier identifier) const noexcept
	{
		return std::find(_childIdentifiers.begin(), _childIdentifiers.end(),
						 identifier)
			!= _childIdentifiers.end();
	}

	bool Hierarchy::addChild(ecs::Entity::Identifier identifier)
	{
		if (hasChild(identifier)) {
			return false;
		}
		_childIdentifiers.push_back(identifier);
		setHasChanged(true);
		return true;
	}

	bool Hierarchy::removeChild(ecs::Entity::Identifier identifier)
	{
		const auto iterator = std::find(_childIdentifiers.begin(),
										_childIdentifiers.end(), identifier);
		if (iterator == _childIdentifiers.end()) {
			return false;
		}
		_childIdentifiers.erase(iterator);
		setHasChanged(true);
		return true;
	}

	std::int32_t Hierarchy::getLayer(void) const noexcept
	{
		return _layer;
	}

	Hierarchy &Hierarchy::setLayer(std::int32_t layer)
	{
		if (_layer != layer) {
			_layer = layer;
			setHasChanged(true);
		}
		return *this;
	}
}	 // namespace guillaume::components
