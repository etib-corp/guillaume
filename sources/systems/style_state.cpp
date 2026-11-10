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

#include "guillaume/systems/style_state.hpp"

#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/hand_pinch_interaction.hpp"
#include "guillaume/components/hand_poke_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"

namespace guillaume::systems
{
	StyleState::StyleState(void)
		: ecs::SystemFiller<components::StyleState>(ecs::Phase::Event)
	{
	}

	void StyleState::prepare(void)
	{
	}

	void StyleState::cleanup(void)
	{
	}

	void StyleState::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::StyleState>(entityIdentifier)) {
			return;
		}

		auto &state = getComponent<components::StyleState>(entityIdentifier);

		bool hovered = false;
		if (hasComponent<components::MouseHoverInteraction>(entityIdentifier)) {
			hovered = getComponent<components::MouseHoverInteraction>(
						  entityIdentifier)
						  .isHovered();
		}
		if (!hovered
			&& hasComponent<components::HandHoverInteraction>(
				entityIdentifier)) {
			hovered =
				getComponent<components::HandHoverInteraction>(entityIdentifier)
					.isHovered();
		}

		bool pressed = false;
		if (hasComponent<components::MouseButtonInteraction>(
				entityIdentifier)) {
			pressed = getComponent<components::MouseButtonInteraction>(
						  entityIdentifier)
						  .isButtonPressed(
							  utility::event::MouseButtonEvent::Button::Left);
		}
		if (!pressed
			&& hasComponent<components::HandButtonInteraction>(
				entityIdentifier)) {
			pressed = getComponent<components::HandButtonInteraction>(
						  entityIdentifier)
						  .isButtonPressed(
							  utility::event::HandButtonEvent::Button::A);
		}
		if (!pressed
			&& hasComponent<components::HandPinchInteraction>(
				entityIdentifier)) {
			pressed =
				getComponent<components::HandPinchInteraction>(entityIdentifier)
					.isPinch();
		}
		if (!pressed
			&& hasComponent<components::HandPokeInteraction>(
				entityIdentifier)) {
			pressed =
				getComponent<components::HandPokeInteraction>(entityIdentifier)
					.isPoke();
		}

		bool dragged = false;
		if (hasComponent<components::DragInteraction>(entityIdentifier)) {
			dragged =
				getComponent<components::DragInteraction>(entityIdentifier)
					.isDragging();
		}

		state.setHovered(hovered).setPressed(pressed).setDragged(dragged);
	}

}	 // namespace guillaume::systems
