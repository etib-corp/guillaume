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

#include <utility/graphic/position.hpp>
#include <utility/graphic/orientation.hpp>
#include <utility/graphic/scale.hpp>

#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component representing a transform in space.
	 */
	class Transform: public ecs::Component
	{
		private:
		utility::graphic::PositionF _position;	  ///< Position of the entity
		utility::graphic::OrientationF
			_orientation;					///< Orientation of the entity
		utility::graphic::ScaleF _scale;	///< Scale of the entity

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
		 * @brief Set the position of the entity.
		 * @param position The new position vector.
		 * @return Reference to this Transform component for chaining.
		 */
		Transform &setPosition(const utility::graphic::PositionF &position)
		{
			_position = position;
			return *this;
		}

		/**
		 * @brief Get the position of the entity.
		 * @return The position vector.
		 */
		utility::graphic::PositionF getPosition(void) const
		{
			return _position;
		}

		/**
		 * @brief Set the orientation of the entity.
		 * @param orientation The new orientation quaternion.
		 * @return Reference to this Transform component for chaining.
		 */
		Transform &
			setOrientation(const utility::graphic::OrientationF &orientation)
		{
			_orientation = orientation;
			return *this;
		}

		/**
		 * @brief Get the orientation of the entity.
		 * @return The orientation quaternion.
		 */
		utility::graphic::OrientationF getOrientation(void) const
		{
			return _orientation;
		}

		/**
		 * @brief Set the scale of the entity.
		 * @param scale The new scale vector.
		 * @return Reference to this Transform component for chaining.
		 */
		Transform &setScale(const utility::graphic::ScaleF &scale)
		{
			_scale = scale;
			return *this;
		}

		/**
		 * @brief Get the scale of the entity.
		 * @return The scale vector.
		 */
		utility::graphic::ScaleF getScale(void) const
		{
			return _scale;
		}
	};

}	 // namespace guillaume::components