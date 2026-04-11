#include "scenes/main.hpp"

#include <guillaume/entities/panel.hpp>
#include <guillaume/entities/icon.hpp>
#include <guillaume/entities/text.hpp>
#include <guillaume/entities/button.hpp>

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

		auto &iconBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Icon::Builder>();
		auto &iconDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Icon::Director>();

		auto &textBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Text::Builder>();
		auto &textDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Text::Director>();

		auto &buttonBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Button::Builder>();
		auto &buttonDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Button::Director>();

		panelDirector.makeColorPanel(
			panelBuilder,
			utility::graphic::PoseF(
				utility::graphic::PositionF(200.0F, 200.0F, 0.0F),
				utility::graphic::OrientationF(0.0F, 0.0F, 0.0F, 1.0F)),
			utility::graphic::Color32Bit(255, 0, 0, 255),
			{ buttonDirector.makeIconTextButton(
				buttonBuilder,
				iconDirector.makeDefaultIcon(iconBuilder, "settings", 24.0f),
				textDirector.makeDefaultText(textBuilder, "Settings"),
				[this]() {
					this->getLogger().info("Button icon text clicked!");
				}) });
	}

	Main::~Main(void)
	{
	}

}	 // namespace simple_application::scenes