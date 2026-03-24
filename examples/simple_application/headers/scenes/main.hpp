#pragma once

#include <guillaume/scene.hpp>
#include "panels/default_panel.hpp"

namespace simple_application::scenes
{

	class Main: public guillaume::SceneFiller<panels::DefaultPanel>
	{
		public:
		Main(void);

		~Main(void);

		void onCreate(void) override;

		void onStart(void) override;

		void onPause(void) override;

		void onResume(void) override;

		void onStop(void) override;

		void onRestart(void) override;

		void onDestroy(void) override;
	};

}	 // namespace simple_application::scenes