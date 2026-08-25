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

#include "guillaume/systems/mouse_button.hpp"

namespace guillaume::systems
{
	MouseButton::MouseButton(event::EventBus &eventBus,
							 std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::MouseButtonInteraction,
							components::Transform, components::Bound>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::MouseButtonEvent>(eventBus)
		, _engine(engine)
	{
	}

	MouseButton::~MouseButton(void)
	{
	}

	void MouseButton::prepare(void)
	{
		consumeNextEvent();
	}

	void MouseButton::cleanup(void)
	{
	}

	void MouseButton::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto mouseButtonEvent = getLastEvent();
		if (!mouseButtonEvent)
			return;

		this->getLogger().info()
			<< "MouseButton::update: event received: " << *mouseButtonEvent;

		const auto button = mouseButtonEvent->getButton();
		if (button == utility::event::MouseButtonEvent::Button::Unknown)
			return;

		const auto &position = mouseButtonEvent->getPosition();

		utility::graphic::RayF ray;
		try {
			ray = _engine->getView().viewPointToRay(position);
		} catch (const std::out_of_range &exception) {
			this->getLogger().warning()
				<< "MouseButton::update: " << exception.what();
			return;
		}

		auto &transform = this->template getComponent<components::Transform>(
			entityIdentifier);
		auto &bound =
			this->template getComponent<components::Bound>(entityIdentifier);
		auto &interaction =
			this->template getComponent<components::MouseButtonInteraction>(
				entityIdentifier);

		const auto size =
			utility::math::Vector2F({ bound.getWidth(), bound.getHeight() });
		const bool isIntersecting =
			ray.intersectRectangle(transform.getPose(), size);
		const bool isButtonPressed = mouseButtonEvent->isButtonPressed();

		if (isButtonPressed && isIntersecting) {
			interaction.setButtonPressed(button, true);
			const auto &handler = interaction.getOnButtonPressHandler(button);
			if (handler) {
				handler();
			}
		} else if (!isButtonPressed && interaction.isButtonPressed(button)
				   && isIntersecting) {
			interaction.setButtonPressed(button, false);
			const auto &handler = interaction.getOnButtonReleaseHandler(button);
			if (handler) {
				handler();
			}
		} else if (!isButtonPressed) {
			interaction.setButtonPressed(button, false);
		}
	}

}	 // namespace guillaume::systems