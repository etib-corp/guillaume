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
#include <optional>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/line.hpp"
#include "guillaume/components/text_field.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Text field entity (Paper `TextInput` and `HelperText`).
	 *
	 * The text field reuses `SurfaceBase` for the layout, geometry and
	 * background surface, `components::TextField` to carry the
	 * label/placeholder/prefix/suffix/supporting/error state and
	 * `components::Line` for the filled variant bottom indicator. The field
	 * lays out an optional leading icon, an input column (floating label over
	 * the input), optional prefix/suffix affixes and an optional trailing icon
	 * (replaced by an error icon in the error state). The supporting or error
	 * text is painted below the field.
	 *
	 * The label rests in the input position (`bodyLarge`) and floats
	 * (`bodySmall`) while the field is focused or populated: above the input in
	 * the filled variant, on the outline in the outlined variant. The active
	 * indicator is 2 dp `primary` while focused, `error` in the error state,
	 * and `onSurface` at the disabled opacities while disabled. The width
	 * follows the content with a 100 dp minimum unless `setWidth()` forces it.
	 *
	 * The Material Design variants are:
	 * - `Filled`: a `surfaceContainerHighest` container with a bottom line.
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
		 * @brief Smallest intrinsic width.
		 */
		static constexpr float MinWidth = 100.0f;

		/**
		 * @brief Builder used to configure and create text fields.
		 */
		class Builder: public EntityBuilderBase<TextField>
		{
			private:
			SurfaceConfig _config;					 ///< Surface config.
			Variant _variant { Variant::Filled };	 ///< Field variant.
			std::string _placeholder {};			 ///< Label text.
			std::string _leadingIcon {};			 ///< Leading icon glyph.
			std::string _trailingIcon {};			 ///< Trailing icon glyph.
			std::string _supportingText {};			 ///< Supporting text.
			bool _error { false };					 ///< Error state.
			std::string _text {};					 ///< Input text.

			public:
			/**
			 * @brief Construct a text field builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the text field.
			 * @return The text field.
			 */
			std::shared_ptr<TextField> buildEntity(void) override;

			/**
			 * @brief Reset the builder to the M3 defaults.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose.
			 * @param pose The pose.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant The variant.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the label (shown as the resting placeholder).
			 * @param placeholder The label.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPlaceholder(const std::string &placeholder);

			/**
			 * @brief Set the leading icon.
			 * @param iconGlyph The glyph.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withLeadingIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the trailing icon.
			 * @param iconGlyph The glyph.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withTrailingIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the supporting text.
			 * @param supportingText The text.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withSupportingText(const std::string &supportingText);

			/**
			 * @brief Set the error state.
			 * @param error The state.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withError(bool error);

			/**
			 * @brief Set the input text.
			 * @param text The text.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withText(const std::string &text);
		};

		/**
		 * @brief Director building text fields.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Build a text field.
			 * @param builder Text field builder.
			 * @param parent Parent entity.
			 * @param variant Field variant.
			 * @param placeholder Label text.
			 * @param leadingIcon Leading icon glyph.
			 * @param trailingIcon Trailing icon glyph.
			 * @param supportingText Supporting text.
			 * @param error Error state.
			 * @param text Input text.
			 * @return The registered text field.
			 */
			std::shared_ptr<TextField> makeTextField(
				Builder &builder, std::shared_ptr<ecs::Entity> parent,
				Variant variant, const std::string &placeholder,
				const std::string &leadingIcon, const std::string &trailingIcon,
				const std::string &supportingText, bool error,
				const std::string &text = "");
		};

		/**
		 * @brief Construct a text field.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The variant.
		 * @param placeholder The label text.
		 * @param leadingIcon The leading icon glyph.
		 * @param trailingIcon The trailing icon glyph.
		 * @param supportingText The supporting text.
		 * @param error The error state.
		 * @param text The input text.
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

		/** @brief Set the variant. @param variant New variant. @return *this.
		 */
		TextField &setVariant(Variant variant);

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const;

		/**
		 * @brief Set the label text.
		 * @param placeholder The label.
		 * @return Reference to this field for chaining.
		 */
		TextField &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Set the input text.
		 * @param text The text.
		 * @return Reference to this field for chaining.
		 */
		TextField &setText(const std::string &text);

		/** @brief Get the input text. @return The text. */
		std::string getText(void) const;

		/** @brief Set the error state. @param error New state. @return *this.
		 */
		TextField &setError(bool error);

		/** @brief Check the error state. @return True in error. */
		bool isError(void) const;

		/**
		 * @brief Set the text shown below the field in the error state.
		 * @param errorText The error text.
		 * @return Reference to this field for chaining.
		 */
		TextField &setErrorText(const std::string &errorText);

		/**
		 * @brief Set the supporting text.
		 * @param supportingText The text.
		 * @return Reference to this field for chaining.
		 */
		TextField &setSupportingText(const std::string &supportingText);

		/**
		 * @brief Set the prefix affix (shown while the label floats).
		 * @param prefix The prefix.
		 * @return Reference to this field for chaining.
		 */
		TextField &setPrefix(const std::string &prefix);

		/**
		 * @brief Set the suffix affix (shown while the label floats).
		 * @param suffix The suffix.
		 * @return Reference to this field for chaining.
		 */
		TextField &setSuffix(const std::string &suffix);

		/**
		 * @brief Enable or disable the field.
		 * @param disabled Whether the field is disabled.
		 * @return Reference to this field for chaining.
		 */
		TextField &setDisabled(bool disabled);

		/** @brief Check the disabled state. @return True when disabled. */
		bool isDisabled(void) const;

		/**
		 * @brief Allow or forbid editing (Paper `editable`).
		 * @param editable Whether the input accepts text.
		 * @return Reference to this field for chaining.
		 */
		TextField &setEditable(bool editable);

		/** @brief Check the editable flag. @return True when editable. */
		bool isEditable(void) const;

		/**
		 * @brief Use the dense height (Paper `dense`).
		 * @param dense Whether the field is dense.
		 * @return Reference to this field for chaining.
		 */
		TextField &setDense(bool dense);

		/** @brief Check the dense flag. @return True when dense. */
		bool isDense(void) const;

		/**
		 * @brief Let the height follow the content (Paper `multiline`).
		 * @param multiline Whether the field is multiline.
		 * @return Reference to this field for chaining.
		 */
		TextField &setMultiline(bool multiline);

		/** @brief Check the multiline flag. @return True when multiline. */
		bool isMultiline(void) const;

		/**
		 * @brief Force the width instead of the intrinsic width.
		 * @param width The width (clamped to MinWidth).
		 * @return Reference to this field for chaining.
		 */
		TextField &setWidth(float width);

		/**
		 * @brief Check whether the label floats.
		 * @return True while focused or populated.
		 */
		bool isLabelFloating(void) const;

		/**
		 * @brief Check whether the field owns the input focus.
		 * @return True when focused.
		 */
		bool isFocused(void) const;

		/**
		 * @brief Set the handler called with the new text when it changes
		 * (Paper `onChangeText`).
		 * @param onChange The handler.
		 * @return Reference to this field for chaining.
		 */
		TextField &
			setOnChange(std::function<void(const std::string &)> onChange);

		/**
		 * @brief Set the handler called when the field gains the focus.
		 * @param onFocus The handler.
		 * @return Reference to this field for chaining.
		 */
		TextField &setOnFocus(std::function<void(void)> onFocus);

		/**
		 * @brief Set the handler called when the field loses the focus.
		 * @param onBlur The handler.
		 * @return Reference to this field for chaining.
		 */
		TextField &setOnBlur(std::function<void(void)> onBlur);

		/** @brief Get the label child. @return The identifier or Invalid. */
		ecs::Entity::Identifier getLabelIdentifier(void) const;

		/** @brief Get the input child. @return The identifier or Invalid. */
		ecs::Entity::Identifier getInputIdentifier(void) const;

		/**
		 * @brief Get the supporting/error text child.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getSupportingIdentifier(void) const;

		/** @brief Get the prefix child. @return The identifier or Invalid. */
		ecs::Entity::Identifier getPrefixIdentifier(void) const;

		/** @brief Get the suffix child. @return The identifier or Invalid. */
		ecs::Entity::Identifier getSuffixIdentifier(void) const;

		/** @brief Get the error icon child. @return The identifier or Invalid.
		 */
		ecs::Entity::Identifier getErrorIconIdentifier(void) const;

		/**
		 * @brief Build the children.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle and re-arrange the field.
		 */
		void update(void) override;

		private:
		Variant _variant { Variant::Filled };	 ///< Text field variant.
		std::string _placeholder {};			 ///< Label text.
		std::string _leadingIcon {};			 ///< Leading icon glyph.
		std::string _trailingIcon {};			 ///< Trailing icon glyph.
		std::string _supportingText {};			 ///< Supporting text.
		bool _error { false };					 ///< Error state.
		std::string _text {};					 ///< Last known input text.
		bool _editable { true };				 ///< Editable flag.
		bool _dense { false };					 ///< Dense flag.
		bool _multiline { false };				 ///< Multiline flag.
		std::optional<float> _width {};			 ///< Forced width.
		std::function<void(const std::string &)>
			_onChange {};							  ///< Text change handler.
		std::function<void(void)> _onFocus {};		  ///< Focus handler.
		std::function<void(void)> _onBlur {};		  ///< Blur handler.
		std::shared_ptr<Icon> _leadingIconEntity;	  ///< Leading icon child.
		std::shared_ptr<Icon> _trailingIconEntity;	  ///< Trailing icon child.
		std::shared_ptr<Icon> _errorIconEntity;		  ///< Error icon child.
		std::shared_ptr<View> _inputColumn;			  ///< Label + input.
		std::shared_ptr<Text> _labelEntity;			  ///< Floating label.
		std::shared_ptr<Text> _textEntity;			  ///< Input text child.
		std::shared_ptr<Text> _prefixEntity;		  ///< Prefix child.
		std::shared_ptr<Text> _suffixEntity;		  ///< Suffix child.
		std::shared_ptr<Text> _supportingEntity;	  ///< Supporting child.

		/**
		 * @brief Apply the variant geometry and state colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Mirror the state on the TextField component.
		 */
		void applyComponent(void);

		/**
		 * @brief Build and arrange the children.
		 */
		void buildContent(void);

		/**
		 * @brief Give the input child the focus component while it may be
		 * edited.
		 */
		void applyEditing(void);

		/**
		 * @brief Place the label (outlined) and the supporting text manually.
		 */
		void placeManualChildren(void);

		/**
		 * @brief Get the field height of the current flags.
		 * @return The height.
		 */
		float getFieldHeight(void) const;

		/**
		 * @brief Get the color of the label and active indicator.
		 * @return The color.
		 */
		utility::graphic::Color32Bit getActiveColor(void) const;
	};

}	 // namespace guillaume::entities
