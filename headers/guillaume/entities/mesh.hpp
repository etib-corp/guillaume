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

#include <utility>
#include <memory>
#include <optional>
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/mesh.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A 3D model loaded from a file, with an optional texture.
	 *
	 * Mesh is a leaf widget: it has no children. Its state lives in
	 * components::Mesh (model path, texture path) and
	 * components::Transform. Each frame systems::MeasureMesh writes the
	 * width and height of the model's bounding box into components::Bound,
	 * in the Measure phase, and systems::MeshRender draws the model at the
	 * Transform pose, in the Render phase. The class itself holds no field.
	 *
	 * @code
	 * Mesh::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto model = builder.makeMesh(parent, "models/chair.obj",
	 * 	"textures/wood.png");
	 * @endcode
	 *
	 * `Mesh::create(registry, meshPath)` builds the same widget without
	 * registering it in an entity registry; call setTexturePath() afterwards
	 * to give it a texture.
	 *
	 * Watch out: the Bound is overwritten every frame with the size of the
	 * model, so a mesh cannot be resized through its Bound.
	 *
	 * @see components::Mesh, systems::MeasureMesh, systems::MeshRender
	 */
	class Mesh:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Mesh>
	{
		public:
		/**
		 * @brief Configures Mesh widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Mesh>
		{
			private:
			std::string _meshPath;		 ///< Path of the model file to draw.
			std::string _texturePath;	 ///< Path of the texture file applied
										 ///< to the model (empty: none).

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built meshes are stored in.
			 * @param entityRegistry The registry that owns the meshes built
			 * by `makeMesh`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a mesh from the current configuration, without
			 * registering it or giving it a parent.
			 * @return The new mesh.
			 */
			std::shared_ptr<Mesh> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: empty model path and empty
			 * texture path.
			 */
			void reset(void) override;

			/**
			 * @brief Choose the model to draw.
			 * @param meshPath The path of the model file, as the resource
			 * provider of the application resolves it. Empty by default.
			 * @return Reference to this builder for chaining.
			 * @see components::Mesh::getMeshPath
			 */
			Builder &withMeshPath(const std::string &meshPath);

			/**
			 * @brief Choose the texture applied to the model.
			 * @param texturePath The path of the texture file. Empty by
			 * default: the model is then drawn with the default material.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withTexturePath(const std::string &texturePath);

			/**
			 * @brief Build a mesh, register it under `parent` and reset the
			 * builder.
			 * @param parent The entity the mesh is attached to, or nullptr
			 * for a root entity.
			 * @param meshPath The path of the model file to draw.
			 * @param texturePath The path of the texture file applied to
			 * the model, or an empty string for the default material.
			 * @return The registered mesh.
			 */
			std::shared_ptr<Mesh> makeMesh(std::shared_ptr<Entity> parent,
										   const std::string &meshPath,
										   const std::string &texturePath = "");
		};

		public:
		/**
		 * @brief Build a mesh without registering it in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, meshPath)`. A mesh has no children to build.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new mesh. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Mesh> create(Arguments &&...arguments)
		{
			std::shared_ptr<Mesh> entity(
				new Mesh(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Mesh(void);

		/**
		 * @brief Change the model to draw.
		 *
		 * The Bound follows at the next Measure phase.
		 * @param meshPath The path of the new model file, written to
		 * components::Mesh.
		 * @return Reference to this mesh for chaining.
		 */
		Mesh &setMeshPath(const std::string &meshPath);

		/**
		 * @brief Change the texture applied to the model.
		 * @param texturePath The path of the new texture file, written to
		 * components::Mesh, or an empty string for the default material.
		 * @return Reference to this mesh for chaining.
		 */
		Mesh &setTexturePath(const std::string &texturePath);

		/**
		 * @brief Scale the model so its longest side has this length.
		 * @param size The length, in dp, or nothing to draw the model in its
		 * file's own units.
		 * @return Reference to this mesh for chaining.
		 * @see components::Mesh::setFitSize
		 */
		Mesh &setFitSize(std::optional<float> size);

		private:
		/**
		 * @brief Construct a mesh without a texture.
		 * @param registry The registry the components are stored in.
		 * @param meshPath The path of the model file to draw.
		 * @see components::Mesh::getMeshPath
		 */
		Mesh(ecs::ComponentRegistry &registry, const std::string &meshPath);
	};

}	 // namespace guillaume::entities
