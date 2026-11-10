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

#include <utility/graphic/ray.hpp>

#include "guillaume/systems/hand_event_system.hpp"
#include "guillaume/systems/hit_test.hpp"

namespace guillaume::systems
{
	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	HandEventSystem<InteractionType, EventType>::HandEventSystem(
		event::EventBus &eventBus)
		: ecs::SystemFiller<InteractionType, components::Transform,
							components::Bound>(ecs::Phase::Event)
		, event::EventManager<EventType>(eventBus)
	{
	}

	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	void HandEventSystem<InteractionType, EventType>::prepare(void)
	{
		this->consumeNextEvent();
	}

	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	void HandEventSystem<InteractionType, EventType>::cleanup(void)
	{
	}

	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	void HandEventSystem<InteractionType, EventType>::update(
		const ecs::Entity::Identifier &)
	{
	}

	template<ecs::InheritFromComponent InteractionType,
			 utility::event::InheritFromEvent EventType>
	void HandRaySystem<InteractionType, EventType>::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		auto handEvent = this->getLastEvent();
		if (!handEvent)
			return;

		auto &transform = this->template getComponent<components::Transform>(
			entityIdentifier);
		auto &bound =
			this->template getComponent<components::Bound>(entityIdentifier);
		auto &interaction =
			this->template getComponent<InteractionType>(entityIdentifier);

		const auto ray = utility::graphic::RayF(
			utility::graphic::PositionF(handEvent->getPose().getPosition()),
			handEvent->getPose().getOrientation().getForward());

		const bool isIntersecting =
			isInteractive(this->getComponentRegistry(), entityIdentifier)
			&& hitTest(
				ray, transform.getPose(), bound,
				hitMargin(this->getComponentRegistry(), entityIdentifier));

		setEngaged(interaction, isIntersecting);
		if (isIntersecting) {
			const auto &handler = interaction.getOnEventHandler();
			if (handler) {
				handler();
			}
		}
	}
}	 // namespace guillaume::systems
