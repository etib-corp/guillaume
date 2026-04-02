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

#include "guillaume/systems/interaction.hpp"

#include <utility/graphic/orientation.hpp>
#include <utility/graphic/position.hpp>
#include <utility/graphic/ray.hpp>

#include <cmath>
#include <array>
#include <optional>

namespace
{

	using WorldPosition	   = utility::graphic::PositionF;
	using WorldOrientation = utility::graphic::OrientationF;
	using Size			   = utility::math::Vector2F;

	static WorldPosition
		rotatePositionByQuaternion(const WorldPosition &position,
								   const WorldOrientation &orientation)
	{
		const auto normalizedOrientation = orientation.normalized();
		const float qx					 = normalizedOrientation.x;
		const float qy					 = normalizedOrientation.y;
		const float qz					 = normalizedOrientation.z;
		const float qw					 = normalizedOrientation.w;
		const float positionX			 = position[0];
		const float positionY			 = position[1];
		const float positionZ			 = position[2];

		const float crossX = (qy * positionZ) - (qz * positionY);
		const float crossY = (qz * positionX) - (qx * positionZ);
		const float crossZ = (qx * positionY) - (qy * positionX);

		const float tX = 2.0f * crossX;
		const float tY = 2.0f * crossY;
		const float tZ = 2.0f * crossZ;

		const float crossTX = (qy * tZ) - (qz * tY);
		const float crossTY = (qz * tX) - (qx * tZ);
		const float crossTZ = (qx * tY) - (qy * tX);

		return WorldPosition(positionX + (qw * tX) + crossTX,
							 positionY + (qw * tY) + crossTY,
							 positionZ + (qw * tZ) + crossTZ);
	}

	static WorldPosition
		computeEntityBoundCenter(const WorldPosition &worldPosition,
								 const Size &boundSize)
	{
		WorldPosition center = worldPosition;
		center[1]			 = worldPosition[1] - (boundSize[1] / 2.0f);
		return center;
	}

	static bool isPointInsideEntityBounds(
		const WorldPosition &point, const WorldPosition &entityCenter,
		const Size &boundSize, const WorldOrientation &entityOrientation)
	{
		const float halfWidth  = boundSize[0] / 2.0f;
		const float halfHeight = boundSize[1] / 2.0f;

		const std::array<WorldPosition, 4> localCorners {
			WorldPosition(-halfWidth, -halfHeight, 0.0f),
			WorldPosition(halfWidth, -halfHeight, 0.0f),
			WorldPosition(halfWidth, halfHeight, 0.0f),
			WorldPosition(-halfWidth, halfHeight, 0.0f),
		};

		std::array<utility::math::Vector2F, 4> projectedCorners;
		for (std::size_t i = 0; i < localCorners.size(); ++i) {
			const auto rotatedCorner =
				rotatePositionByQuaternion(localCorners[i], entityOrientation);
			projectedCorners[i] = utility::math::Vector2F({
				entityCenter[0] + rotatedCorner[0],
				entityCenter[1] + rotatedCorner[1],
			});
		}

		const utility::math::Vector2F projectedPoint({ point[0], point[1] });

		float referenceSign = 0.0f;
		for (std::size_t i = 0; i < projectedCorners.size(); ++i) {
			const auto &a = projectedCorners[i];
			const auto &b = projectedCorners[(i + 1) % projectedCorners.size()];

			const float edgeX  = b[0] - a[0];
			const float edgeY  = b[1] - a[1];
			const float pointX = projectedPoint[0] - a[0];
			const float pointY = projectedPoint[1] - a[1];
			const float cross  = (edgeX * pointY) - (edgeY * pointX);

			if (std::abs(cross) <= 1.0e-5f) {
				continue;
			}

			if (referenceSign == 0.0f) {
				referenceSign = cross;
				continue;
			}

			if ((cross > 0.0f) != (referenceSign > 0.0f)) {
				return false;
			}
		}

		return true;
	}

}	 // namespace

namespace guillaume::systems
{

	Interaction::Interaction(event::EventBus &eventBus, Renderer &renderer)
		: ecs::SystemFiller<components::Transform, components::Bound>()
		, _mouseButtonSubscriber(eventBus)
		, _mouseMotionSubscriber(eventBus)
		, _renderer(renderer)
	{
	}

	void Interaction::updateMouseMotionState(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (_pendingMotionEvent
			&& _evaluatedMotionEntities.contains(entityIdentifier)) {
			_pendingMotionEvent.reset();
			_evaluatedMotionEntities.clear();
		}

		if (!_pendingMotionEvent) {
			while (_mouseMotionSubscriber.hasPendingEvents()) {
				auto nextEvent = _mouseMotionSubscriber.getNextEvent();
				if (!nextEvent) {
					continue;
				}
				_pendingMotionEvent = std::move(nextEvent);
				_evaluatedMotionEntities.clear();
				break;
			}
		}

		if (!_pendingMotionEvent) {
			return;
		}

		_lastMousePosition = _pendingMotionEvent->getPosition();
		_hasMousePosition  = true;
		_evaluatedMotionEntities.insert(entityIdentifier);
	}

	void Interaction::processHover(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (!componentRegistry.hasComponent<components::Hover>(
				entityIdentifier)) {
			return;
		}

		auto &hover =
			componentRegistry.getComponent<components::Hover>(entityIdentifier);

		if (isInside) {
			if (hover.isHovered()) {
				return;
			}

			hover.setHovered(true);
			const auto onHover = hover.getOnHoverHandler();
			if (onHover) {
				onHover();
			}
			return;
		}

		if (!hover.isHovered()) {
			return;
		}

		hover.setHovered(false);
		const auto onUnhover = hover.getOnUnhoverHandler();
		if (onUnhover) {
			onUnhover();
		}
	}

	void Interaction::processClick(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (_pendingClickEvent
			&& _evaluatedClickEntities.contains(entityIdentifier)) {
			_buttonStates = _pendingClickEvent->getButtonsState();
			_pendingClickEvent.reset();
			_evaluatedClickEntities.clear();
		}

		if (!_pendingClickEvent) {
			while (_mouseButtonSubscriber.hasPendingEvents()) {
				auto nextEvent = _mouseButtonSubscriber.getNextEvent();
				if (!nextEvent) {
					continue;
				}
				_pendingClickEvent = std::move(nextEvent);
				_evaluatedClickEntities.clear();
				break;
			}
		}

		if (!_pendingClickEvent) {
			return;
		}

		_evaluatedClickEntities.insert(entityIdentifier);

		if (!componentRegistry.hasComponent<components::Click>(
				entityIdentifier)) {
			return;
		}

		auto &click =
			componentRegistry.getComponent<components::Click>(entityIdentifier);

		utility::event::MouseButtonEvent::MouseButtonsState
			currentButtonStates = _pendingClickEvent->getButtonsState();
		utility::event::MouseButtonEvent::MouseButtonsState changedButtons =
			currentButtonStates ^ _buttonStates;

		for (std::size_t buttonIndex = 0;
			 buttonIndex < static_cast<std::size_t>(
				 utility::event::MouseButtonEvent::MouseButton::Last);
			 ++buttonIndex) {
			if (!changedButtons.test(buttonIndex)) {
				continue;
			}

			const auto button =
				static_cast<utility::event::MouseButtonEvent::MouseButton>(
					buttonIndex);
			const bool isPressed = currentButtonStates.test(buttonIndex);

			if (isPressed && isInside) {
				click.setPressedInside(button, true);
				click.setClicked(button, true);
				const auto &onClickHandler =
					click.getOnClickHandlers().at(button);
				if (onClickHandler) {
					onClickHandler(_pendingClickEvent->getPosition());
				}
				break;
			}

			if (!isPressed && click.isPressedInside(button) && isInside) {
				click.setPressedInside(button, false);
				click.setClicked(button, false);
				const auto &onReleaseHandler =
					click.getOnReleaseHandlers().at(button);
				if (onReleaseHandler) {
					onReleaseHandler(_pendingClickEvent->getPosition());
				}
				break;
			}

			if (!isPressed) {
				click.setClicked(button, false);
				click.setPressedInside(button, false);
			}
		}
	}

	void Interaction::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &componentRegistry = getComponentRegistry();

		getLogger().debug("Updating Interaction system for entity "
						  + std::to_string(entityIdentifier));

		const bool hasHover =
			componentRegistry.hasComponent<components::Hover>(entityIdentifier);
		const bool hasClick =
			componentRegistry.hasComponent<components::Click>(entityIdentifier);

		if (!hasHover && !hasClick) {
			return;
		}

		updateMouseMotionState(entityIdentifier);

		if (!_pendingClickEvent) {
			while (_mouseButtonSubscriber.hasPendingEvents()) {
				auto nextEvent = _mouseButtonSubscriber.getNextEvent();
				if (!nextEvent) {
					continue;
				}
				_pendingClickEvent = std::move(nextEvent);
				_evaluatedClickEntities.clear();
				break;
			}
		}

		const auto &bound =
			componentRegistry.getComponent<components::Bound>(entityIdentifier);

		std::optional<utility::event::MouseMotionEvent::MousePosition>
			pointerScreenPosition = std::nullopt;
		if (_pendingClickEvent) {
			pointerScreenPosition = _pendingClickEvent->getPosition();
		} else if (_hasMousePosition) {
			pointerScreenPosition = _lastMousePosition;
		}

		if (!pointerScreenPosition.has_value()) {
			processHover(componentRegistry, entityIdentifier, false);
			processClick(componentRegistry, entityIdentifier, false);
			return;
		}

		const auto viewPosition = _renderer.getView().getPose().getPosition();
		const WorldPosition worldMousePosition(
			pointerScreenPosition->x + viewPosition[0],
			pointerScreenPosition->y + viewPosition[1], 0.0f);

		const auto &transform =
			componentRegistry.getComponent<components::Transform>(
				entityIdentifier);
		const auto pose = transform.getPose();
		const auto size = bound.getSize();

		const auto trueCenter =
			computeEntityBoundCenter(pose.getPosition(), size);
		const bool isInside = isPointInsideEntityBounds(
			worldMousePosition, trueCenter, size, pose.getOrientation());

		processHover(componentRegistry, entityIdentifier, isInside);
		processClick(componentRegistry, entityIdentifier, isInside);
	}

}	 // namespace guillaume::systems