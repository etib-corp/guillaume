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

#include "guillaume/systems/hand_pinch.hpp"

namespace guillaume::systems
{
	HandPinch::HandPinch(event::EventBus &eventBus)
		: ecs::SystemFiller<components::HandPinchInteraction,
							components::Transform, components::Bound>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::HandPinchEvent>(eventBus)
	{
	}

	HandPinch::~HandPinch(void)
	{
	}

	void HandPinch::prepare(void)
	{
		consumeNextEvent();
	}

	void HandPinch::cleanup(void)
	{
	}

	void HandPinch::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto pinchEvent = this->getLastEvent();
		if (!pinchEvent)
			return;

		auto &transform = this->template getComponent<components::Transform>(
			entityIdentifier);
		auto &bound =
			this->template getComponent<components::Bound>(entityIdentifier);
		auto &interaction =
			this->template getComponent<components::HandPinchInteraction>(
				entityIdentifier);

		const auto size =
			utility::math::Vector2F({ bound.getWidth(), bound.getHeight() });
		const auto ray = utility::graphic::RayF(
			utility::graphic::PositionF(pinchEvent->getPose().getPosition()),
			pinchEvent->getPose().getOrientation().getForward());
		const bool isIntersecting =
			ray.intersectRectangle(transform.getPose(), size);

		if (isIntersecting) {
			interaction.setPinching(true);
			const auto &handler = interaction.getOnEventHandler();
			if (handler) {
				handler();
			}
		} else {
			interaction.setPinching(false);
		}
	}

}	 // namespace guillaume::systems