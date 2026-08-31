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

#include <memory>

#include <guillaume/application.hpp>
#include <guillaume/scene.hpp>

#include <utility/ressource_provider.hpp>
#include <utility/system_io/default_system_io.hpp>

namespace
{

	/**
	 * @brief Minimal engine stub.
	 *
	 * Guillaume delegates rendering and event polling to an Engine
	 * implementation. Replace this with a real engine for your platform to
	 * render actual content.
	 */
	class NoopEngine: public guillaume::Engine
	{
		public:
		void clear(void) override
		{
		}
		void present(void) override
		{
		}
		size_t addMesh(const utility::graphic::Mesh &,
					   const std::string &) override
		{
			return 0;
		}
		bool removeObject(size_t) override
		{
			return true;
		}
		utility::graphic::SizeF
			measureText(const utility::graphic::Text &) const override
		{
			return { 0.0f, 0.0f };
		}
		size_t addText(utility::graphic::Text) override
		{
			return 0;
		}
		size_t addModel(std::shared_ptr<utility::graphic::Model>) override
		{
			return 0;
		}
		utility::graphic::ViewF getView(void) const override
		{
			return utility::graphic::ViewF();
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

	/**
	 * @brief A minimal scene.
	 *
	 * Build your UI in onEnter() by registering components and adding entities
	 * to the entity registry.
	 */
	class HelloWorldScene: public guillaume::Scene
	{
		public:
		using guillaume::Scene::Scene;

		void onEnter(void) override
		{
			// Build entities here.
		}
	};

	/**
	 * @brief A second scene used to satisfy the Application template constraint
	 * (the default scene must be one of the registered scene types).
	 */
	class SecondaryScene: public guillaume::Scene
	{
		public:
		using guillaume::Scene::Scene;

		void onEnter(void) override
		{
		}
	};

}	 // namespace

int main(void)
{
	utility::DefaultSystemIO systemIo;
	auto resources = std::make_shared<utility::RessourceProvider>(systemIo);

	guillaume::Application<HelloWorldScene, HelloWorldScene, SecondaryScene>
		app(resources);
	app.setEngine(std::make_unique<NoopEngine>());

	return app.run();
}
