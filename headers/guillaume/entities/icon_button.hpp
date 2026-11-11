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

#include "guillaume/components/icon_button.hpp"
#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A Material 3 icon button: a small clickable container that
	 * shows a single icon and no label.
	 *
	 * The state shared by every button (icon, toggle, callbacks) lives in
	 * components::Button, and systems::Button turns input into clicks in the
	 * Event phase. What is specific to this variant (color variant, size)
	 * lives in components::IconButton; systems::IconButton colors the
	 * container, sizes it and centers the icon each frame, in the Layout
	 * phase. The class itself holds no field: its setters write the
	 * component and restyle at once.
	 *
	 * @code
	 * IconButton::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto favorite = builder.withToggle(true)
	 * 	.withSelectedIcon("heart_check")
	 * 	.withAccessibilityLabel("Favorite")
	 * 	.makeIconButton(parent, "favorite", [] {
	 * 		// called when the user clicks the button
	 * 	}, IconButton::Variant::Standard, IconButton::Size::Medium);
	 * @endcode
	 *
	 * `IconButton::create(registry, iconGlyphName, selectedIconGlyphName,
	 * iconStyle, isToggle, variant, size, isDisabled, accessibilityLabel,
	 * onClick)` builds the same widget without registering it in an entity
	 * registry.
	 *
	 * The button owns its icon child: it is the entity registry the icon is
	 * registered in. Its Bound is written each frame from its size, so a
	 * size set by hand on the Bound is overwritten. It has no text, so give
	 * it an accessibility label. setSelected() does not call the click
	 * handler; only a user click does.
	 *
	 * @see ButtonBase, components::IconButton, systems::IconButton
	 */
	class IconButton: public ButtonBase
	{
		public:
		/// Color variant: Standard, Filled, Tonal or Outlined (see
		/// components::IconButton::Variant).
		using Variant = components::IconButton::Variant;

		/// Size: Small, Medium or Large (see components::IconButton::Size).
		using Size = components::IconButton::Size;

		/**
		 * @brief Configures, creates and registers IconButton widgets.
		 *
		 * Set options with the `with...` methods then call registerEntity(),
		 * or use makeIconButton() to do both in one call.
		 */
		class Builder: public EntityBuilderBase<IconButton>
		{
			private:
			std::string _iconGlyphName;				///< Icon glyph name
			std::string _selectedIconGlyphName;		///< Icon glyph name shown
													///< when selected
			components::Glyph::Style _iconStyle;	///< Icon glyph style
			std::function<void(void)>
				_onClick;		 ///< Handler called on a user click
			bool _isToggle;		 ///< Whether a click toggles the selection
			Variant _variant;	 ///< Color variant
			Size _size;			 ///< Size
			bool _isDisabled;	 ///< Whether the button is disabled
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
			 * @brief Build an IconButton, with its icon child, from the
			 * current configuration, without registering it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * button and resets the builder.
			 * @return The new icon button.
			 */
			std::shared_ptr<IconButton> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: empty icon and selected icon, Glyph::Style::Outlined,
			 * no click handler, not a toggle, Variant::Standard,
			 * Size::Medium, enabled, empty accessibility label.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon.
			 * @param iconGlyphName The glyph name of the icon (for example
			 * "favorite"). Default: empty.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyphName);

			/**
			 * @brief Set the icon shown while a toggle button is selected.
			 * @param selectedIconGlyphName The glyph name shown when
			 * selected. Default: empty, the regular icon is kept.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelectedIcon(const std::string &selectedIconGlyphName);

			/**
			 * @brief Set the glyph style of the icon.
			 * @param iconStyle The glyph style. Default:
			 * Glyph::Style::Outlined.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIconStyle(const components::Glyph::Style &iconStyle);

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
			 * @brief Set the color variant.
			 * @param variant The color variant. Default: Variant::Standard.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(const Variant &variant);

			/**
			 * @brief Set the size.
			 * @param size The size. Default: Size::Medium.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSize(const Size &size);

			/**
			 * @brief Choose whether the button starts disabled.
			 * @param isDisabled True for a button that ignores input.
			 * Default: false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withDisabled(const bool &isDisabled);

			/**
			 * @brief Set the label read by assistive technology.
			 * @param accessibilityLabel The accessibility label. Default:
			 * empty.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withAccessibilityLabel(const std::string &accessibilityLabel);

			/**
			 * @brief Build an icon button, register it under `parent` and
			 * reset the builder.
			 *
			 * Options set before the call with the other `with...` methods
			 * (toggle, selected icon, accessibility label, ...) also apply.
			 * @param parent The entity the button becomes a child of.
			 * @param iconGlyphName The glyph name of the icon.
			 * @param onClick The function called when the user clicks the
			 * button (may be empty).
			 * @param variant The color variant.
			 * @param size The size.
			 * @return The new icon button, registered in the entity registry.
			 */
			std::shared_ptr<IconButton>
				makeIconButton(std::shared_ptr<Entity> parent,
							   const std::string &iconGlyphName,
							   std::function<void(void)> onClick,
							   Variant variant, Size size);
		};

		protected:
		/**
		 * @brief Get the font size of the icon child for the current size.
		 * @return The icon font size, in dp.
		 */
		float getIconFontSize(void) const override;

		/**
		 * @brief Tell ButtonBase::build() that this variant has no label
		 * child.
		 * @return Always false.
		 */
		bool usesLabel(void) const override
		{
			return false;
		}

		/**
		 * @brief Get the color the icon child is created with.
		 * @return The resting content color of the current variant.
		 */
		utility::graphic::Color32Bit
			getInitialContentColor(void) const override;

		public:
		/**
		 * @brief Create an icon button and its icon child, without
		 * registering it in an entity registry.
		 *
		 * The constructor is private because the icon needs a button already
		 * held by a `std::shared_ptr` as parent: create() constructs the
		 * button then calls build(), which creates the icon.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, iconGlyphName, selectedIconGlyphName, iconStyle,
		 * isToggle, variant, size, isDisabled, accessibilityLabel, onClick).
		 * @return The new icon button.
		 */
		template<typename... Arguments>
		static std::shared_ptr<IconButton> create(Arguments &&...arguments)
		{
			std::shared_ptr<IconButton> entity(
				new IconButton(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~IconButton(void) override = default;

		/**
		 * @brief Set the color variant. Restyles at once.
		 * @param variant The new color variant.
		 * @return Reference to this icon button for chaining.
		 */
		IconButton &setVariant(const Variant &variant);

		/**
		 * @brief Set the size, which decides the container and icon sizes.
		 * Restyles at once.
		 * @param size The new size.
		 * @return Reference to this icon button for chaining.
		 */
		IconButton &setSize(const Size &size);

		/**
		 * @brief Get the color variant.
		 * @return The variant stored in components::IconButton.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Get the size.
		 * @return The size stored in components::IconButton.
		 */
		Size getSize(void) const;

		private:
		/**
		 * @brief Construct an icon button without its icon child; create()
		 * calls it then build().
		 *
		 * Adds components::IconButton to the entity and installs
		 * systems::IconButton on it.
		 * @param registry The registry that stores the button's components.
		 * @param iconGlyphName The glyph name of the icon.
		 * @param selectedIconGlyphName The glyph name shown while a toggle
		 * button is selected; empty to keep the regular icon.
		 * @param iconStyle The glyph style of the icon.
		 * @param isToggle True when a click toggles the selected state.
		 * @param variant The color variant.
		 * @param size The size.
		 * @param isDisabled True when the button starts disabled.
		 * @param accessibilityLabel The label read by assistive technology.
		 * @param onClick The function called on a user click (may be empty).
		 */
		IconButton(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const std::string &selectedIconGlyphName,
				   const components::Glyph::Style &iconStyle, bool isToggle,
				   Variant variant, Size size, bool isDisabled,
				   const std::string &accessibilityLabel,
				   std::function<void(void)> onClick);
	};

}	 // namespace guillaume::entities
