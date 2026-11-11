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
#include <functional>
#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/line.hpp"
#include "guillaume/components/text_field.hpp"
#include "guillaume/components/text_field_configuration.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A box the user types text into, with a label, optional icons,
	 * prefix and suffix, and a helper or error text under it (Paper
	 * `TextInput` and `HelperText`).
	 *
	 * Its state lives in two components: components::TextField holds the
	 * label, prefix, suffix, supporting text, error text and error state;
	 * components::TextFieldConfiguration holds the variant, the flags, the
	 * callbacks and the identifiers of the children. systems::TextField
	 * styles the box, arranges its children and reports edits each frame,
	 * in the Layout phase. The class itself holds no field: its setters
	 * write the components and restyle at once.
	 *
	 * @code
	 * TextField::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto field = builder.makeTextField(parent, TextField::Variant::Outlined,
	 * 	"Email", "mail", "", "We never share it", false);
	 * field->setOnChange([](const std::string &text) {
	 * 	// called when the user edits the text
	 * });
	 * @endcode
	 *
	 * `TextField::create(registry, config, variant, placeholder, leadingIcon,
	 * trailingIcon, supportingText, error, text)` builds the same widget
	 * without registering it in an entity registry.
	 *
	 * On screen, from start to end: the leading icon, the prefix, the input
	 * column (label and typed text), the suffix, then the trailing icon,
	 * which the "error" icon replaces in the error state. The supporting
	 * text, or the error text in the error state when there is one, is
	 * drawn under the box.
	 *
	 * The label sits in the place of the typed text while the field is
	 * empty and unfocused. While the field is focused or holds text the
	 * label floats: it becomes smaller and moves above the text (Filled)
	 * or onto the top border (Outlined). The prefix and suffix only show
	 * while the label floats.
	 *
	 * The variants are:
	 * - `Filled`: a tinted box with a line at the bottom.
	 * - `Outlined`: a transparent box with a border all around.
	 *
	 * The bottom line or the border is 2 dp thick and in the primary color
	 * while the field is focused, 1 dp otherwise, and in the error color in
	 * the error state.
	 *
	 * Things to know:
	 * - The box is 56 dp high; 52 dp when dense with a label, 40 dp when
	 *   dense without one. A multiline field takes the height of its
	 *   content. The width follows the content, with a minimum of
	 *   `MinWidth` (100 dp), unless setWidth() fixes it.
	 * - The typed text lives in the components::Text of the input child
	 *   (getInputIdentifier()). systems::TextField gives that child a
	 *   components::Focus while the field is focused, editable and
	 *   enabled; that is what lets the text input systems edit it.
	 * - The change handler is called by systems::TextField when it sees
	 *   that the typed text differs from the one it knew. setText() does
	 *   not call it.
	 * - The field is the entity registry of its children; they are built
	 *   by this class and destroyed with it.
	 * - systems::TextField overwrites the layout settings, size, colors,
	 *   border and bottom line every frame.
	 * - The leading and trailing icons are fixed at creation.
	 *
	 * @see components::TextField, components::TextFieldConfiguration,
	 * systems::TextField
	 */
	class TextField:
		public SurfaceBase<components::TextField, components::Line,
						   components::TextFieldConfiguration>
	{
		public:
		/// Look of the field: Filled or Outlined.
		using Variant = components::TextFieldConfiguration::Variant;

		/**
		 * @brief Smallest width of the box, in dp (100).
		 */
		static constexpr float MinWidth =
			components::TextFieldConfiguration::MinWidth;

		/**
		 * @brief Configures a TextField, then builds it and registers it in
		 * the entity registry given at construction.
		 */
		class Builder: public EntityBuilderBase<TextField>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and layout of the box.
			Variant _variant { Variant::Filled };	 ///< Field variant.
			std::string _placeholder {};			 ///< Label text.
			std::string _leadingIcon {};	   ///< Leading icon glyph name.
			std::string _trailingIcon {};	   ///< Trailing icon glyph name.
			std::string _supportingText {};	   ///< Text under the box.
			bool _error { false };			   ///< Error state.
			std::string _text {};			   ///< Initial typed text.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry the field's components
			 * are created in.
			 * @param entityRegistry The registry the built fields are added
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a text field and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new text field.
			 */
			std::shared_ptr<TextField> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: Filled variant, no label, no
			 * icon, no supporting text, no error, empty text and a default
			 * pose.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose the field is created with.
			 * @param pose The position and orientation of the box. Default:
			 * a default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the field variant.
			 * @param variant The variant. Default: `Variant::Filled`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the label.
			 *
			 * The label is shown in place of the typed text while the field
			 * is empty and unfocused, and floats above it otherwise.
			 * @param placeholder The label text. Default: empty, which
			 * builds no label.
			 * @return This builder, for chaining.
			 */
			Builder &withPlaceholder(const std::string &placeholder);

			/**
			 * @brief Set the icon shown at the start of the box.
			 * @param iconGlyph The glyph name. Default: empty, no icon.
			 * @return This builder, for chaining.
			 */
			Builder &withLeadingIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the icon shown at the end of the box.
			 * @param iconGlyph The glyph name. Default: empty, no icon. The
			 * "error" icon takes its place in the error state.
			 * @return This builder, for chaining.
			 */
			Builder &withTrailingIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the helper text shown under the box.
			 * @param supportingText The text. Default: empty, no text.
			 * @return This builder, for chaining.
			 */
			Builder &withSupportingText(const std::string &supportingText);

			/**
			 * @brief Choose whether the field starts in the error state.
			 * @param error True for the error state. Default: false.
			 * @return This builder, for chaining.
			 */
			Builder &withError(bool error);

			/**
			 * @brief Set the text the field holds when created.
			 * @param text The initial text. Default: empty.
			 * @return This builder, for chaining.
			 */
			Builder &withText(const std::string &text);

			/**
			 * @brief Build a text field, register it under @p parent and
			 * reset the builder.
			 *
			 * The arguments replace every setting but the pose.
			 * @param parent The entity the field is attached to.
			 * @param variant The field variant.
			 * @param placeholder The label text (empty: no label).
			 * @param leadingIcon The glyph name of the icon at the start of
			 * the box (empty: none).
			 * @param trailingIcon The glyph name of the icon at the end of
			 * the box (empty: none).
			 * @param supportingText The helper text under the box (empty:
			 * none).
			 * @param error True to start in the error state.
			 * @param text The initial text.
			 * @return The registered text field.
			 */
			std::shared_ptr<TextField>
				makeTextField(std::shared_ptr<ecs::Entity> parent,
							  Variant variant, const std::string &placeholder,
							  const std::string &leadingIcon,
							  const std::string &trailingIcon,
							  const std::string &supportingText, bool error,
							  const std::string &text = "");
		};

		/**
		 * @brief Create a text field and its children, without registering
		 * it in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * (registry, config, variant, placeholder, leadingIcon,
		 * trailingIcon, supportingText, error, text). The children are
		 * built here, by build(), because a child needs its parent to be
		 * owned by a shared pointer.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new text field, with no parent.
		 */
		template<typename... Arguments>
		static std::shared_ptr<TextField> create(Arguments &&...arguments)
		{
			std::shared_ptr<TextField> entity(
				new TextField(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~TextField(void) override;

		/**
		 * @brief Change the field variant. Restyles at once.
		 * @param variant The new variant, Filled or Outlined.
		 * @return This field, for chaining.
		 */
		TextField &setVariant(Variant variant);

		/**
		 * @brief Get the field variant.
		 * @return The variant stored in components::TextFieldConfiguration.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the label text.
		 *
		 * Builds the label child when the field had none and
		 * @p placeholder is not empty, then restyles at once.
		 * @param placeholder The new label text.
		 * @return This field, for chaining.
		 */
		TextField &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Replace the typed text. Restyles at once.
		 *
		 * Does not call the change handler: only an edit by the user does.
		 * @param text The new text.
		 * @return This field, for chaining.
		 */
		TextField &setText(const std::string &text);

		/**
		 * @brief Get the typed text.
		 * @return The content of the input child, including what the user
		 * typed; empty when the field holds no text.
		 */
		std::string getText(void) const;

		/**
		 * @brief Put the field in or out of the error state.
		 *
		 * In the error state the line or border and the text under the box
		 * take the error color, the "error" icon replaces the trailing
		 * icon, and the error text, when there is one, replaces the
		 * supporting text. Restyles at once.
		 * @param error True for the error state.
		 * @return This field, for chaining.
		 */
		TextField &setError(bool error);

		/**
		 * @brief Check whether the field is in the error state.
		 * @return True in the error state.
		 */
		bool isError(void) const;

		/**
		 * @brief Set the text shown under the box in the error state.
		 *
		 * Builds the child that shows it when missing, then restyles at
		 * once.
		 * @param errorText The error text. When empty, the supporting text
		 * stays shown in the error state.
		 * @return This field, for chaining.
		 */
		TextField &setErrorText(const std::string &errorText);

		/**
		 * @brief Set the helper text shown under the box.
		 *
		 * Builds the child that shows it when missing, then restyles at
		 * once.
		 * @param supportingText The helper text; empty shows none.
		 * @return This field, for chaining.
		 */
		TextField &setSupportingText(const std::string &supportingText);

		/**
		 * @brief Set the text shown just before the typed text, such as a
		 * currency sign.
		 *
		 * It is visible only while the label floats. Builds the prefix
		 * child when missing, then restyles at once.
		 * @param prefix The prefix text; empty shows none.
		 * @return This field, for chaining.
		 */
		TextField &setPrefix(const std::string &prefix);

		/**
		 * @brief Set the text shown just after the typed text, such as a
		 * unit.
		 *
		 * It is visible only while the label floats. Builds the suffix
		 * child when missing, then restyles at once.
		 * @param suffix The suffix text; empty shows none.
		 * @return This field, for chaining.
		 */
		TextField &setSuffix(const std::string &suffix);

		/**
		 * @brief Enable or disable the field.
		 *
		 * A disabled field is drawn with the disabled colors and cannot be
		 * edited. Restyles at once.
		 * @param disabled True to disable the field.
		 * @return This field, for chaining.
		 */
		TextField &setDisabled(bool disabled);

		/**
		 * @brief Check whether the field is disabled.
		 * @return True when disabled.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Allow or forbid typing in the field (Paper `editable`).
		 *
		 * A field that is not editable keeps its normal look and can still
		 * take the focus. Restyles at once.
		 * @param editable True to let the user edit the text. Default:
		 * true.
		 * @return This field, for chaining.
		 */
		TextField &setEditable(bool editable);

		/**
		 * @brief Check whether the user may edit the text.
		 * @return True when editable.
		 */
		bool isEditable(void) const;

		/**
		 * @brief Use the smaller, dense height (Paper `dense`).
		 *
		 * A dense box is 52 dp high with a label and 40 dp without, instead
		 * of 56 dp. Ignored while the field is multiline. Restyles at once.
		 * @param dense True for the dense height.
		 * @return This field, for chaining.
		 */
		TextField &setDense(bool dense);

		/**
		 * @brief Check whether the field uses the dense height.
		 * @return True when dense.
		 */
		bool isDense(void) const;

		/**
		 * @brief Let the height of the box follow its content instead of
		 * being fixed (Paper `multiline`). Restyles at once.
		 * @param multiline True to let the height follow the content.
		 * @return This field, for chaining.
		 */
		TextField &setMultiline(bool multiline);

		/**
		 * @brief Check whether the height follows the content.
		 * @return True when multiline.
		 */
		bool isMultiline(void) const;

		/**
		 * @brief Fix the width of the box instead of letting it follow the
		 * content. Restyles at once.
		 * @param width The width, in dp. A value under MinWidth (100 dp)
		 * is raised to MinWidth.
		 * @return This field, for chaining.
		 */
		TextField &setWidth(float width);

		/**
		 * @brief Check whether the label floats above the typed text.
		 * @return True while the field is focused or holds text.
		 */
		bool isLabelFloating(void) const;

		/**
		 * @brief Check whether the field has the keyboard focus.
		 * @return True when focused.
		 */
		bool isFocused(void) const;

		/**
		 * @brief Set the handler called when the user edits the text
		 * (Paper `onChangeText`).
		 *
		 * systems::TextField calls it with the new text when it sees that
		 * the typed text differs from the one it knew. setText() does not
		 * call it.
		 * @param onChange The handler, given the new text; an empty one
		 * removes it.
		 * @return This field, for chaining.
		 */
		TextField &
			setOnChange(std::function<void(const std::string &)> onChange);

		/**
		 * @brief Set the handler called when the field gains the focus.
		 * @param onFocus The handler; an empty one removes it.
		 * @return This field, for chaining.
		 */
		TextField &setOnFocus(std::function<void(void)> onFocus);

		/**
		 * @brief Set the handler called when the field loses the focus.
		 * @param onBlur The handler; an empty one removes it.
		 * @return This field, for chaining.
		 */
		TextField &setOnBlur(std::function<void(void)> onBlur);

		/**
		 * @brief Get the identifier of the label child.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * the field has no label.
		 */
		ecs::Entity::Identifier getLabelIdentifier(void) const;

		/**
		 * @brief Get the identifier of the child that holds the typed
		 * text.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier`
		 * before the children are built.
		 */
		ecs::Entity::Identifier getInputIdentifier(void) const;

		/**
		 * @brief Get the identifier of the child that shows the supporting
		 * or error text under the box.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * the field never had such a text.
		 */
		ecs::Entity::Identifier getSupportingIdentifier(void) const;

		/**
		 * @brief Get the identifier of the prefix child.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * the field never had a prefix.
		 */
		ecs::Entity::Identifier getPrefixIdentifier(void) const;

		/**
		 * @brief Get the identifier of the suffix child.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier` when
		 * the field never had a suffix.
		 */
		ecs::Entity::Identifier getSuffixIdentifier(void) const;

		/**
		 * @brief Get the identifier of the "error" icon shown in the error
		 * state.
		 *
		 * The icon exists outside the error state too, painted
		 * transparent.
		 * @return The identifier, or `ecs::Entity::InvalidIdentifier`
		 * before the children are built.
		 */
		ecs::Entity::Identifier getErrorIconIdentifier(void) const;

		private:
		/**
		 * @brief Create the child entities the current state requires and
		 * that do not exist yet, and record them in
		 * components::TextFieldConfiguration.
		 */
		void buildContent(void);

		/**
		 * @brief Get the configuration component of this field.
		 * @return The component.
		 */
		components::TextFieldConfiguration &configuration(void);

		/**
		 * @brief Get the configuration component of this field.
		 * @return The component.
		 */
		const components::TextFieldConfiguration &configuration(void) const;

		/**
		 * @brief Get the text field component of this field.
		 * @return The component.
		 */
		components::TextField &field(void);

		/**
		 * @brief Build the missing children, then restyle and re-arrange
		 * the field.
		 */
		void refresh(void);

		private:
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
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
