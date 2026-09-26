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

#include "guillaume/components/animation.hpp"

#include <algorithm>

namespace guillaume::components
{
	float Animation::getDuration(void) const
	{
		return _duration;
	}

	Animation &Animation::setDuration(float duration)
	{
		if (_duration == duration) {
			return *this;
		}
		_duration = duration;
		setHasChanged(true);
		return *this;
	}

	float Animation::getElapsed(void) const
	{
		return _elapsed;
	}

	Animation &Animation::setElapsed(float elapsed)
	{
		if (_elapsed == elapsed) {
			return *this;
		}
		_elapsed = elapsed;
		setHasChanged(true);
		return *this;
	}

	float Animation::getProgress(void) const
	{
		if (_duration <= 0.0f) {
			return _playing ? 0.0f : 1.0f;
		}
		return std::clamp(_elapsed / _duration, 0.0f, 1.0f);
	}

	bool Animation::isPlaying(void) const
	{
		return _playing;
	}

	Animation &Animation::play(void)
	{
		_playing = true;
		_elapsed = 0.0f;
		setHasChanged(true);
		return *this;
	}

	Animation &Animation::stop(void)
	{
		_playing = false;
		_elapsed = 0.0f;
		setHasChanged(true);
		return *this;
	}

	bool Animation::isLooping(void) const
	{
		return _looping;
	}

	Animation &Animation::setLooping(bool looping)
	{
		if (_looping == looping) {
			return *this;
		}
		_looping = looping;
		setHasChanged(true);
		return *this;
	}

	motion::Easing Animation::getEasing(void) const
	{
		return _easing;
	}

	Animation &Animation::setEasing(motion::Easing easing)
	{
		if (_easing == easing) {
			return *this;
		}
		_easing = easing;
		setHasChanged(true);
		return *this;
	}

	Animation &Animation::setOnSampleHandler(const SampleHandler &handler)
	{
		_onSample = handler;
		return *this;
	}

	Animation::SampleHandler Animation::getOnSampleHandler(void) const
	{
		return _onSample;
	}

	Animation &Animation::setOnCompleteHandler(const CompleteHandler &handler)
	{
		_onComplete = handler;
		return *this;
	}

	Animation::CompleteHandler Animation::getOnCompleteHandler(void) const
	{
		return _onComplete;
	}

	void Animation::advance(float deltaTime)
	{
		if (!_playing) {
			return;
		}

		if (_duration <= 0.0f) {
			_playing = true;
			_elapsed = 0.0f;
			setHasChanged(true);
			if (_onSample) {
				_onSample(1.0f);
			}
			if (_onComplete) {
				_onComplete();
			}
			_playing = _looping;
			return;
		}

		_elapsed += std::max(0.0f, deltaTime);
		setHasChanged(true);

		if (_elapsed >= _duration) {
			_elapsed = _duration;
			if (_onSample) {
				_onSample(1.0f);
			}
			if (_onComplete) {
				_onComplete();
			}
			if (_looping) {
				_elapsed = 0.0f;
				_playing = true;
			} else {
				_playing = false;
			}
			return;
		}

		if (_onSample) {
			_onSample(motion::applyEasing(_easing, getProgress()));
		}
	}

}	 // namespace guillaume::components
