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
#include "guillaume/entities/layout.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/entities/app_bar.hpp"
#include "guillaume/entities/badge.hpp"
#include "guillaume/entities/bottom_sheet.hpp"
#include "guillaume/entities/card.hpp"
#include "guillaume/entities/carousel.hpp"
#include "guillaume/entities/checkbox.hpp"
#include "guillaume/entities/chip.hpp"
#include "guillaume/entities/date_picker.hpp"
#include "guillaume/entities/dialog.hpp"
#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/list.hpp"
#include "guillaume/entities/loading_indicator.hpp"
#include "guillaume/entities/menu.hpp"
#include "guillaume/entities/model.hpp"
#include "guillaume/entities/image.hpp"
#include "guillaume/entities/navigation_bar.hpp"
#include "guillaume/entities/navigation_drawer.hpp"
#include "guillaume/entities/navigation_rail.hpp"
#include "guillaume/entities/progress_indicator.hpp"
#include "guillaume/entities/radio_button.hpp"
#include "guillaume/entities/search.hpp"
#include "guillaume/entities/side_sheet.hpp"
#include "guillaume/entities/slider.hpp"
#include "guillaume/entities/snackbar.hpp"
#include "guillaume/entities/switch.hpp"
#include "guillaume/entities/tabs.hpp"
#include "guillaume/entities/text_field.hpp"
#include "guillaume/entities/time_picker.hpp"
#include "guillaume/entities/toolbar.hpp"
#include "guillaume/entities/tooltip.hpp"

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
				  entities::Layout::Builder, entities::Text::Builder,
				  entities::Button::Builder, entities::Icon::Builder,
				  entities::Model::Builder, entities::Image::Builder,
				  entities::Divider::Builder, entities::Card::Builder,
				  entities::Badge::Builder, entities::Toolbar::Builder,
				  entities::AppBar::Builder, entities::Tooltip::Builder,
				  entities::Snackbar::Builder, entities::List::Builder,
				  entities::Checkbox::Builder, entities::RadioButton::Builder,
				  entities::Switch::Builder, entities::Chip::Builder,
				  entities::Tabs::Builder, entities::Dialog::Builder,
				  entities::BottomSheet::Builder, entities::SideSheet::Builder,
				  entities::Menu::Builder, entities::NavigationBar::Builder,
				  entities::NavigationDrawer::Builder,
				  entities::NavigationRail::Builder, entities::Search::Builder,
				  entities::TextField::Builder,
				  entities::ProgressIndicator::Builder,
				  entities::LoadingIndicator::Builder,
				  entities::Slider::Builder, entities::Carousel::Builder,
				  entities::DatePicker::Builder,
				  entities::TimePicker::Builder>>(_componentRegistry, *this))
		, _entityDirectorManager(
			  std::make_unique<ecs::EntityDirectorManagerFiller<
				  entities::Layout::Director, entities::Text::Director,
				  entities::Button::Director, entities::Icon::Director,
				  entities::Model::Director, entities::Image::Director,
				  entities::Divider::Director, entities::Card::Director,
				  entities::Badge::Director, entities::Toolbar::Director,
				  entities::AppBar::Director, entities::Tooltip::Director,
				  entities::Snackbar::Director, entities::List::Director,
				  entities::Checkbox::Director, entities::RadioButton::Director,
				  entities::Switch::Director, entities::Chip::Director,
				  entities::Tabs::Director, entities::Dialog::Director,
				  entities::BottomSheet::Director,
				  entities::SideSheet::Director, entities::Menu::Director,
				  entities::NavigationBar::Director,
				  entities::NavigationDrawer::Director,
				  entities::NavigationRail::Director,
				  entities::Search::Director, entities::TextField::Director,
				  entities::ProgressIndicator::Director,
				  entities::LoadingIndicator::Director,
				  entities::Slider::Director, entities::Carousel::Director,
				  entities::DatePicker::Director,
				  entities::TimePicker::Director>>())
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
