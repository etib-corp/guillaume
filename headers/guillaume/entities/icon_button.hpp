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

#include <functional>
#include <memory>
#include <string>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Icon button entity representing a compact icon-only button.
	 */
	class IconButton: public ButtonBase
	{
		public:
		/**
		 * @brief Color variant of the icon button.
		 */
		enum class Variant { Standard, Filled, Tonal, Outlined };

		/**
		 * @brief Size of the icon button.
		 */
		enum class Size { Small, Medium, Large };

		/**
		 * @brief Builder used to configure and create `IconButton` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<IconButton>
				_iconButton;			   ///< Icon button entity being built
			std::string _iconGlyphName;	   ///< Icon glyph name to attach
			std::string _selectedIconGlyphName;	   ///< Icon glyph name shown
												   ///< when selected
			components::Glyph::Style
				_iconStyle;	   ///< Style of the icon to attach
			std::function<void(void)> _onClick;	   ///< Click event handler
			bool _isToggle;		 ///< Whether the button is a toggle button
			Variant _variant;	 ///< Color variant
			Size _size;			 ///< Size
			bool _isDisabled;	 ///< Whether the button is disabled
			std::string _accessibilityLabel;	///< Accessibility label

			public:
			/**
			 * @brief Construct a new IconButton Builder object.
			 * @param componentRegistry The component registry used to build
			 * entities.
			 * @param entityRegistry The entity registry used to build entities.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the IconButton Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the icon button entity.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @return A shared pointer to the newly created icon button entity.
			 */
			std::shared_ptr<IconButton>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon glyph name.
			 * @param iconGlyphName The glyph name of the icon to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyphName);

			/**
			 * @brief Set the selected icon glyph name.
			 * @param selectedIconGlyphName The glyph name shown when selected.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelectedIcon(const std::string &selectedIconGlyphName);

			/**
			 * @brief Set the style of the icon.
			 * @param iconStyle The style of the icon to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIconStyle(const components::Glyph::Style &iconStyle);

			/**
			 * @brief Set the click event handler.
			 * @param onClick The new click event handler to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnClick(std::function<void(void)> onClick);

			/**
			 * @brief Set if the button is a toggle button.
			 * @param isToggle Whether the button should be a toggle button.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withToggle(const bool &isToggle);

			/**
			 * @brief Set the color variant.
			 * @param variant The new color variant to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(const Variant &variant);

			/**
			 * @brief Set the size.
			 * @param size The new size to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSize(const Size &size);

			/**
			 * @brief Set if the button is disabled.
			 * @param isDisabled Whether the button should be disabled.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withDisabled(const bool &isDisabled);

			/**
			 * @brief Set the accessibility label.
			 * @param accessibilityLabel The accessibility label to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withAccessibilityLabel(const std::string &accessibilityLabel);
		};

		/**
		 * @brief Director that orchestrates `IconButton::Builder`.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new IconButton Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the IconButton Director class.
			 */
			~Director(void);

			/**
			 * @brief Create an icon button entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the icon button.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @param iconGlyphName The icon glyph name to attach.
			 * @param onClick The click event handler.
			 * @param variant The color variant.
			 * @param size The size.
			 * @return A shared pointer to the newly created icon button entity.
			 */
			std::shared_ptr<IconButton>
				makeIconButton(Builder &builder, std::shared_ptr<Entity> parent,
							   const std::string &iconGlyphName,
							   std::function<void(void)> onClick,
							   Variant variant, Size size);
		};

		private:
		Variant _variant { Variant::Standard };	   ///< Color variant
		Size _size { Size::Medium };			   ///< Size

		protected:
		/**
		 * @brief Re-apply the visual state (colors, borders, radius) from the
		 * current interaction/selected state.
		 */
		void applyStyleState(void) override;

		/**
		 * @brief Re-measure and lay out the button and its children.
		 */
		void applyGeometry(void) override;

		/**
		 * @brief Font size used for the icon child.
		 * @return The icon font size.
		 */
		float getIconFontSize(void) const override;

		/**
		 * @brief Whether this button type displays a text label.
		 * @return False, icon buttons have no label.
		 */
		bool usesLabel(void) const override
		{
			return false;
		}

		/**
		 * @brief Get the initial content color used when creating children.
		 * @return The initial content color.
		 */
		utility::graphic::Color32Bit
			getInitialContentColor(void) const override;

		public:
		/**
		 * @brief Default constructor for the IconButton entity.
		 * @param registry Reference to the component registry.
		 * @param iconGlyphName Icon glyph name to attach.
		 * @param selectedIconGlyphName Icon glyph name shown when selected.
		 * @param iconStyle Style of the icon to attach.
		 * @param isToggle Whether the button should be a toggle button.
		 * @param variant Initial color variant.
		 * @param size Initial size.
		 * @param isDisabled Initial disabled state.
		 * @param accessibilityLabel Initial accessibility label.
		 * @param onClick Click event handler.
		 */
		IconButton(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const std::string &selectedIconGlyphName,
				   const components::Glyph::Style &iconStyle, bool isToggle,
				   Variant variant, Size size, bool isDisabled,
				   const std::string &accessibilityLabel,
				   std::function<void(void)> onClick);

		/**
		 * @brief Default destructor for the IconButton entity.
		 */
		~IconButton(void) override;

		/**
		 * @brief Set the color variant.
		 * @param variant The new color variant to set.
		 * @return Reference to this IconButton for chaining.
		 */
		IconButton &setVariant(const Variant &variant);

		/**
		 * @brief Set the size.
		 * @param size The new size to set.
		 * @return Reference to this IconButton for chaining.
		 */
		IconButton &setSize(const Size &size);

		/**
		 * @brief Get the color variant.
		 * @return The color variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Get the size.
		 * @return The size.
		 */
		Size getSize(void) const;
	};

}	 // namespace guillaume::entities
