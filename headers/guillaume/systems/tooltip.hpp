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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/tooltip.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a tooltip, runs its hover delays and places it.
	 *
	 * Reads components::Tooltip, the hover state of its anchor and the active
	 * theme, and writes the tooltip's Layout, Color, Borders, Elevation,
	 * Overlay, Accessibility and Transform, plus the content and color of the
	 * title and body children. Runs in the Layout phase, after the children
	 * have been measured.
	 */
	class Tooltip: public ecs::SystemFiller<components::Tooltip>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Tooltip(void);

		/**
		 * @brief Default destructor.
		 */
		~Tooltip(void) override = default;

		/**
		 * @brief Make an entity behave as a tooltip.
		 *
		 * Sets the accessibility semantics, configures the Animation as the
		 * looping ticker that advances the hover delays and wires the overlay
		 * dismissal to this system. Call once, after the entity has its
		 * components::Tooltip, Overlay, Elevation, Animation, Accessibility,
		 * Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one tooltip and arrange its children.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Attach the tooltip to the entity whose hover triggers it.
		 *
		 * Resets the hover timers and starts the ticker, or stops it when
		 * @p anchorIdentifier is `ecs::Entity::InvalidIdentifier`.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @param anchorIdentifier The trigger entity.
		 */
		static void setAnchor(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  ecs::Entity::Identifier anchorIdentifier);

		/**
		 * @brief Set the area the tooltip is kept inside, and place the
		 * tooltip again when it is shown.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @param width Area width.
		 * @param height Area height.
		 */
		static void setViewport(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								float width, float height);

		/**
		 * @brief Show the tooltip under its anchor.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the tooltip.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Advance the hover delays.
		 *
		 * Shows the tooltip once its anchor has been hovered for the enter
		 * delay and hides it once the anchor has been left for the leave
		 * delay. Does nothing while the anchor is gone.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @param deltaTime Elapsed seconds.
		 */
		static void advance(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							float deltaTime);

		/**
		 * @brief Place the tooltip under (or, when it would overflow the
		 * viewport, above) its anchor, horizontally centered and clamped.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 */
		static void place(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the tooltip has a live anchor.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The tooltip entity.
		 * @return False when the anchor is `ecs::Entity::InvalidIdentifier` or
		 * has no Transform in the registry.
		 */
		static bool hasAnchor(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the tooltip with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
