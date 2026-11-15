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
#include <functional>
#include <memory>
#include <string>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/common_button.hpp"
#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A Material 3 common button: a clickable container with a text
	 * label and an optional icon (elevated, filled, tonal, outlined or
	 * text).
	 *
	 * The state shared by every button (label, icon, toggle, callbacks) lives
	 * in components::Button, and systems::Button turns input into clicks in
	 * the Event phase. What is specific to this variant (color style, shape,
	 * size) lives in components::CommonButton; systems::CommonButton colors
	 * the container, sizes it and places the icon and the label each frame,
	 * in the Layout phase. The class itself holds no field: its setters
	 * write the component and restyle at once.
	 *
	 * @code
	 * Button::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto save = builder.makeButton(parent, "Save", [] {
	 * 	// called when the user clicks the button
	 * }, Button::Color::Filled, Button::Shape::Round, Button::Size::Small,
	 * 	false);
	 * save->setDisabled(true);
	 * @endcode
	 *
	 * `Button::create(registry, iconGlyphName, iconStyle, labelContent,
	 * isToggle, colorStyle, shape, size, isMorph, isDisabled, isTrailingIcon,
	 * accessibilityLabel, onClick)` builds the same widget without
	 * registering it in an entity registry.
	 *
	 * The button owns its icon and label children: it is the entity registry
	 * they are registered in. Its size is computed from its content and
	 * written to its Bound each frame, so a size set by hand on the Bound is
	 * overwritten. setSelected() does not call the click handler; only a
	 * user click does.
	 *
	 * @see ButtonBase, components::CommonButton, systems::CommonButton
	 */
	class Button: public ButtonBase
	{
		public:
		/// Color style: Elevated, Filled, Tonal, Outlined, Text or Segmented
		/// (see components::CommonButton::Color).
		using Color = components::CommonButton::Color;

		/// Corner shape: Round or Square (see
		/// components::CommonButton::Shape).
		using Shape = components::CommonButton::Shape;

		/// Size, from ExtraSmall to ExtraLarge (see
		/// components::CommonButton::Size).
		using Size = components::CommonButton::Size;

		/**
		 * @brief Configures, creates and registers Button widgets.
		 *
		 * Set options with the `with...` methods then call registerEntity(),
		 * or use makeButton() / makeIconButton() to do both in one call.
		 */
		class Builder: public EntityBuilderBase<Button>
		{
			private:
			std::string
				_iconGlyphName;	   ///< Icon glyph name; empty for no icon
			components::Glyph::Style _iconStyle;	///< Icon glyph style
			std::string _labelContent;				///< Label text
			std::function<void(void)>
				_onClick;			 ///< Handler called on a user click
			bool _isToggle;			 ///< Whether a click toggles the selection
			Color _colorStyle;		 ///< Color style
			Shape _shape;			 ///< Corner shape
			Size _size;				 ///< Size
			bool _isMorph;			 ///< Whether a selected toggle swaps shape
			bool _isDisabled;		 ///< Whether the button is disabled
			bool _isTrailingIcon;	 ///< Whether the icon is placed after the
									 ///< label
			std::string
				_accessibilityLabel;	///< Label read by assistive technology

			public:
			/**
			 * @brief Construct a builder with the default configuration (see
			 * reset()).
			 * @param componentRegistry The registry that stores the
			 * components of the buttons to build.
			 * @param entityRegistry The registry the built buttons are added
			 * to (usually the scene's).
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a Button, with its icon and label children, from
			 * the current configuration, without registering it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * button and resets the builder.
			 * @return The new button.
			 */
			std::shared_ptr<Button> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: no icon, Glyph::Style::Outlined, empty label, no
			 * click handler, not a toggle, Color::Text, Shape::Round,
			 * Size::Small, no morph, enabled, leading icon, empty
			 * accessibility label.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon shown next to the label.
			 * @param iconGlyphName The glyph name of the icon (for example
			 * "add"). Default: empty, no icon.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyphName);

			/**
			 * @brief Set the glyph style of the icon.
			 * @param iconStyle The glyph style. Default:
			 * Glyph::Style::Outlined.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIconStyle(const components::Glyph::Style &iconStyle);

			/**
			 * @brief Set the label text.
			 * @param labelContent The text shown in the button. Default:
			 * empty. It is also the accessibility label when none is set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &labelContent);

			/**
			 * @brief Set the click handler.
			 * @param onClick The function called when the user clicks the
			 * button. Default: empty, nothing is called.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnClick(std::function<void(void)> onClick);

			/**
			 * @brief Make the button a toggle: each click flips its selected
			 * state.
			 * @param isToggle True for a toggle button. Default: false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withToggle(const bool &isToggle);

			/**
			 * @brief Set the color style.
			 * @param colorStyle The color style. Default: Color::Text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColorStyle(const Color &colorStyle);

			/**
			 * @brief Set the corner shape.
			 * @param shape The corner shape. Default: Shape::Round.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withShape(const Shape &shape);

			/**
			 * @brief Set the size.
			 * @param size The size. Default: Size::Small.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSize(const Size &size);

			/**
			 * @brief Choose whether a selected toggle button swaps its shape
			 * (Round becomes Square and Square becomes Round).
			 * @param isMorph True to swap the shape while selected. Default:
			 * false. It has no effect on a button that is not a toggle.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMorph(const bool &isMorph);

			/**
			 * @brief Choose whether the button starts disabled.
			 * @param isDisabled True for a button that ignores input.
			 * Default: false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withDisabled(const bool &isDisabled);

			/**
			 * @brief Choose the side of the icon.
			 * @param isTrailingIcon True to place the icon after the label,
			 * false to place it before. Default: false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTrailingIcon(const bool &isTrailingIcon);

			/**
			 * @brief Set the label read by assistive technology.
			 * @param accessibilityLabel The accessibility label. Default:
			 * empty, in which case the label text is used.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withAccessibilityLabel(const std::string &accessibilityLabel);

			/**
			 * @brief Build a button with a label and no icon, register it
			 * under `parent` and reset the builder.
			 *
			 * Options set before the call with the other `with...` methods
			 * (toggle, disabled, ...) also apply.
			 * @param parent The entity the button becomes a child of.
			 * @param labelContent The text shown in the button.
			 * @param onClick The function called when the user clicks the
			 * button (may be empty).
			 * @param colorStyle The color style.
			 * @param shape The corner shape.
			 * @param size The size.
			 * @param isMorph True for a toggle button that swaps its shape
			 * while selected.
			 * @return The new button, registered in the entity registry.
			 */
			std::shared_ptr<Button>
				makeButton(std::shared_ptr<Entity> parent,
						   const std::string &labelContent,
						   std::function<void(void)> onClick, Color colorStyle,
						   Shape shape, Size size, bool isMorph);

			/**
			 * @brief Build a button with an icon and a label, register it
			 * under `parent` and reset the builder.
			 *
			 * Options set before the call with the other `with...` methods
			 * (toggle, trailing icon, ...) also apply. For a button that
			 * shows an icon only, see IconButton.
			 * @param parent The entity the button becomes a child of.
			 * @param labelContent The text shown in the button.
			 * @param iconGlyphName The glyph name of the icon.
			 * @param iconStyle The glyph style of the icon.
			 * @param onClick The function called when the user clicks the
			 * button (may be empty).
			 * @param colorStyle The color style.
			 * @param shape The corner shape.
			 * @param size The size.
			 * @param isMorph True for a toggle button that swaps its shape
			 * while selected.
			 * @return The new button, registered in the entity registry.
			 */
			std::shared_ptr<Button> makeIconButton(
				std::shared_ptr<Entity> parent, const std::string &labelContent,
				const std::string &iconGlyphName,
				const components::Glyph::Style &iconStyle,
				std::function<void(void)> onClick, Color colorStyle,
				Shape shape, Size size, bool isMorph);
		};

		protected:
		/**
		 * @brief Get the font size of the icon child for the current size.
		 * @return The icon font size, in dp.
		 */
		float getIconFontSize(void) const override;

		/**
		 * @brief Tell ButtonBase::build() that this variant has a label
		 * child.
		 * @return Always true.
		 */
		bool usesLabel(void) const override
		{
			return true;
		}

		/**
		 * @brief Get the color the icon and label children are created with.
		 * @return The resting content color of the current color style.
		 */
		utility::graphic::Color32Bit
			getInitialContentColor(void) const override;

		public:
		/**
		 * @brief Create a button and its icon and label children, without
		 * registering it in an entity registry.
		 *
		 * The constructor is private because the children need a button
		 * already held by a `std::shared_ptr` as parent: create() constructs
		 * the button then calls build(), which creates them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, iconGlyphName, iconStyle, labelContent, isToggle,
		 * colorStyle, shape, size, isMorph, isDisabled, isTrailingIcon,
		 * accessibilityLabel, onClick).
		 * @return The new button.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Button> create(Arguments &&...arguments)
		{
			std::shared_ptr<Button> entity(
				new Button(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Button(void) override = default;

		/**
		 * @brief Use the compact horizontal padding (Paper `compact`).
		 * Restyles at once.
		 * @param compact True for the compact padding, false for the regular
		 * padding of the size.
		 * @return Reference to this button for chaining.
		 */
		Button &setCompact(bool compact);

		/**
		 * @brief Raise the minimum container width above MinWidth.
		 * Restyles at once.
		 *
		 * systems::SegmentedButton writes the same value on the segments of
		 * a segmented button to give them one width, so a value set here on
		 * a segment is overwritten.
		 * @param minWidth The minimum width, in dp. A value below MinWidth
		 * has no effect.
		 * @return Reference to this button for chaining.
		 */
		Button &setMinWidth(float minWidth);

		/**
		 * @brief Check whether the button uses the compact horizontal
		 * padding.
		 * @return True when compact.
		 */
		bool isCompact(void) const;

		/**
		 * @brief Render the label in upper case (Paper `uppercase`).
		 * Restyles at once.
		 *
		 * Only the displayed text changes; the label content stored in
		 * components::Button keeps its case.
		 * @param uppercase True to display the label in upper case.
		 * @return Reference to this button for chaining.
		 */
		Button &setUppercase(bool uppercase);

		/**
		 * @brief Check whether the label is displayed in upper case.
		 * @return True when the label is rendered in upper case.
		 */
		bool isUppercase(void) const;

		/**
		 * @brief Minimum container width, in dp (M3: 64 dp).
		 */
		static constexpr float MinWidth = components::CommonButton::MinWidth;

		/**
		 * @brief Minimum container height, in dp (M3: 40 dp). It does not
		 * apply to Size::ExtraSmall.
		 */
		static constexpr float MinHeight = components::CommonButton::MinHeight;

		/**
		 * @brief Set the color style. Restyles at once.
		 * @param colorStyle The new color style.
		 * @return Reference to this button for chaining.
		 */
		Button &setColorStyle(const Color &colorStyle);

		/**
		 * @brief Set the corner shape. Restyles at once.
		 * @param shape The new corner shape.
		 * @return Reference to this button for chaining.
		 */
		Button &setShape(const Shape &shape);

		/**
		 * @brief Set the size, which decides the paddings, the icon size and
		 * the corner radius. Restyles at once.
		 * @param size The new size.
		 * @return Reference to this button for chaining.
		 */
		Button &setSize(const Size &size);

		/**
		 * @brief Get the color style.
		 * @return The color style stored in components::CommonButton.
		 */
		Color getColorStyle(void) const;

		/**
		 * @brief Get the corner shape.
		 * @return The configured shape. A selected toggle button with morph
		 * is drawn with the other shape, but this value does not change.
		 */
		Shape getShape(void) const;

		/**
		 * @brief Get the size.
		 * @return The size stored in components::CommonButton.
		 */
		Size getSize(void) const;

		private:
		/**
		 * @brief Construct a button without its children; create() calls it
		 * then build().
		 *
		 * Adds components::CommonButton to the entity and installs
		 * systems::CommonButton on it.
		 * @param registry The registry that stores the button's components.
		 * @param iconGlyphName The glyph name of the icon; empty for no icon.
		 * @param iconStyle The glyph style of the icon.
		 * @param labelContent The label text.
		 * @param isToggle True when a click toggles the selected state.
		 * @param colorStyle The color style.
		 * @param shape The corner shape.
		 * @param size The size.
		 * @param isMorph True when a selected toggle swaps its shape.
		 * @param isDisabled True when the button starts disabled.
		 * @param isTrailingIcon True to place the icon after the label.
		 * @param accessibilityLabel The label read by assistive technology;
		 * when empty, the label text is used.
		 * @param onClick The function called on a user click (may be empty).
		 */
		Button(ecs::ComponentRegistry &registry,
			   const std::string &iconGlyphName,
			   const components::Glyph::Style &iconStyle,
			   const std::string &labelContent, bool isToggle, Color colorStyle,
			   Shape shape, Size size, bool isMorph, bool isDisabled,
			   bool isTrailingIcon, const std::string &accessibilityLabel,
			   std::function<void(void)> onClick);
	};

}	 // namespace guillaume::entities
