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

#include "guillaume/systems/animation.hpp"

#include <algorithm>

namespace guillaume::systems
{
	Animation::Animation(void)
		: ecs::SystemFiller<components::Animation>(ecs::Phase::Event)
		, _lastTime(std::chrono::steady_clock::now())
		, _deltaTime(0.0f)
	{
	}

	void Animation::prepare(void)
	{
		const auto now = std::chrono::steady_clock::now();
		const std::chrono::duration<float> elapsed = now - _lastTime;
		_lastTime								   = now;
		_deltaTime								   = elapsed.count();
	}

	void Animation::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Animation>(entityIdentifier)) {
			return;
		}

		// Clamp large deltas (e.g. after a stall) so animations do not jump.
		const float deltaTime = std::clamp(_deltaTime, 0.0f, 0.1f);

		getComponent<components::Animation>(entityIdentifier)
			.advance(deltaTime);
	}

}	 // namespace guillaume::systems
