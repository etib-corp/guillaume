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

#include "guillaume/systems/backdrop.hpp"

#include <cmath>
#include <cstdint>

#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	Backdrop::Backdrop(void)
		: ecs::SystemFiller<components::Backdrop, components::Scrim>(
			  ecs::Phase::Layout)
	{
	}

	void Backdrop::apply(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &state =
			componentRegistry.getComponent<components::Backdrop>(
				entityIdentifier);
		const auto backdrop = getActiveColor(SchemeColorRole::Backdrop);
		const float alpha	= state.isVisible()
			  ? static_cast<float>(backdrop.getAlpha()) * state.getProgress()
			  : 0.0f;

		componentRegistry.getComponent<components::Scrim>(entityIdentifier)
			.setColor(backdrop.withAlpha(
				static_cast<std::uint8_t>(std::lround(alpha))));
	}

	void Backdrop::setVisible(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  bool visible)
	{
		auto &state = componentRegistry.getComponent<components::Backdrop>(
			entityIdentifier);
		if (state.isVisible() == visible) {
			return;
		}
		state.setVisible(visible).setProgress(0.0f);

		auto &animation = componentRegistry.getComponent<components::Animation>(
			entityIdentifier);
		if (visible) {
			animation.setElapsed(0.0f).play();
		} else {
			animation.stop();
		}
		apply(componentRegistry, entityIdentifier);
	}

	void Backdrop::follow(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  const utility::graphic::PoseF &ownerPose,
						  float ownerWidth, float ownerHeight)
	{
		const auto &orientation				 = ownerPose.getOrientation();
		utility::graphic::PositionF position = ownerPose.getPosition();
		position.translate(utility::graphic::PositionF(
			-(orientation.getRight() * ((Extent - ownerWidth) / 2.0f))
			- (orientation.getUp() * ((Extent - ownerHeight) / 2.0f))
			+ orientation.getForward() * (Layout::LayerDepthStep * 0.5f)));

		componentRegistry.getComponent<components::Transform>(entityIdentifier)
			.setPose(utility::graphic::PoseF(position, orientation));
	}

	void Backdrop::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Backdrop>(entityIdentifier)
			|| !requireComponent<components::Scrim>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
