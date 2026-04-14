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

#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component representing a bounding rectangle.
	 */
	class Bound: public ecs::Component
	{
		private:
		std::size_t _width { 0 };	  ///< Width of the bounding rectangle
		std::size_t _height { 0 };	  ///< Height of the bounding rectangle

		public:
		/**
		 * @brief Default constructor for the Bound component.
		 */
		Bound(void) = default;

		/**
		 * @brief Default destructor for the Bound component.
		 */
		~Bound(void) = default;

		/**
		 * @brief Set the width of the bounding rectangle.
		 * @param width The new width of the bounding rectangle.
		 * @return Reference to this Bound for chaining.
		 */
		Bound &setWidth(std::size_t width)
		{
			if (_width == width) {
				return *this;
			}
			_width = width;
			setHasChanged(true);
			return *this;
		}

		/**
		 * @brief Get the width of the bounding rectangle.
		 * @return The width of the bounding rectangle.
		 */
		std::size_t getWidth(void) const
		{
			return _width;
		}

		/**
		 * @brief Set the height of the bounding rectangle.
		 * @param height The new height of the bounding rectangle.
		 * @return Reference to this Bound for chaining.
		 */
		Bound &setHeight(std::size_t height)
		{
			if (_height == height) {
				return *this;
			}
			_height = height;
			setHasChanged(true);
			return *this;
		}

		/**
		 * @brief Get the height of the bounding rectangle.
		 * @return The height of the bounding rectangle.
		 */
		std::size_t getHeight(void) const
		{
			return _height;
		}
	};

}	 // namespace guillaume::components