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
	 * @brief Renderable wrapper that exposes an explicit material name.
	 *
	 * Guillaume builds CPU-space meshes (rectangles, images, models) and needs
	 * to associate each one with a specific material. The utility renderable
	 * types hardcode their material name, so this local subclass carries the
	 * requested material name alongside its meshes.
	 */
	class MeshRenderable: public utility::graphic::Renderable
	{
		private:
		std::string _materialName;	  ///< Material name used to render the mesh

		public:
		/**
		 * @brief Construct a mesh renderable with a material name.
		 * @param meshes The meshes to render.
		 * @param materialName The name of the material to use.
		 */
		MeshRenderable(std::vector<utility::graphic::Mesh> meshes,
					   std::string materialName);

		/**
		 * @brief Default destructor.
		 */
		~MeshRenderable(void) override = default;

		/**
		 * @brief Get the type of this renderable object.
		 * @return RenderableType::Mesh.
		 */
		utility::graphic::Renderable::RenderableType
			getRenderableType(void) const override;

		/**
		 * @brief Get the material name used to render the mesh.
		 * @return The configured material name.
		 */
		std::string getMaterialName(void) const override;
	};

}	 // namespace guillaume
