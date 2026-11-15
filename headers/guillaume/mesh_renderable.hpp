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
#include <vector>

#include <utility/graphic/mesh.hpp>
#include <utility/graphic/renderable.hpp>

namespace guillaume
{
	/**
	 * @brief What a render system hands to the engine to draw geometry: a
	 * list of meshes and the name of the material to draw them with.
	 *
	 * The render systems (systems::RectangleRender, systems::ImageRender,
	 * systems::MeshRender, ...) build the meshes of an entity on the CPU
	 * with the vertices already in world coordinates, wrap them in a
	 * MeshRenderable, and pass it to `utility::Engine::createObject` or
	 * `updateObject`. The material name tells the engine how to shade them:
	 * `"mesh_material"` for flat colored shapes, `"image_<path>"` for a
	 * textured one.
	 *
	 * The renderable types of the `utility` library each have a fixed
	 * material name; this class exists to choose it per object.
	 *
	 * @note Because the vertices are in world coordinates, the pose of the
	 * renderable is the identity. Its color is left default-constructed.
	 * @see TextRenderable
	 */
	class MeshRenderable: public utility::graphic::Renderable
	{
		private:
		std::string _materialName;	  ///< Name of the material the engine
									  ///< draws the meshes with

		public:
		/**
		 * @brief Create a renderable from meshes and a material name.
		 * @param meshes The meshes to draw, with vertices in world
		 * coordinates. Each one is copied into the renderable.
		 * @param materialName The name of the material the engine should
		 * use, for instance `"mesh_material"`.
		 */
		MeshRenderable(std::vector<utility::graphic::Mesh> meshes,
					   std::string materialName);

		/**
		 * @brief Default destructor.
		 */
		~MeshRenderable(void) override = default;

		/**
		 * @brief Tell the engine which kind of renderable this is.
		 * @return Always `RenderableType::Mesh`.
		 */
		utility::graphic::Renderable::RenderableType
			getRenderableType(void) const override;

		/**
		 * @brief Get the name of the material the engine draws the meshes
		 * with.
		 * @return The name given to the constructor.
		 */
		std::string getMaterialName(void) const override;
	};

}	 // namespace guillaume
