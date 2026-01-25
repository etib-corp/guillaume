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

// - **Components**:
//   - `Transform` (position, size)
//   - `Sprite` (background image/color)
//   - `Text` (label)
//   - `Clickable` (on_click callback)
//   - `Hoverable` (hover effects)
//   - `Border` (optional styling)
//   - `Animator` (optional for animations)

#include "components/animation/animator.hpp"
#include "components/interaction/clickable.hpp"
#include "components/interaction/hoverable.hpp"
#include "components/visual/border.hpp"
#include "components/visual/sprite.hpp"
#include "components/visual/text.hpp"
#include "components/visual/transform.hpp"
#include "ecs/entity_filler.hpp"

namespace guillaume::entities {

class Button : public ecs::EntityFiller<>