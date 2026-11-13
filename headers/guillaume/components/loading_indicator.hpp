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

#include <optional>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a loading indicator.
	 *
	 * This component is the whole state of a loading indicator: its
	 * configuration (variant, diameter, color override, whether it hides when
	 * stopped), its animated child, and the transient values of its looping
	 * animation and of its fade. systems::LoadingIndicator turns it into the
	 * size of the widget and into the geometry and color of the child.
	 */
	class LoadingIndicator: public ecs::Component
	{
		public:
		/**
		 * @brief Loading indicator visual variant.
		 */
		enum class Variant {
			Linear,		///< Linear sliding bar.
			Circular	///< Circular rotating arc.
		};

		/// Length of the linear variant, in dp.
		static constexpr float LinearWidth = 200.0f;

		/// Thickness of the linear variant, in dp.
		static constexpr float LinearHeight = 4.0f;

		/// Length of the sliding bar, as a fraction of the linear length.
		static constexpr float LinearSegment = 0.4f;

		/// Default diameter of the circular variant, in dp.
		static constexpr float DefaultSize = 48.0f;

		/// Stroke width of the arc, as a fraction of the diameter (M3: 4 dp
		/// at 48 dp).
		static constexpr float StrokeRatio = 4.0f / 48.0f;

		/**
		 * @brief Default constructor.
		 */
		LoadingIndicator(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~LoadingIndicator(void) = default;

		/** @brief Get: visual variant. @return The value. */
		Variant getVariant(void) const noexcept;

		/** @brief Set: visual variant. @param variant The value. @return It. */
		LoadingIndicator &setVariant(Variant variant);

		/** @brief Get: circular diameter, in dp. @return The value. */
		float getSize(void) const noexcept;

		/** @brief Set: circular diameter, in dp. @param size The value.
		 * @return It. */
		LoadingIndicator &setSize(float size);

		/** @brief Get: animated child. @return The value. */
		ecs::Entity::Identifier getIndicator(void) const noexcept;

		/** @brief Set: animated child. @param identifier The value. @return
		 * It. */
		LoadingIndicator &setIndicator(ecs::Entity::Identifier identifier);

		/** @brief Get: whether the animated child has been built for the
		 * current variant. @return The value. */
		bool isBuilt(void) const noexcept;

		/** @brief Set: whether the animated child has been built for the
		 * current variant. @param built The value. @return It. */
		LoadingIndicator &setBuilt(bool built);

		/** @brief Get: latest eased progress of the looping animation, in
		 * [0, 1]. @return The value. */
		float getProgress(void) const noexcept;

		/** @brief Set: latest eased progress of the looping animation.
		 * @param progress The value. @return It. */
		LoadingIndicator &setProgress(float progress);

		/** @brief Get: whether the indicator fades out when stopped (Paper
		 * `hidesWhenStopped`). @return The value. */
		bool hidesWhenStopped(void) const noexcept;

		/** @brief Set: whether the indicator fades out when stopped.
		 * @param hides The value. @return It. */
		LoadingIndicator &setHidesWhenStopped(bool hides);

		/** @brief Get: current fade opacity, in [0, 1]. @return The value. */
		float getOpacity(void) const noexcept;

		/** @brief Set: current fade opacity. @param opacity The value.
		 * @return It. */
		LoadingIndicator &setOpacity(float opacity);

		/** @brief Get: opacity when the fade began. @return The value. */
		float getOpacityFrom(void) const noexcept;

		/** @brief Set: opacity when the fade began. @param opacity The value.
		 * @return It. */
		LoadingIndicator &setOpacityFrom(float opacity);

		/** @brief Get: opacity the fade moves to. @return The value. */
		float getOpacityTarget(void) const noexcept;

		/** @brief Set: opacity the fade moves to. @param opacity The value.
		 * @return It. */
		LoadingIndicator &setOpacityTarget(float opacity);

		/** @brief Get: indicator color override, empty for the theme
		 * `Primary`. @return The value. */
		const std::optional<utility::graphic::Color32Bit> &
			getIndicatorColor(void) const noexcept;

		/** @brief Set: indicator color override. @param color The value.
		 * @return It. */
		LoadingIndicator &setIndicatorColor(
			const std::optional<utility::graphic::Color32Bit> &color);

		private:
		Variant _variant { Variant::Circular };	   ///< Visual variant
		float _size { DefaultSize };			   ///< Circular diameter
		ecs::Entity::Identifier _indicator {
			ecs::Entity::InvalidIdentifier
		};	  ///< Animated child

		bool _built { false };				///< Whether the child is built
		float _progress { 0.0f };			///< Looping animation progress
		bool _hidesWhenStopped { true };	///< Paper `hidesWhenStopped`
		float _opacity { 1.0f };			///< Current fade opacity
		float _opacityFrom { 1.0f };		///< Opacity when the fade began
		float _opacityTarget { 1.0f };		///< Opacity the fade moves to
		std::optional<utility::graphic::Color32Bit>
			_indicatorColor {};	   ///< Indicator color override
	};

}	 // namespace guillaume::components
