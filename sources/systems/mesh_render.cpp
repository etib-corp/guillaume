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

#include <vector>

#include <utility/graphic/model.hpp>

#include "guillaume/systems/mesh_render.hpp"

#include "guillaume/systems/shape_mesh.hpp"
#include "guillaume/mesh_renderable.hpp"

namespace guillaume::systems
{
	MeshRender::MeshRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		utility::Engine &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Mesh>(ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
	{
	}

	MeshRender::~MeshRender(void)
	{
		clear();
	}

	void MeshRender::prepare(void)
	{
		markAllUnused();
	}

	void MeshRender::cleanup(void)
	{
		removeUnused(_engine);
	}

	void MeshRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating MeshRender system for entity " << entityIdentifier;
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Mesh>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &modelComponent =
			getComponent<components::Mesh>(entityIdentifier);

		const std::string &meshPath	   = modelComponent.getMeshPath();
		const std::string &texturePath = modelComponent.getTexturePath();
		const auto &pose			   = transformComponent.getPose();

		getLogger().debug() << "Rendering model '" << meshPath
							<< "' for entity " << entityIdentifier;

		auto model = texturePath.empty()
			? _ressourceProvider->loadModel(meshPath, pose)
			: _ressourceProvider->loadModel(meshPath, pose, texturePath);

		if (!model) {
			getLogger().warning() << "Failed to load model '" << meshPath
								  << "' for entity " << entityIdentifier;
			return;
		}

		// The model's vertices are in its own local units, around its own
		// origin, and MeshRenderable draws vertices as given: move them into
		// the entity's box. The model is scaled to its fit size (see
		// components::Mesh::setFitSize), centred in the Bound, and rotated
		// with the entity, like the other renderers do.
		utility::graphic::PositionF localMin(0.0f, 0.0f, 0.0f);
		utility::graphic::PositionF localMax(0.0f, 0.0f, 0.0f);
		bool first = true;
		for (const auto &mesh: model->getMeshes()) {
			for (const auto &vertex: mesh->getVertices()) {
				const auto position = vertex.getPosition();
				for (std::size_t axis = 0; axis < 3; ++axis) {
					if (first || position[axis] < localMin[axis]) {
						localMin[axis] = position[axis];
					}
					if (first || position[axis] > localMax[axis]) {
						localMax[axis] = position[axis];
					}
				}
				first = false;
			}
		}
		const float scale = modelComponent.scaleFor(localMax[0] - localMin[0],
													localMax[1] - localMin[1]);
		const auto &bound = getComponent<components::Bound>(entityIdentifier);
		// The model has depth: move its centre toward the viewer by half its
		// depth, so it sits in front of the surface it is laid out on
		// instead of half inside it.
		const float halfDepth = (localMax[2] - localMin[2]) * scale / 2.0f;
		const auto towardViewer =
			-pose.getOrientation().getForward() * (halfDepth + 1.0f);
		const utility::graphic::PositionF center(
			pose.getPosition().x + bound.getWidth() / 2.0f + towardViewer[0],
			pose.getPosition().y - bound.getHeight() / 2.0f + towardViewer[1],
			pose.getPosition().z + towardViewer[2]);

		std::vector<utility::graphic::Mesh> meshes;
		meshes.reserve(model->getMeshes().size());
		for (const auto &mesh: model->getMeshes()) {
			std::vector<utility::graphic::VertexF> vertices;
			vertices.reserve(mesh->getVertices().size());
			for (auto vertex: mesh->getVertices()) {
				const auto position = vertex.getPosition();
				const utility::graphic::PositionF local(
					(position[0] - (localMin[0] + localMax[0]) / 2.0f) * scale,
					(position[1] - (localMin[1] + localMax[1]) / 2.0f) * scale,
					(position[2] - (localMin[2] + localMax[2]) / 2.0f) * scale);
				const auto rotated = shape::rotatePositionByQuaternion(
					local, pose.getOrientation());
				vertex.setPosition(utility::graphic::PositionF(
					center[0] + rotated[0], center[1] + rotated[1],
					center[2] + rotated[2]));
				vertices.push_back(vertex);
			}
			meshes.emplace_back(vertices, mesh->getIndices());
		}

		auto renderable = std::make_shared<MeshRenderable>(
			std::move(meshes),
			texturePath.empty() ? "default_material" : "image_" + texturePath);

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine.createObject(renderable);
			insert(entityIdentifier,
				   RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine.updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine.createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems
