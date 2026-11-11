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
	 * @brief A timer that runs from 0 to a duration and reports its eased
	 * progress, from 0 to 1, to a callback on every frame.
	 *
	 * The component does not know what it animates: the sample handler
	 * receives the progress and interpolates whatever it wants (a width, a
	 * color, a pose). systems::Animation calls advance() on every Animation
	 * in the Event phase with the time of the frame, capped at
	 * theme::MotionScale::MaxFrameStep.
	 *
	 * @code
	 * registry.addComponent<components::Animation>(identifier)
	 * 	.setDuration(0.2f)
	 * 	.setEasing(motion::Easing::Standard)
	 * 	.setOnSampleHandler([&registry, identifier](float progress) {
	 * 		registry.getComponent<components::Bound>(identifier)
	 * 			.setWidth(40.0f + 60.0f * progress);
	 * 	})
	 * 	.play();
	 * @endcode
	 *
	 * Nothing runs until play() is called. A playing animation marks the
	 * component changed on every frame, so a looping one never lets the frame
	 * settle. The handlers run inside the Event phase of the frame.
	 *
	 * @see systems::Animation, motion::Easing
	 */
	class Animation: public ecs::Component
	{
		public:
		using SampleHandler =
			std::function<void(float)>;	   ///< Receives the eased progress.
		using CompleteHandler =
			std::function<void(void)>;	  ///< Called on completion.

		private:
		float _duration { 0.0f };	 ///< Duration, in seconds.
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
		 * @brief Construct a stopped animation lasting the Material 3
		 * `Medium2` motion duration of the theme.
		 */
		Animation(void);

		/**
		 * @brief Default destructor.
		 */
		~Animation(void) = default;

		/**
		 * @brief Get how long one run of the animation lasts.
		 * @return The duration, in seconds.
		 */
		float getDuration(void) const;

		/**
		 * @brief Set how long one run of the animation lasts.
		 *
		 * Marks the component changed only when the duration differs.
		 *
		 * @param duration The duration, in seconds. With 0 or less the
		 * animation completes on its first advance().
		 * @return This component, to chain calls.
		 */
		Animation &setDuration(float duration);

		/**
		 * @brief Get the time spent in the current run.
		 * @return The elapsed time, in seconds; 0 when stopped.
		 */
		float getElapsed(void) const;

		/**
		 * @brief Move the animation to a point in time, without calling any
		 * handler.
		 *
		 * Marks the component changed only when the time differs.
		 *
		 * @param elapsed The time spent in the current run, in seconds.
		 * @return This component, to chain calls.
		 */
		Animation &setElapsed(float elapsed);

		/**
		 * @brief Get how far the current run is, before easing.
		 * @return The elapsed time divided by the duration, clamped to 0 to
		 * 1. With a duration of 0 or less: 0 while playing, 1 otherwise.
		 */
		float getProgress(void) const;

		/**
		 * @brief Check whether the animation is running.
		 * @return True between play() and the end of the run or stop().
		 */
		bool isPlaying(void) const;

		/**
		 * @brief Start the animation from the beginning, even when it is
		 * already running.
		 *
		 * Always marks the component changed. The first sample is delivered
		 * by the next advance().
		 *
		 * @return This component, to chain calls.
		 */
		Animation &play(void);

		/**
		 * @brief Stop the animation and bring the elapsed time back to 0.
		 *
		 * Calls neither the sample handler nor the completion handler. Always
		 * marks the component changed.
		 *
		 * @return This component, to chain calls.
		 */
		Animation &stop(void);

		/**
		 * @brief Check whether the animation starts again when it completes.
		 * @return True when looping.
		 */
		bool isLooping(void) const;

		/**
		 * @brief Set whether the animation starts again when it completes.
		 *
		 * Marks the component changed only when the flag differs.
		 *
		 * @param looping True to restart from 0 after each completion; the
		 * completion handler still runs at the end of every run.
		 * @return This component, to chain calls.
		 */
		Animation &setLooping(bool looping);

		/**
		 * @brief Get the curve that turns the linear progress into the value
		 * passed to the sample handler.
		 * @return The easing curve; motion::Easing::Standard by default.
		 */
		motion::Easing getEasing(void) const;

		/**
		 * @brief Set the curve that turns the linear progress into the value
		 * passed to the sample handler.
		 *
		 * Marks the component changed only when the curve differs.
		 *
		 * @param easing The easing curve.
		 * @return This component, to chain calls.
		 */
		Animation &setEasing(motion::Easing easing);

		/**
		 * @brief Store the callback that applies the animation.
		 *
		 * advance() calls it with the eased progress, 0 to 1, while the
		 * animation plays, and with exactly 1 on completion. Does not mark
		 * the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		Animation &setOnSampleHandler(const SampleHandler &handler);

		/**
		 * @brief Get the callback that applies the animation.
		 * @return A copy of the handler; empty when none was set.
		 */
		SampleHandler getOnSampleHandler(void) const;

		/**
		 * @brief Store the callback run when the animation reaches its
		 * duration.
		 *
		 * It runs after the last sample, at the end of every run of a looping
		 * animation. Does not mark the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		Animation &setOnCompleteHandler(const CompleteHandler &handler);

		/**
		 * @brief Get the callback run when the animation reaches its duration.
		 * @return A copy of the handler; empty when none was set.
		 */
		CompleteHandler getOnCompleteHandler(void) const;

		/**
		 * @brief Move the animation forward in time and call the handlers.
		 *
		 * Called once per frame by systems::Animation. Does nothing when the
		 * animation is not playing. Otherwise it adds the delta to the elapsed
		 * time, marks the component changed and calls the sample handler with
		 * the eased progress. When the duration is reached, the sample handler
		 * receives 1, the completion handler runs, then the animation stops,
		 * or restarts from 0 when looping.
		 *
		 * @param deltaTime The time since the previous frame, in seconds. A
		 * negative value counts as 0.
		 */
		void advance(float deltaTime);
	};

}	 // namespace guillaume::components
