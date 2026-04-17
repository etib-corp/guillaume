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

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include <utility/demangle.hpp>

#include "guillaume/ecs/system.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Exception thrown when a requested system is not found in the
	 * registry.
	 * @tparam SystemType The type of the missing system.
	 */
	template<InheritFromSystem SystemType> class SystemNotFoundException:
		public std::exception
	{
		private:
		std::type_index _systemIndex { typeid(
			SystemType) };	  ///< Type index of the missing system
		std::string _systemName {
			_systemIndex.name()
		};	  ///< Name of the missing system type
		mutable std::string _message;	 ///< Cached exception message

		public:
		/**
		 * @brief Construct a new System Not Found Exception object.
		 */
		SystemNotFoundException(void) = default;

		/**
		 * @brief Get the exception message.
		 * @return The exception message.
		 */
		const char *what(void) const noexcept override
		{
			if (_message.empty()) {
				_message = "System of type " + _systemName + " not found";
			}
			return _message.c_str();
		}

		/**
		 * @brief Get the type index of the missing system.
		 * @return The type index of the missing system.
		 */
		std::type_index getSystemTypeIndex(void) const
		{
			return _systemIndex;
		}
	};

	/**
	 * @brief Non-template base class for managing systems within the ECS
	 * architecture.
	 *
	 * This base class provides common functionality that doesn't depend on
	 * system types.
	 * @see System
	 */
	class SystemRegistry:
		public utility::logging::Loggable<SystemRegistry,
										  utility::logging::StandardLogger>
	{
		public:
		private:
		std::map<ecs::System::Phase, std::vector<std::unique_ptr<System>>>
			_systems;	 ///< Map of registered systems

		public:
		/**
		 * @brief Default constructor
		 */
		SystemRegistry(void) = default;

		/**
		 * @brief Default destructor
		 */
		~SystemRegistry(void) = default;

		/**
		 * @brief Register a new system in the registry.
		 * @param system Unique pointer to the system instance.
		 * @note If a system of the same type is already registered, it is
		 * replaced.
		 */
		void registerNewSystem(std::unique_ptr<System> system);

		/**
		 * @brief Get all systems registered for a specific update phase.
		 * @param phase The update phase to query.
		 * @return Vector of pointers to systems registered for the specified
		 * phase. Returns an empty vector if no systems are registered for the
		 * phase.
		 * @throws std::runtime_error if no systems are registered for the
		 * specified phase.
		 */
		const std::vector<std::unique_ptr<System>> &
			getSystemsByPhase(System::Phase phase) const;
	};

}	 // namespace guillaume::ecs