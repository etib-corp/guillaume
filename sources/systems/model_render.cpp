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

#include "guillaume/systems/model_render.hpp"
#include "guillaume/mesh_renderable.hpp"

namespace guillaume::systems
{
	ModelRender::ModelRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Model>(ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
	{
	}

	ModelRender::~ModelRender(void)
	{
		clear();
	}

	void ModelRender::prepare(void)
	{
		markAllUnused();
	}

	void ModelRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void ModelRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating ModelRender system for entity " << entityIdentifier;
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Model>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &modelComponent =
			getComponent<components::Model>(entityIdentifier);

		const std::string &modelPath	= modelComponent.getModelPath();
		const std::string &texturePath	= modelComponent.getTexturePath();
		const auto &pose				= transformComponent.getPose();

		getLogger().debug() << "Rendering model '" << modelPath
							<< "' for entity " << entityIdentifier;

		auto model = texturePath.empty()
			? _ressourceProvider->loadModel(modelPath, pose)
			: _ressourceProvider->loadModel(modelPath, pose, texturePath);

		if (!model) {
			getLogger().warning()
				<< "Failed to load model '" << modelPath << "' for entity "
				<< entityIdentifier;
			return;
		}

		std::vector<utility::graphic::Mesh> meshes;
		meshes.reserve(model->getMeshes().size());
		for (const auto &mesh: model->getMeshes()) {
			meshes.push_back(*mesh);
		}

		auto renderable = std::make_shared<MeshRenderable>(
			std::move(meshes),
			texturePath.empty() ? "default_material" : "image_" + texturePath);

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine->createObject(renderable);
			insert(entityIdentifier, RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	  = true;
		}
	}

}	 // namespace guillaume::systems
