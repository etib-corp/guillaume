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

#include "guillaume/entities/backdrop.hpp"

#include <cmath>

#include "guillaume/systems/backdrop.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Backdrop::Backdrop(ecs::ComponentRegistry &registry)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Scrim, components::Animation,
							components::Backdrop>(registry)
	{
		// The fade-in handler outlives this constructor: capture the
		// registry and the identifier, never the entity object.
		const auto identifier = getIdentifier();

		registry.getComponent<components::Bound>(identifier)
			.setWidth(Extent)
			.setHeight(Extent);
		registry.getComponent<components::Animation>(identifier)
			.setDuration(getMotionDuration(theme::MotionDuration::Medium2))
			.setEasing(theme::MotionScale::enter())
			.setOnSampleHandler([&registry, identifier](float progress) {
				registry.getComponent<components::Backdrop>(identifier)
					.setProgress(progress);
				systems::Backdrop::apply(registry, identifier);
			});
		systems::Backdrop::apply(registry, identifier);
	}

	void Backdrop::setVisible(bool visible)
	{
		systems::Backdrop::setVisible(getComponentRegistry(), getIdentifier(),
									  visible);
	}

	bool Backdrop::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Backdrop>(getIdentifier())
			.isVisible();
	}

	void Backdrop::follow(const utility::graphic::PoseF &ownerPose,
						  float ownerWidth, float ownerHeight)
	{
		systems::Backdrop::follow(getComponentRegistry(), getIdentifier(),
								  ownerPose, ownerWidth, ownerHeight);
	}

}	 // namespace guillaume::entities
