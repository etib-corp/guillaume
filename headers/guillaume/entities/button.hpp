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
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/hand_squeeze_interaction.hpp"
#include "guillaume/components/hand_pinch_interaction.hpp"
#include "guillaume/components/hand_poke_interaction.hpp"
#include "guillaume/components/hand_thumb_rest_interaction.hpp"
#include "guillaume/components/hand_thumb_stick_interaction.hpp"
#include "guillaume/components/hand_trigger_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Button entity class representing a UI button with various
	 * components.
	 */
	class Button:
		public std::enable_shared_from_this<Button>,
		public ecs::ParentEntityFiller<
			components::Transform, components::Bound, components::Color,
			components::Borders, components::Focus,
			components::HandButtonInteraction, components::HandHoverInteraction,
			components::MouseHoverInteraction,
			components::MouseButtonInteraction>
	{
		public:
		/**
		 * @brief Color style of the button.
		 */
		enum class Color { Elevated, Filled, Tonal, Outlined, Text };

		/**
		 * @brief Shape of the button.
		 */
		enum class Shape { Round, Square };

		/**
		 * @brief Size of the button.
		 */
		enum class Size { ExtraSmall, Small, Medium, Large, ExtraLarge };

		/**
		 * @brief Builder used to configure and create `Button` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<Button>
				_button;	///< Unique pointer to the Button entity being built
			std::string
				_iconGlyphName;	   ///< Icon glyph name to attach to the button
			components::Glyph::Style
				_iconStyle;	   ///< Style of the icon to attach to the button
			std::string
				_labelContent;	  ///< Label content to attach to the button
			std::function<void(void)>
				_onClick;		  ///< Click event handler for the button
			bool _isToggle;		  ///< Whether the button is a toggle button
			Color _colorStyle;	  ///< Button color style
			Shape _shape;		  ///< Button shape
			Size _size;			  ///< Button size
			bool _isMorph;		  ///< Whether the button is in a morph state

			public:
			/**
			 * @brief Construct a new Button Builder object.
			 * @param componentRegistry The component registry used to build
			 * entities.
			 * @param entityRegistry The entity registry used to build entities.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Button Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the button entity.
			 * @param parent The parent entity to which the new button entity
			 * will be attached.
			 * @return A shared pointer to the newly created button entity.
			 */
			std::shared_ptr<Button>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Button entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon for the button.
			 * @param iconGlyphName The glyph name of the icon to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyphName);

			/**
			 * @brief Set the style of the icon for the button.
			 * @param iconStyle The style of the icon to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIconStyle(const components::Glyph::Style &iconStyle);

			/**
			 * @brief Set the label text for the button.
			 * @param labelContent The label content to attach to the button.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &labelContent);

			/**
			 * @brief Set the click event handler for the button.
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
			 * @brief Set the color style of the button.
			 * @param colorStyle The new color style to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColorStyle(const Color &colorStyle);

			/**
			 * @brief Set the shape of the button.
			 * @param shape The new shape to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withShape(const Shape &shape);

			/**
			 * @brief Set the size of the button.
			 * @param size The new size to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSize(const Size &size);

			/**
			 * @brief Set the morph state of the button.
			 * @param isMorph The new morph state to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMorph(const bool &isMorph);
		};

		/**
		 * @brief Director that orchestrates `Button::Builder` to create
		 * preconfigured button entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Button Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Button Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a text button entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the text button.
			 * @param parent The parent entity to which the new button entity
			 * will be attached.
			 * @param labelContent The label content to attach to the button.
			 * @param onClick The click event handler for the text button.
			 * @param colorStyle The color style of the button.
			 * @param shape The shape of the button.
			 * @param size The size of the button.
			 * @param isMorph Whether the button is in a morph state.
			 * @return A shared pointer to the newly created text button
			 * entity.
			 */
			std::shared_ptr<Button>
				makeButton(Builder &builder, std::shared_ptr<Entity> parent,
						   const std::string &labelContent,
						   std::function<void(void)> onClick, Color colorStyle,
						   Shape shape, Size size, bool isMorph);

			/**
			 * @brief Create an icon button entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the icon button.
			 * @param parent The parent entity to which the new button entity
			 * will be attached.
			 * @param labelContent The label content to attach to the button.
			 * @param iconGlyphName The icon glyph name to attach.
			 * @param iconStyle The style of the icon to attach.
			 * @param onClick The click event handler for the icon button.
			 * @param colorStyle The color style of the button.
			 * @param shape The shape of the button.
			 * @param size The size of the button.
			 * @param isMorph Whether the button is in a morph state.
			 * @return A shared pointer to the newly created icon button
			 * entity.
			 */
			std::shared_ptr<Button>
				makeIconButton(Builder &builder, std::shared_ptr<Entity> parent,
							   const std::string &labelContent,
							   const std::string &iconGlyphName,
							   const components::Glyph::Style &iconStyle,
							   std::function<void(void)> onClick,
							   Color colorStyle, Shape shape, Size size,
							   bool isMorph);
		};

		private:
		std::string _iconGlyphName {};	  ///< Icon glyph name to attach.
		components::Glyph::Style _iconStyle {
			components::Glyph::Style::Outlined
		};	  ///< Style of the icon to attach.
		std::string _labelContent {};	 ///< Label content to attach.
		std::shared_ptr<Icon> _icon;	 ///< Internal child icon entity
		std::shared_ptr<Text> _label;	 ///< Internal child label entity
		bool _isToggle { false };	 ///< Whether the button is a toggle button
		Color _colorStyle { Color::Filled };	///< Color style of the button
		Shape _shape { Shape::Round };			///< Shape of the button
		Size _size { Size::Small };				///< Size of the button
		bool _isMorph { false };	///< Whether the button is in a morph state
		std::function<void(void)>
			_onClick {};	///< Click event handler for the button
		static constexpr float _layerDepthStep {
			1.0f
		};	  ///< Distance pushed toward the camera per layer.
		bool _isSelected;	 ///< Whether the button is currently selected (for
							 ///< toggle buttons)

		private:
		/**
		 * @brief Hover event handler for the button.
		 */
		void hoverHandler(void);

		/**
		 * @brief Unhover event handler for the button.
		 */
		void unHoverHandler(void);

		/**
		 * @brief Left click event handler for the button.
		 */
		void buttonPressHandler(void);

		/**
		 * @brief Left click release event handler for the button.
		 */
		void buttonReleaseHandler(void);

		public:
		/**
		 * @brief Default constructor for the Button entity.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param iconGlyphName Icon glyph name to attach to the button.
		 * @param iconStyle Style of the icon to attach to the button.
		 * @param labelContent Label content to attach to the button.
		 * @param isToggle Whether the button should be a toggle button.
		 * @param colorStyle Initial color style for the button.
		 * @param shape Initial shape for the button.
		 * @param size Initial size for the button.
		 * @param isMorph Initial morph state for the button.
		 * @param onClick Click event handler for the button.
		 */
		Button(ecs::ComponentRegistry &registry,
			   const std::string &iconGlyphName,
			   const components::Glyph::Style &iconStyle,
			   const std::string &labelContent, bool isToggle, Color colorStyle,
			   Shape shape, Size size, bool isMorph,
			   std::function<void(void)> onClick);

		/**
		 * @brief Default destructor for the Button entity.
		 */
		~Button(void);

		/**
		 * @brief Set the icon glyph name for the button.
		 * @param iconGlyphName The glyph name of the icon to attach.
		 * @return Reference to this Button for chaining.
		 */
		Button &setIconGlyphName(const std::string &iconGlyphName);

		/**
		 * @brief Set the style of the icon for the button.
		 * @param iconStyle The style of the icon to attach.
		 * @return Reference to this Button for chaining.
		 */
		Button &setIconStyle(const components::Glyph::Style &iconStyle);

		/**
		 * @brief Set the label content for the button.
		 * @param labelContent The label content to attach to the button.
		 * @return Reference to this Button for chaining.
		 */
		Button &setLabelContent(const std::string &labelContent);

		/**
		 * @brief Set if the button is a toggle button.
		 * @param isToggle Whether the button should be a toggle button.
		 * @return Reference to this Button for chaining.
		 */
		Button &setIsToggle(const bool &isToggle);

		/**
		 * @brief Set the color style of the button.
		 * @param colorStyle The new color style to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setColorStyle(const Color &colorStyle);

		/**
		 * @brief Set the shape of the button.
		 * @param shape The new shape to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setShape(const Shape &shape);

		/**
		 * @brief Set the size of the button.
		 * @param size The new size to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setSize(const Size &size);

		/**
		 * @brief Set the morph state of the button.
		 * @param isMorph The new morph state to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setMorph(const bool &isMorph);

		/**
		 * @brief Set the click event handler for the button.
		 * @param onClick The new click event handler to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setOnClick(std::function<void(void)> onClick);

		/**
		 * @brief Initialize the button entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the button entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities