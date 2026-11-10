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
	 * @brief Register every built-in system into a registry.
	 *
	 * This is the single place that lists the core systems: adding a built-in
	 * system means adding it here, not editing Application.
	 * @param systemRegistry Registry receiving the systems.
	 * @param eventBus Event bus the input systems subscribe to.
	 * @param engine Engine handle used by the measuring and rendering systems.
	 * It is held by reference and may still be null at registration time.
	 * @param ressourceProvider Provider used to load fonts, images and meshes.
	 */
	void registerCoreSystems(
		ecs::SystemRegistry &systemRegistry, event::EventBus &eventBus,
		std::unique_ptr<utility::Engine> &engine,
		std::shared_ptr<utility::RessourceProvider> ressourceProvider);

}	 // namespace guillaume::systems
