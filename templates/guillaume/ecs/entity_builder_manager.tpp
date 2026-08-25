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

#include "guillaume/ecs/entity_builder_manager.hpp"

namespace guillaume::ecs
{
	template<InheritFromEntityBuilder BuilderType> void
		EntityBuilderManager::addBuilder(ComponentRegistry &componentRegistry,
										 EntityRegistry &entityRegistry)
	{
		std::type_index typeIndex(typeid(BuilderType));
		if (_builders.find(typeIndex) != _builders.end()) {
			throw std::runtime_error(
				"Builder for this entity type already exists");
		}
		_builders[typeIndex] = std::make_unique<BuilderType>(componentRegistry, entityRegistry);
	}

	template<InheritFromEntityBuilder BuilderType>
	BuilderType &EntityBuilderManager::getBuilder(void)
	{
		std::type_index typeIndex(typeid(BuilderType));
		auto it = _builders.find(typeIndex);
		if (it == _builders.end()) {
			throw std::runtime_error("No builder found for this entity type");
		}
		return *static_cast<BuilderType *>(it->second.get());
	}
}	 // namespace guillaume::ecs