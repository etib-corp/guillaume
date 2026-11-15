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
#include <utility/graphic/scale.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Where an entity is in space: its pose (position and
	 * orientation) and its scale.
	 *
	 * The position is the top-left corner of the entity's components::Bound,
	 * in dp; X grows to the right and Y downwards. The render systems draw
	 * the bound from that corner, rotated by the orientation.
	 *
	 * Written by whoever places the entity: the application for a root
	 * entity, systems::Layout for the children listed in a
	 * components::Layout, and each widget system for the children it places
	 * itself. Read by every render system, by the hit-testing of the input
	 * systems and by systems::Clip.
	 *
	 * @code
	 * auto &transform = registry.getComponent<components::Transform>(id);
	 * auto pose = transform.getPose();
	 * pose.setPosition(utility::graphic::PositionF(16.0f, 24.0f, 0.0f));
	 * transform.setPose(pose);
	 * @endcode
	 *
	 * @note The pose of a child arranged by systems::Layout is overwritten
	 * every frame; move its parent instead. Such a child is also pushed
	 * towards the viewer, in steps of systems::Layout::LayerDepthStep, so
	 * it is drawn in front of its parent: its depth is not its parent's.
	 * @note No system of the library reads the scale: the render systems
	 * draw the bound at its own size.
	 *
	 * @see components::Bound, systems::Layout, systems::RectangleRender
	 */
	class Transform: public ecs::Component
	{
		private:
		utility::graphic::PoseF
			_pose {};	 ///< Top-left corner and orientation
		utility::graphic::ScaleF _scale {};	   ///< Scale; read by no system

		public:
		/**
		 * @brief Default constructor for the Transform component.
		 */
		Transform(void) = default;

		/**
		 * @brief Default destructor for the Transform component.
		 */
		~Transform(void) = default;

		/**
		 * @brief Move and orient the entity.
		 *
		 * Marks the component changed only when @p pose differs from the
		 * stored one.
		 *
		 * @param pose The position of the top-left corner of the bound, in
		 * dp, and the orientation.
		 * @return This component, for chaining.
		 */
		Transform &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Store a scale for the entity.
		 *
		 * Marks the component changed only when @p scale differs from the
		 * stored one. The systems of the library do not apply it.
		 *
		 * @param scale The scale factor per axis.
		 * @return This component, for chaining.
		 */
		Transform &setScale(const utility::graphic::ScaleF &scale);

		/**
		 * @brief Get where the entity is.
		 * @return A copy of the pose: the position of the top-left corner
		 * of the bound, in dp, and the orientation.
		 */
		utility::graphic::PoseF getPose(void) const;

		/**
		 * @brief Get the stored scale.
		 * @return A copy of the scale factor per axis.
		 */
		utility::graphic::ScaleF getScale(void) const;
	};

}	 // namespace guillaume::components