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

#include <evan/Engine.hpp>
#include <evan/glfw/platform/LinuxDesktopPlatform.hpp>

#include <guillaume/application.hpp>
#include <guillaume/scene.hpp>

#include <guillaume/components/bound.hpp>
#include <guillaume/entities/button.hpp>
#include <guillaume/entities/container.hpp>
#include <guillaume/entities/model.hpp>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/ressource_provider.hpp>
#include <utility/system_io/default_system_io.hpp>

namespace
{
	using utility::graphic::Color32Bit;
	using utility::graphic::PoseF;

	/**
	 * @brief Scene displaying a colored rectangle, a button and a 3D model.
	 *
	 * The three entities are registered as root entities so the scene places
	 * them side by side in front of the camera.
	 */
	class DemoScene: public guillaume::Scene
	{
		public:
		using guillaume::Scene::Scene;

		void onEnter(void) override
		{
			auto &containerBuilder =
				getBuilderManager()
					.getBuilder<guillaume::entities::Container::Builder>();
			auto &containerDirector =
				getDirectorManager()
					.getDirector<guillaume::entities::Container::Director>();

			auto container = containerDirector.makeColorContainer(
				containerBuilder, nullptr, PoseF(),
				Color32Bit(64, 144, 240, 255), {});
			container->setBorderRadius(24.0f);

			getComponentRegistry()
				.getComponent<guillaume::components::Bound>(
					container->getIdentifier())
				.setWidth(260.0f)
				.setHeight(140.0f);

			addRootEntity("container", container);

			auto &buttonBuilder =
				getBuilderManager()
					.getBuilder<guillaume::entities::Button::Builder>();
			auto &buttonDirector =
				getDirectorManager()
					.getDirector<guillaume::entities::Button::Director>();

			auto button = buttonDirector.makeButton(
				buttonBuilder, nullptr, "Click me", []() {},
				guillaume::entities::Button::Color::Filled,
				guillaume::entities::Button::Shape::Round,
				guillaume::entities::Button::Size::Medium, false);

			addRootEntity("button", button);

			auto &modelBuilder =
				getBuilderManager()
					.getBuilder<guillaume::entities::Model::Builder>();
			auto &modelDirector =
				getDirectorManager()
					.getDirector<guillaume::entities::Model::Director>();

			auto model = modelDirector.makeModel(modelBuilder, nullptr,
												 "models/teapot.obj", "");

			addRootEntity("model", model);
		}
	};

}	 // namespace

int main(void)
{
	auto platform = std::make_shared<evan::LinuxDesktopPlatform>(
		"Guillaume + Evan", 1280, 720);

	utility::DefaultSystemIO systemIo;
	auto resources = std::make_shared<utility::RessourceProvider>(systemIo);

	auto engine =
		std::make_unique<evan::Engine>(resources, std::move(platform));

	guillaume::Application<DemoScene, DemoScene> app(resources);
	app.setEngine(std::move(engine));

	return app.run();
}
