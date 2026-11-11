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
	 * @brief A 3D model shown at the entity's pose: the model file and an
	 * optional texture.
	 *
	 * entities::Mesh (or the application) writes the two paths.
	 * systems::MeasureMesh (Measure phase) loads the model and writes the
	 * width and height of its bounding box into the entity's
	 * components::Bound. systems::MeshRender (Render phase) loads the
	 * model at the pose of the entity's components::Transform and uploads
	 * its meshes to the engine.
	 *
	 * @code
	 * registry.getComponent<components::Mesh>(id)
	 * 	.setMeshPath("models/chair.obj")
	 * 	.setTexturePath("models/chair.png");
	 * @endcode
	 *
	 * @note The Bound of the entity is overwritten every frame with the
	 * size of the model; setting it does not resize the model.
	 * @note This class is the model file reference. The geometry type the
	 * render systems build is `utility::graphic::Mesh`.
	 *
	 * @see systems::MeasureMesh, systems::MeshRender, entities::Mesh
	 */
	class Mesh: public ecs::Component
	{
		private:
		std::string _meshPath {};		///< Model file, resource path
		std::string _texturePath {};	///< Texture file; empty for none

		public:
		/**
		 * @brief Default constructor for the Mesh component.
		 */
		Mesh(void) = default;

		/**
		 * @brief Default destructor for the Mesh component.
		 */
		~Mesh(void) = default;

		/**
		 * @brief Choose the model to show.
		 *
		 * Marks the component changed only when the path differs.
		 *
		 * @param path The path of the model file, as understood by the
		 * application's resource provider.
		 * @return This component, for chaining.
		 */
		Mesh &setMeshPath(const std::string &path);

		/**
		 * @brief Choose the texture applied to the model.
		 *
		 * Marks the component changed only when the path differs.
		 *
		 * @param path The path of the texture file; empty to draw the
		 * model with the default material.
		 * @return This component, for chaining.
		 */
		Mesh &setTexturePath(const std::string &path);

		/**
		 * @brief Get the path of the model file.
		 * @return A copy of the path; empty by default.
		 */
		std::string getMeshPath(void) const;

		/**
		 * @brief Get the path of the texture file.
		 * @return A copy of the path; empty (the default) when the model
		 * has no texture.
		 */
		std::string getTexturePath(void) const;
	};
}	 // namespace guillaume::components
