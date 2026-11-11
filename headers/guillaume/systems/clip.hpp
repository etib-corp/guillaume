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
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/clip.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Turns the bound of an entity into the rectangle the engine
	 * clips drawing to.
	 *
	 * Runs in the Layout phase on entities with Transform, Bound and Clip.
	 * - Reads: components::Transform (top-left corner), components::Bound
	 *   (size) and the margin and enabled flag of components::Clip.
	 * - Writes: the resolved rectangle into components::Clip
	 *   (components::Clip::setRect), and the engine scissor through
	 *   utility::Engine::setScissor.
	 *
	 * The scissor is cleared once per frame in prepare(), then set by each
	 * enabled clip entity. The render systems that run later in the frame
	 * draw with whatever scissor is left.
	 *
	 * Things to know:
	 * - The engine has one scissor. With several enabled clip entities the
	 *   last one visited wins, and it applies to everything drawn, not only
	 *   to the descendants of that entity.
	 * - The rectangle is axis-aligned: it is built from the X and Y of the
	 *   position and ignores the orientation.
	 * - This system is registered before systems::Layout, which runs in the
	 *   same phase. It therefore reads the Transform and Bound as the
	 *   previous frame left them.
	 *
	 * @see components::Clip
	 */
	class Clip:
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Clip>
	{
		private:
		utility::Engine &_engine;	 ///< Engine whose scissor is set

		public:
		/**
		 * @brief Create the system in the Layout phase.
		 * @param engine The engine that receives the scissor rectangle. It
		 * must outlive the system.
		 */
		Clip(utility::Engine &engine);

		/**
		 * @brief Destroy the system. The engine scissor is left as it is.
		 */
		~Clip(void) override = default;

		/**
		 * @brief Clear the engine scissor. Called once per frame before the
		 * entities are visited, so a frame without any enabled clip draws
		 * unclipped.
		 */
		void prepare(void) override;

		/**
		 * @brief Resolve the clip rectangle of one entity, store it in its
		 * components::Clip and make it the engine scissor.
		 * @param entityIdentifier The entity to resolve. It is skipped when
		 * its clip is disabled, and skipped with a log message when one of
		 * the three components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Compute a clip rectangle from the pose and size of a bound.
		 *
		 * The rectangle starts at the X and Y of the pose and is shrunk by
		 * @p margin on its four sides. The orientation is ignored.
		 * @param pose Pose of the top-left corner of the bound.
		 * @param width Width of the bound, in dp.
		 * @param height Height of the bound, in dp.
		 * @param margin Inset applied to each side, in dp. A negative value
		 * grows the rectangle.
		 * @return The rectangle. Its width or height is negative when the
		 * margin exceeds half the matching size.
		 */
		static components::Clip::Rect
			computeRect(const utility::graphic::PoseF &pose, float width,
						float height, float margin);
	};

}	 // namespace guillaume::systems
