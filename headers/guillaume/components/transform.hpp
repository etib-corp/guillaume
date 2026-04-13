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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component representing a transform in space.
	 */
	class Transform: public ecs::Component
	{
		private:
		utility::graphic::PoseF _pose {};  ///< Pose of the entity

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
		 * @brief Set the pose of the entity.
		 * @param pose The new pose.
		 * @return Reference to this Transform component for chaining.
		 */
		Transform &setPose(const utility::graphic::PoseF &pose)
		{
			if (_pose == pose) {
				return *this;
			}
			_pose = pose;
			setHasChanged(true);
			return *this;
		}

		/**
		 * @brief Get the pose of the entity.
		 * @return The pose.
		 */
		utility::graphic::PoseF getPose(void) const
		{
			return _pose;
		}
	};

}	 // namespace guillaume::components