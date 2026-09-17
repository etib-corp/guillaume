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

#include "test_application.hpp"

#include <memory>
#include <utility>

#include <utility/system_io/default_system_io.hpp>

namespace guillaume::tests
{
	namespace
	{
		class StubScene: public guillaume::Scene
		{
			public:
			using guillaume::Scene::Scene;
		};

		class StubEngine: public guillaume::Engine
		{
			public:
			StubEngine(void)
				: guillaume::Engine()
			{
			}
			~StubEngine(void) override = default;

			void clear(void) override
			{
			}
			void present(void) override
			{
			}
			size_t addMesh(const utility::graphic::Mesh &,
						   const std::string &) override
			{
				return 0u;
			}
			bool removeObject(size_t) override
			{
				return true;
			}
			utility::graphic::SizeF
				measureText(const utility::graphic::Text &) const override
			{
				return {};
			}
			size_t addText(utility::graphic::Text) override
			{
				return 0u;
			}
			size_t addModel(std::shared_ptr<utility::graphic::Model>) override
			{
				return 0u;
			}
			utility::graphic::ViewF getView(void) const override
			{
				return {};
			}
			void addScene(size_t) override
			{
			}
			void pollEvents(void) override
			{
			}
			void update(void) override
			{
			}
		};
	}	 // namespace

	TEST_F(TestApplication, ConstructsWithRessourceProvider)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene, StubScene> application(
			ressourceProvider);

		EXPECT_FALSE(application.shouldQuit());
	}

	TEST_F(TestApplication, SetEngineAcceptsAnEngine)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene, StubScene> application(
			ressourceProvider);

		auto engine = std::make_unique<StubEngine>();
		EXPECT_NO_THROW(application.setEngine(std::move(engine)));
	}

	TEST_F(TestApplication, LifecycleMethodsRunAfterEngineAttached)
	{
		utility::DefaultSystemIO systemIo;
		auto ressourceProvider =
			std::make_shared<utility::RessourceProvider>(systemIo);

		guillaume::Application<StubScene, StubScene> application(
			ressourceProvider);

		application.setEngine(std::make_unique<StubEngine>());

		EXPECT_NO_THROW(application.update());
		EXPECT_NO_THROW(application.clear());
		EXPECT_NO_THROW(application.present());
	}
}	 // namespace guillaume::tests
