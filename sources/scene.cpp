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

#include "guillaume/scene.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/entities/panel.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/model.hpp"

namespace guillaume
{
	Scene::Scene(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
				 LocalStorage &localStorage, SessionStorage &sessionStorage)
		: _ressourceProvider(ressourceProvider)
		, _localStorage(localStorage)
		, _sessionStorage(sessionStorage)
		, _componentRegistry()
		, _entityBuilderManager(
			  std::make_unique<ecs::EntityBuilderManagerFiller<
				  entities::Panel::Builder, entities::Text::Builder,
				  entities::Button::Builder, entities::Icon::Builder,
				  entities::Model::Builder>>(_componentRegistry, *this))
		, _entityDirectorManager(
			  std::make_unique<ecs::EntityDirectorManagerFiller<
				  entities::Panel::Director, entities::Text::Director,
				  entities::Button::Director, entities::Icon::Director,
				  entities::Model::Director>>())
		, _nextSceneType(typeid(void))
	{
		getLogger().info()
			<< "Scene initialized with default entity builders and "
			   "directors";
	}

	Scene::~Scene(void)
	{
		getLogger().info() << "Scene destroyed with "
						   << getEntitiesBreadthFirst().size()
						   << " entity/entities in hierarchy";
	}

	std::shared_ptr<utility::RessourceProvider>
		Scene::getRessourceProvider(void)
	{
		return _ressourceProvider;
	}

	ecs::EntityBuilderManager &Scene::getBuilderManager(void)
	{
		return *_entityBuilderManager;
	}

	ecs::EntityDirectorManager &Scene::getDirectorManager(void)
	{
		return *_entityDirectorManager;
	}

	ecs::ComponentRegistry &Scene::getComponentRegistry(void)
	{
		return _componentRegistry;
	}

	void Scene::addRootEntity(const std::string &name,
							  std::shared_ptr<ecs::Entity> entity)
	{
		if (_rootEntities.find(name) != _rootEntities.end()) {
			getLogger().warning()
				<< "Root entity with name '" << name
				<< "' already exists. Overwriting the existing entity.";
			throw std::runtime_error("Root entity with name '" + name
									 + "' already exists.");
		}
		_rootEntities[name] = entity;
	}

	ecs::EntityRegistry &Scene::getEntityRegistry(void)
	{
		return *this;
	}

	bool Scene::wantsToSwitch(void) const noexcept
	{
		return _nextSceneType != typeid(void);
	}

	std::type_index Scene::getNextSceneType(void) const noexcept
	{
		return _nextSceneType;
	}

	void Scene::clearNextScene(void) noexcept
	{
		_nextSceneType = typeid(void);
	}

	void Scene::placeEntitiesInFrontOfView(const utility::graphic::ViewF &view)
	{
		constexpr float distance = 300.0f;
		constexpr float gap		 = 50.0f;

		const auto centerRay = view.centerRay();
		const auto origin	 = centerRay.origin();
		const auto forward	 = centerRay.direction();
		const auto right	 = view.getRight();

		const auto anchor		   = origin + forward * distance;
		const auto &directEntities = accessDirectEntities();

		std::vector<const ecs::Entity *> placeableEntities;
		placeableEntities.reserve(directEntities.size());

		float totalSpread = 0.0f;

		for (const auto &entity: directEntities) {
			const auto id = entity->getIdentifier();

			if (!_componentRegistry.hasComponent<components::Bound>(id)
				|| !_componentRegistry.hasComponent<components::Transform>(
					id)) {
				continue;
			}

			placeableEntities.push_back(entity.get());

			const auto &bound =
				_componentRegistry.getComponent<components::Bound>(id);

			totalSpread += bound.getWidth();
		}

		if (placeableEntities.empty()) {
			return;
		}

		totalSpread += static_cast<float>(placeableEntities.size() - 1) * gap;

		float cursor = -totalSpread / 2.0f;

		for (const auto *entity: placeableEntities) {
			const auto id = entity->getIdentifier();

			const auto &bound =
				_componentRegistry.getComponent<components::Bound>(id);

			const float width		 = bound.getWidth();
			const float centerOffset = cursor + width * 0.5f;

			utility::graphic::PositionF entityPosition(anchor
													   + right * centerOffset);

			auto &transform =
				_componentRegistry.getComponent<components::Transform>(id);

			auto pose = transform.getPose();
			pose.setPosition(entityPosition);
			pose.setOrientation(view.getPose().getOrientation());
			transform.setPose(pose);

			cursor += width + gap;
		}
	}

	void Scene::onEnter(void)
	{
		getLogger().info() << "Scene entered";
	}

	void Scene::onExit(void)
	{
		getLogger().info() << "Scene exited";
	}

}	 // namespace guillaume
