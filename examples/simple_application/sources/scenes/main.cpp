#include "scenes/main.hpp"

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/panel.hpp>
#include <guillaume/entities/text.hpp>

#include <iostream>

namespace simple_application::scenes
{

	Main::Main(guillaume::LocalStorage &localStorage,
			   guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(localStorage, sessionStorage)
	{
		getLogger().info("Main scene created");

		auto &textBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Text::Builder>();
		auto &textDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Text::Director>();
		auto textId = textDirector.makeDefaultText(
			textBuilder, "Hello, World!", 24, utility::graphic::Color32Bit(255, 255, 255, 255));

		getComponentRegistry()
			.getComponent<guillaume::components::Transform>(textId)
				.setPose(utility::graphic::PoseF(
					utility::graphic::PositionF(200.0f, 200.0f, 0.0f),
					utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)
				));

		auto &buttonBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Button::Builder>();
		auto &buttonDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Button::Director>();

		std::cout << "ici" << std::endl;
		auto buttonId = buttonDirector.makeTextButton(
			buttonBuilder, textId,
			[]() { std::cout << "Button clicked!" << std::endl; }
		);

		getComponentRegistry()
			.getComponent<guillaume::components::Transform>(buttonId)
				.setPose(utility::graphic::PoseF(
					utility::graphic::PositionF(400.0f, 400.0f, 0.0f),
					utility::graphic::OrientationF(0.0f, 0.0f, 0.0f, 1.0f)
				));
		getComponentRegistry()
			.getComponent<guillaume::components::Bound>(buttonId)
				.setSize({ 200.0f, 60.0f });

		getComponentRegistry()
			.getComponent<guillaume::components::Color>(buttonId)
				.setColor(utility::graphic::Color32Bit(128, 0, 255, 255));
	}

	Main::~Main(void)
	{
	}

}	 // namespace simple_application::scenes