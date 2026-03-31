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

#include "guillaume/ecs/components/parent.hpp"

#include <cmath>
#include <optional>

namespace
{

	using WorldPosition	   = guillaume::components::Transform::Position;
	using WorldOrientation = guillaume::components::Transform::Orientation;

	static WorldPosition
		rotatePositionByQuaternion(const WorldPosition &position,
								   const WorldOrientation &orientation)
	{
		const auto normalizedOrientation = orientation.normalizedQuaternion();
		const float qx					 = normalizedOrientation.getX();
		const float qy					 = normalizedOrientation.getY();
		const float qz					 = normalizedOrientation.getZ();
		const float qw					 = normalizedOrientation.getW();
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

		return WorldPosition({ positionX + (qw * tX) + crossTX,
							   positionY + (qw * tY) + crossTY,
							   positionZ + (qw * tZ) + crossTZ });
	}

	static float extractYawRadians(const WorldOrientation &orientation)
	{
		const auto normalizedOrientation = orientation.normalizedQuaternion();
		const float x					 = normalizedOrientation.getX();
		const float y					 = normalizedOrientation.getY();
		const float z					 = normalizedOrientation.getZ();
		const float w					 = normalizedOrientation.getW();

		const float sinYaw = 2.0f * ((w * z) + (x * y));
		const float cosYaw = 1.0f - (2.0f * ((y * y) + (z * z)));
		return std::atan2(sinYaw, cosYaw);
	}

	struct WorldTransform {
		guillaume::components::Transform::Position position;
		guillaume::components::Transform::Orientation orientation;
		guillaume::components::Transform::Scale scale;
	};

	static WorldTransform calculateWorldTransform(
		guillaume::ecs::ComponentRegistry &componentRegistry,
		const guillaume::ecs::Entity::Identifier &entityId)
	{
		const auto &transform =
			componentRegistry.getComponent<guillaume::components::Transform>(
				entityId);

		WorldTransform worldTransform { transform.getPosition(),
										transform.getOrientation(),
										transform.getScale() };

		if (!componentRegistry.hasComponent<guillaume::ecs::components::Parent>(
				entityId)) {
			return worldTransform;
		}

		const auto &parent =
			componentRegistry.getComponent<guillaume::ecs::components::Parent>(
				entityId);
		const auto parentId = parent.getParentIdentifier();

		if (parentId == guillaume::ecs::Entity::InvalidIdentifier) {
			return worldTransform;
		}

		const auto parentWorldTransform =
			calculateWorldTransform(componentRegistry, parentId);

		guillaume::components::Transform::Position scaledLocalPosition;
		scaledLocalPosition[0] =
			worldTransform.position[0] * parentWorldTransform.scale[0];
		scaledLocalPosition[1] =
			worldTransform.position[1] * parentWorldTransform.scale[1];
		scaledLocalPosition[2] =
			worldTransform.position[2] * parentWorldTransform.scale[2];

		const auto rotatedLocalPosition = rotatePositionByQuaternion(
			scaledLocalPosition, parentWorldTransform.orientation);

		worldTransform.position[0] =
			parentWorldTransform.position[0] + rotatedLocalPosition[0];
		worldTransform.position[1] =
			parentWorldTransform.position[1] + rotatedLocalPosition[1];
		worldTransform.position[2] =
			parentWorldTransform.position[2] + rotatedLocalPosition[2];

		worldTransform.orientation =
			parentWorldTransform.orientation * worldTransform.orientation;
		worldTransform.orientation.normalizeQuaternion();

		worldTransform.scale[0] *= parentWorldTransform.scale[0];
		worldTransform.scale[1] *= parentWorldTransform.scale[1];
		worldTransform.scale[2] *= parentWorldTransform.scale[2];

		return worldTransform;
	}

	static std::optional<WorldPosition>
		intersectRayWithUiPlane(const guillaume::Renderer::Ray &ray,
								const float planeZ = 0.0f)
	{
		const auto origin	 = ray.getOrigin();
		const auto direction = ray.getDirection();

		constexpr float epsilon = 1.0e-6f;
		if (std::abs(direction[2]) <= epsilon) {
			return std::nullopt;
		}

		const float distanceParameter = (planeZ - origin[2]) / direction[2];
		if (distanceParameter < 0.0f) {
			return std::nullopt;
		}

		const auto point = ray.pointAt(distanceParameter);
		return WorldPosition({ point[0], point[1], point[2] });
	}

	static WorldPosition computeEntityBoundCenter(
		const guillaume::components::Transform::Position &worldPosition,
		const guillaume::components::Bound::Size &boundSize,
		const guillaume::components::Transform::Scale &worldScale)
	{
		guillaume::components::Transform::Position center = worldPosition;
		center[1] = worldPosition[1] - (boundSize[1] * worldScale[1] / 2.0f);
		return center;
	}

	static bool isPointInsideEntityBounds(
		const guillaume::components::Transform::Position &point,
		const guillaume::components::Transform::Position &entityCenter,
		const guillaume::components::Bound::Size &boundSize,
		const guillaume::components::Transform::Scale &entityScale,
		const guillaume::components::Transform::Orientation &entityOrientation)
	{
		const float orientationRadians = -extractYawRadians(entityOrientation);
		const float cosine			   = std::cos(orientationRadians);
		const float sine			   = std::sin(orientationRadians);

		const float dx = point[0] - entityCenter[0];
		const float dy = point[1] - entityCenter[1];

		const float localX = dx * cosine - dy * sine;
		const float localY = dx * sine + dy * cosine;

		const float halfWidth  = (boundSize[0] * entityScale[0]) / 2.0f;
		const float halfHeight = (boundSize[1] * entityScale[1]) / 2.0f;

		return std::abs(localX) <= halfWidth && std::abs(localY) <= halfHeight;
	}

}	 // namespace

namespace guillaume::systems
{

	Interaction::Interaction(event::EventBus &eventBus, Renderer &renderer)
		: _mouseButtonSubscriber(eventBus)
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

		const auto mousePosition = _pendingMotionEvent->getPosition();
		components::Transform::Position mousePos3D;
		mousePos3D[0] = mousePosition[0];
		mousePos3D[1] = mousePosition[1];
		mousePos3D[2] = 0.0f;

		const auto mouseRay =
			_renderer.getViewRayFromScreenPosition(mousePos3D);
		_renderer.setLastMouseRay(mouseRay);

		const auto worldMousePos = intersectRayWithUiPlane(mouseRay);
		if (worldMousePos) {
			_renderer.setLastMousePosition(*worldMousePos);
		}

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
			if (changedButtons.test(buttonIndex)) {
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

		const auto &bound =
			componentRegistry.getComponent<components::Bound>(entityIdentifier);
		const auto worldMousePos = _renderer.getLastMousePosition();
		const auto worldTransform =
			calculateWorldTransform(componentRegistry, entityIdentifier);
		const auto size				  = bound.getSize();
		const auto worldMousePosition = components::Transform::Position(
			{ worldMousePos[0], worldMousePos[1], worldMousePos[2] });

		const auto trueCenter = computeEntityBoundCenter(
			worldTransform.position, size, worldTransform.scale);
		const bool isInside = isPointInsideEntityBounds(
			worldMousePosition, trueCenter, size, worldTransform.scale,
			worldTransform.orientation);

		processHover(componentRegistry, entityIdentifier, isInside);
		processClick(componentRegistry, entityIdentifier, isInside);
	}

}	 // namespace guillaume::systems