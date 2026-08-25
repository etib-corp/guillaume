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

#include "guillaume/ecs/entity_director.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Manager class for handling multiple entity directors.
	 */
	class EntityDirectorManager
	{
		private:
		std::map<std::type_index, std::unique_ptr<EntityDirector>>
			_directors;	   ///< Map of entity directors indexed by their type

		public:
		/**
		 * @brief Add a director for a specific entity type.
		 * @tparam DirectorType The type of the director being added, which must
		 * inherit from EntityDirector.
		 * @return Reference to the added director.
		 * @throws std::runtime_error if a director for the specified type
		 * already exists.
		 */
		template<InheritFromEntityDirector DirectorType>
		DirectorType &addDirector(void);

		/**
		 * @brief Construct a new EntityDirectorManager object.
		 */
		EntityDirectorManager(void);

		/**
		 * @brief Default destructor for the EntityDirectorManager class.
		 */
		virtual ~EntityDirectorManager(void);

		/**
		 * @brief Get a reference to the director for a specific entity type.
		 * @tparam DirectorType The type of the director to retrieve, which must
		 * inherit from EntityDirector.
		 * @return Reference to the director for the specified entity type.
		 * @throws std::runtime_error if no director exists for the specified
		 * entity type.
		 */
		template<InheritFromEntityDirector DirectorType>
		DirectorType &getDirector(void);
	};

}	 // namespace guillaume::ecs

// Include the implementation of the EntityDirectorManager template class
#include "guillaume/ecs/entity_director_manager.tpp"