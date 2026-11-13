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

#include <utility>
#include <cstdint>
#include <memory>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/components/animation.hpp"
#include "guillaume/components/backdrop.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/scrim.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_filler.hpp"
#include "guillaume/systems/backdrop.hpp"

namespace guillaume::entities
{

	/**
	 * @brief The translucent veil painted behind a dialog or a modal drawer
	 * to dim the rest of the scene.
	 *
	 * Backdrop is a leaf widget: it has no children. Its state lives in
	 * components::Backdrop (visibility and fade-in progress);
	 * systems::Backdrop turns it into the color of components::Scrim each
	 * frame, in the Layout phase, and systems::ScrimRender draws the quad.
	 * components::Animation drives the fade-in: it lasts the Medium2 motion
	 * duration of the theme, with the enter easing. The class itself holds
	 * no field.
	 *
	 * Applications rarely create one: Dialog and NavigationDrawer each build
	 * their own backdrop, and their systems show, hide and place it every
	 * frame. Backdrop has no Builder; `Backdrop::create(registry)` is the
	 * only way to build one.
	 *
	 * @code
	 * auto backdrop = Backdrop::create(registry);
	 * backdrop->setParent(owner);
	 * backdrop->follow(ownerPose, ownerWidth, ownerHeight);
	 * backdrop->setVisible(true);
	 * @endcode
	 *
	 * Watch out:
	 * - The quad is `Extent` dp wide and high, so that it covers any
	 *   viewport. follow() must be called again when the owner moves or
	 *   changes size.
	 * - Hiding is immediate: only showing is animated.
	 * - create() does not register the backdrop: add it to an entity
	 *   registry, or keep the returned pointer alive.
	 *
	 * @see components::Backdrop, systems::Backdrop, Dialog
	 */
	class Backdrop:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Scrim, components::Animation,
								 components::Backdrop>
	{
		public:
		/**
		 * @brief Side of the backdrop quad, in dp.
		 */
		static constexpr float Extent = systems::Backdrop::Extent;

		/**
		 * @brief Build a hidden backdrop without registering it in an
		 * entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry)`. A backdrop has no children to build.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new backdrop. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Backdrop> create(Arguments &&...arguments)
		{
			return std::shared_ptr<Backdrop>(
				new Backdrop(std::forward<Arguments>(arguments)...));
		}

		/**
		 * @brief Destructor.
		 */
		~Backdrop(void) override = default;

		/**
		 * @brief Show or hide the backdrop.
		 *
		 * Showing restarts the fade-in from transparent. Hiding stops the
		 * animation and makes the backdrop transparent at once. The Scrim
		 * color is updated before the call returns. Passing the current
		 * visibility does nothing.
		 * @param visible True to show the backdrop, false to hide it.
		 */
		void setVisible(bool visible);

		/**
		 * @brief Check whether the backdrop is shown.
		 * @return True from the moment setVisible(true) is called, even
		 * while the fade-in is still running.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Centre the backdrop on its owner, half a layer behind it.
		 *
		 * Writes the pose of components::Transform once; the backdrop does
		 * not track the owner afterwards.
		 * @param ownerPose The pose of the owner (its top-left corner). The
		 * backdrop takes its orientation.
		 * @param ownerWidth The width of the owner, in dp.
		 * @param ownerHeight The height of the owner, in dp.
		 */
		void follow(const utility::graphic::PoseF &ownerPose, float ownerWidth,
					float ownerHeight);

		private:
		/**
		 * @brief Construct a hidden backdrop: an `Extent`-sided quad with a
		 * transparent Scrim and the fade-in animation configured.
		 * @param registry The registry the components are stored in. It
		 * must outlive the backdrop: the animation handler keeps a
		 * reference to it.
		 */
		explicit Backdrop(ecs::ComponentRegistry &registry);
	};

}	 // namespace guillaume::entities
