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

#include <functional>

#include "guillaume/ecs/component.hpp"
#include "guillaume/motion/easing.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component holding a time-based animation state.
	 *
	 * The `systems::Animation` system advances the animation each frame and
	 * calls the sample handler with the eased progress in `[0, 1]`, letting the
	 * owning entity interpolate whichever value it needs (float, color, pose).
	 *
	 * @see systems::Animation
	 */
	class Animation: public ecs::Component
	{
		public:
		using SampleHandler =
			std::function<void(float)>;	   ///< Eased progress handler.
		using CompleteHandler =
			std::function<void(void)>;	  ///< Completion handler.

		private:
		float _duration { 0.3f };	 ///< Duration, in seconds.
		float _elapsed { 0.0f };	 ///< Elapsed time, in seconds.
		bool _playing { false };	 ///< Whether the animation is running.
		bool _looping { false };	 ///< Whether the animation loops.
		motion::Easing _easing {
			motion::Easing::Standard
		};	  ///< Easing curve.
		SampleHandler _onSample {};		   ///< Eased progress handler.
		CompleteHandler _onComplete {};	   ///< Completion handler.

		public:
		/**
		 * @brief Default constructor for the Animation component.
		 */
		Animation(void) = default;

		/**
		 * @brief Default destructor for the Animation component.
		 */
		~Animation(void) = default;

		/**
		 * @brief Get the animation duration.
		 * @return The duration, in seconds.
		 */
		float getDuration(void) const;

		/**
		 * @brief Set the animation duration.
		 * @param duration The new duration, in seconds.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &setDuration(float duration);

		/**
		 * @brief Get the elapsed time.
		 * @return The elapsed time, in seconds.
		 */
		float getElapsed(void) const;

		/**
		 * @brief Set the elapsed time.
		 * @param elapsed The new elapsed time, in seconds.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &setElapsed(float elapsed);

		/**
		 * @brief Get the linear progress within `[0, 1]`.
		 * @return The progress.
		 */
		float getProgress(void) const;

		/**
		 * @brief Check whether the animation is running.
		 * @return True when playing.
		 */
		bool isPlaying(void) const;

		/**
		 * @brief Start (or restart) the animation.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &play(void);

		/**
		 * @brief Stop the animation and reset the elapsed time.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &stop(void);

		/**
		 * @brief Check whether the animation loops.
		 * @return True when looping.
		 */
		bool isLooping(void) const;

		/**
		 * @brief Set whether the animation loops.
		 * @param looping Whether the animation should loop.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &setLooping(bool looping);

		/**
		 * @brief Get the easing curve.
		 * @return The easing curve.
		 */
		motion::Easing getEasing(void) const;

		/**
		 * @brief Set the easing curve.
		 * @param easing The new easing curve.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &setEasing(motion::Easing easing);

		/**
		 * @brief Set the eased progress handler.
		 * @param handler The handler to call with the eased progress.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &setOnSampleHandler(const SampleHandler &handler);

		/**
		 * @brief Get the eased progress handler.
		 * @return The eased progress handler.
		 */
		SampleHandler getOnSampleHandler(void) const;

		/**
		 * @brief Set the completion handler.
		 * @param handler The handler to call when the animation completes.
		 * @return Reference to this Animation for chaining.
		 */
		Animation &setOnCompleteHandler(const CompleteHandler &handler);

		/**
		 * @brief Get the completion handler.
		 * @return The completion handler.
		 */
		CompleteHandler getOnCompleteHandler(void) const;

		/**
		 * @brief Advance the animation by a time delta.
		 *
		 * Does nothing when the animation is not playing. On completion the
		 * eased progress reaches 1, the sample handler is called a final time,
		 * the completion handler runs and the animation stops (or restarts when
		 * looping).
		 *
		 * @param deltaTime The elapsed time since the previous frame, in
		 * seconds.
		 */
		void advance(float deltaTime);
	};

}	 // namespace guillaume::components
