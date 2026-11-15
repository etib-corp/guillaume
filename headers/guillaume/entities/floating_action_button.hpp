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
#include <optional>
#include <memory>
#include <string>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/floating_action_button.hpp"
#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A Material 3 floating action button (FAB): a raised, square
	 * button with an icon, for the main action of a screen.
	 *
	 * The state shared by every button (icon, label, callbacks) lives in
	 * components::Button, and systems::Button turns input into clicks in the
	 * Event phase. What is specific to this variant (color variant, size,
	 * shadow mode, lowered flag, custom size) lives in
	 * components::FloatingActionButton; systems::FloatingActionButton colors
	 * the container, sets its shadow and size and centers the content each
	 * frame, in the Layout phase. The class itself holds no field: its
	 * setters write the component and restyle at once.
	 *
	 * @code
	 * FloatingActionButton::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto compose = builder.withAccessibilityLabel("Compose")
	 * 	.makeFloatingActionButton(parent, "edit", [] {
	 * 		// called when the user clicks the button
	 * 	}, FloatingActionButton::Variant::Primary,
	 * 	FloatingActionButton::Size::Medium);
	 * @endcode
	 *
	 * `FloatingActionButton::create(registry, iconGlyphName, iconStyle,
	 * labelContent, variant, size, lowered, isDisabled, accessibilityLabel,
	 * onClick)` builds the same widget without registering it in an entity
	 * registry.
	 *
	 * The button owns its icon and label children: it is the entity registry
	 * they are registered in. Its Bound and Elevation are written each frame,
	 * so values set by hand on them are overwritten. It does not place
	 * itself: position it through its Transform or its parent's layout. For
	 * a FAB that shows a label next to the icon, prefer
	 * ExtendedFloatingActionButton.
	 *
	 * @see ButtonBase, components::FloatingActionButton,
	 * systems::FloatingActionButton, ExtendedFloatingActionButton
	 */
	class FloatingActionButton: public ButtonBase
	{
		public:
		/// Color variant: Surface, Primary, Secondary or Tertiary (see
		/// components::FloatingActionButton::Variant).
		using Variant = components::FloatingActionButton::Variant;

		/// Shadow mode: Elevated or Flat (see
		/// components::FloatingActionButton::Mode).
		using Mode = components::FloatingActionButton::Mode;

		/// Size: Small, Medium or Large (see
		/// components::FloatingActionButton::Size).
		using Size = components::FloatingActionButton::Size;

		/**
		 * @brief Configures, creates and registers FloatingActionButton
		 * widgets.
		 *
		 * Set options with the `with...` methods then call registerEntity(),
		 * or use makeFloatingActionButton() to do both in one call.
		 */
		class Builder: public EntityBuilderBase<FloatingActionButton>
		{
			private:
			std::string _iconGlyphName;				///< Icon glyph name
			components::Glyph::Style _iconStyle;	///< Icon glyph style
			std::string _labelContent;				///< Label text
			std::function<void(void)>
				_onClick;		 ///< Handler called on a user click
			Variant _variant;	 ///< Color variant
			Size _size;			 ///< Size
			bool _lowered;		 ///< Whether the FAB uses the lowered style
			bool _isDisabled;	 ///< Whether the FAB is disabled
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
			 * @brief Build a FloatingActionButton, with its icon and label
			 * children, from the current configuration, without registering
			 * it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * button and resets the builder.
			 * @return The new FAB.
			 */
			std::shared_ptr<FloatingActionButton> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: empty icon, Glyph::Style::Outlined, empty label, no
			 * click handler, Variant::Primary, Size::Medium, not lowered,
			 * enabled, empty accessibility label.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon.
			 * @param iconGlyphName The glyph name of the icon (for example
			 * "add"). Default: empty.
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
			 * @brief Set the label text, shown after the icon.
			 * @param labelContent The label text. Default: empty. It is also
			 * the accessibility label when none is set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &labelContent);

			/**
			 * @brief Set the click handler.
			 * @param onClick The function called when the user clicks the
			 * FAB. Default: empty, nothing is called.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnClick(std::function<void(void)> onClick);

			/**
			 * @brief Set the color variant.
			 * @param variant The color variant. Default: Variant::Primary.
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
			 * @brief Choose the lowered style: a weaker shadow (elevation
			 * level 1 instead of 3) and, for Variant::Surface, the matching
			 * level 1 surface color.
			 * @param lowered True for the lowered style. Default: false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLowered(const bool &lowered);

			/**
			 * @brief Choose whether the FAB starts disabled.
			 * @param isDisabled True for a FAB that ignores input. Default:
			 * false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withDisabled(const bool &isDisabled);

			/**
			 * @brief Set the label read by assistive technology.
			 * @param accessibilityLabel The accessibility label. Default:
			 * empty, in which case the label text is used.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withAccessibilityLabel(const std::string &accessibilityLabel);

			/**
			 * @brief Build a FAB, register it under `parent` and reset the
			 * builder.
			 *
			 * Options set before the call with the other `with...` methods
			 * (lowered, accessibility label, ...) also apply.
			 * @param parent The entity the FAB becomes a child of.
			 * @param iconGlyphName The glyph name of the icon.
			 * @param onClick The function called when the user clicks the
			 * FAB (may be empty).
			 * @param variant The color variant.
			 * @param size The size.
			 * @return The new FAB, registered in the entity registry.
			 */
			std::shared_ptr<FloatingActionButton>
				makeFloatingActionButton(std::shared_ptr<Entity> parent,
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
		 * @return The resting content color of the current variant.
		 */
		utility::graphic::Color32Bit
			getInitialContentColor(void) const override;

		public:
		/**
		 * @brief Create a FAB and its icon and label children, without
		 * registering it in an entity registry.
		 *
		 * The constructor is private because the children need a button
		 * already held by a `std::shared_ptr` as parent: create() constructs
		 * the FAB then calls build(), which creates them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, iconGlyphName, iconStyle, labelContent, variant, size,
		 * lowered, isDisabled, accessibilityLabel, onClick).
		 * @return The new FAB.
		 */
		template<typename... Arguments>
		static std::shared_ptr<FloatingActionButton>
			create(Arguments &&...arguments)
		{
			std::shared_ptr<FloatingActionButton> entity(
				new FloatingActionButton(
					std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButton(void) override = default;

		/**
		 * @brief Set the color variant. Restyles at once.
		 * @param variant The new color variant.
		 * @return Reference to this FAB for chaining.
		 */
		FloatingActionButton &setVariant(const Variant &variant);

		/**
		 * @brief Set the size, which decides the container size, the corner
		 * radius and the icon size. Restyles at once.
		 * @param size The new size. A custom size, when set, still decides
		 * the container size.
		 * @return Reference to this FAB for chaining.
		 */
		FloatingActionButton &setSize(const Size &size);

		/**
		 * @brief Set the shadow mode. Restyles at once.
		 * @param mode Mode::Elevated for a shadow, Mode::Flat for none
		 * (elevation level 0).
		 * @return Reference to this FAB for chaining.
		 */
		FloatingActionButton &setMode(Mode mode);

		/**
		 * @brief Get the shadow mode.
		 * @return Mode::Elevated (the default) or Mode::Flat.
		 */
		Mode getMode(void) const;

		/**
		 * @brief Override the side of the square container (Paper
		 * `customSize`). Restyles at once.
		 *
		 * The corner radius becomes the custom size divided by 3.5.
		 * @param size The side of the container, in dp, or `std::nullopt`
		 * to use the size preset.
		 * @return Reference to this FAB for chaining.
		 * @throws std::invalid_argument If @p size holds a value that is
		 * zero or negative.
		 */
		FloatingActionButton &setCustomSize(std::optional<float> size);

		/**
		 * @brief Choose the lowered style: a weaker shadow (elevation level 1
		 * instead of 3) and, for Variant::Surface, the matching level 1
		 * surface color. Restyles at once.
		 * @param lowered True for the lowered style.
		 * @return Reference to this FAB for chaining.
		 */
		FloatingActionButton &setLowered(const bool &lowered);

		/**
		 * @brief Get the color variant.
		 * @return The variant stored in components::FloatingActionButton.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Get the size.
		 * @return The size preset, even when a custom size overrides it.
		 */
		Size getSize(void) const;

		/**
		 * @brief Get whether the FAB uses the lowered style.
		 * @return True when lowered, false otherwise.
		 */
		bool isLowered(void) const;

		private:
		/**
		 * @brief Construct a FAB without its children; create() calls it
		 * then build().
		 *
		 * Adds components::FloatingActionButton to the entity and installs
		 * systems::FloatingActionButton on it. A FAB is never a toggle.
		 * @param registry The registry that stores the FAB's components.
		 * @param iconGlyphName The glyph name of the icon.
		 * @param iconStyle The glyph style of the icon.
		 * @param labelContent The label text; may be empty.
		 * @param variant The color variant.
		 * @param size The size.
		 * @param lowered True for the lowered style.
		 * @param isDisabled True when the FAB starts disabled.
		 * @param accessibilityLabel The label read by assistive technology;
		 * when empty, the label text is used.
		 * @param onClick The function called on a user click (may be empty).
		 */
		FloatingActionButton(ecs::ComponentRegistry &registry,
							 const std::string &iconGlyphName,
							 const components::Glyph::Style &iconStyle,
							 const std::string &labelContent, Variant variant,
							 Size size, bool lowered, bool isDisabled,
							 const std::string &accessibilityLabel,
							 std::function<void(void)> onClick);
	};

}	 // namespace guillaume::entities
