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

#include "guillaume/systems/drag.hpp"
#include "guillaume/systems/hit_test.hpp"

#include <optional>

#include <utility/graphic/pose.hpp>
#include <utility/graphic/ray.hpp>

namespace guillaume::systems
{
	namespace
	{
		using MouseMotionManager =
			event::EventManager<utility::event::MouseMotionEvent>;
		using HandMotionManager =
			event::EventManager<utility::event::HandMotionEvent>;

		/**
		 * @brief Find where a hand ray meets the plane of an entity.
		 * @param ray The aim ray of the hand.
		 * @param pose The pose of the entity; its forward axis is the
		 * normal of the plane.
		 * @return The point, measured along the right and up axes of the
		 * entity, or nullopt when the ray is parallel to the plane or
		 * points away from it.
		 */
		std::optional<utility::math::Vector2F>
			pointOnPlane(const utility::graphic::RayF &ray,
						 const utility::graphic::PoseF &pose)
		{
			const auto &orientation = pose.getOrientation();
			const utility::math::Vector<float, 3> normal =
				orientation.getForward();
			const utility::math::Vector<float, 3> corner(pose.getPosition());

			const float denominator =
				utility::math::dot(ray.getDirection(), normal);
			if (denominator == 0.0f) {
				return std::nullopt;
			}

			const float distance =
				utility::math::dot(corner - ray.getOrigin(), normal)
				/ denominator;
			if (distance < 0.0f) {
				return std::nullopt;
			}

			const utility::math::Vector<float, 3> point = ray.at(distance);
			const utility::math::Vector<float, 3> right =
				orientation.getRight();
			const utility::math::Vector<float, 3> up = orientation.getUp();

			return utility::math::Vector2F({ utility::math::dot(point, right),
											 utility::math::dot(point, up) });
		}
	}	 // namespace

	Drag::Drag(event::EventBus &eventBus)
		: ecs::SystemFiller<components::DragInteraction, components::Transform,
							components::Bound>(ecs::Phase::Event)
		, event::EventManager<utility::event::MouseMotionEvent>(eventBus)
		, event::EventManager<utility::event::HandMotionEvent>(eventBus)
	{
	}

	Drag::~Drag(void)
	{
	}

	void Drag::prepare(void)
	{
		MouseMotionManager::consumeNextEvent();
		HandMotionManager::consumeNextEvent();
	}

	void Drag::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &drag =
			getComponent<components::DragInteraction>(entityIdentifier);

		const bool mousePressed =
			hasComponent<components::MouseButtonInteraction>(entityIdentifier)
			&& getComponent<components::MouseButtonInteraction>(
				   entityIdentifier)
				   .isButtonPressed(
					   utility::event::MouseButtonEvent::Button::Left);

		const bool handPressed = !mousePressed
			&& hasComponent<components::HandButtonInteraction>(entityIdentifier)
			&& getComponent<components::HandButtonInteraction>(entityIdentifier)
				   .isButtonPressed(utility::event::HandButtonEvent::Button::A);

		// A disabled entity ends any drag in progress and cannot start one.
		const bool pressed = (mousePressed || handPressed)
			&& isInteractive(this->getComponentRegistry(), entityIdentifier);

		// The release ends the drag at once, whether a pointer moves or not.
		if (!pressed) {
			if (drag.isDragging()) {
				drag.setDragging(false);
				if (const auto handler = drag.getOnDragEndHandler()) {
					handler();
				}
			}
			return;
		}

		// The pointer is the one that holds the entity. Its position is a
		// state, kept from the last motion event; whether it moved is told
		// by the event of this frame only.
		std::optional<utility::math::Vector2F> position;
		bool moved = false;

		if (mousePressed) {
			if (const auto motion = MouseMotionManager::getRetainedEvent()) {
				position = motion->getPosition();
			}
			moved = MouseMotionManager::getLastEvent() != nullptr;
		} else {
			if (const auto motion = HandMotionManager::getRetainedEvent()) {
				position = pointOnPlane(
					motion->getAim().toForwardRay(),
					getComponent<components::Transform>(entityIdentifier)
						.getPose());
			}
			moved = HandMotionManager::getLastEvent() != nullptr;
		}

		if (!position) {
			return;
		}

		if (!drag.isDragging()) {
			drag.setLastPosition(*position);
			drag.setDragging(true);
			if (const auto handler = drag.getOnDragStartHandler()) {
				handler();
			}
			return;
		}

		if (!moved) {
			return;
		}

		const auto delta = *position - drag.getLastPosition();
		drag.setLastPosition(*position);
		drag.setDelta(delta);
		if (const auto handler = drag.getOnDragHandler()) {
			handler(delta);
		}
	}

}	 // namespace guillaume::systems
