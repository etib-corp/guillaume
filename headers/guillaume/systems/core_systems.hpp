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

#include <memory>

#include <utility/engine.hpp>
#include <utility/ressource_provider.hpp>

#include "guillaume/ecs/system_registry.hpp"
#include "guillaume/event/event_bus.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Register every built-in system of the library in a registry.
	 *
	 * This function is the one list of the built-in systems: a new one is
	 * added in `sources/systems/core_systems.cpp`, nowhere else.
	 *
	 * A frame runs the phases in the order Event, Measure, Layout, Render
	 * (see ecs::Phase). The registry keeps one list per phase, so the order
	 * of registration only matters between systems of the same phase, where
	 * it is the order they run in. The constraints the list respects:
	 * - **Measure**: systems::Typography first, because it resolves the
	 *   font metrics systems::MeasureText measures with.
	 * - **Event**: the systems that turn events into interaction
	 *   components (MouseMotion, MouseButton, the hand systems) before
	 *   Focus and StyleState, which read those components. Drag runs after
	 *   MouseMotion and MouseButton but before the hand systems, so it sees
	 *   a hand button press one frame after it happened.
	 * - **Layout**: systems::Layout before the widget systems (Checkbox,
	 *   Menu, ...), which place the children Layout does not arrange from
	 *   the bounds it has just resolved.
	 * - **Layout**: a destination system before the system of the bar or
	 *   drawer that owns it (NavigationBarDestination before NavigationBar,
	 *   NavigationDrawerDestination before NavigationDrawer).
	 *
	 * @param systemRegistry The registry receiving the systems.
	 * @param eventBus The event bus the input systems subscribe to.
	 * @param engine The engine used by the input, measuring and rendering
	 * systems. It must outlive the registered systems.
	 * @param ressourceProvider The provider the measuring and rendering
	 * systems load fonts, images and meshes from.
	 * @throws std::out_of_range If a system reports a phase the registry
	 * does not know (ecs::SystemRegistry::registerNewSystem).
	 * @see ecs::SystemRegistry
	 * @see ecs::Phase
	 */
	void registerCoreSystems(
		ecs::SystemRegistry &systemRegistry, event::EventBus &eventBus,
		utility::Engine &engine,
		std::shared_ptr<utility::RessourceProvider> ressourceProvider);

}	 // namespace guillaume::systems
