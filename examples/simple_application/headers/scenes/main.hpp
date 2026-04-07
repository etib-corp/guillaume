#pragma once

#include <guillaume/scene.hpp>

namespace simple_application::scenes
{

	class Main: public guillaume::Scene
	{
		public:
		Main(guillaume::LocalStorage &localStorage,
			 guillaume::SessionStorage &sessionStorage);

		~Main(void);
	};

}	 // namespace simple_application::scenes