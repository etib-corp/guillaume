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
#include "guillaume/ecs/node_entity_builder.hpp"
#include "guillaume/ecs/node_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/click.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/hover.hpp"
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
		public ecs::NodeEntityFiller<components::Transform, components::Bound,
									 components::Hover, components::Click,
									 components::Color, components::Borders>
	{
		public:
		/**
		 * @brief State of a toggle button, which can be either Default or
		 * Selected.
		 */
		enum class ToggleState { Default, Selected };

		/**
		 * @brief Color style of the button.
		 */
		enum class ColorStyle { Elevated, Filled, Tonal, Outlined, Text };

		/**
		 * @brief Shape of the button.
		 */
		enum class Shape { Round, Square };

		/**
		 * @brief Size of the button.
		 */
		enum class Size { ExtraSmall, Small, Medium, Large, ExtraLarge };

		/**
		 * @brief Morph state of the button, which can be Default, Pressed, or
		 * Selected.
		 */
		enum class MorphState { Default, Pressed, Selected };

		/**
		 * @brief Builder used to configure and create `Button` entities.
		 */
		class Builder: public ecs::NodeEntityBuilder
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
				_onClick;				 ///< Click event handler for the button
			ToggleState _toggleState;	 ///< Current toggle state
			ColorStyle _colorStyle;		 ///< Button color style
			Shape _shape;				 ///< Button shape
			Size _size;					 ///< Button size
			MorphState _morphState;		 ///< Current morph state

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
			 * @brief Set the toggle state of the button.
			 * @param toggleState The new toggle state to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withToggleState(const ToggleState &toggleState);

			/**
			 * @brief Set the color style of the button.
			 * @param colorStyle The new color style to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColorStyle(const ColorStyle &colorStyle);

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
			 * @param morphState The new morph state to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMorphState(const MorphState &morphState);
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
		ecs::Entity::Identifier _iconIdentifier;	 ///< Entity identifier of
													 ///< the prebuilt icon to
													 ///< attach to the button
		ecs::Entity::Identifier _labelIdentifier;	 ///< Entity identifier of
													 ///< the prebuilt label to
													 ///< attach to the button
		ToggleState _toggleState;	 ///< Current toggle state of the button
		ColorStyle _colorStyle;		 ///< Color style of the button
		Shape _shape;				 ///< Shape of the button
		Size _size;					 ///< Size of the button
		MorphState _morphState;		 ///< Current morph state of the button
		std::function<void(void)>
			_onClick;	 ///< Click event handler for the button

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

		public:
		/**
		 * @brief Default constructor for the Button entity.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param iconIdentifier The entity identifier of the prebuilt icon to
		 * attach to the button.
		 * @param labelIdentifier The entity identifier of the prebuilt label
		 * to attach to the button.
		 * @param toggleState Initial toggle state for the button.
		 * @param colorStyle Initial color style for the button.
		 * @param shape Initial shape for the button.
		 * @param size Initial size for the button.
		 * @param morphState Initial morph state for the button.
		 * @param onClick Click event handler for the button.
		 */
		Button(ecs::ComponentRegistry &registry,
			   ecs::Entity::Identifier iconIdentifier,
			   ecs::Entity::Identifier labelIdentifier, ToggleState toggleState,
			   ColorStyle colorStyle, Shape shape, Size size,
			   MorphState morphState, std::function<void(void)> onClick);

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
		 */
		Button &setLabelIdentifier(ecs::Entity::Identifier labelIdentifier);

		/**
		 * @brief Set the toggle state of the button.
		 * @param toggleState The new toggle state to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setToggleState(const ToggleState &toggleState);

		/**
		 * @brief Set the color style of the button.
		 * @param colorStyle The new color style to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setColorStyle(const ColorStyle &colorStyle);

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
		 * @param morphState The new morph state to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setMorphState(const MorphState &morphState);

		/**
		 * @brief Set the click event handler for the button.
		 * @param onClick The new click event handler to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setOnClick(const std::function<void(void)> &onClick);

		/**
		 * @brief Set the click event handler for the button using an rvalue
		 * reference.
		 * @param onClick The new click event handler to set.
		 * @return Reference to this Button for chaining.
		 */
		Button &setOnClick(std::function<void(void)> &&onClick);
	};

}	 // namespace guillaume::entities