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

#include "guillaume/systems/hand_motion.hpp"

namespace guillaume::systems
{
	HandMotion::HandMotion(event::EventBus &eventBus)
		: ecs::SystemFiller<components::HandHoverInteraction,
							components::Transform, components::Bound>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::HandMotionEvent>(eventBus)
	{
	}

	HandMotion::~HandMotion(void)
	{
	}

	void HandMotion::prepare(void)
	{
		consumeNextEvent();
	}

	void HandMotion::cleanup(void)
	{
	}

	void HandMotion::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto handMotionEvent = getLastEvent();
		if (!handMotionEvent)
			return;

		this->getLogger().info()
			<< "HandMotion::update: event received: " << *handMotionEvent;

		auto ray = handMotionEvent->getAim().toForwardRay();

		auto &transform = getComponent<components::Transform>(entityIdentifier);
		auto &bound		= getComponent<components::Bound>(entityIdentifier);
		auto &handHoverInteraction =
			getComponent<components::HandHoverInteraction>(entityIdentifier);

		const auto size =
			utility::math::Vector2F({ bound.getWidth(), bound.getHeight() });
		const bool isIntersecting =
			ray.intersectRectangle(transform.getPose(), size);

		if (isIntersecting) {
			if (!handHoverInteraction.isHovered()) {
				handHoverInteraction.setHovered(true);
				if (const auto onHover =
						handHoverInteraction.getOnHoverHandler()) {
					onHover();
				}
			}
			return;
		}

		if (handHoverInteraction.isHovered()) {
			handHoverInteraction.setHovered(false);
			if (const auto onUnhover =
					handHoverInteraction.getOnUnhoverHandler()) {
				onUnhover();
			}
		}
	}

}	 // namespace guillaume::systems