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

#include "guillaume/ecs/component_registry_filler.hpp"

#include "guillaume/ecs/components/children.hpp"
#include "guillaume/ecs/components/parent.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Templated registry helper that registers multiple component
	 * types for an entity.
	 * @tparam ComponentTypes The component types to register.
	 * @see ComponentRegistry
	 */
	template<InheritFromComponent... ComponentTypes>
	class TreeComponentRegistryFiller:
		public ComponentRegistryFiller<ComponentTypes..., components::Children,
									   components::Parent>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		TreeComponentRegistryFiller(void)
			: ComponentRegistryFiller<ComponentTypes..., components::Children,
									  components::Parent>()
		{
		}

		/**
		 * @brief Default destructor for the Component Registry Filler class.
		 */
		virtual ~TreeComponentRegistryFiller(void) = default;
	};

}	 // namespace guillaume::ecs