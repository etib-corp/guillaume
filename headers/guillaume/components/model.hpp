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
	 * @brief Component representing a 3D model attached to an entity.
	 */
	class Model: public ecs::Component
	{
		private:
		std::string _modelPath {};		///< Path to the model file
		std::string _texturePath {};	///< Path to the texture file

		public:
		/**
		 * @brief Default constructor for the Model component.
		 */
		Model(void) = default;

		/**
		 * @brief Default destructor for the Model component.
		 */
		~Model(void) = default;

		/**
		 * @brief Set the model path.
		 * @param path The new model path.
		 * @return Reference to this Model component for chaining.
		 */
		Model &setModelPath(const std::string &path);

		/**
		 * @brief Set the texture path.
		 * @param path The new texture path.
		 * @return Reference to this Model component for chaining.
		 */
		Model &setTexturePath(const std::string &path);

		/**
		 * @brief Get the model path.
		 * @return The model path.
		 */
		std::string getModelPath(void) const;

		/**
		 * @brief Get the texture path.
		 * @return The texture path.
		 */
		std::string getTexturePath(void) const;
	};
}	 // namespace guillaume::components
