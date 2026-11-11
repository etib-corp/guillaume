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
	 * @brief Drives modal backdrops.
	 *
	 * Each frame it resolves the Scrim color of every entity carrying a
	 * components::Backdrop from the theme's Backdrop role, the visibility and
	 * the fade-in progress. The static helpers hold the rest of the backdrop
	 * behavior (showing, hiding and following an owner), so the entity is
	 * only an identifier with components.
	 */
	class Backdrop:
		public ecs::SystemFiller<components::Backdrop, components::Scrim>
	{
		public:
		/// Width and height of a backdrop, large enough to cover any view.
		static constexpr float Extent = 8192.0f;

		/**
		 * @brief Construct the system.
		 */
		Backdrop(void);

		/**
		 * @brief Default destructor.
		 */
		~Backdrop(void) override = default;

		/**
		 * @brief Write the Scrim color for the current state right now.
		 * @param componentRegistry Registry holding the components.
		 * @param entityIdentifier Identifier of the backdrop entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show or hide a backdrop and start or stop its fade-in.
		 *
		 * The entity must also carry a components::Animation.
		 * @param componentRegistry Registry holding the components.
		 * @param entityIdentifier Identifier of the backdrop entity.
		 * @param visible True to show the backdrop.
		 */
		static void setVisible(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   bool visible);

		/**
		 * @brief Center a backdrop on its owner, just behind it.
		 * @param componentRegistry Registry holding the components.
		 * @param entityIdentifier Identifier of the backdrop entity.
		 * @param ownerPose Pose of the owner (its top-left corner).
		 * @param ownerWidth Width of the owner.
		 * @param ownerHeight Height of the owner.
		 */
		static void follow(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   const utility::graphic::PoseF &ownerPose,
						   float ownerWidth, float ownerHeight);

		/**
		 * @brief Resolve the Scrim color of one backdrop entity.
		 * @param entityIdentifier Identifier of the entity being updated.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
