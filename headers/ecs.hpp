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

#include "ecs/ecs.hpp"

#include "component_registry.hpp"

#include "systems/animation/animation_system.hpp"
#include "systems/animation/tween_system.hpp"

#include "systems/core/input_system.hpp"
#include "systems/core/layout_system.hpp"
#include "systems/core/render_system.hpp"
#include "systems/core/state_system.hpp"

#include "systems/specialized/drag_drop_system.hpp"
#include "systems/specialized/localization_system.hpp"
#include "systems/specialized/progress_bar_system.hpp"

#include "systems/utility/event_system.hpp"
#include "systems/utility/focus_system.hpp"
#include "systems/utility/hierarchy_system.hpp"
#include "systems/utility/scroll_system.hpp"

namespace guillaume {

/**
 * @brief Main ECS class using the default ComponentRegistry.
 */
class ECS : public ecs::ECS<ComponentRegistry> {
  private:
    /**
     * @brief Utility to register all core systems.
     * @tparam SystemTypes The system types to register.
     */
    template <ecs::InheritFromSystem... SystemTypes>
    void registerSystems(void) {
        (_systemRegistry.registerNewSystem<SystemTypes>(
             std::make_unique<SystemTypes>()),
         ...);
    }

  public:
    /**
     * @brief Default constructor.
     */
    ECS(void) {
        registerSystems<
            systems::animation::AnimationSystem,
            systems::animation::TweenSystem, systems::core::InputSystem,
            systems::core::LayoutSystem, systems::core::RenderSystem,
            systems::core::StateSystem, systems::specialized::DragDropSystem,
            systems::specialized::LocalizationSystem,
            systems::specialized::ProgressBarSystem,
            systems::utility::EventSystem, systems::utility::FocusSystem,
            systems::utility::HierarchySystem,
            systems::utility::ScrollSystem>();
    }

    /**
     * @brief Default destructor.
     */
    ~ECS(void) = default;
};

} // namespace guillaume