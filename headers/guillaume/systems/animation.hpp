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
	 * @brief System advancing time-based animations each frame.
	 *
	 * The per-frame delta time is taken from the engine (`Engine::getDeltaTime`)
	 * when it provides one, otherwise it is derived from an internal monotonic
	 * clock. Each `components::Animation` is then advanced by that delta.
	 *
	 * @see components::Animation
	 */
	class Animation: public ecs::SystemFiller<components::Animation>
	{
		private:
		std::unique_ptr<utility::Engine> &_engine;	 ///< Engine instance.
		std::chrono::steady_clock::time_point
			_lastTime;	  ///< Last frame time for the fallback clock.
		float _deltaTime { 0.0f };	  ///< Delta time for the current frame.

		public:
		/**
		 * @brief Construct an animation system running in the Event phase.
		 * @param engine The engine providing the frame delta time.
		 */
		Animation(std::unique_ptr<utility::Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~Animation(void) override = default;

		/**
		 * @brief Compute the delta time for the current frame.
		 */
		void prepare(void) override;

		/**
		 * @brief Advance one animation entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
