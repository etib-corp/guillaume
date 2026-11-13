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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/button.hpp"
#include "guillaume/components/icon_button.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Colors, sizes and lays out an icon button: a round button that
	 * shows a single icon and no label.
	 *
	 * This is the variant half of a button; systems::Button owns the input
	 * half (press, toggle, callbacks) and calls back into `apply` through
	 * the restyle function of components::Button.
	 *
	 * - **Reads**: components::IconButton (variant and size),
	 *   components::Button (toggle, selection, color overrides, icon
	 *   child), the hover, press and enabled state, the components::Bound
	 *   of the measured icon, the active theme.
	 * - **Writes**: the button's components::Color, components::Borders and
	 *   components::Bound (a square of 32, 40 or 48 dp for Small, Medium
	 *   and Large), the hover margin that makes up the touch target, and
	 *   the color, font size and components::Transform of the icon child.
	 * - **Phase**: Layout, registered after systems::Layout. The icon was
	 *   measured in the Measure phase of the same frame.
	 *
	 * The size of the button is fixed by its size setting: any width or
	 * height written to its components::Bound elsewhere is overwritten each
	 * frame.
	 *
	 * @see guillaume::components::IconButton, guillaume::systems::Button,
	 * guillaume::entities::IconButton
	 */
	class IconButton:
		public ecs::SystemFiller<components::IconButton, components::Button>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have both a components::IconButton and a components::Button.
		 */
		IconButton(void);

		/**
		 * @brief Default destructor.
		 */
		~IconButton(void) override = default;

		/**
		 * @brief Make an entity behave as an icon button.
		 *
		 * Adds a components::Elevation at level 0 when the entity has none,
		 * wires input through systems::Button::install, and stores `apply`
		 * in components::Button as the restyle function, so a hover, a
		 * press or a toggle redraws the button without waiting for the
		 * Layout phase. Call it once when the entity is built;
		 * entities::IconButton does it for you.
		 * @param registry The registry holding the components. It must
		 * outlive the entity.
		 * @param identifier The button entity. It must already have its
		 * components::Button and the hand and mouse hover and button
		 * interaction components.
		 */
		static void install(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the font size of the icon for the configured size.
		 *
		 * The widget class uses it to create the icon child at the right
		 * size.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The font size, in dp: 18 for Small, 24 for Medium, 30 for
		 * Large.
		 * @throws std::runtime_error If the configured size is not a valid
		 * enumerator.
		 */
		static float iconFontSize(const ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the color of the icon of the button when it is
		 * enabled.
		 *
		 * It depends on the variant and on whether the button counts as
		 * active: a selected button always does, and a Filled or Tonal
		 * button that is not a toggle does too.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The content color override of components::Button when one
		 * is set, the themed color otherwise. The disabled color is not
		 * taken into account.
		 * @throws std::runtime_error If the configured variant is not a
		 * valid enumerator.
		 */
		static utility::graphic::Color32Bit
			contentColor(const ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief First half of `apply`: write the colors.
		 *
		 * - Disabled: the container takes the disabled container color for
		 *   the Filled and Tonal variants and is transparent for the
		 *   others; only the Outlined variant keeps a (disabled) outline;
		 *   the icon takes the disabled content color.
		 * - Enabled: container and icon colors come from the variant and
		 *   the selection, replaced by the color overrides of
		 *   components::Button when set. While pressed, or else hovered,
		 *   the content color is blended over the container at the theme
		 *   opacity of that state; over a transparent container the tint
		 *   is the content color at that opacity. Only an unselected
		 *   Outlined button has a visible outline.
		 *
		 * The outline width is always written as 1 dp; a button without an
		 * outline gets a transparent outline color.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void styleState(ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Second half of `apply`: write the size and the corner
		 * radius of the button and center its icon.
		 *
		 * The button becomes a square of the container size of its size
		 * setting with fully rounded corners, and its touch target is
		 * grown through systems::Button::applyTouchTarget. The icon, when
		 * there is one, gets its font size and is centered from its
		 * measured components::Bound, in front of the container.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void geometry(ecs::ComponentRegistry &registry,
							 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Style and lay out one button at once: styleState, then
		 * geometry.
		 *
		 * The frame calls it through `update`. Call it yourself after
		 * changing components::IconButton or components::Button when the
		 * result must be visible before the next Layout phase;
		 * systems::Button does on every hover, press and release.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must have the components
		 * listed in the class description.
		 * @throws std::runtime_error If the configured size or variant is
		 * not a valid enumerator.
		 */
		static void apply(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Frame entry point: call `apply` on one button.
		 * @param entityIdentifier The button entity. An entity missing
		 * components::IconButton or components::Button is logged and
		 * skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
