#include "scenes/settings.hpp"

namespace simple_application::scenes
{

	Settings::Settings(guillaume::LocalStorage &localStorage,
					   guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(localStorage, sessionStorage)
	{
		getLogger().info("Settings scene created");
	}

	Settings::~Settings(void)
	{
	}

}	 // namespace simple_application::scenes