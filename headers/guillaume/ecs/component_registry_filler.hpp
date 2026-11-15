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

#include "guillaume/ecs/component_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief A ComponentRegistry whose storages for a fixed list of component
	 * types exist from construction.
	 *
	 * It behaves exactly like ComponentRegistry afterwards. Preparing the
	 * storages is optional, since a ComponentRegistry creates the storage of
	 * a type on its first use; no component is created for any entity.
	 *
	 * @code
	 * ecs::ComponentRegistryFiller<components::Bound, components::Transform>
	 * 	registry;
	 * @endcode
	 *
	 * @tparam ComponentTypes The component types to prepare storage for.
	 * @see ComponentRegistry
	 */
	template<InheritFromComponent... ComponentTypes>
	class ComponentRegistryFiller: public ComponentRegistry
	{
		public:
		/**
		 * @brief Create the registry with one empty storage per listed
		 * component type.
		 */
		ComponentRegistryFiller(void);

		/**
		 * @brief Destroy the registry and every component it stores.
		 */
		virtual ~ComponentRegistryFiller(void) = default;
	};

}	 // namespace guillaume::ecs

// Include the implementation of the ComponentRegistryFiller template class
#include "guillaume/ecs/component_registry_filler.tpp"