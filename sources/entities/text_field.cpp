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

#include <memory>
#include <utility>
#include <vector>

#include "guillaume/entities/text_field.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/text_field.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	TextField::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<TextField>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<TextField> TextField::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<TextField>(
			this->getComponentRegistry(), _config, _variant, _placeholder,
			_leadingIcon, _trailingIcon, _supportingText, _error, _text);
		return entity;
	}

	void TextField::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Filled;
		_placeholder.clear();
		_leadingIcon.clear();
		_trailingIcon.clear();
		_supportingText.clear();
		_error = false;
		_text.clear();

		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = systems::TextField::Spacing;
		_config.padding = systems::TextField::Padding;
		_config.setFixedWidth(MinWidth);
		_config.setFixedHeight(systems::TextField::Height);
	}

	TextField::Builder &
		TextField::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	TextField::Builder &TextField::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	TextField::Builder &
		TextField::Builder::withPlaceholder(const std::string &placeholder)
	{
		_placeholder = placeholder;
		return *this;
	}

	TextField::Builder &
		TextField::Builder::withLeadingIcon(const std::string &iconGlyph)
	{
		_leadingIcon = iconGlyph;
		return *this;
	}

	TextField::Builder &
		TextField::Builder::withTrailingIcon(const std::string &iconGlyph)
	{
		_trailingIcon = iconGlyph;
		return *this;
	}

	TextField::Builder &TextField::Builder::withSupportingText(
		const std::string &supportingText)
	{
		_supportingText = supportingText;
		return *this;
	}

	TextField::Builder &TextField::Builder::withError(bool error)
	{
		_error = error;
		return *this;
	}

	TextField::Builder &TextField::Builder::withText(const std::string &text)
	{
		_text = text;
		return *this;
	}

	std::shared_ptr<TextField> TextField::Builder::makeTextField(
		std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &placeholder, const std::string &leadingIcon,
		const std::string &trailingIcon, const std::string &supportingText,
		bool error, const std::string &text)
	{
		return withVariant(variant)
			.withPlaceholder(placeholder)
			.withLeadingIcon(leadingIcon)
			.withTrailingIcon(trailingIcon)
			.withSupportingText(supportingText)
			.withError(error)
			.withText(text)
			.registerEntity(parent);
	}

	TextField::TextField(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 const std::string &placeholder,
						 const std::string &leadingIcon,
						 const std::string &trailingIcon,
						 const std::string &supportingText, bool error,
						 const std::string &text)
		: SurfaceBase<components::TextField, components::Line,
					  components::TextFieldConfiguration>(registry, config)
	{
		configuration()
			.setVariant(variant)
			.setLeadingIcon(leadingIcon)
			.setTrailingIcon(trailingIcon)
			.setText(text);
		field()
			.setPlaceholder(placeholder)
			.setSupportingText(supportingText)
			.setError(error);
		systems::TextField::install(getComponentRegistry(), getIdentifier());
		systems::TextField::apply(getComponentRegistry(), getIdentifier());
	}

	TextField::~TextField(void)
	{
	}

	components::TextFieldConfiguration &TextField::configuration(void)
	{
		return getComponentRegistry()
			.getComponent<components::TextFieldConfiguration>(getIdentifier());
	}

	const components::TextFieldConfiguration &
		TextField::configuration(void) const
	{
		return getComponentRegistry()
			.getComponent<components::TextFieldConfiguration>(getIdentifier());
	}

	components::TextField &TextField::field(void)
	{
		return getComponentRegistry().getComponent<components::TextField>(
			getIdentifier());
	}

	void TextField::refresh(void)
	{
		buildContent();
		systems::TextField::apply(getComponentRegistry(), getIdentifier());
	}

	bool TextField::isFocused(void) const
	{
		return systems::TextField::isFocused(getComponentRegistry(),
											 getIdentifier());
	}

	bool TextField::isLabelFloating(void) const
	{
		return systems::TextField::isLabelFloating(getComponentRegistry(),
												   getIdentifier());
	}

	bool TextField::isDisabled(void) const
	{
		return systems::TextField::isDisabled(getComponentRegistry(),
											  getIdentifier());
	}

	TextField &TextField::setVariant(Variant variant)
	{
		configuration().setVariant(variant);
		refresh();
		return *this;
	}

	TextField::Variant TextField::getVariant(void) const
	{
		return configuration().getVariant();
	}

	TextField &TextField::setPlaceholder(const std::string &placeholder)
	{
		field().setPlaceholder(placeholder);
		refresh();
		return *this;
	}

	TextField &TextField::setText(const std::string &text)
	{
		systems::TextField::setText(getComponentRegistry(), getIdentifier(),
									text);
		return *this;
	}

	std::string TextField::getText(void) const
	{
		return systems::TextField::text(getComponentRegistry(),
										getIdentifier());
	}

	TextField &TextField::setError(bool error)
	{
		field().setError(error);
		refresh();
		return *this;
	}

	bool TextField::isError(void) const
	{
		return getComponentRegistry()
			.getComponent<components::TextField>(getIdentifier())
			.isError();
	}

	TextField &TextField::setErrorText(const std::string &errorText)
	{
		field().setErrorText(errorText);
		refresh();
		return *this;
	}

	TextField &TextField::setSupportingText(const std::string &supportingText)
	{
		field().setSupportingText(supportingText);
		refresh();
		return *this;
	}

	TextField &TextField::setPrefix(const std::string &prefix)
	{
		field().setPrefix(prefix);
		refresh();
		return *this;
	}

	TextField &TextField::setSuffix(const std::string &suffix)
	{
		field().setSuffix(suffix);
		refresh();
		return *this;
	}

	TextField &TextField::setDisabled(bool disabled)
	{
		systems::TextField::setDisabled(getComponentRegistry(), getIdentifier(),
										disabled);
		return *this;
	}

	TextField &TextField::setEditable(bool editable)
	{
		configuration().setEditable(editable);
		systems::TextField::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool TextField::isEditable(void) const
	{
		return configuration().isEditable();
	}

	TextField &TextField::setDense(bool dense)
	{
		configuration().setDense(dense);
		systems::TextField::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool TextField::isDense(void) const
	{
		return configuration().isDense();
	}

	TextField &TextField::setMultiline(bool multiline)
	{
		configuration().setMultiline(multiline);
		systems::TextField::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool TextField::isMultiline(void) const
	{
		return configuration().isMultiline();
	}

	TextField &TextField::setWidth(float width)
	{
		// The component clamps the width to MinWidth.
		configuration().setWidth(width);
		systems::TextField::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	TextField &TextField::setOnChange(
		std::function<void(const std::string &)> onChange)
	{
		configuration().setOnChange(std::move(onChange));
		return *this;
	}

	TextField &TextField::setOnFocus(std::function<void(void)> onFocus)
	{
		configuration().setOnFocus(std::move(onFocus));
		return *this;
	}

	TextField &TextField::setOnBlur(std::function<void(void)> onBlur)
	{
		configuration().setOnBlur(std::move(onBlur));
		return *this;
	}

	ecs::Entity::Identifier TextField::getLabelIdentifier(void) const
	{
		return configuration().getLabelEntity();
	}

	ecs::Entity::Identifier TextField::getInputIdentifier(void) const
	{
		return configuration().getInputEntity();
	}

	ecs::Entity::Identifier TextField::getSupportingIdentifier(void) const
	{
		return configuration().getSupportingEntity();
	}

	ecs::Entity::Identifier TextField::getPrefixIdentifier(void) const
	{
		return configuration().getPrefixEntity();
	}

	ecs::Entity::Identifier TextField::getSuffixIdentifier(void) const
	{
		return configuration().getSuffixEntity();
	}

	ecs::Entity::Identifier TextField::getErrorIconIdentifier(void) const
	{
		return configuration().getErrorIconEntity();
	}

	void TextField::initialize(void)
	{
		refresh();
	}

	void TextField::buildContent(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry		 = getComponentRegistry();
		auto &config		 = configuration();
		const auto &state	 = field();
		const auto self		 = shared_from_this();
		const auto invalid	 = ecs::Entity::InvalidIdentifier;
		const float iconSize = systems::TextField::IconSize;
		// Starting colors only: systems::TextField owns the state colors.
		const auto content = schemeColor(SchemeColorRole::OnSurface);
		const auto variantColor =
			schemeColor(SchemeColorRole::OnSurfaceVariant);
		bool rowChanged = false;

		if (config.getLeadingIconEntity() == invalid) {
			const auto icon =
				buildIcon(registry, *this, self, config.getLeadingIcon(),
						  iconSize, variantColor);
			if (icon != nullptr) {
				config.setLeadingIconEntity(icon->getIdentifier());
				rowChanged = true;
			}
		}

		if (config.getTrailingIconEntity() == invalid) {
			const auto icon =
				buildIcon(registry, *this, self, config.getTrailingIcon(),
						  iconSize, variantColor);
			if (icon != nullptr) {
				config.setTrailingIconEntity(icon->getIdentifier());
				rowChanged = true;
			}
		}

		if (config.getErrorIconEntity() == invalid) {
			const auto icon =
				buildIcon(registry, *this, self, "error", iconSize,
						  schemeColor(SchemeColorRole::Error));
			if (icon != nullptr) {
				config.setErrorIconEntity(icon->getIdentifier());
				rowChanged = true;
			}
		}

		auto column = getEntity<View>(config.getInputColumn());
		if (column == nullptr) {
			View::Builder builder(registry, *this);
			column =
				builder.makeDefaultView(self, utility::graphic::PoseF(), {});
			column->setColor(transparentColor());
			column->setAxis(components::Layout::Axis::Vertical);
			column->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::Center);
			column->setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Start);
			column->setPadding(0.0f);
			column->setSpacing(0.0f);
			config.setInputColumn(column->getIdentifier());
			rowChanged = true;
		}

		if (config.getInputEntity() == invalid) {
			// Built even when empty so the text-input systems can edit it.
			Text::Builder builder(registry, *column);
			const auto input = builder.makeText(
				column, config.getText(), theme::TypeRole::BodyLarge, content);
			input->setContent(config.getText());
			config.setInputEntity(input->getIdentifier());
		}

		if (config.getLabelEntity() == invalid
			&& !state.getPlaceholder().empty()) {
			Text::Builder builder(registry, *column);
			const auto label =
				builder.makeText(column, state.getPlaceholder(),
								 theme::TypeRole::BodyLarge, variantColor);
			label->setContent(state.getPlaceholder());
			config.setLabelEntity(label->getIdentifier());
		}

		if (config.getPrefixEntity() == invalid) {
			const auto prefix =
				buildText(registry, *this, self, state.getPrefix(),
						  theme::TypeRole::BodyLarge, variantColor);
			if (prefix != nullptr) {
				config.setPrefixEntity(prefix->getIdentifier());
				rowChanged = true;
			}
		}

		if (config.getSuffixEntity() == invalid) {
			const auto suffix =
				buildText(registry, *this, self, state.getSuffix(),
						  theme::TypeRole::BodyLarge, variantColor);
			if (suffix != nullptr) {
				config.setSuffixEntity(suffix->getIdentifier());
				rowChanged = true;
			}
		}

		// The supporting text is placed by systems::TextField, outside the
		// row.
		if (config.getSupportingEntity() == invalid) {
			const auto supporting = buildText(
				registry, *this, self,
				systems::TextField::supportingText(registry, getIdentifier()),
				theme::TypeRole::BodySmall, variantColor);
			if (supporting != nullptr) {
				config.setSupportingEntity(supporting->getIdentifier());
			}
		}

		if (!rowChanged) {
			return;
		}

		// Every row child is attached; systems::TextField then narrows the
		// layout children to the ones currently shown.
		std::vector<std::shared_ptr<ecs::Entity>> children;
		if (auto icon = getEntity<Icon>(config.getLeadingIconEntity())) {
			children.push_back(std::move(icon));
		}
		if (auto prefix = getEntity<Text>(config.getPrefixEntity())) {
			children.push_back(std::move(prefix));
		}
		children.push_back(column);
		if (auto suffix = getEntity<Text>(config.getSuffixEntity())) {
			children.push_back(std::move(suffix));
		}
		if (auto icon = getEntity<Icon>(config.getErrorIconEntity())) {
			children.push_back(std::move(icon));
		}
		if (auto icon = getEntity<Icon>(config.getTrailingIconEntity())) {
			children.push_back(std::move(icon));
		}
		setChildren(children);
	}

}	 // namespace guillaume::entities
