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

namespace guillaume::systems
{

	void Interaction::updateLastInputEvents(void)
	{
		while (_mouseButtonSubscriber.hasPendingEvents()) {
			_lastMouseButtonEvent = _mouseButtonSubscriber.getNextEvent();
		}
		while (_mouseMotionSubscriber.hasPendingEvents()) {
			_lastMouseMotionEvent = _mouseMotionSubscriber.getNextEvent();
		}
		while (_handButtonSubscriber.hasPendingEvents()) {
			_lastHandButtonEvent =
				_handButtonSubscriber.getNextEvent();
		}
		while (_handMotionSubscriber.hasPendingEvents()) {
			_lastHandMotionEvent =
				_handMotionSubscriber.getNextEvent();
		}
		while (_handPinchSubscriber.hasPendingEvents()) {
			_lastHandPinchEvent = _handPinchSubscriber.getNextEvent();
		}
		while (_handPokeSubscriber.hasPendingEvents()) {
			_lastHandPokeEvent = _handPokeSubscriber.getNextEvent();
		}
	}

	void Interaction::processMouseHover(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto &interaction =
			getComponentRegistry().getComponent<components::Interaction>(
				entityIdentifier);

		if (isInside) {
			if (interaction.isMouseHovered()) {
				return;
			}

			interaction.setMouseHovered(true);
			const auto onHover = interaction.getMouseOnHoverHandler();
			if (onHover) {
				onHover();
			}
			return;
		}

		if (!interaction.isMouseHovered()) {
			return;
		}

		interaction.setMouseHovered(false);
		if (interaction.getMouseOnUnhoverHandler()) {
			interaction.getMouseOnUnhoverHandler()();
		}
	}

	void Interaction::processMouseButtonClick(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (_lastMouseButtonEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponentRegistry().getComponent<components::Interaction>(
				entityIdentifier);

		for (std::size_t buttonIndex = 0;
			 buttonIndex < static_cast<std::size_t>(
				 utility::event::MouseButtonEvent::Button::Last);
			 ++buttonIndex) {
			if (!_lastMouseButtonEvent->getButtonsState().test(buttonIndex)) {
				continue;
			}

			const auto button =
				static_cast<utility::event::MouseButtonEvent::Button>(
					buttonIndex);
			const bool isPressed =
				_lastMouseButtonEvent->getButtonsState().test(buttonIndex);

			if (isPressed && isInside) {
				interaction.setMouseButtonClicked(button, true);
				const auto &onClickHandler =
					interaction.getMouseButtonOnClickHandlers().at(button);
				if (onClickHandler) {
					onClickHandler();
				}
				break;
			}

			if (!isPressed && interaction.isMouseButtonClicked(button)
				&& isInside) {
				interaction.setMouseButtonClicked(button, false);
				const auto &onReleaseHandler =
					interaction.getMouseButtonOnClickReleaseHandlers().at(
						button);
				if (onReleaseHandler) {
					onReleaseHandler();
				}
				break;
			}

			if (!isPressed) {
				interaction.setMouseButtonClicked(button, false);
			}
		}
	}

	void Interaction::processHandHover(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (_lastHandMotionEvent == nullptr) {
			return;
		}
	}

	void Interaction::processHandButtonClick(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (_lastHandButtonEvent == nullptr) {
			return;
		}
	}

	void Interaction::processHandPinch(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (_lastHandPinchEvent == nullptr) {
			return;
		}
	}

	void Interaction::processHandPoke(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		if (_lastHandPokeEvent == nullptr) {
			return;
		}
	}

	Interaction::Interaction(event::EventBus &eventBus, Renderer &renderer)
		: ecs::SystemFiller<components::Interaction, components::Transform,
							components::Bound>(ecs::System::Phase::Event)
		, _mouseButtonSubscriber(eventBus)
		, _mouseMotionSubscriber(eventBus)
		, _handButtonSubscriber(eventBus)
		, _handMotionSubscriber(eventBus)
		, _handPinchSubscriber(eventBus)
		, _handPokeSubscriber(eventBus)
		, _renderer(renderer)
	{
	}

	void Interaction::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating Interaction system for entity "
						  + std::to_string(entityIdentifier));

		if (!requireComponent<components::Interaction>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)) {
			return;
		}

		bool isInside = false;

		updateLastInputEvents();

		processMouseHover(entityIdentifier, isInside);

		processMouseButtonClick(entityIdentifier, isInside);

		processHandHover(entityIdentifier, isInside);

		processHandButtonClick(entityIdentifier, isInside);

		processHandPinch(entityIdentifier, isInside);

		processHandPoke(entityIdentifier, isInside);
	}

}	 // namespace guillaume::systems