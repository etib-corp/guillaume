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

#include "guillaume/systems/text_field.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/typography.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/placement_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/typography.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Configuration = components::TextFieldConfiguration;
		using Variant		= Configuration::Variant;
		using style::disabledContainerColor;
		using style::disabledContentColor;
		using style::schemeColor;
		using style::transparentColor;

		/// Paper filled disabled container opacity (onSurface at 4%).
		constexpr std::uint8_t DisabledFilledAlpha = 10U;

		/**
		 * @brief Get the color of the label and active indicator.
		 * @param disabled Whether the field is disabled.
		 * @param error Whether the field is in error.
		 * @param focused Whether the field is focused.
		 * @return The color.
		 */
		utility::graphic::Color32Bit activeColor(bool disabled, bool error,
												 bool focused)
		{
			if (disabled) {
				return disabledContentColor();
			}
			if (error) {
				return schemeColor(SchemeColorRole::Error);
			}
			if (focused) {
				return schemeColor(SchemeColorRole::Primary);
			}
			return schemeColor(SchemeColorRole::OnSurfaceVariant);
		}

		/**
		 * @brief Set the color of a child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param color The color to write.
		 */
		void colorChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const utility::graphic::Color32Bit &color)
		{
			if (registry.hasComponent<components::Color>(child)) {
				registry.getComponent<components::Color>(child).setColor(color);
			}
		}

		/**
		 * @brief Set the text of a child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param content The text to write.
		 */
		void writeChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const std::string &content)
		{
			if (registry.hasComponent<components::Text>(child)) {
				registry.getComponent<components::Text>(child).setContent(
					content);
			}
		}

		/**
		 * @brief Arrange the layout children of a container, when it is one.
		 * @param registry The registry holding the components.
		 * @param container The container identifier (may be invalid).
		 */
		void arrange(ecs::ComponentRegistry &registry,
					 ecs::Entity::Identifier container)
		{
			if (!registry.hasComponent<components::Layout>(container)
				|| !registry.hasComponent<components::Transform>(container)
				|| !registry.hasComponent<components::Bound>(container)) {
				return;
			}
			Layout::apply(registry, container,
						  registry.getComponent<components::Layout>(container)
							  .getChildren(),
						  Layout::layerOf(registry, container));
		}

		/**
		 * @brief Call the change callback when the input text differs from
		 * the last known one.
		 * @param registry The registry holding the components.
		 * @param identifier The text field entity.
		 */
		void reportChange(ecs::ComponentRegistry &registry,
						  ecs::Entity::Identifier identifier)
		{
			auto &configuration =
				registry.getComponent<Configuration>(identifier);
			const std::string current = TextField::text(registry, identifier);

			if (current == configuration.getText()) {
				return;
			}
			configuration.setText(current);

			// Copy: the callback may replace itself.
			const auto onChange = configuration.getOnChange();
			if (onChange) {
				onChange(current);
			}
		}

		/**
		 * @brief Write the content, type role and color of every child for
		 * the enabled, error and floating states.
		 * @param registry The registry holding the components.
		 * @param identifier The text field entity.
		 */
		void styleChildren(ecs::ComponentRegistry &registry,
						   ecs::Entity::Identifier identifier)
		{
			const auto &configuration =
				registry.getComponent<Configuration>(identifier);
			const auto &field =
				registry.getComponent<components::TextField>(identifier);
			const bool disabled = TextField::isDisabled(registry, identifier);
			const bool floating =
				TextField::isLabelFloating(registry, identifier);
			const bool error		= field.isError();
			const auto content		= disabled
					 ? disabledContentColor()
					 : schemeColor(SchemeColorRole::OnSurface);
			const auto variantColor = disabled
				? disabledContentColor()
				: schemeColor(SchemeColorRole::OnSurfaceVariant);
			const auto errorColor	= (error && !disabled)
				  ? schemeColor(SchemeColorRole::Error)
				  : transparentColor();
			const auto label		= configuration.getLabelEntity();

			writeChild(registry, label, field.getPlaceholder());
			writeChild(registry, configuration.getPrefixEntity(),
					   field.getPrefix());
			writeChild(registry, configuration.getSuffixEntity(),
					   field.getSuffix());
			writeChild(registry, configuration.getSupportingEntity(),
					   TextField::supportingText(registry, identifier));

			// The label rests at the input size and floats at the small one.
			if (registry.hasComponent<components::Typography>(label)
				&& registry.hasComponent<components::Text>(label)) {
				registry.getComponent<components::Typography>(label)
					.setRole(floating ? theme::TypeRole::BodySmall
									  : theme::TypeRole::BodyLarge)
					.setFontSize(std::nullopt);
				Typography::apply(registry, label);
			}

			colorChild(registry, configuration.getInputEntity(), content);
			colorChild(registry, label,
					   floating ? activeColor(disabled, error,
											  TextField::isFocused(registry,
																   identifier))
								: variantColor);
			colorChild(registry, configuration.getLeadingIconEntity(),
					   variantColor);
			colorChild(registry, configuration.getTrailingIconEntity(),
					   error ? transparentColor() : variantColor);
			colorChild(registry, configuration.getErrorIconEntity(),
					   errorColor);
			colorChild(registry, configuration.getPrefixEntity(),
					   floating ? variantColor : transparentColor());
			colorChild(registry, configuration.getSuffixEntity(),
					   floating ? variantColor : transparentColor());
			colorChild(registry, configuration.getSupportingEntity(),
					   (error && !disabled) ? errorColor : variantColor);
		}

		/**
		 * @brief Order the children in the input column and in the row, and
		 * size the field to them.
		 * @param registry The registry holding the components.
		 * @param identifier The text field entity.
		 */
		void arrangeChildren(ecs::ComponentRegistry &registry,
							 ecs::Entity::Identifier identifier)
		{
			const auto invalid = ecs::Entity::InvalidIdentifier;
			const auto &configuration =
				registry.getComponent<Configuration>(identifier);
			const auto &field =
				registry.getComponent<components::TextField>(identifier);
			const bool floating =
				TextField::isLabelFloating(registry, identifier);
			const auto column = configuration.getInputColumn();
			const auto label  = configuration.getLabelEntity();
			const auto input  = configuration.getInputEntity();

			// Input column: the filled label floats above the input, the
			// outlined label floats on the outline (placed manually).
			if (registry.hasComponent<components::Layout>(column)) {
				std::vector<ecs::Entity::Identifier> columnChildren;
				if (label != invalid
					&& (!floating
						|| configuration.getVariant() == Variant::Filled)) {
					columnChildren.push_back(label);
				}
				if (input != invalid && (floating || label == invalid)) {
					columnChildren.push_back(input);
				}
				registry.getComponent<components::Layout>(column).setChildren(
					columnChildren);
				// Resolves the column bound the row is sized from.
				arrange(registry, column);
			}

			std::vector<ecs::Entity::Identifier> children;
			if (configuration.getLeadingIconEntity() != invalid) {
				children.push_back(configuration.getLeadingIconEntity());
			}
			if (configuration.getPrefixEntity() != invalid && floating) {
				children.push_back(configuration.getPrefixEntity());
			}
			if (column != invalid) {
				children.push_back(column);
			}
			if (configuration.getSuffixEntity() != invalid && floating) {
				children.push_back(configuration.getSuffixEntity());
			}
			if (field.isError()) {
				if (configuration.getErrorIconEntity() != invalid) {
					children.push_back(configuration.getErrorIconEntity());
				}
			} else if (configuration.getTrailingIconEntity() != invalid) {
				children.push_back(configuration.getTrailingIconEntity());
			}

			// Intrinsic width: content plus padding, never below MinWidth.
			float width = 2.0f * TextField::Padding;
			for (const auto child: children) {
				if (registry.hasComponent<components::Bound>(child)) {
					width += registry.getComponent<components::Bound>(child)
								 .getWidth();
				}
			}
			if (children.size() > 1) {
				width += TextField::Spacing
					* static_cast<float>(children.size() - 1);
			}

			auto &layout =
				registry.getComponent<components::Layout>(identifier);
			layout
				.setFixedWidth(
					std::max(Configuration::MinWidth,
							 configuration.getWidth().value_or(width)))
				.setChildren(children);
			arrange(registry, identifier);
			// The row has just moved the column: carry its children along.
			arrange(registry, column);
		}

		/**
		 * @brief Place the supporting text below the field and the outlined
		 * floating label on the outline.
		 * @param registry The registry holding the components.
		 * @param identifier The text field entity.
		 */
		void placeManualChildren(ecs::ComponentRegistry &registry,
								 ecs::Entity::Identifier identifier)
		{
			const auto invalid = ecs::Entity::InvalidIdentifier;
			const auto &configuration =
				registry.getComponent<Configuration>(identifier);
			const auto layer = Layout::layerOf(registry, identifier);
			const float height =
				registry.getComponent<components::Bound>(identifier)
					.getHeight();
			const auto supporting = configuration.getSupportingEntity();
			const auto label	  = configuration.getLabelEntity();

			if (supporting != invalid) {
				placement::placeChild(registry, identifier, supporting,
									  utility::graphic::PositionF(
										  TextField::Padding,
										  height + TextField::SupportingGap,
										  0.0f),
									  layer);
			}

			if (label != invalid
				&& configuration.getVariant() == Variant::Outlined
				&& TextField::isLabelFloating(registry, identifier)) {
				float x = TextField::Padding;
				if (configuration.getLeadingIconEntity() != invalid) {
					x += TextField::IconSize + TextField::Spacing;
				}
				const float lineHeight =
					getTypeToken(theme::TypeRole::BodySmall).lineHeight;
				placement::placeChild(
					registry, identifier, label,
					utility::graphic::PositionF(x, -lineHeight / 2.0f, 0.0f),
					layer);
			}
		}
	}	 // namespace

	TextField::TextField(void)
		: ecs::SystemFiller<components::TextField>(ecs::Phase::Layout)
	{
	}

	void TextField::install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		interaction::makeInteractive(componentRegistry, identifier,
									 components::AccessibilityRole::TextField,
									 {}, [registry, identifier]() {
										 applyContainer(*registry, identifier);
									 });

		auto &focus =
			componentRegistry.getComponent<components::Focus>(identifier);
		focus.setOnFocusGainedHandler([registry, identifier]() {
			// Copy: the callback may replace itself.
			const auto onFocus =
				registry->getComponent<Configuration>(identifier).getOnFocus();
			if (onFocus) {
				onFocus();
			}
		});
		focus.setOnFocusLostHandler([registry, identifier]() {
			// Copy: the callback may replace itself.
			const auto onBlur =
				registry->getComponent<Configuration>(identifier).getOnBlur();
			if (onBlur) {
				onBlur();
			}
		});
	}

	bool TextField::isFocused(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		if (componentRegistry.hasComponent<components::Focus>(entityIdentifier)
			&& componentRegistry
				   .getComponent<components::Focus>(entityIdentifier)
				   .isFocused()) {
			return true;
		}
		return componentRegistry.hasComponent<components::StyleState>(
				   entityIdentifier)
			&& componentRegistry
				   .getComponent<components::StyleState>(entityIdentifier)
				   .isFocused();
	}

	bool TextField::isDisabled(const ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier)
	{
		return !interaction::isEnabled(componentRegistry, entityIdentifier);
	}

	bool TextField::isLabelFloating(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return isFocused(componentRegistry, entityIdentifier)
			|| !text(componentRegistry, entityIdentifier).empty();
	}

	std::string TextField::text(const ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &configuration =
			componentRegistry.getComponent<Configuration>(entityIdentifier);
		const auto input = configuration.getInputEntity();

		if (!componentRegistry.hasComponent<components::Text>(input)) {
			return configuration.getText();
		}
		return componentRegistry.getComponent<components::Text>(input)
			.getContent();
	}

	void TextField::setText(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							const std::string &text)
	{
		auto &configuration =
			componentRegistry.getComponent<Configuration>(entityIdentifier);

		// Recording the text first keeps the change callback silent.
		configuration.setText(text);
		writeChild(componentRegistry, configuration.getInputEntity(), text);
		apply(componentRegistry, entityIdentifier);
	}

	void TextField::setDisabled(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								bool disabled)
	{
		interaction::setEnabled(componentRegistry, entityIdentifier, !disabled);
		apply(componentRegistry, entityIdentifier);
	}

	std::string TextField::supportingText(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &field =
			componentRegistry.getComponent<components::TextField>(
				entityIdentifier);

		return (field.isError() && !field.getErrorText().empty())
			? field.getErrorText()
			: field.getSupportingText();
	}

	void TextField::applyContainer(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &configuration =
			componentRegistry.getComponent<Configuration>(entityIdentifier);
		const auto &field =
			componentRegistry.getComponent<components::TextField>(
				entityIdentifier);
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		auto &color =
			componentRegistry.getComponent<components::Color>(entityIdentifier);
		auto &borders = componentRegistry.getComponent<components::Borders>(
			entityIdentifier);
		auto &line =
			componentRegistry.getComponent<components::Line>(entityIdentifier);

		const bool disabled = isDisabled(componentRegistry, entityIdentifier);
		const bool focused =
			isFocused(componentRegistry, entityIdentifier) && !disabled;
		const bool error   = field.isError();
		const float stroke = focused ? 2.0f : 1.0f;

		layout.setAxis(components::Layout::Axis::Horizontal)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(Spacing)
			.setPadding(Padding);
		if (configuration.isMultiline()) {
			layout.clearFixedHeight();
		} else if (!configuration.isDense()) {
			layout.setFixedHeight(Height);
		} else {
			layout.setFixedHeight(field.getPlaceholder().empty()
									  ? DenseHeight
									  : DenseLabelHeight);
		}

		if (configuration.getVariant() == Variant::Outlined) {
			auto outline = schemeColor(SchemeColorRole::Outline);
			if (disabled) {
				outline = disabledContainerColor();
			} else if (error) {
				outline = schemeColor(SchemeColorRole::Error);
			} else if (focused) {
				outline = schemeColor(SchemeColorRole::Primary);
			}
			color.setColor(transparentColor());
			borders
				.setBorderRadius(getShapeRadius(theme::ShapeRole::ExtraSmall))
				.setColor(outline)
				.setWidth(stroke);
			line.setThickness(0.0f).clearColor();
		} else {
			color.setColor(
				disabled
					? style::withAlpha(schemeColor(SchemeColorRole::OnSurface),
									   DisabledFilledAlpha)
					: schemeColor(SchemeColorRole::SurfaceContainerHighest));
			borders.setBorderRadius(getShapeRadius(theme::ShapeRole::None))
				.setColor(transparentColor())
				.setWidth(0.0f);
			line.setThickness(stroke)
				.setPlacement(components::Line::Placement::Bottom)
				.setColor(activeColor(disabled, error, focused));
		}
	}

	void
		TextField::applyEditing(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &configuration =
			componentRegistry.getComponent<Configuration>(entityIdentifier);
		const auto input = configuration.getInputEntity();

		if (input == ecs::Entity::InvalidIdentifier) {
			return;
		}

		const bool editing = configuration.isEditable()
			&& !isDisabled(componentRegistry, entityIdentifier)
			&& isFocused(componentRegistry, entityIdentifier);
		const bool hasFocus =
			componentRegistry.hasComponent<components::Focus>(input);

		if (editing && !hasFocus) {
			componentRegistry.addComponent<components::Focus>(input);
		} else if (!editing && hasFocus) {
			componentRegistry.removeComponent<components::Focus>(input);
		}
	}

	void TextField::apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier)
	{
		// May run user code: nothing is read before it returns.
		reportChange(componentRegistry, entityIdentifier);

		applyContainer(componentRegistry, entityIdentifier);

		auto &field = componentRegistry.getComponent<components::TextField>(
			entityIdentifier);
		field.setLabel(field.getPlaceholder())
			.setLabelFloating(
				isLabelFloating(componentRegistry, entityIdentifier));
		if (componentRegistry.hasComponent<components::Accessibility>(
				entityIdentifier)) {
			componentRegistry
				.getComponent<components::Accessibility>(entityIdentifier)
				.setLabel(field.getPlaceholder())
				.setHint(field.getSupportingText());
		}

		styleChildren(componentRegistry, entityIdentifier);
		arrangeChildren(componentRegistry, entityIdentifier);
		placeManualChildren(componentRegistry, entityIdentifier);
		applyEditing(componentRegistry, entityIdentifier);
	}

	void TextField::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::TextField>(entityIdentifier)) {
			return;
		}

		auto &registry = getComponentRegistry();

		if (registry.hasComponent<Configuration>(entityIdentifier)) {
			apply(registry, entityIdentifier);
			return;
		}

		// A bare field over its own Text only derives the floating state.
		if (registry.hasComponent<components::Text>(entityIdentifier)) {
			reconcile(
				registry.getComponent<components::TextField>(entityIdentifier),
				!registry.getComponent<components::Text>(entityIdentifier)
					 .getContent()
					 .empty());
		}
	}

	bool TextField::reconcile(components::TextField &textField, bool hasContent)
	{
		const bool floating = hasContent || textField.isError();
		textField.setLabelFloating(floating);
		return floating;
	}

}	 // namespace guillaume::systems
