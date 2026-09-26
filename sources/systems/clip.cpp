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

#include "guillaume/systems/clip.hpp"

namespace guillaume::systems
{
	Clip::Clip(std::unique_ptr<utility::Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Clip>(ecs::Phase::Layout)
		, _engine(engine)
	{
	}

	void Clip::prepare(void)
	{
		_engine->clearScissor();
	}

	components::Clip::Rect Clip::computeRect(
		const utility::graphic::PoseF &pose, float width, float height,
		float margin)
	{
		components::Clip::Rect rect;

		rect.x		= pose.getPosition().getX() + margin;
		rect.y		= pose.getPosition().getY() + margin;
		rect.width	= width - 2.0f * margin;
		rect.height = height - 2.0f * margin;

		return rect;
	}

	void Clip::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Clip>(entityIdentifier)) {
			return;
		}

		auto &clip = getComponent<components::Clip>(entityIdentifier);
		if (!clip.isEnabled()) {
			return;
		}

		const auto &pose =
			getComponent<components::Transform>(entityIdentifier).getPose();
		const auto &bound = getComponent<components::Bound>(entityIdentifier);

		const auto rect = computeRect(pose, bound.getWidth(),
									  bound.getHeight(), clip.getMargin());
		clip.setRect(rect);

		_engine->setScissor(utility::graphic::ScissorRect {
			rect.x, rect.y, rect.width, rect.height });
	}

}	 // namespace guillaume::systems
