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

#include "guillaume/theme/tokens.hpp"

#include <cmath>
#include <stdexcept>

namespace guillaume::theme
{
	namespace
	{
		template<typename Enum> std::size_t toIndex(Enum value)
		{
			const auto index = static_cast<std::size_t>(value);
			if (index >= static_cast<std::size_t>(Enum::Count)) {
				throw std::out_of_range("Invalid theme token");
			}
			return index;
		}
	}	 // namespace

	float ShapeScale::getRoundness(void) const
	{
		return _roundness;
	}

	void ShapeScale::setRoundness(float roundness)
	{
		if (roundness < 0.0f) {
			throw std::invalid_argument("Roundness must be non-negative");
		}
		_roundness = roundness;
	}

	float ShapeScale::getRadius(ShapeRole role) const
	{
		static constexpr std::array<float,
									static_cast<std::size_t>(ShapeRole::Count)>
			base { 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 28.0f, FullRadius };

		const auto index = toIndex(role);
		if (role == ShapeRole::None || role == ShapeRole::Full) {
			return base[index];
		}
		return base[index] * (_roundness / DefaultRoundness);
	}

	ElevationScale::ElevationScale(void)
		: _tokens { {
			  { 0.00f, 0.0f, 0 },
			  { 0.05f, 3.0f, 48 },
			  { 0.08f, 6.0f, 56 },
			  { 0.11f, 8.0f, 64 },
			  { 0.12f, 10.0f, 72 },
			  { 0.14f, 12.0f, 80 },
		  } }
	{
	}

	const ElevationToken &ElevationScale::get(ElevationLevel level) const
	{
		return _tokens[toIndex(level)];
	}

	void ElevationScale::set(ElevationLevel level, const ElevationToken &token)
	{
		_tokens[toIndex(level)] = token;
	}

	utility::graphic::Color32Bit
		ElevationScale::tint(const utility::graphic::Color32Bit &surface,
							 const utility::graphic::Color32Bit &tint,
							 ElevationLevel level) const
	{
		const float opacity = get(level).tintOpacity;
		const auto alpha =
			static_cast<std::uint8_t>(std::lround(opacity * 255.0f));
		return tint.withAlpha(alpha).blendOver(surface);
	}

	StateLayerScale::StateLayerScale(void)
		: _opacities { 0.08f, 0.12f, 0.12f, 0.16f, 0.38f, 0.12f }
	{
	}

	float StateLayerScale::getOpacity(StateLayer layer) const
	{
		return _opacities[toIndex(layer)];
	}

	std::uint8_t StateLayerScale::getAlpha(StateLayer layer) const
	{
		return static_cast<std::uint8_t>(
			std::lround(getOpacity(layer) * 255.0f));
	}

	void StateLayerScale::setOpacity(StateLayer layer, float opacity)
	{
		const auto index = toIndex(layer);
		if (opacity < 0.0f || opacity > 1.0f) {
			throw std::invalid_argument("Opacity must be within [0, 1]");
		}
		_opacities[index] = opacity;
	}

	MotionScale::MotionScale(void)
		: _durations { 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f,
					   0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f,
					   0.70f, 0.80f, 0.90f, 1.00f }
	{
	}

	float MotionScale::getDuration(MotionDuration duration) const
	{
		return _durations[toIndex(duration)];
	}

	void MotionScale::setDuration(MotionDuration duration, float seconds)
	{
		const auto index = toIndex(duration);
		if (seconds <= 0.0f) {
			throw std::invalid_argument("Duration must be positive");
		}
		_durations[index] = seconds;
	}

}	 // namespace guillaume::theme
