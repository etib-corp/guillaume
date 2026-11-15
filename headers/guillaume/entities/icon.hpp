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
#include <memory>
#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A Material Symbols icon, chosen by name and drawn as one glyph
	 * of the icon font.
	 *
	 * Icon is a leaf widget: it has no children. Its state lives in
	 * components::Glyph (name, font size, style), components::Color and
	 * components::Transform. Each frame systems::MeasureGlyph writes the
	 * measured glyph size into components::Bound, in the Measure phase, and
	 * systems::GlyphRender draws the glyph, in the Render phase. The class
	 * itself holds no field.
	 *
	 * @code
	 * Icon::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto icon = builder.makeIcon(parent, "favorite", 24.0f,
	 * 	getActiveColor(SchemeColorRole::Primary),
	 * 	components::Glyph::Style::Outlined);
	 * icon->setAccessibilityLabel("Add to favorites");
	 * @endcode
	 *
	 * `Icon::create(registry, glyphName, fontSize, color, style)` builds the
	 * same widget without registering it in an entity registry.
	 *
	 * Watch out:
	 * - The Bound is overwritten every frame with the measured size; size
	 *   an icon with its font size.
	 * - A name missing from the code point table of the font is drawn as
	 *   `?`.
	 * - The style is stored in components::Glyph, but both systems load the
	 *   Material Symbols Outlined font whatever its value.
	 * - An icon is decorative for assistive technologies until
	 *   setAccessibilityLabel() gives it a label.
	 *
	 * @see components::Glyph, systems::MeasureGlyph, systems::GlyphRender
	 */
	class Icon:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Glyph, components::Color>
	{
		public:
		/**
		 * @brief Configures Icon widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Icon>
		{
			private:
			std::string _glyphName;	   ///< Material Symbols name of the icon.
			float _fontSize;		   ///< Font size of the glyph, in dp.
			utility::graphic::Color32Bit _color;	///< Icon color (RGBA).
			components::Glyph::Style
				_style;	   ///< Material Symbols style of the glyph.

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built icons are stored in.
			 * @param entityRegistry The registry that owns the icons built
			 * by `makeIcon`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build an icon from the current configuration, without
			 * registering it or giving it a parent.
			 * @return The new icon.
			 */
			std::shared_ptr<Icon> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: empty glyph name, font size 24,
			 * the OnSurface color of the active theme, and the Outlined
			 * style.
			 */
			void reset(void) override;

			/**
			 * @brief Choose the icon to draw.
			 * @param glyphName The Material Symbols name of the icon, such
			 * as `"favorite"`. Empty by default.
			 * @return Reference to this builder for chaining.
			 * @see components::Glyph::getName
			 */
			Builder &withGlyphName(const std::string &glyphName);

			/**
			 * @brief Set the size of the icon.
			 * @param fontSize The font size of the glyph, in dp. 24 by
			 * default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFontSize(float fontSize);

			/**
			 * @brief Set the icon color.
			 * @param color The color (RGBA). By default the OnSurface color
			 * the active theme had when the builder was last reset.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the Material Symbols style of the glyph.
			 * @param style The style stored in components::Glyph. Outlined
			 * by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withStyle(const components::Glyph::Style &style);

			/**
			 * @brief Build an icon, register it under `parent` and reset
			 * the builder.
			 * @param parent The entity the icon is attached to, or nullptr
			 * for a root entity.
			 * @param iconName The Material Symbols name of the icon, such
			 * as `"favorite"`.
			 * @param fontSize The font size of the glyph, in dp.
			 * @param color The icon color (RGBA).
			 * @param style The Material Symbols style of the glyph.
			 * @return The registered icon.
			 */
			std::shared_ptr<Icon>
				makeIcon(std::shared_ptr<Entity> parent,
						 const std::string &iconName, const float &fontSize,
						 const utility::graphic::Color32Bit &color,
						 const components::Glyph::Style &style);
		};

		public:
		/**
		 * @brief Build an icon without registering it in an entity
		 * registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, glyphName, fontSize, color, style)`. An icon has no
		 * children to build.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new icon. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Icon> create(Arguments &&...arguments)
		{
			std::shared_ptr<Icon> entity(
				new Icon(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Icon(void);

		/**
		 * @brief Change the icon to draw.
		 * @param glyphName The Material Symbols name of the new icon,
		 * written to components::Glyph.
		 * @return Reference to this icon for chaining.
		 * @see components::Glyph::getName
		 */
		Icon &setGlyphName(const std::string &glyphName);

		/**
		 * @brief Set the size of the icon.
		 *
		 * The Bound follows at the next Measure phase.
		 * @param fontSize The new font size of the glyph, in dp.
		 * @return Reference to this icon for chaining.
		 */
		Icon &setFontSize(const float &fontSize);

		/**
		 * @brief Set the icon color.
		 * @param color The new color (RGBA), written to components::Color.
		 * @return Reference to this icon for chaining.
		 */
		Icon &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the Material Symbols style of the glyph.
		 * @param style The new style, written to components::Glyph.
		 * @return Reference to this icon for chaining.
		 */
		Icon &setStyle(const components::Glyph::Style &style);

		/**
		 * @brief Give the icon a meaning for assistive technologies.
		 *
		 * An icon has no components::Accessibility until this is called
		 * with a non-empty label, which adds the component with the Image
		 * role. A later empty label sets the role back to None and clears
		 * the label; the component stays.
		 *
		 * @param label The text announced for the icon, or an empty string
		 * to make the icon decorative.
		 * @return Reference to this icon for chaining.
		 */
		Icon &setAccessibilityLabel(const std::string &label);

		private:
		/**
		 * @brief Construct an icon and write its components.
		 * @param registry The registry the components are stored in.
		 * @param glyphName The Material Symbols name of the icon.
		 * @param fontSize The font size of the glyph, in dp.
		 * @param color The icon color (RGBA).
		 * @param style The Material Symbols style of the glyph.
		 * @see components::Glyph::getName
		 */
		Icon(ecs::ComponentRegistry &registry, const std::string &glyphName,
			 const float &fontSize, const utility::graphic::Color32Bit &color,
			 const components::Glyph::Style &style);
	};

}	 // namespace guillaume::entities
