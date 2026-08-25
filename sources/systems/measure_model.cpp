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
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <utility/graphic/model.hpp>
#include <utility/ressource_provider.hpp>

#include "guillaume/systems/measure_model.hpp"

namespace guillaume::systems
{
	MeasureModel::MeasureModel(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Model, components::Bound,
							components::Transform>(ecs::Phase::Measure)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
	{
	}

	MeasureModel::~MeasureModel(void)
	{
	}

	void MeasureModel::prepare(void)
	{
	}

	void MeasureModel::cleanup(void)
	{
	}

	void MeasureModel::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating MeasureModel system for entity " << entityIdentifier;
		if (!requireComponent<components::Model>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)) {
			return;
		}

		const auto &modelComponent =
			getComponent<components::Model>(entityIdentifier);
		auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);

		MeasureModelCacheKey cacheKey { modelComponent.getModelPath() };

		getLogger().debug() << "Measuring model '" << cacheKey.modelPath
							<< "' for entity " << entityIdentifier;

		if (contains(cacheKey)) {
			getLogger().debug() << "Model '" << cacheKey.modelPath
								<< "' already measured, skipping.";
			const auto &cacheValue = get(cacheKey);
			if (cacheValue.has_value()) {
				getComponent<components::Bound>(entityIdentifier)
					.setWidth(cacheValue->getWidth())
					.setHeight(cacheValue->getHeight());
			}
			return;
		}

		auto model		 = _ressourceProvider->loadModel(cacheKey.modelPath);
		auto boundingBox = model->computeBoundingSize();

		getComponent<components::Bound>(entityIdentifier)
			.setWidth(boundingBox.getWidth())
			.setHeight(boundingBox.getHeight());

		put(cacheKey, boundingBox);
	}

}	 // namespace guillaume::systems
