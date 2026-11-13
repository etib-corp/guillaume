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

#include <utility/graphic/size.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Whether an XR hand (controller) aims at an entity, what to call
	 * when the aim enters or leaves, and how far the hit area extends past the
	 * bound.
	 *
	 * systems::HandMotion writes the hovered flag in the Event phase: on each
	 * hand motion event it tests the aim ray of the hand against the entity's
	 * Transform and Bound, grown by the hit margin. It calls the hover handler
	 * once when the ray enters and the unhover handler once when it leaves. An
	 * entity disabled through StyleState is never hovered.
	 * systems::StyleState copies the flag into StyleState::isHovered.
	 *
	 * @code
	 * registry.addComponent<components::HandHoverInteraction>(identifier)
	 * 	.setOnHoverHandler([] { showHint(); })
	 * 	.setOnUnhoverHandler([] { hideHint(); });
	 * @endcode
	 *
	 * The accessibility margin stored here is only used when the entity has no
	 * MouseHoverInteraction: the margin of that component comes first. The
	 * flag only changes in a frame that received a hand motion event.
	 *
	 * @see systems::HandMotion, MouseHoverInteraction, StyleState
	 */
	class HandHoverInteraction: public ecs::Component
	{
		public:
		using HoverHandler =
			std::function<void(void)>;	  ///< Called on hover start
		using UnhoverHandler =
			std::function<void(void)>;	  ///< Called on hover end

		private:
		HoverHandler _onHover;		  ///< Called when the ray enters
		UnhoverHandler _onUnhover;	  ///< Called when the ray leaves
		bool _isHovered { false };	  ///< Whether a hand aims at the entity
		utility::graphic::SizeUI _accessibilityMargin {
			0, 0
		};	  ///< Hit area added on each side of the bound, in dp (width:
			  ///< left and right, height: top and bottom)

		public:
		/**
		 * @brief Construct a component that is not hovered, has no handler and
		 * no margin.
		 */
		HandHoverInteraction(void);

		/**
		 * @brief Default destructor.
		 */
		~HandHoverInteraction(void) = default;

		/**
		 * @brief Store the callback run when the hand ray enters the entity.
		 *
		 * Always marks the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		HandHoverInteraction &setOnHoverHandler(const HoverHandler &handler);

		/**
		 * @brief Store the callback run when the hand ray leaves the entity.
		 *
		 * Always marks the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		HandHoverInteraction &
			setOnUnhoverHandler(const UnhoverHandler &handler);

		/**
		 * @brief Get the callback run when the hand ray enters the entity.
		 * @return A copy of the handler; empty (`nullptr`) when none was set.
		 */
		HoverHandler getOnHoverHandler(void) const;

		/**
		 * @brief Get the callback run when the hand ray leaves the entity.
		 * @return A copy of the handler; empty (`nullptr`) when none was set.
		 */
		UnhoverHandler getOnUnhoverHandler(void) const;

		/**
		 * @brief Check whether a hand aims at the entity.
		 * @return True from the motion event whose ray entered the hit area
		 * until the one that left it.
		 */
		bool isHovered(void) const;

		/**
		 * @brief Record whether a hand aims at the entity.
		 *
		 * Called by systems::HandMotion; it calls no handler. Marks the
		 * component changed only when the flag differs.
		 *
		 * @param isHovered True when a hand ray hits the entity.
		 * @return This component, to chain calls.
		 */
		HandHoverInteraction &setHovered(bool isHovered);

		/**
		 * @brief Get the hit area added around the entity's bound.
		 * @return The margin, in dp: the width is added on the left and on the
		 * right, the height above and below.
		 */
		utility::graphic::SizeUI getAccessibilityMargin(void) const;

		/**
		 * @brief Grow the hit area past the entity's bound without growing the
		 * entity on screen.
		 *
		 * Ignored by the hit tests when the entity also has a
		 * MouseHoverInteraction; widget systems set the same margin on both.
		 * Marks the component changed only when the margin differs.
		 *
		 * @param margin The margin, in dp: the width is added on the left and
		 * on the right, the height above and below.
		 * @return This component, to chain calls.
		 */
		HandHoverInteraction &
			setAccessibilityMargin(const utility::graphic::SizeUI &margin);
	};

}	 // namespace guillaume::components
