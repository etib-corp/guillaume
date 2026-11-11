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

#include "guillaume/systems/interaction_helpers.hpp"

#include <memory>
#include <utility>

#include "guillaume/components/focus.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/systems/style_helpers.hpp"

namespace guillaume::systems::interaction
{
	namespace
	{
		template<typename Component>
		Component &ensure(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &entityIdentifier)
		{
			if (!registry.hasComponent<Component>(entityIdentifier)) {
				registry.addComponent<Component>(entityIdentifier);
			}
			return registry.getComponent<Component>(entityIdentifier);
		}
	}	 // namespace

	void makeInteractive(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 components::AccessibilityRole role,
						 std::function<void(void)> onPress,
						 std::function<void(void)> onStateChange)
	{
		ensure<components::StyleState>(registry, entityIdentifier);
		ensure<components::Focus>(registry, entityIdentifier);
		ensure<components::Accessibility>(registry, entityIdentifier)
			.setRole(role);

		auto press =
			std::make_shared<std::function<void(void)>>(std::move(onPress));
		auto restyle = std::make_shared<std::function<void(void)>>(
			std::move(onStateChange));
		ecs::ComponentRegistry *registryPtr		 = &registry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		const auto changed = [restyle]() {
			if (*restyle) {
				(*restyle)();
			}
		};
		const auto released = [press, restyle, registryPtr, identifier]() {
			if (!isEnabled(*registryPtr, identifier)) {
				return;
			}
			if (*press) {
				(*press)();
			}
			if (*restyle) {
				(*restyle)();
			}
		};

		ensure<components::MouseHoverInteraction>(registry, entityIdentifier)
			.setOnHoverHandler(changed)
			.setOnUnhoverHandler(changed);
		ensure<components::HandHoverInteraction>(registry, entityIdentifier)
			.setOnHoverHandler(changed)
			.setOnUnhoverHandler(changed);
		ensure<components::MouseButtonInteraction>(registry, entityIdentifier)
			.setOnButtonPressHandler(
				utility::event::MouseButtonEvent::Button::Left, changed)
			.setOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left, released);
		ensure<components::HandButtonInteraction>(registry, entityIdentifier)
			.setOnButtonPressHandler(utility::event::HandButtonEvent::Button::A,
									 changed)
			.setOnButtonReleaseHandler(
				utility::event::HandButtonEvent::Button::A, released);
	}

	void setEnabled(ecs::ComponentRegistry &registry,
					const ecs::Entity::Identifier &entityIdentifier,
					bool enabled)
	{
		ensure<components::StyleState>(registry, entityIdentifier)
			.setEnabled(enabled);
	}

	bool isEnabled(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier)
	{
		return !registry.hasComponent<components::StyleState>(entityIdentifier)
			|| registry.getComponent<components::StyleState>(entityIdentifier)
				   .isEnabled();
	}

	bool isHovered(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!isEnabled(registry, entityIdentifier)) {
			return false;
		}
		if (registry.hasComponent<components::MouseHoverInteraction>(
				entityIdentifier)
			&& registry
				   .getComponent<components::MouseHoverInteraction>(
					   entityIdentifier)
				   .isHovered()) {
			return true;
		}
		return registry.hasComponent<components::HandHoverInteraction>(
				   entityIdentifier)
			&& registry
				   .getComponent<components::HandHoverInteraction>(
					   entityIdentifier)
				   .isHovered();
	}

	bool isPressed(const ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!isEnabled(registry, entityIdentifier)) {
			return false;
		}
		if (registry.hasComponent<components::MouseButtonInteraction>(
				entityIdentifier)
			&& registry
				   .getComponent<components::MouseButtonInteraction>(
					   entityIdentifier)
				   .isButtonPressed(
					   utility::event::MouseButtonEvent::Button::Left)) {
			return true;
		}
		return registry.hasComponent<components::HandButtonInteraction>(
				   entityIdentifier)
			&& registry
				   .getComponent<components::HandButtonInteraction>(
					   entityIdentifier)
				   .isButtonPressed(utility::event::HandButtonEvent::Button::A);
	}

	utility::graphic::Color32Bit
		stateLayerColor(const ecs::ComponentRegistry &registry,
						const ecs::Entity::Identifier &entityIdentifier,
						const utility::graphic::Color32Bit &base,
						const utility::graphic::Color32Bit &content)
	{
		if (!isEnabled(registry, entityIdentifier)) {
			return base;
		}

		std::optional<theme::StateLayer> layer;
		if (registry.hasComponent<components::StyleState>(entityIdentifier)) {
			layer =
				registry.getComponent<components::StyleState>(entityIdentifier)
					.getActiveLayer();
		}
		// The interaction components are authoritative within the frame the
		// StyleState system has not yet mirrored.
		if (isPressed(registry, entityIdentifier)) {
			layer = theme::StateLayer::Pressed;
		} else if (!layer.has_value()
				   && isHovered(registry, entityIdentifier)) {
			layer = theme::StateLayer::Hovered;
		}

		if (!layer.has_value()) {
			return base;
		}

		// A transparent container (text/outlined styles) shows the layer over
		// nothing, so the result is the content color at the layer opacity.
		if (base.getAlpha() == 0) {
			return style::withAlpha(content, getStateLayerAlpha(*layer));
		}
		return style::stateLayer(base, content, *layer);
	}

}	 // namespace guillaume::systems::interaction
