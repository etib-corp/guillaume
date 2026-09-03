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

#include <string>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief Component representing a 2D image attached to an entity.
	 */
	class Image: public ecs::Component
	{
		private:
		std::string _texturePath {};	///< Path to the image texture file

		public:
		/**
		 * @brief Default constructor for the Image component.
		 */
		Image(void) = default;

		/**
		 * @brief Default destructor for the Image component.
		 */
		~Image(void) = default;

		/**
		 * @brief Set the image texture path.
		 * @param path The new image texture path.
		 * @return Reference to this Image component for chaining.
		 */
		Image &setTexturePath(const std::string &path);

		/**
		 * @brief Get the image texture path.
		 * @return The image texture path.
		 */
		std::string getTexturePath(void) const;
	};
}	 // namespace guillaume::components
