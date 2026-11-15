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
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/typography.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

#include "guillaume/theme/typescale.hpp"

#include <optional>

namespace guillaume::entities
{

	/**
	 * @brief A single run of text drawn with a typescale role, a color and
	 * an optional explicit font size.
	 *
	 * Text is a leaf widget: it has no children. Its state lives in four
	 * components: components::Text (content and font metrics),
	 * components::Typography (typescale role and explicit size),
	 * components::Color and components::Transform. Each frame, in the
	 * Measure phase, systems::Typography resolves the role against the
	 * active theme into the font metrics, then systems::MeasureText writes
	 * the measured size into components::Bound. systems::TextRender draws
	 * the text in the Render phase. The class itself holds no field.
	 *
	 * @code
	 * Text::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto title = builder.makeText(parent, "Settings",
	 * 	theme::TypeRole::TitleLarge,
	 * 	getActiveColor(SchemeColorRole::OnSurface));
	 * title->setContent("Preferences");
	 * @endcode
	 *
	 * `Text::create(registry, content, role, color)` or
	 * `Text::create(registry, content, fontSize, color)` builds the same
	 * widget without registering it in an entity registry.
	 *
	 * Watch out:
	 * - The Bound is overwritten every frame with the measured size; do not
	 *   size a text by hand.
	 * - The font weight, line height and letter spacing always come from
	 *   the role. An explicit font size only replaces the size of the role.
	 * - The font metrics in components::Text are overwritten every frame
	 *   from components::Typography; change them through setTypeRole() and
	 *   setFontSize().
	 *
	 * @see components::Typography, systems::Typography, systems::MeasureText
	 */
	class Text:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Text, components::Color,
								 components::Typography>
	{
		public:
		/**
		 * @brief Font family kinds. No function of the library reads this
		 * enumeration.
		 */
		enum class Type { Normal, Condensed, Monospace, Serif, Slab };

		/**
		 * @brief Configures Text widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Text>
		{
			public:
			private:
			std::string _content;		  ///< Characters to draw.
			theme::TypeRole _typeRole;	  ///< Typescale role of the text.
			std::optional<float>
				_fontSize;	  ///< Explicit size overriding the role size, in
							  ///< dp; std::nullopt uses the role size.
			utility::graphic::Color32Bit _color;	///< Text color (RGBA).

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built texts are stored in.
			 * @param entityRegistry The registry that owns the texts built
			 * by `makeText`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a text from the current configuration, without
			 * registering it or giving it a parent.
			 * @return The new text.
			 */
			std::shared_ptr<Text> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: empty content, role `BodyMedium`,
			 * no explicit font size, and the OnSurface color of the active
			 * theme.
			 */
			void reset(void) override;

			/**
			 * @brief Set the characters to draw.
			 * @param content The text content. Empty by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withContent(const std::string &content);

			/**
			 * @brief Override the role font size with an explicit size.
			 *
			 * The other metrics still come from the role. A later call to
			 * withTypeRole() drops this size.
			 * @param fontSize The font size, in dp. By default the role
			 * size is used.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFontSize(const float &fontSize);

			/**
			 * @brief Set the typescale role and drop any explicit font
			 * size.
			 * @param role The typescale role, resolved against the active
			 * theme each frame. `BodyMedium` by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withTypeRole(theme::TypeRole role);

			/**
			 * @brief Set the text color.
			 * @param color The color (RGBA). By default the OnSurface color
			 * the active theme had when the builder was last reset.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Build a text with an explicit font size, register it
			 * under `parent` and reset the builder.
			 *
			 * The role is the one configured on the builder (`BodyMedium`
			 * unless withTypeRole() was called): it gives the weight, line
			 * height and letter spacing.
			 * @param parent The entity the text is attached to, or nullptr
			 * for a root entity.
			 * @param content The characters to draw.
			 * @param fontSize The font size, in dp.
			 * @param color The text color (RGBA).
			 * @return The registered text.
			 */
			std::shared_ptr<Text>
				makeText(std::shared_ptr<Entity> parent,
						 const std::string &content, const float &fontSize,
						 const utility::graphic::Color32Bit &color);

			/**
			 * @brief Build a text styled by a typescale role, register it
			 * under `parent` and reset the builder.
			 * @param parent The entity the text is attached to, or nullptr
			 * for a root entity.
			 * @param content The characters to draw.
			 * @param role The typescale role giving the font size, weight,
			 * line height and letter spacing.
			 * @param color The text color (RGBA).
			 * @return The registered text.
			 */
			std::shared_ptr<Text>
				makeText(std::shared_ptr<Entity> parent,
						 const std::string &content, theme::TypeRole role,
						 const utility::graphic::Color32Bit &color);
		};

		public:
		/**
		 * @brief Build a text without registering it in an entity registry.
		 *
		 * The arguments are forwarded to a private constructor:
		 * `(registry, content, role, color)`, optionally followed by an
		 * explicit font size, or `(registry, content, fontSize, color)`,
		 * which uses the role `theme::TypeRole::Default`. A text has no
		 * children to build.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new text. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Text> create(Arguments &&...arguments)
		{
			std::shared_ptr<Text> entity(
				new Text(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Text(void);

		/**
		 * @brief Replace the characters to draw.
		 *
		 * Writes components::Text. The Bound follows at the next Measure
		 * phase.
		 * @param content The new text content.
		 * @return Reference to this text for chaining.
		 */
		Text &setContent(const std::string &content);

		/**
		 * @brief Override the role font size with an explicit size.
		 *
		 * Writes components::Typography and resolves the font metrics at
		 * once. The weight, line height and letter spacing still come from
		 * the role.
		 * @param fontSize The new font size, in dp.
		 * @return Reference to this text for chaining.
		 */
		Text &setFontSize(const float &fontSize);

		/**
		 * @brief Set the typescale role and drop any explicit font size.
		 *
		 * Writes components::Typography and resolves the font metrics at
		 * once.
		 * @param role The typescale role, resolved against the active theme
		 * each frame.
		 * @return Reference to this text for chaining.
		 */
		Text &setTypeRole(theme::TypeRole role);

		/**
		 * @brief Get the typescale role the text is styled with.
		 * @return The role stored in components::Typography.
		 */
		theme::TypeRole getTypeRole(void) const;

		/**
		 * @brief Set the text color.
		 * @param color The new color (RGBA), written to components::Color.
		 * @return Reference to this text for chaining.
		 */
		Text &setColor(const utility::graphic::Color32Bit &color);

		private:
		/**
		 * @brief Access the typography component.
		 * @return The component holding the role and explicit size.
		 */
		components::Typography &typography(void);

		/**
		 * @brief Access the typography component.
		 * @return The component holding the role and explicit size.
		 */
		const components::Typography &typography(void) const;

		/**
		 * @brief Run systems::Typography on this entity right now, so the
		 * font metrics of components::Text match the role and explicit size
		 * without waiting for the next frame.
		 */
		void applyTypography(void);

		private:
		/**
		 * @brief Construct a text with an explicit font size and the role
		 * `theme::TypeRole::Default`.
		 * @param registry The registry the components are stored in.
		 * @param content The characters to draw.
		 * @param fontSize The font size, in dp.
		 * @param color The text color (RGBA).
		 */
		Text(ecs::ComponentRegistry &registry, const std::string &content,
			 const float &fontSize, const utility::graphic::Color32Bit &color);

		/**
		 * @brief Construct a text styled by a typescale role.
		 * @param registry The registry the components are stored in.
		 * @param content The characters to draw.
		 * @param role The typescale role giving the font metrics.
		 * @param color The text color (RGBA).
		 * @param fontSize An explicit size, in dp, overriding the role
		 * size; std::nullopt uses the role size.
		 */
		Text(ecs::ComponentRegistry &registry, const std::string &content,
			 theme::TypeRole role, const utility::graphic::Color32Bit &color,
			 std::optional<float> fontSize = std::nullopt);
	};

}	 // namespace guillaume::entities
