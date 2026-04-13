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
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/interaction.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Button entity class representing a UI button with various
	 * components.
	 */
	class Button:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Interaction, components::Color,
								 components::Borders>
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
			std::unique_ptr<Button>
				_button;	///< Unique pointer to the Button entity being built
			ecs::Entity::Identifier
				_iconIdendifier;	///< Entity identifier of the prebuilt icon
									///< to attach to the button
			ecs::Entity::Identifier
				_labelIdentifier;	 ///< Entity identifier of the prebuilt
									 ///< label to attach to the button
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
			 * @return The entity identifier of the newly created button entity.
			 */
			ecs::Entity::Identifier registerEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Button entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the icon for the button.
			 * @param iconIdentifier The entity identifier of the prebuilt icon
			 * to attach to the button.ƒ
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(ecs::Entity::Identifier iconIdentifier);

			/**
			 * @brief Set the label text for the button.
			 * @param labelIdentifier The entity identifier of the prebuilt
			 * label to attach to the button.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(ecs::Entity::Identifier labelIdentifier);

			/**
			 * @brief Set the click event handler for the button.
			 * @param onClick The new click event handler to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnClick(const std::function<void(void)> &onClick);

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
			 * @param labelIdentifier The entity identifier of the prebuilt
			 * label to attach to the button.
			 * @param onClick The click event handler for the text button.
			 * @return The entity identifier of the newly created text button
			 * entity.
			 */
			ecs::Entity::Identifier
				makeTextButton(Builder &builder,
							   ecs::Entity::Identifier labelIdentifier,
							   std::function<void(void)> onClick);

			/**
			 * @brief Create an icon button entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the icon button.
			 * @param iconIdentifier The entity identifier of the prebuilt icon
			 * to attach
			 * @param onClick The click event handler for the icon button.
			 * @return The entity identifier of the newly created icon button
			 * entity.
			 */
			ecs::Entity::Identifier
				makeIconButton(Builder &builder,
							   ecs::Entity::Identifier iconIdentifier,
							   std::function<void(void)> onClick);

			/**
			 * @brief Create an icon text button entity using the
			 * builder.
			 * @param builder The builder instance used to configure
			 * and create the icon text button.
			 * @param iconIdentifier The entity identifier of the prebuilt icon
			 * to attach to the button.
			 * @param labelIdentifier The entity identifier of the prebuilt
			 * label to attach to the button.
			 * @param onClick The click event handler for the button.
			 * @return The entity identifier of the newly created icon text
			 * button entity.
			 */
			ecs::Entity::Identifier
				makeIconTextButton(Builder &builder,
								   ecs::Entity::Identifier iconIdentifier,
								   ecs::Entity::Identifier labelIdentifier,
								   std::function<void(void)> onClick);
		};

		private:
		ecs::Entity::Identifier _iconIdentifier {
			ecs::Entity::InvalidIdentifier
		};	 ///< Entity identifier of
													 ///< the prebuilt icon to
													 ///< attach to the button
		ecs::Entity::Identifier _labelIdentifier {
			ecs::Entity::InvalidIdentifier
		};	 ///< Entity identifier of
													 ///< the prebuilt label to
													 ///< attach to the button
		bool _isToggle { false };		  ///< Whether the button is a toggle button
		Color _colorStyle { Color::Filled };		  ///< Color style of the button
		Shape _shape { Shape::Round };		  ///< Shape of the button
		Size _size { Size::Small };			  ///< Size of the button
		bool _isMorph { false };		  ///< Whether the button is in a morph state
		std::function<void(void)>
			_onClick {};	 ///< Click event handler for the button

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
		 * @param mousePosition The position of the mouse when the left click
		 * event occurred.
		 */
		void leftClickPressHandler(
			utility::event::MouseMotionEvent::MousePosition mousePosition);

		/**
		 * @brief Left click release event handler for the button.
		 * @param mousePosition The position of the mouse when the left click
		 * release event occurred.
		 */
		void leftClickReleaseHandler(
			utility::event::MouseMotionEvent::MousePosition mousePosition);

		/**
		 * @brief Apply the current Material button state to components.
		 */
		void applyMaterialState(void);

		/**
		 * @brief Calculate the pose for the button's text component when no
		 * icon is present. This method computes the appropriate position and
		 * orientation for the text based on the button's current properties
		 * and returns it as a PoseF object.
		 * @return The calculated PoseF for the text component without an icon.
		 */
		utility::graphic::PoseF calculTextPoseWithoutIcon(void);

		/**
		 * @brief Calculate the pose for the button's text component when an
		 * icon is present. This method computes the appropriate position and
		 * orientation for the text based on the button's current properties,
		 * including the presence of an icon, and returns it as a PoseF object.
		 * @return The calculated PoseF for the text component with an icon.
		 */
		utility::graphic::PoseF calculTextPoseWithIcon(void);

		/**
		 * @brief Calculate the width of the button based on its size and
		 * other properties. This method determines the appropriate width for
		 * the button by considering factors such as the button's size category
		 * and any additional margins or padding that may be required.
		 * @return The calculated width of the button.
		 */
		std::size_t calculWidth(void);

		public:
		/**
		 * @brief Default constructor for the Button entity.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param iconIdentifier The entity identifier of the prebuilt icon to
		 * attach to the button.
		 * @param labelIdentifier The entity identifier of the prebuilt label
		 * to attach to the button.
		 * @param isToggle Whether the button should be a toggle button.
		 * @param colorStyle Initial color style for the button.
		 * @param shape Initial shape for the button.
		 * @param size Initial size for the button.
		 * @param isMorph Initial morph state for the button.
		 * @param onClick Click event handler for the button.
		 */
		Button(ecs::ComponentRegistry &registry,
			   ecs::Entity::Identifier iconIdentifier,
			   ecs::Entity::Identifier labelIdentifier, bool isToggle,
			   Color colorStyle, Shape shape, Size size, bool isMorph,
			   std::function<void(void)> onClick);

		/**
		 * @brief Default destructor for the Button entity.
		 */
		~Button(void);

		/**
		 * @brief Set the icon for the button.
		 * @param iconIdentifier The entity identifier of the prebuilt icon to
		 * attach to the button.
		 * @return Reference to this Button for chaining.
		 */
		Button &setIconIdentifier(ecs::Entity::Identifier iconIdentifier);

		/**
		 * @brief Set the label for the button.
		 * @param labelIdentifier The entity identifier of the prebuilt label
		 * to attach to the button.
		 * @return Reference to this Button for chaining.
		 * @throws std::runtime_error if the provided labelIdentifier does not
		 * correspond to a valid Text entity in the registry.
		 */
		Button &setLabelIdentifier(ecs::Entity::Identifier labelIdentifier);

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
		Button &setOnClick(const std::function<void(void)> &onClick);

		/**
		 * @brief Recompute the button entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities