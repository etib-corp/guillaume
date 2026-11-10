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

#include <cstdint>
#include <memory>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/components/animation.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/scrim.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_filler.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Modal backdrop painted behind a dialog or a modal drawer.
	 *
	 * The backdrop is a `components::Scrim` quad, `Backdrop::Extent` wide and
	 * high, centred on its owner and pushed half a layer behind it, so it
	 * covers any viewport. It fades in over the M3 medium2 duration with the
	 * emphasized-decelerate easing when shown and is transparent when hidden.
	 */
	class Backdrop:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Scrim, components::Animation>
	{
		public:
		/**
		 * @brief Side of the backdrop quad, in dp.
		 */
		static constexpr float Extent = 8192.0f;

		/**
		 * @brief Construct a hidden backdrop.
		 * @param registry Component registry.
		 */
		explicit Backdrop(ecs::ComponentRegistry &registry);

		/**
		 * @brief Default destructor.
		 */
		~Backdrop(void) override = default;

		/**
		 * @brief Show or hide the backdrop; showing starts the fade-in.
		 * @param visible New visibility.
		 */
		void setVisible(bool visible);

		/**
		 * @brief Check whether the backdrop is shown.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Centre the backdrop on its owner, half a layer behind it.
		 * @param ownerPose Owner pose (top-left corner).
		 * @param ownerWidth Owner width.
		 * @param ownerHeight Owner height.
		 */
		void follow(const utility::graphic::PoseF &ownerPose, float ownerWidth,
					float ownerHeight);

		/**
		 * @brief Nothing to initialize.
		 */
		void initialize(void) override;

		/**
		 * @brief Re-apply the current color.
		 */
		void update(void) override;

		private:
		bool _visible { false };	 ///< Whether the backdrop is shown.
		float _progress { 0.0f };	 ///< Fade-in progress in [0, 1].

		/**
		 * @brief Push the faded Backdrop color to the Scrim component.
		 */
		void applyColor(void);
	};

}	 // namespace guillaume::entities
