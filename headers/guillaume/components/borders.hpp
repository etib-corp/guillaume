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

#include <cmath>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component representing a borders.
	 */
	class Borders: public ecs::Component
	{
		public:
		using BorderRadius = float;	   ///< Type representing the border
									   ///< radius of the border

		private:
		BorderRadius
			_topLeftRadius;	   ///< Borders radius for the top-left corner
		BorderRadius
			_topRightRadius;	///< Borders radius for the top-right corner
		BorderRadius _bottomRightRadius;	///< Borders radius for the
											///< bottom-right corner
		BorderRadius
			_bottomLeftRadius;	  ///< Borders radius for the bottom-left corner

		public:
		/**
		 * @brief Constructor for the Borders component.
		 */
		explicit Borders(void);

		/**
		 * @brief Default destructor for the Borders component.
		 */
		~Borders(void) = default;

		/**
		 * @brief Get the border radius for the top-left corner.
		 * @return The border radius for the top-left corner.
		 */
		BorderRadius getTopLeftRadius(void) const;

		/**
		 * @brief Set the border radius for the top-left corner.
		 * @param radius The new border radius for the top-left corner.
		 * @return Reference to this Borders component for chaining.
		 */
		Borders &setTopLeftRadius(BorderRadius radius);

		/**
		 * @brief Get the border radius for the top-right corner.
		 * @return The border radius for the top-right corner.
		 */
		BorderRadius getTopRightRadius(void) const;

		/**
		 * @brief Set the border radius for the top-right corner.
		 * @param radius The new border radius for the top-right corner.
		 * @return Reference to this Borders component for chaining.
		 */
		Borders &setTopRightRadius(BorderRadius radius);

		/**
		 * @brief Get the border radius for the bottom-right corner.
		 * @return The border radius for the bottom-right corner.
		 */
		BorderRadius getBottomRightRadius(void) const;

		/**
		 * @brief Set the border radius for the bottom-right corner.
		 * @param radius The new border radius for the bottom-right corner.
		 * @return Reference to this Borders component for chaining.
		 */
		Borders &setBottomRightRadius(BorderRadius radius);

		/**
		 * @brief Get the border radius for the bottom-left corner.
		 * @return The border radius for the bottom-left corner.
		 */
		BorderRadius getBottomLeftRadius(void) const;

		/**
		 * @brief Set the border radius for the bottom-left corner.
		 * @param radius The new border radius for the bottom-left corner.
		 * @return Reference to this Borders component for chaining.
		 */
		Borders &setBottomLeftRadius(BorderRadius radius);

		/**
		 * @brief Set the same border radius for all corners.
		 * @param radius The new border radius to set for all corners.
		 * @return Reference to this Borders component for chaining.
		 */
		Borders &setBorderRadius(BorderRadius radius);
	};

}	 // namespace guillaume::components