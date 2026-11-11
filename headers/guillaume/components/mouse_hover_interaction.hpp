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
	 * @brief Whether the mouse pointer is over an entity, what to call when it
	 * enters or leaves, and how far the hit area extends past the bound.
	 *
	 * systems::MouseMotion writes the hovered flag in the Event phase: on each
	 * mouse motion event it casts a ray from the pointer and tests it against
	 * the entity's Transform and Bound, grown by the accessibility margin. It
	 * calls the hover handler once when the pointer enters and the unhover
	 * handler once when it leaves. An entity disabled through StyleState is
	 * never hovered. systems::StyleState copies the flag into
	 * StyleState::isHovered.
	 *
	 * @code
	 * registry.addComponent<components::MouseHoverInteraction>(identifier)
	 * 	.setOnHoverHandler([] { showHint(); })
	 * 	.setOnUnhoverHandler([] { hideHint(); });
	 * @endcode
	 *
	 * The accessibility margin of this component is the hit margin of the
	 * entity for every pointer and hand test (hover, buttons, pinch, poke),
	 * not only for the mouse hover. The flag only changes in a frame that
	 * received a mouse motion event.
	 *
	 * @see systems::MouseMotion, HandHoverInteraction, StyleState
	 */
	class MouseHoverInteraction: public ecs::Component
	{
		public:
		using HoverHandler =
			std::function<void(void)>;	  ///< Called on hover start
		using UnhoverHandler =
			std::function<void(void)>;	  ///< Called on hover end

		private:
		HoverHandler _onHover;		  ///< Called when the pointer enters
		UnhoverHandler _onUnhover;	  ///< Called when the pointer leaves
		bool _isHovered { false };	  ///< Pointer is over the entity

		utility::graphic::SizeUI _accessibilityMargin {
			0, 0
		};	  ///< Hit area added on each side of the bound, in dp (width:
			  ///< left and right, height: top and bottom)

		public:
		/**
		 * @brief Construct a component that is not hovered, has no handler and
		 * no margin.
		 */
		MouseHoverInteraction(void);

		/**
		 * @brief Default destructor.
		 */
		~MouseHoverInteraction(void) = default;

		/**
		 * @brief Store the callback run when the pointer enters the entity.
		 *
		 * Always marks the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		MouseHoverInteraction &setOnHoverHandler(const HoverHandler &handler);
		/**
		 * @brief Store the callback run when the pointer leaves the entity.
		 *
		 * Always marks the component changed.
		 *
		 * @param handler The callback, or `nullptr` to remove the current one.
		 * @return This component, to chain calls.
		 */
		MouseHoverInteraction &
			setOnUnhoverHandler(const UnhoverHandler &handler);

		/**
		 * @brief Get the callback run when the pointer enters the entity.
		 * @return A copy of the handler; empty (`nullptr`) when none was set.
		 */
		HoverHandler getOnHoverHandler(void) const;

		/**
		 * @brief Get the callback run when the pointer leaves the entity.
		 * @return A copy of the handler; empty (`nullptr`) when none was set.
		 */
		UnhoverHandler getOnUnhoverHandler(void) const;

		/**
		 * @brief Check whether the pointer is over the entity.
		 * @return True from the motion event that entered the hit area until
		 * the one that left it.
		 */
		bool isHovered(void) const;

		/**
		 * @brief Record whether the pointer is over the entity.
		 *
		 * Called by systems::MouseMotion; it calls no handler. Marks the
		 * component changed only when the flag differs.
		 *
		 * @param isHovered True when the pointer is over the entity.
		 * @return This component, to chain calls.
		 */
		MouseHoverInteraction &setHovered(bool isHovered);

		/**
		 * @brief Get the hit area added around the bound.
		 * @return The margin, in dp: the width is added on the left and on the
		 * right, the height above and below.
		 */
		utility::graphic::SizeUI getAccessibilityMargin(void) const;

		/**
		 * @brief Grow the hit area past the bound without growing the entity
		 * on screen.
		 *
		 * Widget systems use it to give a small control the 48 dp touch
		 * target: a 40 dp wide control gets a margin of 4. Marks the component
		 * changed only when the margin differs.
		 *
		 * @param margin The margin, in dp: the width is added on the left and
		 * on the right, the height above and below.
		 * @return This component, to chain calls.
		 */
		MouseHoverInteraction &
			setAccessibilityMargin(const utility::graphic::SizeUI &margin);
	};

}	 // namespace guillaume::components
