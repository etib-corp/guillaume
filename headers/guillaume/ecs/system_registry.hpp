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

#include <array>
#include <exception>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/demangle.hpp>

#include "guillaume/ecs/system.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Owns the systems of an application, grouped by phase and kept
	 * in registration order.
	 *
	 * Application has one registry, filled once at start-up by
	 * systems::registerCoreSystems, and shared by every scene. Each frame,
	 * for each phase, Application::routine runs the systems returned by
	 * getSystemsByPhase() in order.
	 *
	 * @code
	 * ecs::SystemRegistry systems;
	 * systems.registerNewSystem(std::make_unique<MySystem>());
	 *
	 * for (const auto &system:
	 * 	 systems.getSystemsByPhase(ecs::Phase::Layout)) {
	 * 	system->routine(componentRegistry, entityRegistry, traveler);
	 * }
	 * @endcode
	 *
	 * @note Within a phase, registration order is execution order. When a
	 * system needs what another system of the same phase produces in the
	 * same frame, register that other system first.
	 * @see System
	 * @see Phase
	 */
	class SystemRegistry:
		public utility::logging::Loggable<SystemRegistry,
										  utility::logging::DefaultLogger>
	{
		public:
		private:
		std::array<std::vector<std::unique_ptr<System>>,
				   static_cast<std::size_t>(Phase::Count)>
			_systems;	 ///< One list of systems per phase, each in
						 ///< registration order

		public:
		/**
		 * @brief Create a registry with no system.
		 */
		SystemRegistry(void) = default;

		/**
		 * @brief Destroy the registry and the systems it owns.
		 */
		~SystemRegistry(void) = default;

		/**
		 * @brief Take ownership of a system and append it to the list of
		 * its phase (System::getPhase).
		 *
		 * Nothing prevents registering two systems of the same type: both
		 * are kept and both run.
		 * @param system The system to add. Must not be null.
		 * @throws std::out_of_range When the phase of the system is not one
		 * of the four phases (Phase::Count, for instance).
		 */
		void registerNewSystem(std::unique_ptr<System> system);

		/**
		 * @brief Get the systems of a phase, in the order they run.
		 * @param phase The phase to list.
		 * @return Reference to the list, in registration order; empty when
		 * no system is registered for the phase. It stays valid as long as
		 * the registry, and grows when a system is registered.
		 * @throws std::out_of_range When `phase` is not one of the four
		 * phases (Phase::Count, for instance).
		 */
		const std::vector<std::unique_ptr<System>> &
			getSystemsByPhase(Phase phase) const;
	};

}	 // namespace guillaume::ecs