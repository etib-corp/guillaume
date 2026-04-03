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

	/**
	 * @brief Manager class for handling multiple entity builders.
	 */
	template<InheritFromEntityBuilder... BuilderTypes>
	class EntityBuilderManagerFiller: public EntityBuilderManager
	{
		public:
		/**
		 * @brief Default constructor.
		 * @param componentRegistry The component registry to pass to builders.
		 * @param entityRegistry The entity registry to pass to builders.
		 */
		EntityBuilderManagerFiller(ComponentRegistry &componentRegistry,
								   EntityRegistry &entityRegistry)
			: EntityBuilderManager()
		{
			(addBuilder<BuilderTypes>(componentRegistry, entityRegistry), ...);
		}

		/**
		 * @brief Default destructor for the Entity Builder Manager Filler
		 * class.
		 */
		virtual ~EntityBuilderManagerFiller(void) = default;
	};

}	 // namespace guillaume::ecs