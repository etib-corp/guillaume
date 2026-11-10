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

#include "guillaume/systems/divider.hpp"

#include <algorithm>

namespace guillaume::systems
{
	namespace
	{
		/// Shift a pose along X.
		utility::graphic::PoseF shifted(const utility::graphic::PoseF &pose,
										float offset)
		{
			auto result = pose;
			result.setPosition(utility::graphic::PositionF(
				pose.getPosition().getX() + offset, pose.getPosition().getY(),
				pose.getPosition().getZ()));
			return result;
		}
	}	 // namespace

	Divider::Divider(void)
		: ecs::SystemFiller<components::Divider, components::Transform,
							components::Bound, components::Color,
							components::Line>(ecs::Phase::Measure)
	{
	}

	void Divider::apply(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &divider = componentRegistry.getComponent<components::Divider>(
			entityIdentifier);
		auto &transform = componentRegistry.getComponent<components::Transform>(
			entityIdentifier);

		const float leading	 = divider.getLeadingInset();
		const float trailing = divider.getTrailingInset();

		// Something else moved the entity: adopt the new position. The
		// Transform holds the origin shifted by the leading inset, so shift
		// it back to recover the origin.
		if (transform.getPose() != divider.getAppliedPose()) {
			divider.setOriginPose(shifted(transform.getPose(), -leading));
		}

		const auto pose = shifted(divider.getOriginPose(), leading);
		transform.setPose(pose);
		divider.setAppliedPose(pose);

		componentRegistry.getComponent<components::Bound>(entityIdentifier)
			.setWidth(std::max(0.0f, divider.getLength() - leading - trailing))
			.setHeight(divider.getThickness());

		componentRegistry.getComponent<components::Line>(entityIdentifier)
			.setThickness(divider.getThickness());

		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(divider.getColor());
	}

	void Divider::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Divider>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)
			|| !requireComponent<components::Line>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
