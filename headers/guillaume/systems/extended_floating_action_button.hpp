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
#include "guillaume/components/extended_floating_action_button.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Colors, sizes and lays out an extended floating action button:
	 * a FAB that shows a label beside its icon and can collapse to the icon
	 * alone.
	 *
	 * This is the variant half of a button; systems::Button owns the input
	 * half (press, callbacks) and calls back into `apply` through the
	 * restyle function of components::Button.
	 *
	 * - **Reads**: components::ExtendedFloatingActionButton (variant, size,
	 *   custom size, mode, lowered, extended), components::Button (visible,
	 *   label, color overrides, icon and label children), the hover, press
	 *   and enabled state, the components::Bound of the measured children,
	 *   the active theme.
	 * - **Writes**: the button's components::Color, components::Elevation,
	 *   components::Bound and the corner radius of its components::Borders,
	 *   the hover margin that makes up the touch target, and on the
	 *   children: color, font, label text and components::Transform.
	 * - **Phase**: Layout, registered after systems::Layout. The children
	 *   were measured in the Measure phase of the same frame.
	 *
	 * The width follows the label: 16 dp, the icon, 12 dp, the label, 20 dp.
	 * Collapsed (`setExtended(false)`), or when the label measures nothing,
	 * the button is a square holding the centered icon; the label child
	 * stays in the tree and is drawn transparent.
	 *
	 * @see guillaume::components::ExtendedFloatingActionButton,
	 * guillaume::systems::Button, guillaume::systems::fab_style,
	 * guillaume::systems::FloatingActionButton
	 */
	class ExtendedFloatingActionButton:
		public ecs::SystemFiller<components::ExtendedFloatingActionButton,
								 components::Button>
	{
		public:
		/**
		 * @brief Create the system for the Layout phase, matching entities
		 * that have both a components::ExtendedFloatingActionButton and a
		 * components::Button.
		 */
		ExtendedFloatingActionButton(void);

		/**
		 * @brief Default destructor.
		 */
		~ExtendedFloatingActionButton(void) override = default;

		/**
		 * @brief Make an entity behave as an extended floating action
		 * button.
		 *
		 * Adds a components::Elevation at level 0 when the entity has none,
		 * wires input through systems::Button::install, and stores `apply`
		 * in components::Button as the restyle function, so a hover or a
		 * press redraws the button without waiting for the Layout phase.
		 * Call it once when the entity is built;
		 * entities::ExtendedFloatingActionButton does it for you.
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
		 * In an extended FAB the icon uses the same font size as the label
		 * (fab_style::getLabelFontSize), not the icon size of a regular
		 * FAB. The widget class uses it to create the icon child at the
		 * right size.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The font size, in dp: 18 for Small, 24 for Medium, 36 for
		 * Large.
		 * @throws std::runtime_error If the configured size is not a valid
		 * enumerator.
		 */
		static float iconFontSize(const ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the color of the icon and label of the button when it
		 * is enabled and visible.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @return The content color override of components::Button when one
		 * is set, the color of the variant otherwise.
		 * @throws std::runtime_error If the configured variant is not a
		 * valid enumerator.
		 */
		static utility::graphic::Color32Bit
			contentColor(const ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Write the shadow level of the button into its
		 * components::Elevation.
		 *
		 * Level 3 at rest, or level 1 when the FAB is lowered; one level
		 * higher while hovered and not pressed; level 0 when the button is
		 * disabled, not visible or in the Flat mode. styleState calls it
		 * first, so `apply` covers it.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void elevation(ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief First half of `apply`: write the elevation and the colors.
		 *
		 * - Not visible: container, icon and label become fully
		 *   transparent.
		 * - Disabled: the container takes the disabled container color,
		 *   icon and label the disabled content color.
		 * - Otherwise: container and content colors come from fab_style for
		 *   the variant, replaced by the color overrides of
		 *   components::Button when set. While pressed, or else hovered,
		 *   the content color is blended over the container at the theme
		 *   opacity of that state.
		 *
		 * In every case the label is transparent while it is not shown,
		 * that is when the button is collapsed or the label measures a
		 * width of 0.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void styleState(ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Second half of `apply`: write the size and the corner
		 * radius of the button and place its icon and label.
		 *
		 * The height is the custom size of
		 * components::ExtendedFloatingActionButton when set, else the
		 * container size of the size setting; the corner radius is the
		 * custom size divided by 3.5, or the radius of the size setting.
		 * With the label shown, the width is 16 dp, the icon and a 12 dp
		 * gap when the icon measures something, the label, then 20 dp.
		 * Otherwise the button is a square of that height with the icon
		 * centered and the label hidden. The label gets the LabelLarge type
		 * role; icon and label share one font size.
		 *
		 * A button with neither an icon nor a label only gets its corner
		 * radius: its components::Bound is left untouched.
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
		 * changing components::ExtendedFloatingActionButton or
		 * components::Button when the result must be visible before the
		 * next Layout phase; systems::Button does on every hover, press and
		 * release.
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
		 * components::ExtendedFloatingActionButton or components::Button is
		 * logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
