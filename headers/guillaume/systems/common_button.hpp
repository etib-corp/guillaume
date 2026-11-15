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
#include "guillaume/components/common_button.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Colors, sizes and lays out a common button: the elevated,
	 * filled, tonal, outlined and text buttons, and the segments of a
	 * segmented button.
	 *
	 * This is the variant half of a button; systems::Button owns the input
	 * half (press, toggle, callbacks) and calls back into `apply` through
	 * the restyle function of components::Button.
	 *
	 * - **Reads**: components::CommonButton (color style, shape, size,
	 *   compact, minimum width, uppercase, corner override),
	 *   components::Button (toggle, selection, label, color overrides, icon
	 *   and label children), the hover, press and enabled state, the
	 *   components::Bound of the measured icon and label, the active theme.
	 * - **Writes**: the button's components::Color, components::Borders,
	 *   components::Elevation and components::Bound, the hover margin that
	 *   makes up the touch target, and on the children: color, font, label
	 *   text and components::Transform.
	 * - **Phase**: Layout, registered after systems::Layout. The icon and
	 *   the label were measured in the Measure phase of the same frame.
	 *
	 * The button sets its own size from its content: any width or height
	 * written to its components::Bound elsewhere is overwritten each frame.
	 * Use `components::CommonButton::setMinWidth` to make it wider.
	 *
	 * @see guillaume::components::CommonButton, guillaume::systems::Button,
	 * guillaume::systems::button_style, guillaume::entities::Button
	 */
	class CommonButton:
		public ecs::SystemFiller<components::CommonButton, components::Button>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have both a components::CommonButton and a
		 * components::Button.
		 */
		CommonButton(void);

		/**
		 * @brief Default destructor.
		 */
		~CommonButton(void) override = default;

		/**
		 * @brief Make an entity behave as a common button.
		 *
		 * Adds a components::Elevation at level 0 when the entity has none,
		 * wires input through systems::Button::install, and stores `apply`
		 * in components::Button as the restyle function, so a hover, a
		 * press or a toggle redraws the button without waiting for the
		 * Layout phase. Call it once when the entity is built;
		 * entities::Button does it for you.
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
		 * @return The font size, in dp (see button_style::getIconFontSize).
		 * @throws std::runtime_error If the configured size is not a valid
		 * enumerator.
		 */
		static float iconFontSize(const ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the color of the icon and label of the button when it
		 * is enabled.
		 *
		 * It depends on the color style and, for a toggle, on the
		 * selection. The disabled color and the content color override of
		 * components::Button are not taken into account: styleState applies
		 * them.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The themed content color.
		 * @throws std::runtime_error If the configured color style is not a
		 * valid enumerator.
		 */
		static utility::graphic::Color32Bit
			contentColor(const ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief First half of `apply`: write the colors and the elevation.
		 *
		 * - Elevation: level 1 for an enabled Elevated button, level 2
		 *   while it is hovered and not pressed; level 0 for every other
		 *   style and for any disabled button.
		 * - Disabled: container and outline take the disabled container
		 *   color, icon and label the disabled content color.
		 * - Enabled: container, outline and content colors come from
		 *   button_style for the current hover, press and selection; the
		 *   color overrides of components::Button replace the container and
		 *   content colors.
		 *
		 * The outline width is always written as 1 dp; a style without an
		 * outline gets a transparent outline color.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void styleState(ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Second half of `apply`: write the corner radius, the size
		 * of the button and the position of its icon and label.
		 *
		 * - Corners: the per-corner override of components::CommonButton
		 *   when set, else one radius from the shape, the size and the
		 *   press (a pressed button, or a selected toggle that morphs,
		 *   changes shape).
		 * - Content: the icon gets the icon font size of the button size;
		 *   the label gets the LabelLarge type role and its text,
		 *   upper-cased when asked.
		 * - Size: the content plus the paddings of the size, at least
		 *   components::CommonButton::MinWidth wide and the configured
		 *   minimum width, and at least components::CommonButton::MinHeight
		 *   high except for the ExtraSmall size.
		 * - Placement: icon and label are centered as a group, the icon
		 *   first unless components::Button asks for a trailing icon.
		 *
		 * A button with neither an icon nor a label only gets its corners:
		 * its components::Bound is left untouched. The child bounds read
		 * here are those of the last Measure phase, so a label changed in
		 * the same call is sized from its previous text until the next
		 * frame.
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
		 * changing components::CommonButton or components::Button when the
		 * result must be visible before the next Layout phase; the setters
		 * of entities::Button do, and systems::Button does on every hover,
		 * press and release.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must have the components
		 * listed in the class description.
		 * @throws std::runtime_error If the configured size or color style
		 * is not a valid enumerator.
		 */
		static void apply(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Frame entry point: call `apply` on one button.
		 * @param entityIdentifier The button entity. An entity missing
		 * components::CommonButton or components::Button is logged and
		 * skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
