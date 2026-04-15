#include "scenes/main.hpp"

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/panel.hpp>

namespace simple_application::scenes
{

	Main::Main(guillaume::LocalStorage &localStorage,
			   guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(localStorage, sessionStorage)
	{
		getLogger().info("Main scene created");

		auto &panelBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Panel::Builder>();
		auto &panelDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Panel::Director>();

		auto &buttonBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Button::Builder>();
		auto &buttonDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Button::Director>();

		buttonDirector.makeIconTextButton(
			buttonBuilder, "settings", "Settings", [this]() {
				this->getLogger().info("Button icon text clicked!");
			});
	}

	Main::~Main(void)
	{
	}

}	 // namespace simple_application::scenes