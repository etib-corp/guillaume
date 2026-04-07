#include "scenes/main.hpp"

#include <guillaume/entities/panel.hpp>
#include <guillaume/entities/icon.hpp>

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
		panelDirector.makeColorPanel(
			panelBuilder,
			utility::graphic::PoseF(
				utility::graphic::PositionF(200.0F, 200.0F, 0.0F),
				utility::graphic::OrientationF(0.0F, 0.0F, 0.0F, 1.0F)),
			utility::graphic::Color32Bit(255, 0, 0, 255));

		iconDirector.makeDefaultIcon(iconBuilder, "arrow_split", 200.0f);
	}

	Main::~Main(void)
	{
	}

}	 // namespace simple_application::scenes