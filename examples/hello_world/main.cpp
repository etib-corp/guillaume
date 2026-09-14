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
#include <stdexcept>
#include <string>

#include <evan/Engine.hpp>
#include <evan/IPlatform.hpp>

#if defined(__APPLE__)
	#include <evan/glfw/platform/MacOsDesktopPlatform.hpp>
#elif defined(__linux__)
	#include <evan/glfw/platform/LinuxDesktopPlatform.hpp>
#elif defined(_WIN32)
	#include <evan/glfw/platform/WindowsPlatform.hpp>
#endif

#include <guillaume/application.hpp>
#include <guillaume/scene.hpp>

#include <utility/ressource_provider.hpp>
#include <utility/system_io/default_system_io.hpp>

namespace
{

	/**
	 * @brief Evan-backed `guillaume::Engine` implementation.
	 *
	 * Guillaume delegates rendering and event polling to an `Engine`
	 * implementation. This example uses the Evan Vulkan engine, so real
	 * content is drawn instead of a no-op stub.
	 */
	class EvanEngine: public guillaume::Engine
	{
		private:
		std::unique_ptr<evan::Engine> _evanEngine;	  ///< Evan graphics engine

		public:
		EvanEngine(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::shared_ptr<evan::IPlatform> platform)
			: guillaume::Engine()
			, _evanEngine(
				  std::make_unique<evan::Engine>(ressourceProvider, platform))
		{
		}

		~EvanEngine(void) override = default;

		void clear(void) override
		{
		}

		void present(void) override
		{
			if (_evanEngine) {
				_evanEngine->render();
			}
		}

		size_t addMesh(const utility::graphic::Mesh &mesh,
					   const std::string &materialName) override
		{
			return _evanEngine->addMesh(mesh, materialName);
		}

		bool removeObject(size_t objectID) override
		{
			return _evanEngine->removeObject(objectID);
		}

		utility::graphic::SizeF
			measureText(const utility::graphic::Text &text) const override
		{
			return text.getTextDimensions();
		}

		size_t addText(utility::graphic::Text text) override
		{
			return _evanEngine->addText(
				std::make_shared<utility::graphic::Text>(std::move(text)));
		}

		size_t addModel(std::shared_ptr<utility::graphic::Model> model) override
		{
			return _evanEngine->addModel(std::move(model));
		}

		utility::graphic::ViewF getView(void) const override
		{
			if (_evanEngine) {
				return _evanEngine->getView();
			}
			throw std::runtime_error("Engine not initialized, cannot get view");
		}

		void addScene(size_t sceneIndex) override
		{
			_evanEngine->addScene(sceneIndex);
		}

		void pollEvents(void) override
		{
			auto events	  = _evanEngine->pollEvents();
			auto callback = this->getEventCallback();

			if (!callback) {
				getLogger().warning()
					<< "No event callback set, skipping event dispatch.";
				return;
			}
			for (auto &event: events) {
				callback(event);
			}
		}

		void update(void) override
		{
			if (_evanEngine) {
				_evanEngine->update();
			}
		}
	};

	/**
	 * @brief Create a platform appropriate for the current OS.
	 */
	std::shared_ptr<evan::IPlatform> makePlatform(void)
	{
#if defined(__APPLE__)
		return std::make_shared<evan::MacOsDesktopPlatform>("Hello World", 1280,
															720);
#elif defined(__linux__)
		return std::make_shared<evan::LinuxDesktopPlatform>("Hello World", 1280,
															720);
#elif defined(_WIN32)
		return std::make_shared<evan::WindowsDesktopPlatform>("Hello World",
															  1280, 720);
#else
		return nullptr;
#endif
	}

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

	auto platform = makePlatform();
	if (platform == nullptr) {
		return 1;
	}

	guillaume::Application<HelloWorldScene, HelloWorldScene, SecondaryScene>
		app(resources);
	app.setEngine(std::make_unique<EvanEngine>(resources, platform));

	return app.run();
}
