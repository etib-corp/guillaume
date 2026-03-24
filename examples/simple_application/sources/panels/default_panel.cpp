#include "panels/default_panel.hpp"

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/icon.hpp>
#include <guillaume/entities/text.hpp>

namespace simple_application::panels
{
	DefaultPanel::DefaultPanel(guillaume::ecs::ComponentRegistry &registry,
							   guillaume::ecs::EntityRegistry &entityRegistry,
							   const std::string &name)
		: guillaume::entities::Panel(registry, entityRegistry, name)
	{
		guillaume::entities::Button::Builder buttonBuilder(
			getComponentRegistry(), getEntityRegistry());
		guillaume::entities::Button::Director buttonDirector;
		buttonDirector.makeTextButton(buttonBuilder, "Click Me", [this]() {
			this->getLogger().info("Button clicked in DefaultPanel!");
		});
	}

	DefaultPanel::~DefaultPanel(void)
	{
	}

}	 // namespace simple_application::panels