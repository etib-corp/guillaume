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
#include "guillaume/components/hierarchy.hpp"
#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

#include <utility/ressource_provider.hpp>

#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/ecs/system_phase.hpp>
#include <guillaume/ecs/system_registry.hpp>
#include <guillaume/event/event_bus.hpp>
#include <guillaume/systems/core_systems.hpp>

#include "mocks/engine_mock.hpp"
#include "mocks/fake_system_io.hpp"

namespace guillaume::tests
{

	/**
	 * @brief Fixture running the real frame loop on a mock engine: the core
	 * systems, phase by phase, on a registry the test fills with widgets.
	 *
	 * It mirrors Application::routine but leaves the changed flags in place
	 * until the test has looked at them.
	 */
	class TestFrameLoop: public ::testing::Test
	{
		protected:
		EngineMock _engine;		   ///< Mock engine.
		FakeSystemIO _systemIo;	   ///< No files on disk.
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;					   ///< Resources.
		event::EventBus _eventBus;				   ///< Event dispatch.
		ecs::SystemRegistry _systemRegistry;	   ///< Core systems.
		ecs::SystemPhaseList _phases;			   ///< Phases, in order.
		ecs::ComponentRegistry _registry;		   ///< Component storage.
		ecs::EntityRegistryContainer _entities;	   ///< Scene entities.

		void SetUp(void) override
		{
			// Give every text a size, so labels take part in the layout.
			_engine.textMeasurement = { 40.0f, 20.0f };
			_ressourceProvider =
				std::make_shared<utility::RessourceProvider>(_systemIo);
			systems::registerCoreSystems(_systemRegistry, _eventBus, _engine,
										 _ressourceProvider);
		}

		/**
		 * @brief Build a widget with its builder's defaults and register it.
		 * @tparam EntityType The widget class.
		 * @return The widget.
		 */
		template<typename EntityType> std::shared_ptr<EntityType> build(void)
		{
			typename EntityType::Builder builder(_registry, _entities);

			return builder.registerEntity(nullptr);
		}

		/**
		 * @brief Run every phase once, without resetting the changed flags.
		 */
		void runPhases(void)
		{
			std::apply(
				[this](auto &...phase) {
					(this->runPhase(phase), ...);
				},
				_phases);
		}

		/**
		 * @brief Run one full frame, as Application::routine does.
		 */
		void runFrame(void)
		{
			runPhases();
			_registry.resetChangedFlags();
		}

		/**
		 * @brief List the entities a frame left marked as changed.
		 * @return Their identifiers.
		 */
		std::vector<ecs::Entity::Identifier> changedEntities(void)
		{
			std::vector<ecs::Entity::Identifier> changed;

			for (const auto &entity: _entities.getEntitiesBreadthFirst()) {
				if (_registry.hasChanged(entity->getIdentifier())) {
					changed.push_back(entity->getIdentifier());
				}
			}
			return changed;
		}

		private:
		/**
		 * @brief Run the systems of one phase.
		 * @tparam PhaseType The phase definition type.
		 * @param phase The phase definition.
		 */
		template<typename PhaseType> void runPhase(PhaseType &phase)
		{
			for (const auto &system:
				 _systemRegistry.getSystemsByPhase(phase.getPhase())) {
				system->routine(_registry, _entities, phase.getTraveler());
			}
		}
	};

}	 // namespace guillaume::tests
