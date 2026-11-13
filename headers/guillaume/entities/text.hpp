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
	 * @brief Text component
	 */
	class Text:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Text, components::Color,
								 components::Typography>
	{
		public:
		/**
		 * @brief Text type, which can be Normal, Condensed, Monospace, Serif,
		 * or Slab.
		 */
		enum class Type { Normal, Condensed, Monospace, Serif, Slab };

		/**
		 * @brief Builder used to configure and create `Text` entities.
		 */
		class Builder: public EntityBuilderBase<Text>
		{
			public:
			private:
			std::string
				_content;	 ///< Text content to be set in the Text component
			theme::TypeRole _typeRole;	  ///< Typescale role of the text
			std::optional<float>
				_fontSize;	  ///< Explicit size overriding the role size
			utility::graphic::Color32Bit
				_color;	   ///< Color to be set in the Text component (RGBA)

			public:
			/**
			 * @brief Construct a new Text Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Text entity from the current configuration.
			 * @return The newly created Text.
			 */
			std::shared_ptr<Text> buildEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Text entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the text content for the Text component.
			 * @param content The text content to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withContent(const std::string &content);

			/**
			 * @brief Set the font size for the Text component.
			 * @param fontSize The font size to set.
			 * @return Reference to the builder for chaining.
			 */
			/**
			 * @brief Override the role font size with an explicit size.
			 * @param fontSize Font size, in dp.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFontSize(const float &fontSize);

			/**
			 * @brief Set the typescale role (default `BodyMedium`).
			 * @param role Typescale role.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withTypeRole(theme::TypeRole role);

			/**
			 * @brief Set the color for the Text component.
			 * @param color The color to set (RGBA).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Create a default Text entity using the builder.
			 * @param parent The parent entity to which the new text entity will
			 * be attached.
			 * @param content The text content for the default Text entity.
			 * @param fontSize The font size for the default Text entity.
			 * @param color The color for the default Text entity (RGBA).
			 * @return The entity identifier of the newly created text entity.
			 */
			std::shared_ptr<Text>
				makeText(std::shared_ptr<Entity> parent,
						 const std::string &content, const float &fontSize,
						 const utility::graphic::Color32Bit &color);

			/**
			 * @brief Build a text styled by a typescale role.
			 * @param parent Parent entity.
			 * @param content Text content.
			 * @param role Typescale role.
			 * @param color Text color.
			 * @return The registered text entity.
			 */
			std::shared_ptr<Text>
				makeText(std::shared_ptr<Entity> parent,
						 const std::string &content, theme::TypeRole role,
						 const utility::graphic::Color32Bit &color);
		};

		public:
		/**
		 * @brief Create the entity, fully built.
		 *
		 * Constructors are private: an entity must be owned before it can
		 * parent its children, so construction and child creation happen
		 * together here.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments.
		 * @return The new entity.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Text> create(Arguments &&...arguments)
		{
			std::shared_ptr<Text> entity(
				new Text(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Default destructor for the Text component.
		 */
		~Text(void);

		/**
		 * @brief Set the text content for this Text component.
		 * @param content The new text content to set.
		 * @return Reference to this Text component for chaining.
		 */
		Text &setContent(const std::string &content);

		/**
		 * @brief Set the font size for this Text component.
		 * @param fontSize The new font size to set.
		 * @return Reference to this Text component for chaining.
		 */
		Text &setFontSize(const float &fontSize);

		/**
		 * @brief Set the typescale role and drop any explicit size override.
		 * @param role Typescale role.
		 * @return Reference to this text for chaining.
		 */
		Text &setTypeRole(theme::TypeRole role);

		/**
		 * @brief Get the typescale role.
		 * @return The typescale role.
		 */
		theme::TypeRole getTypeRole(void) const;

		/**
		 * @brief Set the color for this Text component.
		 * @param color The new color to set (RGBA).
		 * @return Reference to this Text component for chaining.
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
		 * @brief Run systems::Typography on this entity right now.
		 */
		void applyTypography(void);

		private:
		/**
		 * @brief Default constructor for the Text component.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param content The text content to initialize the Text component
		 * with.
		 * @param fontSize The font size to initialize the Text component with.
		 * @param color The color to initialize the Text component with (RGBA).
		 */
		Text(ecs::ComponentRegistry &registry, const std::string &content,
			 const float &fontSize, const utility::graphic::Color32Bit &color);

		/**
		 * @brief Construct a text styled by a typescale role.
		 * @param registry Component registry.
		 * @param content Text content.
		 * @param role Typescale role.
		 * @param color Text color.
		 * @param fontSize Optional size overriding the role size.
		 */
		Text(ecs::ComponentRegistry &registry, const std::string &content,
			 theme::TypeRole role, const utility::graphic::Color32Bit &color,
			 std::optional<float> fontSize = std::nullopt);
	};

}	 // namespace guillaume::entities
