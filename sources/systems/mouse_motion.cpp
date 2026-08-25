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

#include "guillaume/systems/mouse_motion.hpp"

namespace guillaume::systems
{
	MouseMotion::MouseMotion(event::EventBus &eventBus,
							 std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::MouseHoverInteraction,
							components::Transform, components::Bound>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::MouseMotionEvent>(eventBus)
		, _engine(engine)
	{
	}

	MouseMotion::~MouseMotion(void)
	{
	}

	void MouseMotion::prepare(void)
	{
		consumeNextEvent();
	}

	void MouseMotion::cleanup(void)
	{
	}

	void MouseMotion::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto mouseMotionEvent = getLastEvent();
		if (!mouseMotionEvent)
			return;

		this->getLogger().info()
			<< "MouseMotion::update: event received: " << *mouseMotionEvent;

		const auto &position = mouseMotionEvent->getPosition();

		utility::graphic::RayF ray;
		try {
			ray = _engine->getView().viewPointToRay(position);
		} catch (const std::out_of_range &exception) {
			this->getLogger().warning()
				<< "MouseMotion::update: " << exception.what();
			return;
		}

		auto &transform = getComponent<components::Transform>(entityIdentifier);
		auto &bound		= getComponent<components::Bound>(entityIdentifier);
		auto &mouseHoverInteraction =
			getComponent<components::MouseHoverInteraction>(entityIdentifier);

		const auto size =
			utility::math::Vector2F({ bound.getWidth(), bound.getHeight() });
		const bool isIntersecting =
			ray.intersectRectangle(transform.getPose(), size);

		if (isIntersecting) {
			if (!mouseHoverInteraction.isHovered()) {
				mouseHoverInteraction.setHovered(true);
				if (const auto onHover =
						mouseHoverInteraction.getOnHoverHandler()) {
					onHover();
				}
			}
			return;
		}

		if (mouseHoverInteraction.isHovered()) {
			mouseHoverInteraction.setHovered(false);
			if (const auto onUnhover =
					mouseHoverInteraction.getOnUnhoverHandler()) {
				onUnhover();
			}
		}
	}

}	 // namespace guillaume::systems