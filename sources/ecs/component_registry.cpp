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

#include <utility/demangle.hpp>

#include "guillaume/ecs/component_registry.hpp"

namespace guillaume::ecs
{
	bool ComponentRegistry::hasChanged(
		const Entity::Identifier &entityIdentifier) const
	{
		for (const auto &[typeIndex, storage]: _storages) {
			if (storage->hasChanged(entityIdentifier)) {
				return true;
			}
		}
		return false;
	}

	void ComponentRegistry::resetChangedFlags(void)
	{
		for (auto &[typeIndex, storage]: _storages) {
			(void)typeIndex;
			storage->resetChangedFlags();
		}
	}
}	 // namespace guillaume::ecs