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

#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/animation.hpp"
#include "guillaume/components/backdrop.hpp"
#include "guillaume/components/scrim.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Fades the backdrop of a modal widget in and keeps it behind
	 * its owner.
	 *
	 * A backdrop is the dimmed layer shown behind a dialog or a modal
	 * navigation drawer. Its state is a components::Backdrop (shown or not,
	 * fade-in progress from 0 to 1); what is drawn is a components::Scrim.
	 *
	 * Runs in the Layout phase on entities with Backdrop and Scrim. Each
	 * frame it reads the Backdrop and the Backdrop color of the active theme
	 * and writes the Scrim color: the theme color with its alpha multiplied
	 * by the progress, or fully transparent while the backdrop is hidden.
	 * systems::ScrimRender draws the result in the Render phase.
	 *
	 * The progress itself is advanced by systems::Animation, through the
	 * sample handler that entities::Backdrop installs on its
	 * components::Animation.
	 *
	 * The static functions are called by the system of the owner
	 * (systems::Dialog, systems::NavigationDrawer) when it opens, closes or
	 * moves:
	 * @code
	 * systems::Backdrop::setVisible(registry, backdrop, true);
	 * systems::Backdrop::follow(registry, backdrop, ownerPose, ownerWidth,
	 * 	ownerHeight);
	 * @endcode
	 *
	 * There is no fade-out: hiding a backdrop makes it transparent at once.
	 *
	 * @see components::Backdrop, components::Scrim
	 * @see entities::Backdrop, systems::ScrimRender
	 */
	class Backdrop:
		public ecs::SystemFiller<components::Backdrop, components::Scrim>
	{
		public:
		/// Width and height given to a backdrop, in dp: large enough to
		/// cover the view wherever its owner is.
		static constexpr float Extent = 8192.0f;

		/**
		 * @brief Create the system in the Layout phase.
		 */
		Backdrop(void);

		/**
		 * @brief Destroy the system. It owns no resource.
		 */
		~Backdrop(void) override = default;

		/**
		 * @brief Write the Scrim color that matches the current state of a
		 * backdrop.
		 *
		 * Call it after changing components::Backdrop by hand when the new
		 * color must be in place before the next frame.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The backdrop entity. It must have Backdrop
		 * and Scrim.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show or hide a backdrop.
		 *
		 * Showing resets the progress to 0 and starts the fade-in animation
		 * from its beginning. Hiding resets the progress, stops the
		 * animation and makes the scrim transparent at once. Nothing happens
		 * when the backdrop is already in the requested state.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The backdrop entity. It must have
		 * Backdrop, Scrim and components::Animation.
		 * @param visible True to show the backdrop, false to hide it.
		 */
		static void setVisible(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool visible);

		/**
		 * @brief Center a backdrop on its owner and place it half a layer
		 * behind it.
		 *
		 * Writes the backdrop's components::Transform, with the orientation
		 * of the owner. The computation assumes the backdrop is Extent wide
		 * and high. Call it whenever the owner moves or is resized.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The backdrop entity. It must have a
		 * Transform.
		 * @param ownerPose Pose of the top-left corner of the owner.
		 * @param ownerWidth Width of the owner, in dp.
		 * @param ownerHeight Height of the owner, in dp.
		 */
		static void follow(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   const utility::graphic::PoseF &ownerPose,
						   float ownerWidth, float ownerHeight);

		/**
		 * @brief Write the Scrim color of one backdrop for this frame, so a
		 * theme change is picked up without any call.
		 * @param entityIdentifier The backdrop entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
