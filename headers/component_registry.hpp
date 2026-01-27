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

#include "components/animation/animator.hpp"
#include "components/animation/tween.hpp"
#include "components/functional/button.hpp"
#include "components/functional/dropdown.hpp"
#include "components/functional/input_field.hpp"
#include "components/functional/progress_bar.hpp"
#include "components/functional/slider.hpp"
#include "components/hierarchy/children.hpp"
#include "components/hierarchy/parent.hpp"
#include "components/interaction/clickable.hpp"
#include "components/interaction/draggable.hpp"
#include "components/interaction/focusable.hpp"
#include "components/interaction/hoverable.hpp"
#include "components/layout/anchor.hpp"
#include "components/layout/layout.hpp"
#include "components/layout/scrollable.hpp"
#include "components/state/enabled.hpp"
#include "components/state/selected.hpp"
#include "components/state/tooltip.hpp"
#include "components/state/visibility.hpp"
#include "components/visual/border.hpp"
#include "components/visual/opacity.hpp"
#include "components/visual/rectangle.hpp"
#include "components/visual/sprite.hpp"
#include "components/visual/transform.hpp"
#include "ecs/component_registry_filler.hpp"

namespace guillaume {

/**
 * @brief Component registry class registering all core components.
 */
class ComponentRegistry
    : public ecs::ComponentRegistryFiller<
          components::animation::Animator, components::animation::Tween,
          components::functional::Button, components::functional::Dropdown,
          components::functional::InputField,
          components::functional::ProgressBar, components::functional::Slider,
          components::hierarchy::Children, components::hierarchy::Parent,
          components::interaction::Clickable,
          components::interaction::Draggable,
          components::interaction::Focusable,
          components::interaction::Hoverable, components::layout::Anchor,
          components::layout::Layout, components::layout::Scrollable,
          components::state::Enabled, components::state::Selected,
          components::state::Tooltip, components::state::Visibility,
          components::visual::Border, components::visual::Opacity,
          components::visual::Rectangle, components::visual::Sprite,
          components::visual::Transform> {
  public:
    /**
     * @brief Default constructor.
     */
    ComponentRegistry(void)
        : ecs::ComponentRegistryFiller<
              components::animation::Animator, components::animation::Tween,
              components::functional::Button, components::functional::Dropdown,
              components::functional::InputField,
              components::functional::ProgressBar,
              components::functional::Slider, components::hierarchy::Children,
              components::hierarchy::Parent, components::interaction::Clickable,
              components::interaction::Draggable,
              components::interaction::Focusable,
              components::interaction::Hoverable, components::layout::Anchor,
              components::layout::Layout, components::layout::Scrollable,
              components::state::Enabled, components::state::Selected,
              components::state::Tooltip, components::state::Visibility,
              components::visual::Border, components::visual::Opacity,
              components::visual::Rectangle, components::visual::Sprite,
              components::visual::Transform>() {}

    /**
     * @brief Default destructor.
     */
    virtual ~ComponentRegistry(void) = default;
};

} // namespace guillaume