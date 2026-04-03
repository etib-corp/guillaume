#pragma once

#include <guillaume/scene.hpp>

#include "panels/default_panel.hpp"

namespace simple_application::scenes
{

	class Settings: public guillaume::Scene
	{
		public:
		Settings(guillaume::LocalStorage &localStorage,
				 guillaume::SessionStorage &sessionStorage);

		~Settings(void);
	};

}	 // namespace simple_application::scenes