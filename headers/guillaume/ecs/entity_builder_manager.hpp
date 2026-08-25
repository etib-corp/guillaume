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

#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Manager class for handling multiple entity builders.
	 */
	class EntityBuilderManager
	{
		private:
		std::map<std::type_index, std::unique_ptr<EntityBuilder>>
			_builders;	  ///< Map of entity builders indexed by their type

		public:
		/**
		 * @brief Add a builder for a specific entity type.
		 * @tparam BuilderType The type of the builder being added, which must
		 * inherit from EntityBuilder
		 * @param componentRegistry Component registry passed to the builder
		 * constructor.
		 * @param entityRegistry Entity registry passed to the builder
		 * constructor.
		 * @throws std::runtime_error if a builder for the specified entity type
		 * already exists
		 */
		template<InheritFromEntityBuilder BuilderType>
		void addBuilder(ComponentRegistry &componentRegistry,
						EntityRegistry &entityRegistry);

		public:
		/**
		 * @brief Construct a new EntityBuilderManager object.
		 */
		EntityBuilderManager(void);

		/**
		 * @brief Default destructor for the EntityBuilderManager class.
		 */
		virtual ~EntityBuilderManager(void);

		/**
		 * @brief Get a reference to the builder for a specific entity type.
		 * @tparam BuilderType The type of the builder to retrieve, which must
		 * inherit from EntityBuilder
		 * @return Reference to the builder for the specified entity type
		 * @throws std::runtime_error if no builder exists for the specified
		 * entity type
		 */
		template<InheritFromEntityBuilder BuilderType>
		BuilderType &getBuilder(void);
	};

}	 // namespace guillaume::ecs

// Include the implementation of the EntityBuilderManager template class
#include "guillaume/ecs/entity_builder_manager.tpp"