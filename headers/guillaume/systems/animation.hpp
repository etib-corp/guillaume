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

#include <chrono>
#include <memory>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/animation.hpp"

#include <utility/engine.hpp>

namespace guillaume::systems
{

	/**
	 * @brief Moves every running animation forward by the time of one frame.
	 *
	 * Runs in the Event phase on each entity with a components::Animation.
	 * It reads the frame time and calls components::Animation::advance, which
	 * updates the animation progress and calls its sample and completion
	 * handlers. The system writes nothing else: what an animation changes
	 * (a color, a position) is decided by the handlers set on the component.
	 *
	 * The frame time comes from `utility::Engine::getDeltaTime` when the
	 * engine reports a positive value, and from a steady clock otherwise. It
	 * is clamped to theme::MotionScale::MaxFrameStep, so a stalled frame
	 * slows an animation down instead of skipping it.
	 *
	 * @see components::Animation
	 */
	class Animation: public ecs::SystemFiller<components::Animation>
	{
		private:
		utility::Engine &_engine;	 ///< Engine asked for the frame time.
		std::chrono::steady_clock::time_point
			_lastTime;	  ///< When the fallback clock was last read.
		float _deltaTime {
			0.0f
		};	  ///< Time of the current frame, in seconds, before clamping.

		public:
		/**
		 * @brief Create the system, in the Event phase.
		 * @param engine The engine providing the frame time. It must outlive
		 * the system.
		 */
		Animation(utility::Engine &engine);

		/**
		 * @brief Default destructor.
		 */
		~Animation(void) override = default;

		/**
		 * @brief Read the time of the current frame, once per frame.
		 *
		 * Uses the engine's delta time when it is positive, otherwise the
		 * time elapsed on the steady clock since the previous fallback read.
		 */
		void prepare(void) override;

		/**
		 * @brief Advance the animation of one entity by the frame time,
		 * clamped to theme::MotionScale::MaxFrameStep.
		 * @param entityIdentifier The entity holding the
		 * components::Animation.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
