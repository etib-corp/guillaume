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

#include "guillaume/systems/focus.hpp"

namespace guillaume::systems
{

	Focus::Focus(event::EventBus &eventBus, std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Focus, components::Transform,
							components::Bound,
							components::MouseButtonInteraction,
							components::HandButtonInteraction,
							components::HandPinchInteraction,
							components::HandPokeInteraction>(ecs::Phase::Event)
		, _engine(engine)
		, _focusedEntity(std::nullopt)

	{
		this->getLogger().info() << "Focus system initialized";
	}

	void Focus::setFocus(const ecs::Entity::Identifier &entityIdentifier)
	{
		// Save the current focused entity as last focused before changing
		if (_focusedEntity.has_value()
			&& _focusedEntity.value() != entityIdentifier) {
			_lastFocusedEntity = _focusedEntity.value();
		}

		// Clear focus from previous entity
		if (_focusedEntity.has_value()
			&& _focusedEntity.value() != entityIdentifier) {
			auto previousEntity = _focusedEntity.value();
			if (this->template hasComponent<components::Focus>(
					previousEntity)) {
				auto &previousFocus =
					this->template getComponent<components::Focus>(
						previousEntity);
				auto handler = previousFocus.getOnFocusLostHandler();
				if (handler) {
					this->getLogger().debug()
						<< "Triggering focus lost handler for entity "
						<< previousEntity;
					handler();
				}
			}
		}

		// Set focus to new entity
		_focusedEntity = entityIdentifier;
		this->getLogger().info() << "Focus set to entity " << entityIdentifier;

		// Trigger focus handler for new entity
		if (this->template hasComponent<components::Focus>(entityIdentifier)) {
			auto &focus = this->template getComponent<components::Focus>(
				entityIdentifier);
			auto handler = focus.getOnFocusGainedHandler();
			if (handler) {
				this->getLogger().debug()
					<< "Triggering focus gained handler for entity "
					<< entityIdentifier;
				handler();
			}
		}
	}

	void Focus::clearFocus(void)
	{
		if (_focusedEntity.has_value()) {
			// Save the current focused entity as last focused before clearing
			_lastFocusedEntity = _focusedEntity.value();

			auto previousEntity = _focusedEntity.value();
			if (this->template hasComponent<components::Focus>(
					previousEntity)) {
				auto &previousFocus =
					this->template getComponent<components::Focus>(
						previousEntity);
				auto handler = previousFocus.getOnFocusLostHandler();
				if (handler) {
					this->getLogger().debug()
						<< "Triggering focus lost handler for entity "
						<< previousEntity;
					handler();
				}
			}
			this->getLogger().info()
				<< "Focus cleared from entity " << previousEntity;
			_focusedEntity = std::nullopt;
		}
	}

	std::optional<ecs::Entity::Identifier> Focus::getFocusedEntity(void) const
	{
		return _focusedEntity;
	}

	std::optional<ecs::Entity::Identifier>
		Focus::getLastFocusedEntity(void) const
	{
		return _lastFocusedEntity;
	}

	void Focus::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		this->getLogger().debug()
			<< "Updating Focus system for entity " << entityIdentifier;

		bool entityClicked = false;

		// Check if entity was clicked with mouse
		if (this->template hasComponent<components::MouseButtonInteraction>(
				entityIdentifier)) {
			auto &mouseInteraction =
				this->template getComponent<components::MouseButtonInteraction>(
					entityIdentifier);
			if (mouseInteraction.isButtonPressed(
					utility::event::MouseButtonEvent::Button::Left)) {
				entityClicked = true;
			}
		}

		// Check if entity was clicked with hand button
		if (!entityClicked
			&& this->template hasComponent<components::HandButtonInteraction>(
				entityIdentifier)) {
			auto &handInteraction =
				this->template getComponent<components::HandButtonInteraction>(
					entityIdentifier);
			// Check common hand buttons (A, B, X, Y)
			if (handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::A)
				|| handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::B)
				|| handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::X)
				|| handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::Y)) {
				entityClicked = true;
			}
		}

		// Check if entity was clicked with hand pinch
		if (!entityClicked
			&& this->template hasComponent<components::HandPinchInteraction>(
				entityIdentifier)) {
			auto &pinchInteraction =
				this->template getComponent<components::HandPinchInteraction>(
					entityIdentifier);
			if (pinchInteraction.isPinch()) {
				entityClicked = true;
			}
		}

		// Check if entity was clicked with hand poke
		if (!entityClicked
			&& this->template hasComponent<components::HandPokeInteraction>(
				entityIdentifier)) {
			auto &pokeInteraction =
				this->template getComponent<components::HandPokeInteraction>(
					entityIdentifier);
			if (pokeInteraction.isPoke()) {
				entityClicked = true;
			}
		}

		// If entity was clicked, set focus and disable viewport input
		if (entityClicked) {
			setFocus(entityIdentifier);
			if (_engine) {
				_engine->setShouldCaptureViewportInput(false);
			}
			return;
		}

		// If focus was cleared and we have an engine, re-enable viewport input
		if (!_focusedEntity.has_value() && _engine) {
			_engine->setShouldCaptureViewportInput(true);
		}
	}

}	 // namespace guillaume::systems
