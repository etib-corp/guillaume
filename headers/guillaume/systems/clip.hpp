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

#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/clip.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System resolving clip rectangles for `components::Clip`.
	 *
	 * For every entity carrying a `components::Clip`, the system derives a
	 * rectangle from the entity `components::Transform` and `components::Bound`
	 * (inset by the clip margin) and stores it on the component, ready for a
	 * renderer or an engine scissor layer to consume.
	 *
	 * @see components::Clip
	 */
	class Clip:
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Clip>
	{
		public:
		/**
		 * @brief Construct a clip system running in the Layout phase.
		 */
		Clip(void);

		/**
		 * @brief Default destructor.
		 */
		~Clip(void) override = default;

		/**
		 * @brief Resolve the clip rectangle of one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Compute a clip rectangle from a pose and a size.
		 * @param pose The entity pose.
		 * @param width The entity bound width.
		 * @param height The entity bound height.
		 * @param margin The inset applied to the rectangle.
		 * @return The resolved rectangle.
		 */
		static components::Clip::Rect
			computeRect(const utility::graphic::PoseF &pose, float width,
						float height, float margin);
	};

}	 // namespace guillaume::systems
