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

#include <benchmark/benchmark.h>

#include <memory>
#include <vector>

#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/ecs/level_order_traveler.hpp>
#include <guillaume/ecs/reverse_level_order_traveler.hpp>
#include <guillaume/event/event_bus.hpp>

#include <utility/event/keyboard_event.hpp>

namespace
{

	class DummyEntity: public guillaume::ecs::Entity
	{
		public:
		void initialize(void) override
		{
		}
		void update(void) override
		{
		}
	};

	guillaume::ecs::EntityRegistryContainer
		buildRegistry(const std::size_t entityCount)
	{
		guillaume::ecs::EntityRegistryContainer registry;
		for (std::size_t i = 0; i < entityCount; ++i) {
			auto entity = std::make_shared<DummyEntity>();
			guillaume::ecs::Entity::Signature signature;
			signature.set(i % 8);
			entity->setSignature(signature);
			registry.addEntity(std::move(entity));
		}
		return registry;
	}

	void BM_BreadthFirstTraversal(benchmark::State &state)
	{
		const auto registry = buildRegistry(state.range(0));
		for (auto _: state) {
			auto entities = registry.getEntitiesBreadthFirst();
			benchmark::DoNotOptimize(entities);
		}
	}
	BENCHMARK(BM_BreadthFirstTraversal)->Range(8, 8 << 10);

	void BM_SignatureQuery(benchmark::State &state)
	{
		const auto registry = buildRegistry(state.range(0));
		guillaume::ecs::Entity::Signature signature;
		signature.set(3);
		for (auto _: state) {
			auto ids = registry.getEntityWithSignature(signature);
			benchmark::DoNotOptimize(ids);
		}
	}
	BENCHMARK(BM_SignatureQuery)->Range(8, 8 << 10);

	void BM_EventDispatch(benchmark::State &state)
	{
		guillaume::event::EventBus bus;
		bus.subscribe<utility::event::KeyboardEvent>(
			[](std::shared_ptr<utility::event::Event>) {});
		for (auto _: state) {
			auto event = std::make_shared<utility::event::KeyboardEvent>();
			bus.publish(std::move(event));
		}
	}
	BENCHMARK(BM_EventDispatch)->Threads(1);

}	 // namespace

BENCHMARK_MAIN();
