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

#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Backdrop::Backdrop(ecs::ComponentRegistry &registry)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Scrim, components::Animation>(registry)
	{
		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(Extent)
			.setHeight(Extent);
		getComponentRegistry()
			.getComponent<components::Animation>(getIdentifier())
			.setDuration(getMotionDuration(theme::MotionDuration::Medium2))
			.setEasing(theme::MotionScale::enter())
			.setOnSampleHandler([this](float progress) {
				_progress = progress;
				applyColor();
			});
		applyColor();
	}

	void Backdrop::setVisible(bool visible)
	{
		if (_visible == visible) {
			return;
		}
		_visible = visible;

		auto &animation =
			getComponentRegistry().getComponent<components::Animation>(
				getIdentifier());
		if (visible) {
			_progress = 0.0f;
			animation.setElapsed(0.0f).play();
		} else {
			animation.stop();
			_progress = 0.0f;
		}
		applyColor();
	}

	bool Backdrop::isVisible(void) const
	{
		return _visible;
	}

	void Backdrop::follow(const utility::graphic::PoseF &ownerPose,
						  float ownerWidth, float ownerHeight)
	{
		const auto &orientation				 = ownerPose.getOrientation();
		utility::graphic::PositionF position = ownerPose.getPosition();
		position.translate(utility::graphic::PositionF(
			-(orientation.getRight() * ((Extent - ownerWidth) / 2.0f))
			- (orientation.getUp() * ((Extent - ownerHeight) / 2.0f))
			+ orientation.getForward()
				* (systems::Layout::LayerDepthStep * 0.5f)));
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(utility::graphic::PoseF(position, orientation));
	}

	void Backdrop::initialize(void)
	{
	}

	void Backdrop::update(void)
	{
		applyColor();
	}

	void Backdrop::applyColor(void)
	{
		const auto backdrop = getActiveColor(SchemeColorRole::Backdrop);
		const float alpha	= _visible
			  ? static_cast<float>(backdrop.getAlpha()) * _progress
			  : 0.0f;
		getComponentRegistry()
			.getComponent<components::Scrim>(getIdentifier())
			.setColor(backdrop.withAlpha(
				static_cast<std::uint8_t>(std::lround(alpha))));
	}

}	 // namespace guillaume::entities
