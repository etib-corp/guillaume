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

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/line.hpp"
#include "guillaume/components/text_field.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Text field entity: an editable input surface with optional affixes
	 * and supporting text.
	 *
	 * The text field reuses `SurfaceBase` for the layout, geometry and
	 * background surface, `components::TextField` to carry the
	 * placeholder/label/supporting/error state and `components::Line` for the
	 * filled variant bottom border. The Material Design variants are:
	 * - `Filled`: a surface-variant container with a bottom line.
	 * - `Outlined`: a transparent container with a full outline border.
	 *
	 * @see SurfaceBase
	 * @see components::TextField
	 * @see components::Line
	 */
	class TextField: public SurfaceBase<components::TextField, components::Line>
	{
		public:
		/**
		 * @brief Text field variant.
		 */
		enum class Variant { Filled, Outlined };

		/**
		 * @brief Builder used to configure and create `TextField` entities.
		 */
		class Builder: public EntityBuilderBase<TextField>
		{
			private:
			SurfaceConfig _config;					 ///< Surface config.
			Variant _variant { Variant::Filled };	 ///< Field variant.
			std::string _placeholder {};			 ///< Placeholder text.
			std::string _leadingIcon {};			 ///< Leading icon glyph.
			std::string _trailingIcon {};			 ///< Trailing icon glyph.
			std::string _supportingText {};			 ///< Supporting text.
			bool _error { false };					 ///< Error state.
			std::string _text {};					 ///< Input text.

			public:
			/**
			 * @brief Construct a new TextField Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the TextField entity from the current configuration.
			 * @return The newly created TextField.
			 */
			std::shared_ptr<TextField> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the text field pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the text field variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the placeholder text.
			 * @param placeholder The new placeholder.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPlaceholder(const std::string &placeholder);

			/**
			 * @brief Set the leading icon glyph name.
			 * @param iconGlyph The new leading icon glyph name.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLeadingIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the trailing icon glyph name.
			 * @param iconGlyph The new trailing icon glyph name.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTrailingIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the supporting text.
			 * @param supportingText The new supporting text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSupportingText(const std::string &supportingText);

			/**
			 * @brief Set the error state.
			 * @param error The new error state.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withError(bool error);

			/**
			 * @brief Set the initial input text.
			 * @param text The new input text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withText(const std::string &text);
		};

		/**
		 * @brief Director that orchestrates `TextField::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a text field using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The text field variant.
			 * @param placeholder The placeholder text.
			 * @param leadingIcon The leading icon glyph name.
			 * @param trailingIcon The trailing icon glyph name.
			 * @param supportingText The supporting text.
			 * @param error The error state.
			 * @param text The initial input text.
			 * @return The newly created text field.
			 */
			std::shared_ptr<TextField> makeTextField(
				Builder &builder, std::shared_ptr<ecs::Entity> parent,
				Variant variant, const std::string &placeholder,
				const std::string &leadingIcon, const std::string &trailingIcon,
				const std::string &supportingText, bool error,
				const std::string &text = "");
		};

		private:
		Variant _variant { Variant::Filled };		  ///< Text field variant.
		std::string _placeholder {};				  ///< Placeholder text.
		std::string _leadingIcon {};				  ///< Leading icon glyph.
		std::string _trailingIcon {};				  ///< Trailing icon glyph.
		std::string _supportingText {};				  ///< Supporting text.
		bool _error { false };						  ///< Error state.
		std::string _text {};						  ///< Input text.
		std::shared_ptr<Icon> _leadingIconEntity;	  ///< Leading icon child.
		std::shared_ptr<Icon> _trailingIconEntity;	  ///< Trailing icon child.
		std::shared_ptr<Text> _textEntity;			  ///< Input text child.
		std::shared_ptr<Text> _supportingEntity;	  ///< Supporting child.

		private:
		/**
		 * @brief Apply the variant-specific geometry, colors and line.
		 */
		void applyVariant(void);

		/**
		 * @brief Synchronize the text field component fields.
		 */
		void applyComponent(void);

		/**
		 * @brief Build the affix, input and supporting child entities.
		 */
		void buildContent(void);

		public:
		/**
		 * @brief Construct a TextField entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The text field variant.
		 * @param placeholder The placeholder text.
		 * @param leadingIcon The leading icon glyph name.
		 * @param trailingIcon The trailing icon glyph name.
		 * @param supportingText The supporting text.
		 * @param error The error state.
		 * @param text The initial input text.
		 */
		TextField(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				  Variant variant, const std::string &placeholder,
				  const std::string &leadingIcon,
				  const std::string &trailingIcon,
				  const std::string &supportingText, bool error,
				  const std::string &text);

		/**
		 * @brief Default destructor.
		 */
		~TextField(void) override;

		/**
		 * @brief Set the text field variant.
		 * @param variant The new variant.
		 * @return Reference to this field for chaining.
		 */
		TextField &setVariant(Variant variant);

		/**
		 * @brief Get the text field variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the placeholder text.
		 * @param placeholder The new placeholder.
		 * @return Reference to this field for chaining.
		 */
		TextField &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Set the input text.
		 * @param text The new input text.
		 * @return Reference to this field for chaining.
		 */
		TextField &setText(const std::string &text);

		/**
		 * @brief Get the input text.
		 * @return The input text.
		 */
		std::string getText(void) const;

		/**
		 * @brief Set the error state.
		 * @param error The new error state.
		 * @return Reference to this field for chaining.
		 */
		TextField &setError(bool error);

		/**
		 * @brief Whether the field is in an error state.
		 * @return True when in error.
		 */
		bool isError(void) const;

		/**
		 * @brief Set the supporting text.
		 * @param supportingText The new supporting text.
		 * @return Reference to this field for chaining.
		 */
		TextField &setSupportingText(const std::string &supportingText);

		/**
		 * @brief Initialize the text field derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the text field derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
