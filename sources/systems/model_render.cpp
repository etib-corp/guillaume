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

#include <utility/graphic/model.hpp>

#include "guillaume/systems/model_render.hpp"

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
		apply([this](const ModelRenderCacheKey &key,
					 ModelRenderCacheEntry &entry) {
			entry.used = false;
		});
	}

	void ModelRender::cleanup(void)
	{
		erase_if([this](const ModelRenderCacheKey &key,
						const ModelRenderCacheEntry &entry) {
			if (!entry.used) {
				_engine->removeObject(entry.value);
				return true;
			}
			return false;
		});
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
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &modelComponent =
			getComponent<components::Model>(entityIdentifier);

		ModelRenderCacheKey cacheKey {
			modelComponent.getModelPath(),
			modelComponent.getTexturePath(),
			transformComponent.getPose(),
			transformComponent.getScale(),
		};

		getLogger().debug() << "Rendering model '" << cacheKey.modelPath
							<< "' for entity " << entityIdentifier;
		getLogger().debug()
			<< "Model pose: " << cacheKey.pose << ", scale: " << cacheKey.scale;

		if (const auto &entry = get(cacheKey); entry.has_value()) {
			ModelRenderCacheEntry newEntry { .used	= true,
											 .value = entry->value };
			put(cacheKey, std::move(newEntry));
			getLogger().debug() << "Cache hit for entity " << entityIdentifier;
			return;
		}

		auto model		= cacheKey.texturePath.empty()
			? _ressourceProvider->loadModel(cacheKey.modelPath, cacheKey.pose)
			: _ressourceProvider->loadModel(cacheKey.modelPath,
										    cacheKey.pose,
										    cacheKey.texturePath);
		auto identifier = _engine->addModel(std::move(model));

		ModelRenderCacheEntry cacheEntry { .used = true, .value = identifier };
		put(cacheKey, std::move(cacheEntry));
	}

}	 // namespace guillaume::systems
