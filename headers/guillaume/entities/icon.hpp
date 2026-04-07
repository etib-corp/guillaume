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

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Icon component
	 */
	class Icon:
		public ecs::EntityFiller<components::Transform, components::Glyph>
	{
		public:
		/**
		 * @brief Style of the icon, which can be Outlined, Rounded, or Sharp.
		 */
		enum class Style { Outlined, Rounded, Sharp };

		/**
		 * @brief Builder used to configure and create `Icon` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::unique_ptr<Icon>
				_icon;	  ///< Unique pointer to the Icon entity being built
			std::string _glyphName;	   ///< Name of the glyph to be used
			utility::graphic::Color32Bit
				_color;		 ///< Color of the icon to be used (RGBA)
			Style _style;	 ///< Style of the icon to be used

			public:
			/**
			 * @brief Construct a new Icon Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Icon Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the icon entity.
			 * @return The entity identifier of the newly created icon entity.
			 */
			ecs::Entity::Identifier registerEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Icon entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the name of the glyph to be used for the Icon entity.
			 * @param glyphName The name of the glyph to set (corresponds to
			 * utility::graphic::Glyph::_name).
			 * @return Reference to the builder for chaining.
			 * @see utility::graphic::Glyph::_name
			 */
			Builder &withGlyphName(const std::string &glyphName);

			/**
			 * @brief Set the color of the icon to be used for the Icon entity.
			 * @param color The color of the icon to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the style of the glyph to be used for the Icon entity.
			 * @param style The style of the glyph to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withStyle(const Style &style);
		};

		/**
		 * @brief Director that orchestrates `Icon::Builder` to create
		 * preconfigured icon entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Icon Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Icon Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a default icon entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the default icon.
			 * @param iconName The icon name to assign to the created entity.
			 * @return The entity identifier of the newly created icon entity.
			 * @see utility::graphic::Glyph::_name
			 */
			ecs::Entity::Identifier
				makeDefaultIcon(Builder &builder, const std::string &iconName);
		};

		private:
		std::string _glyphName;	   ///< Name of the glyph to be used for this
								   ///< Icon entity (corresponds to
								   ///< utility::graphic::Glyph::_name)
		utility::graphic::Color32Bit
			_color;		 ///< Color of the icon to be used for this Icon entity
						 ///< (RGBA)
		Style _style;	 ///< Style of the glyph to be used for this Icon entity

		public:
		/**
		 * @brief Default constructor for the Icon component.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param glyphName The name of the glyph to be used for this Icon entity.
		 * @param color The color of the icon to be used for this Icon entity
		 * (RGBA).
		 * @param style The style of the icon to be used for this Icon entity.
		 * @see utility::graphic::Glyph::_name
		 */
		Icon(ecs::ComponentRegistry &registry, const std::string &glyphName,
			 const utility::graphic::Color32Bit &color, const Style &style);

		/**
		 * @brief Default destructor for the Icon component.
		 */
		~Icon(void);

		/**
		 * @brief Set the name of the glyph for this Icon entity.
		 * @param glyphName The new name of the glyph to set.
		 * @return Reference to this Icon for chaining.
		 * @see utility::graphic::Glyph::_name
		 */
		Icon &setGlyphName(const std::string &glyphName);

		/**
		 * @brief Set the color of the icon for this Icon entity.
		 * @param color The new color of the icon to set (RGBA).
		 * @return Reference to this Icon for chaining.
		 */
		Icon &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the style of the glyph for this Icon entity.
		 * @param style The new style of the glyph to set.
		 * @return Reference to this Icon for chaining.
		 */
		Icon &setStyle(const Style &style);
	};

}	 // namespace guillaume::entities
