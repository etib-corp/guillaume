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

#include <algorithm>
#include <memory>
#include <vector>

#include "guillaume/entities/text_field.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/style_state.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief M3 text field height.
		 */
		constexpr float FieldHeight = 56.0f;

		/**
		 * @brief Paper dense height with a label.
		 */
		constexpr float DenseLabelHeight = 52.0f;

		/**
		 * @brief Paper dense height without a label.
		 */
		constexpr float DenseHeight = 40.0f;

		/**
		 * @brief M3 horizontal padding.
		 */
		constexpr float FieldPadding = theme::SpacingScale::get(4);

		/**
		 * @brief M3 gap between icons, affixes and the input.
		 */
		constexpr float FieldSpacing = theme::SpacingScale::get(3);

		/**
		 * @brief M3 gap between the field and the supporting text.
		 */
		constexpr float SupportingGap = theme::SpacingScale::get(1);

		/**
		 * @brief M3 icon size.
		 */
		constexpr float FieldIconSize = 24.0f;

		/**
		 * @brief Paper filled disabled container opacity (onSurface at 4%).
		 */
		constexpr std::uint8_t DisabledFilledAlpha = 10U;
	}	 // namespace

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
		_config.spacing = FieldSpacing;
		_config.padding = FieldPadding;
		_config.setFixedWidth(MinWidth);
		_config.setFixedHeight(FieldHeight);
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
		: SurfaceBase<components::TextField, components::Line>(registry, config)
		, _variant(variant)
		, _placeholder(placeholder)
		, _leadingIcon(leadingIcon)
		, _trailingIcon(trailingIcon)
		, _supportingText(supportingText)
		, _error(error)
		, _text(text)
	{
		makeInteractive(getComponentRegistry(), getIdentifier(),
						components::AccessibilityRole::TextField, {}, [this]() {
							applyVariant();
						});
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(placeholder)
			.setHint(supportingText);

		auto &focus = getComponentRegistry().getComponent<components::Focus>(
			getIdentifier());
		focus.setOnFocusGainedHandler([this]() {
			if (_onFocus) {
				_onFocus();
			}
		});
		focus.setOnFocusLostHandler([this]() {
			if (_onBlur) {
				_onBlur();
			}
		});

		applyVariant();
		applyComponent();
	}

	TextField::~TextField(void)
	{
	}

	bool TextField::isFocused(void) const
	{
		const auto &registry = getComponentRegistry();
		if (registry.hasComponent<components::Focus>(getIdentifier())
			&& registry.getComponent<components::Focus>(getIdentifier())
				   .isFocused()) {
			return true;
		}
		return registry.hasComponent<components::StyleState>(getIdentifier())
			&& registry.getComponent<components::StyleState>(getIdentifier())
				   .isFocused();
	}

	bool TextField::isLabelFloating(void) const
	{
		return isFocused() || !getText().empty();
	}

	bool TextField::isDisabled(void) const
	{
		return !isEnabled(getComponentRegistry(), getIdentifier());
	}

	float TextField::getFieldHeight(void) const
	{
		if (!_dense) {
			return FieldHeight;
		}
		return _placeholder.empty() ? DenseHeight : DenseLabelHeight;
	}

	utility::graphic::Color32Bit TextField::getActiveColor(void) const
	{
		if (isDisabled()) {
			return disabledContentColor();
		}
		if (_error) {
			return schemeColor(SchemeColorRole::Error);
		}
		if (isFocused()) {
			return schemeColor(SchemeColorRole::Primary);
		}
		return schemeColor(SchemeColorRole::OnSurfaceVariant);
	}

	void TextField::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Horizontal;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		config.spacing		  = FieldSpacing;
		config.padding		  = FieldPadding;
		config.hasFixedHeight = !_multiline;
		config.fixedHeight	  = getFieldHeight();

		setSurfaceConfig(config);

		auto &borders =
			this->getComponentRegistry().getComponent<components::Borders>(
				this->getIdentifier());
		auto &line =
			this->getComponentRegistry().getComponent<components::Line>(
				this->getIdentifier());

		const bool disabled = isDisabled();
		const bool focused	= isFocused() && !disabled;
		const float stroke	= focused ? 2.0f : 1.0f;

		if (_variant == Variant::Outlined) {
			auto outline = schemeColor(SchemeColorRole::Outline);
			if (disabled) {
				outline = disabledContainerColor();
			} else if (_error) {
				outline = schemeColor(SchemeColorRole::Error);
			} else if (focused) {
				outline = schemeColor(SchemeColorRole::Primary);
			}
			setColor(transparentColor());
			borders
				.setBorderRadius(getShapeRadius(theme::ShapeRole::ExtraSmall))
				.setColor(outline)
				.setWidth(stroke);
			line.setThickness(0.0f).clearColor();
		} else {
			setColor(
				disabled
					? withAlpha(schemeColor(SchemeColorRole::OnSurface),
								DisabledFilledAlpha)
					: schemeColor(SchemeColorRole::SurfaceContainerHighest));
			borders.setBorderRadius(getShapeRadius(theme::ShapeRole::None))
				.setColor(transparentColor())
				.setWidth(0.0f);
			line.setThickness(stroke)
				.setPlacement(components::Line::Placement::Bottom)
				.setColor(getActiveColor());
		}
	}

	void TextField::applyComponent(void)
	{
		auto &textField =
			this->getComponentRegistry().getComponent<components::TextField>(
				this->getIdentifier());

		textField.setPlaceholder(_placeholder);
		textField.setLabel(_placeholder);
		textField.setSupportingText(_supportingText);
		textField.setError(_error);
		textField.setLabelFloating(isLabelFloating());
	}

	void TextField::applyEditing(void)
	{
		if (_textEntity == nullptr) {
			return;
		}

		auto &registry		  = getComponentRegistry();
		const auto identifier = _textEntity->getIdentifier();
		const bool editing	  = _editable && !isDisabled() && isFocused();
		const bool hasFocus =
			registry.hasComponent<components::Focus>(identifier);

		if (editing && !hasFocus) {
			registry.addComponent<components::Focus>(identifier);
		} else if (!editing && hasFocus) {
			registry.removeComponent<components::Focus>(identifier);
		}
	}

	void TextField::buildContent(void)
	{
		const bool disabled		= isDisabled();
		const auto content		= disabled ? disabledContentColor()
										   : schemeColor(SchemeColorRole::OnSurface);
		const auto variantColor = disabled
			? disabledContentColor()
			: schemeColor(SchemeColorRole::OnSurfaceVariant);

		createChildren(content, variantColor);
		applyContentColors(content, variantColor);
		arrangeChildren();
		placeManualChildren();
	}

	void TextField::createChildren(
		const utility::graphic::Color32Bit &content,
		const utility::graphic::Color32Bit &variantColor)
	{
		auto &registry = this->getComponentRegistry();
		const auto &component =
			registry.getComponent<components::TextField>(this->getIdentifier());

		if (_leadingIconEntity == nullptr && !_leadingIcon.empty()) {
			_leadingIconEntity =
				buildIcon(registry, *this, this->shared_from_this(),
						  _leadingIcon, FieldIconSize, variantColor);
		}

		if (_trailingIconEntity == nullptr && !_trailingIcon.empty()) {
			_trailingIconEntity =
				buildIcon(registry, *this, this->shared_from_this(),
						  _trailingIcon, FieldIconSize, variantColor);
		}

		if (_errorIconEntity == nullptr) {
			_errorIconEntity =
				buildIcon(registry, *this, this->shared_from_this(), "error",
						  FieldIconSize, schemeColor(SchemeColorRole::Error));
		}

		if (_inputColumn == nullptr) {
			View::Builder builder(registry, *this);
			_inputColumn = builder.makeDefaultView(
				this->shared_from_this(), utility::graphic::PoseF(), {});
			_inputColumn->setColor(transparentColor());
			_inputColumn->setAxis(components::Layout::Axis::Vertical);
			_inputColumn->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::Center);
			_inputColumn->setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Start);
			_inputColumn->setPadding(0.0f);
			_inputColumn->setSpacing(0.0f);
		}

		if (_textEntity == nullptr) {
			// Built even when empty so the text-input systems can edit it.
			Text::Builder builder(registry, *_inputColumn);
			_textEntity = builder.makeText(_inputColumn, _text,
										   theme::TypeRole::BodyLarge, content);
			_textEntity->setContent(_text);
		}

		if (_labelEntity == nullptr && !_placeholder.empty()) {
			Text::Builder builder(registry, *_inputColumn);
			_labelEntity =
				builder.makeText(_inputColumn, _placeholder,
								 theme::TypeRole::BodyLarge, variantColor);
			_labelEntity->setContent(_placeholder);
		}

		if (_prefixEntity == nullptr && !component.getPrefix().empty()) {
			_prefixEntity = buildText(registry, *this, this->shared_from_this(),
									  component.getPrefix(),
									  theme::TypeRole::BodyLarge, variantColor);
		}

		if (_suffixEntity == nullptr && !component.getSuffix().empty()) {
			_suffixEntity = buildText(registry, *this, this->shared_from_this(),
									  component.getSuffix(),
									  theme::TypeRole::BodyLarge, variantColor);
		}

		const std::string &errorText = component.getErrorText();
		const std::string &support =
			(_error && !errorText.empty()) ? errorText : _supportingText;

		if (_supportingEntity == nullptr && !support.empty()) {
			_supportingEntity =
				buildText(registry, *this, this->shared_from_this(), support,
						  theme::TypeRole::BodySmall, variantColor);
		} else if (_supportingEntity != nullptr) {
			_supportingEntity->setContent(support);
		}
	}

	void TextField::applyContentColors(
		const utility::graphic::Color32Bit &content,
		const utility::graphic::Color32Bit &variantColor)
	{
		const bool disabled = isDisabled();
		const bool floating = isLabelFloating();

		_textEntity->setColor(content);
		if (_labelEntity != nullptr) {
			_labelEntity->setTypeRole(floating ? theme::TypeRole::BodySmall
											   : theme::TypeRole::BodyLarge);
			_labelEntity->setColor(floating ? getActiveColor() : variantColor);
		}
		if (_leadingIconEntity != nullptr) {
			_leadingIconEntity->setColor(variantColor);
		}
		if (_trailingIconEntity != nullptr) {
			_trailingIconEntity->setColor(_error ? transparentColor()
												 : variantColor);
		}
		_errorIconEntity->setColor(_error && !disabled
									   ? schemeColor(SchemeColorRole::Error)
									   : transparentColor());
		if (_prefixEntity != nullptr) {
			_prefixEntity->setColor(floating ? variantColor
											 : transparentColor());
		}
		if (_suffixEntity != nullptr) {
			_suffixEntity->setColor(floating ? variantColor
											 : transparentColor());
		}
		if (_supportingEntity != nullptr) {
			_supportingEntity->setColor(
				_error && !disabled ? schemeColor(SchemeColorRole::Error)
									: variantColor);
		}
	}

	void TextField::arrangeChildren(void)
	{
		auto &registry		= this->getComponentRegistry();
		const bool floating = isLabelFloating();

		// Input column: the filled label floats above the input, the
		// outlined label floats on the outline (placed manually).
		std::vector<std::shared_ptr<ecs::Entity>> column;
		if (_labelEntity != nullptr
			&& (!floating || _variant == Variant::Filled)) {
			column.push_back(_labelEntity);
		}
		if (floating || _labelEntity == nullptr) {
			column.push_back(_textEntity);
		}
		_inputColumn->setEntities(column);

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_leadingIconEntity != nullptr) {
			children.push_back(_leadingIconEntity);
		}
		if (_prefixEntity != nullptr && floating) {
			children.push_back(_prefixEntity);
		}
		children.push_back(_inputColumn);
		if (_suffixEntity != nullptr && floating) {
			children.push_back(_suffixEntity);
		}
		if (_error) {
			children.push_back(_errorIconEntity);
		} else if (_trailingIconEntity != nullptr) {
			children.push_back(_trailingIconEntity);
		}

		// Intrinsic width: content plus padding, never below MinWidth.
		float width = 2.0f * FieldPadding;
		for (const auto &child: children) {
			if (registry.hasComponent<components::Bound>(
					child->getIdentifier())) {
				width +=
					registry
						.getComponent<components::Bound>(child->getIdentifier())
						.getWidth();
			}
		}
		if (children.size() > 1) {
			width += FieldSpacing * static_cast<float>(children.size() - 1);
		}

		auto config			 = getSurfaceConfig();
		config.hasFixedWidth = true;
		config.fixedWidth	 = std::max(MinWidth, _width.value_or(width));
		setSurfaceConfig(config);

		setChildren(children);
	}

	void TextField::placeManualChildren(void)
	{
		auto &registry	 = getComponentRegistry();
		const auto layer = static_cast<std::uint32_t>(getLayer());
		const float height =
			registry.getComponent<components::Bound>(getIdentifier())
				.getHeight();

		if (_supportingEntity != nullptr) {
			placeChild(registry, getIdentifier(),
					   _supportingEntity->getIdentifier(),
					   utility::graphic::PositionF(
						   FieldPadding, height + SupportingGap, 0.0f),
					   layer);
		}

		if (_labelEntity != nullptr && _variant == Variant::Outlined
			&& isLabelFloating()) {
			float x = FieldPadding;
			if (_leadingIconEntity != nullptr) {
				x += FieldIconSize + FieldSpacing;
			}
			const float lineHeight =
				getTypeToken(theme::TypeRole::BodySmall).lineHeight;
			placeChild(registry, getIdentifier(), _labelEntity->getIdentifier(),
					   utility::graphic::PositionF(x, -lineHeight / 2.0f, 0.0f),
					   layer);
		}
	}

	TextField &TextField::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	TextField::Variant TextField::getVariant(void) const
	{
		return _variant;
	}

	TextField &TextField::setPlaceholder(const std::string &placeholder)
	{
		_placeholder = placeholder;
		if (_labelEntity != nullptr) {
			_labelEntity->setContent(placeholder);
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(placeholder);
		applyComponent();
		return *this;
	}

	TextField &TextField::setText(const std::string &text)
	{
		_text = text;

		if (_textEntity != nullptr) {
			_textEntity->setContent(text);
		}

		applyComponent();
		return *this;
	}

	std::string TextField::getText(void) const
	{
		if (_textEntity == nullptr) {
			return _text;
		}

		return this->getComponentRegistry()
			.getComponent<components::Text>(_textEntity->getIdentifier())
			.getContent();
	}

	TextField &TextField::setError(bool error)
	{
		_error = error;
		applyVariant();
		applyComponent();
		return *this;
	}

	bool TextField::isError(void) const
	{
		return _error;
	}

	TextField &TextField::setErrorText(const std::string &errorText)
	{
		getComponentRegistry()
			.getComponent<components::TextField>(getIdentifier())
			.setErrorText(errorText);
		return *this;
	}

	TextField &TextField::setSupportingText(const std::string &supportingText)
	{
		_supportingText = supportingText;
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setHint(supportingText);
		applyComponent();

		if (_supportingEntity != nullptr && !_error) {
			_supportingEntity->setContent(supportingText);
		}

		return *this;
	}

	TextField &TextField::setPrefix(const std::string &prefix)
	{
		getComponentRegistry()
			.getComponent<components::TextField>(getIdentifier())
			.setPrefix(prefix);
		if (_prefixEntity != nullptr) {
			_prefixEntity->setContent(prefix);
		}
		return *this;
	}

	TextField &TextField::setSuffix(const std::string &suffix)
	{
		getComponentRegistry()
			.getComponent<components::TextField>(getIdentifier())
			.setSuffix(suffix);
		if (_suffixEntity != nullptr) {
			_suffixEntity->setContent(suffix);
		}
		return *this;
	}

	TextField &TextField::setDisabled(bool disabled)
	{
		setEnabled(getComponentRegistry(), getIdentifier(), !disabled);
		applyVariant();
		applyEditing();
		return *this;
	}

	TextField &TextField::setEditable(bool editable)
	{
		_editable = editable;
		applyEditing();
		return *this;
	}

	bool TextField::isEditable(void) const
	{
		return _editable;
	}

	TextField &TextField::setDense(bool dense)
	{
		_dense = dense;
		applyVariant();
		return *this;
	}

	bool TextField::isDense(void) const
	{
		return _dense;
	}

	TextField &TextField::setMultiline(bool multiline)
	{
		_multiline = multiline;
		applyVariant();
		return *this;
	}

	bool TextField::isMultiline(void) const
	{
		return _multiline;
	}

	TextField &TextField::setWidth(float width)
	{
		_width = std::max(MinWidth, width);
		return *this;
	}

	TextField &TextField::setOnChange(
		std::function<void(const std::string &)> onChange)
	{
		_onChange = std::move(onChange);
		return *this;
	}

	TextField &TextField::setOnFocus(std::function<void(void)> onFocus)
	{
		_onFocus = std::move(onFocus);
		return *this;
	}

	TextField &TextField::setOnBlur(std::function<void(void)> onBlur)
	{
		_onBlur = std::move(onBlur);
		return *this;
	}

	ecs::Entity::Identifier TextField::getLabelIdentifier(void) const
	{
		return _labelEntity != nullptr ? _labelEntity->getIdentifier()
									   : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier TextField::getInputIdentifier(void) const
	{
		return _textEntity != nullptr ? _textEntity->getIdentifier()
									  : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier TextField::getSupportingIdentifier(void) const
	{
		return _supportingEntity != nullptr ? _supportingEntity->getIdentifier()
											: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier TextField::getPrefixIdentifier(void) const
	{
		return _prefixEntity != nullptr ? _prefixEntity->getIdentifier()
										: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier TextField::getSuffixIdentifier(void) const
	{
		return _suffixEntity != nullptr ? _suffixEntity->getIdentifier()
										: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier TextField::getErrorIconIdentifier(void) const
	{
		return _errorIconEntity != nullptr ? _errorIconEntity->getIdentifier()
										   : ecs::Entity::InvalidIdentifier;
	}

	void TextField::initialize(void)
	{
		update();
		applyVariant();
		applyComponent();
		buildContent();
		applyEditing();
	}

	void TextField::update(void)
	{
		applyGeometry();

		const std::string current = getText();
		if (current != _text) {
			_text = current;
			if (_onChange) {
				_onChange(current);
			}
		}

		applyVariant();
		applyComponent();
		buildContent();
		applyEditing();
	}

}	 // namespace guillaume::entities
