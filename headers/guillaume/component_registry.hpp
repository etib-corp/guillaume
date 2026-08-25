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

#pragma once

#include "guillaume/ecs/component_registry_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/hand_squeeze_interaction.hpp"
#include "guillaume/components/hand_pinch_interaction.hpp"
#include "guillaume/components/hand_poke_interaction.hpp"
#include "guillaume/components/hand_thumb_rest_interaction.hpp"
#include "guillaume/components/hand_thumb_stick_interaction.hpp"
#include "guillaume/components/hand_trigger_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume
{

	/**
	 * @brief Component registry class registering all core components.
	 * @see ecs::ComponentRegistryFiller
	 */
	class ComponentRegistry:
		public ecs::ComponentRegistryFiller<
			components::Bound, components::Focus,
			components::MouseHoverInteraction,
			components::MouseButtonInteraction,
			components::HandHoverInteraction, components::HandButtonInteraction,
			components::HandPinchInteraction, components::HandPokeInteraction,
			components::HandSqueezeInteraction,
			components::HandThumbRestInteraction,
			components::HandThumbStickInteraction,
			components::HandTriggerInteraction, components::Text,
			components::Transform, components::Color, components::Borders>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		ComponentRegistry(void);

		/**
		 * @brief Default destructor.
		 */
		virtual ~ComponentRegistry(void) = default;
	};

}	 // namespace guillaume