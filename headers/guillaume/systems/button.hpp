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

#include <chrono>
#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/button.hpp"
#include "guillaume/theme/typescale.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Input and content rules shared by every button variant: press,
	 * release, toggle, click and long press.
	 *
	 * A button is described by two components: components::Button (what
	 * every button has: icon, label, toggle, callbacks) and one variant
	 * component (components::CommonButton, components::IconButton,
	 * components::FloatingActionButton or
	 * components::ExtendedFloatingActionButton). This system owns the first
	 * half; the variant system of the same name owns colors and geometry.
	 *
	 * - **Reads**: components::Button, the hand and mouse hover and button
	 *   interaction components, components::StyleState (enabled or not).
	 * - **Writes**: the press start and selection of components::Button, the
	 *   name and style of the icon child's components::Glyph, the selected
	 *   flag of components::Accessibility. It then calls the restyle
	 *   function stored in components::Button, which is the `apply` of the
	 *   variant system, so the new state is drawn at once.
	 * - **Phase**: Event. `update` only calls syncContent.
	 *
	 * Most of the work does not wait for the frame: install wires the
	 * interaction components of the entity to the static functions press,
	 * release and hoverChanged, which run when the input systems deliver
	 * the event. The variant systems call install from their own `install`.
	 *
	 * @code
	 * // Select a toggle button from code, without a click and without
	 * // calling its callback.
	 * systems::Button::setSelected(registry, identifier, true);
	 * @endcode
	 *
	 * @note registerCoreSystems does not register this system, so `update`
	 * does not run each frame in a default application. syncContent is
	 * reached through release, setSelected and the callers that invoke it
	 * directly.
	 *
	 * @see guillaume::components::Button, guillaume::systems::CommonButton,
	 * guillaume::systems::IconButton, guillaume::entities::ButtonBase
	 */
	class Button: public ecs::SystemFiller<components::Button>
	{
		public:
		/**
		 * @brief Short-lived handle on the icon or label child of a button,
		 * used by the variant systems to style it.
		 *
		 * It holds a registry reference and an identifier, nothing else.
		 * Every setter writes one component of the child and does nothing
		 * when the child lacks that component, so a button without an icon
		 * or without a label needs no special case. Setters return the
		 * handle for chaining.
		 *
		 * @code
		 * const Button::Child icon(registry, button.getIcon());
		 * icon.setColor(contentColor).setFontSize(24.0f);
		 * @endcode
		 *
		 * The handle keeps a reference to the registry: do not store it
		 * beyond the function that created it.
		 */
		class Child
		{
			private:
			ecs::ComponentRegistry &_registry;		///< Component storage
			ecs::Entity::Identifier _identifier;	///< Child identifier

			public:
			/**
			 * @brief Create a handle on a child.
			 * @param registry The registry holding the components of the
			 * child. It must outlive the handle.
			 * @param identifier The child entity, as stored in
			 * components::Button. ecs::Entity::InvalidIdentifier is accepted
			 * and gives a handle whose setters do nothing.
			 */
			Child(ecs::ComponentRegistry &registry,
				  ecs::Entity::Identifier identifier);

			/**
			 * @brief Whether the child exists, so a caller can write
			 * `if (icon) { ... }` before measuring or placing it.
			 * @return True when the identifier is valid and the entity has a
			 * components::Transform in the registry.
			 */
			explicit operator bool(void) const;

			/**
			 * @brief Get the entity this handle points to.
			 * @return The child identifier, or
			 * ecs::Entity::InvalidIdentifier when the button has no such
			 * child.
			 */
			ecs::Entity::Identifier getIdentifier(void) const noexcept;

			/**
			 * @brief Write the color of the child's components::Color.
			 * @param color The icon or label color.
			 * @return This handle, for chaining.
			 */
			const Child &
				setColor(const utility::graphic::Color32Bit &color) const;

			/**
			 * @brief Write the font size of the child's components::Glyph.
			 *
			 * Meant for the icon child; a label has no Glyph and is left
			 * untouched (use setTypeRole for it).
			 * @param size The icon font size, in dp.
			 * @return This handle, for chaining.
			 */
			const Child &setFontSize(float size) const;

			/**
			 * @brief Give the label a role of the type scale and resolve it
			 * at once.
			 *
			 * Writes the role into the child's components::Typography,
			 * clears its font size override, then calls
			 * systems::Typography::apply so the font is up to date before
			 * the label is measured. Does nothing unless the child has both
			 * components::Typography and components::Text.
			 * @param role The type role, for example
			 * theme::TypeRole::LabelLarge.
			 * @return This handle, for chaining.
			 */
			const Child &setTypeRole(theme::TypeRole role) const;

			/**
			 * @brief Write the string of the child's components::Text.
			 * @param content The label to show.
			 * @return This handle, for chaining.
			 */
			const Child &setContent(const std::string &content) const;
		};

		/// Name of the glyph shown in place of the icon while the button is
		/// loading.
		static constexpr const char *LoadingGlyph = "progress_activity";

		/// How long a press must last for its release to count as a long
		/// press instead of a click.
		static constexpr std::chrono::milliseconds LongPressDuration { 500 };

		/**
		 * @brief Create the system for the Event phase, matching entities
		 * that have a components::Button.
		 */
		Button(void);

		/**
		 * @brief Default destructor.
		 */
		~Button(void) override = default;

		/**
		 * @brief Make an entity react to input as a button.
		 *
		 * Sets the handlers of its four interaction components: the A
		 * button of a hand and the left mouse button call press and
		 * release, and a hover or unhover of a hand or of the mouse calls
		 * hoverChanged. Call it once when the entity is built; the variant
		 * systems do it from their own `install`.
		 *
		 * The handlers capture the address of the registry, which must
		 * outlive the entity. They replace any handler already set for
		 * these buttons and for hover.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must already have
		 * components::HandButtonInteraction,
		 * components::MouseButtonInteraction,
		 * components::HandHoverInteraction and
		 * components::MouseHoverInteraction.
		 */
		static void install(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief React to a pointer or a hand entering or leaving the
		 * button: restyle it so the hover tint appears or disappears.
		 *
		 * A disabled button is left as it is. Called by the handlers install
		 * sets; the hover flag itself is written by the input systems.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void hoverChanged(ecs::ComponentRegistry &registry,
								 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief React to the button being pressed: record when the press
		 * started and restyle.
		 *
		 * The start time is what release compares against LongPressDuration.
		 * Ignored when the button is disabled or not visible. No callback is
		 * called here: a click is reported on release.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void press(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief React to the button being released: this is the click.
		 *
		 * In order: a toggle button flips its selection; the icon and the
		 * accessibility state are synced; the button is restyled; then one
		 * callback is called. It is the long-press callback when one is set
		 * and the press lasted at least LongPressDuration, and the click
		 * callback otherwise. Ignored when the button is disabled or not
		 * visible.
		 *
		 * Calling it directly simulates a click, which is how a test drives
		 * a button on a bare identifier. Without a preceding press it is
		 * always a click, never a long press.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void release(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Select or unselect a toggle button from code and show it
		 * at once.
		 *
		 * Writes the selection, syncs the icon and the accessibility state,
		 * and restyles. Unlike release, it calls no callback and works on a
		 * disabled button. systems::SegmentedButton uses it to keep its
		 * segments consistent.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @param selected True to select the button. The call does nothing
		 * when the button is not a toggle.
		 */
		static void setSelected(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier,
								bool selected);

		/**
		 * @brief Copy what components::Button says onto the icon child and
		 * the accessibility state.
		 *
		 * Writes the glyph name returned by activeIconGlyphName and the icon
		 * style into the icon child's components::Glyph (skipped when the
		 * button has no icon), and sets the selected flag of
		 * components::Accessibility, true only for a selected toggle. Call
		 * it after changing the icon, the loading flag or the selection of
		 * a components::Button directly.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must have a
		 * components::Accessibility.
		 */
		static void syncContent(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Recompute the colors and geometry of the button at once.
		 *
		 * Calls the function stored in components::Button by
		 * `setRestyle`, which is the `apply` of the variant system
		 * (for example systems::CommonButton::apply). Does nothing when no
		 * function is set. This is how this system redraws a button without
		 * knowing its variant.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void restyle(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Choose the icon glyph a button shows in its current state.
		 * @param button The button state.
		 * @return LoadingGlyph while the button is loading; else the
		 * selected glyph name when the button is selected and has one; else
		 * its regular glyph name, which is empty for a button without an
		 * icon. The reference points into `button` or to a static string.
		 */
		static const std::string &
			activeIconGlyphName(const components::Button &button);

		/**
		 * @brief Whether the button ignores input and is drawn disabled.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must have a
		 * components::StyleState.
		 * @return True when its components::StyleState is not enabled.
		 */
		static bool isDisabled(const ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Whether a pointer is over the button.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must have
		 * components::HandHoverInteraction and
		 * components::MouseHoverInteraction.
		 * @return True when a hand or the mouse hovers it.
		 */
		static bool isHovered(const ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Whether the button is being held down.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity. It must have
		 * components::HandButtonInteraction and
		 * components::MouseButtonInteraction.
		 * @return True while the A button of a hand or the left mouse
		 * button is pressed on it.
		 */
		static bool isPressed(const ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Let a custom container color replace the themed one.
		 *
		 * Variant systems pass every container color through it before
		 * writing components::Color.
		 * @param button The button state.
		 * @param color The color the theme gives for the current state.
		 * @return The container color override of the button when one is
		 * set, `color` otherwise.
		 */
		static utility::graphic::Color32Bit
			resolveContainerColor(const components::Button &button,
								  const utility::graphic::Color32Bit &color);

		/**
		 * @brief Let a custom content color replace the themed one.
		 *
		 * Variant systems pass the icon and label color through it before
		 * writing the children.
		 * @param button The button state.
		 * @param color The color the theme gives for the current state.
		 * @return The content color override of the button when one is set,
		 * `color` otherwise.
		 */
		static utility::graphic::Color32Bit
			resolveContentColor(const components::Button &button,
								const utility::graphic::Color32Bit &color);

		/**
		 * @brief Make a small button as easy to hit as a large one, without
		 * drawing it larger.
		 *
		 * Sets the accessibility margin of the hand and mouse hover
		 * components to what each axis lacks to reach the minimum touch
		 * target of the theme (theme::SpacingScale::touchMargin, rounded
		 * up), and to 0 on an axis that is already large enough. Variant
		 * systems call it each time they resize the button.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @param width The drawn width of the button, in dp.
		 * @param height The drawn height of the button, in dp.
		 */
		static void applyTouchTarget(ecs::ComponentRegistry &registry,
									 const ecs::Entity::Identifier &identifier,
									 float width, float height);

		/**
		 * @brief Frame entry point: call syncContent on one button.
		 * @param entityIdentifier The button entity. An entity without a
		 * components::Button is logged and skipped.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
