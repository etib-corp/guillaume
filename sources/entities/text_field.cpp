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

#include "guillaume/entities/text_field.hpp"

#include <vector>

#include "guillaume/components/focus.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
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
		_config.spacing = 8.0f;
		_config.padding = 16.0f;
		_config.setFixedWidth(280.0f);
		_config.setFixedHeight(56.0f);
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

	std::shared_ptr<TextField> TextField::Director::makeTextField(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::string &placeholder, const std::string &leadingIcon,
		const std::string &trailingIcon, const std::string &supportingText,
		bool error, const std::string &text)
	{
		return builder.withVariant(variant)
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
		, _leadingIconEntity()
		, _trailingIconEntity()
		, _textEntity()
		, _supportingEntity()
	{
		if (!getComponentRegistry().hasComponent<components::Focus>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Focus>(
				getIdentifier());
		}

		applyVariant();
		applyComponent();
	}

	TextField::~TextField(void)
	{
	}

	void TextField::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Horizontal;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		config.spacing		  = 8.0f;
		config.padding		  = 16.0f;
		config.hasFixedWidth  = true;
		config.fixedWidth	  = 280.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = 56.0f;

		setSurfaceConfig(config);

		auto &borders =
			this->getComponentRegistry().getComponent<components::Borders>(
				this->getIdentifier());
		auto &line =
			this->getComponentRegistry().getComponent<components::Line>(
				this->getIdentifier());

		const auto outlineColor = _error
			? schemeColor(SchemeColorRole::Error)
			: schemeColor(SchemeColorRole::Outline);

		if (_variant == Variant::Outlined) {
			setColor(transparentColor());
			borders.setBorderRadius(4.0f).setColor(outlineColor);
			line.setThickness(0.0f);
		} else {
			setColor(schemeColor(SchemeColorRole::SurfaceVariant));
			borders.setBorderRadius(0.0f).setColor(transparentColor());
			line.setThickness(1.0f);
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
	}

	void TextField::buildContent(void)
	{
		const auto foreground	   = schemeColor(SchemeColorRole::OnSurface);
		const auto supportingColor = _error
			? schemeColor(SchemeColorRole::Error)
			: schemeColor(SchemeColorRole::OnSurfaceVariant);

		if (_leadingIconEntity == nullptr && !_leadingIcon.empty()) {
			_leadingIconEntity =
				buildIcon(this->getComponentRegistry(), *this,
						  this->shared_from_this(), _leadingIcon, 24.0f,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
		}

		if (_textEntity == nullptr && !_text.empty()) {
			_textEntity =
				buildText(this->getComponentRegistry(), *this,
						  this->shared_from_this(), _text, 16.0f, foreground);
		}

		if (_trailingIconEntity == nullptr && !_trailingIcon.empty()) {
			_trailingIconEntity = buildIcon(
				this->getComponentRegistry(), *this, this->shared_from_this(),
				_trailingIcon, 24.0f, supportingColor);
		}

		const auto &textField =
			this->getComponentRegistry().getComponent<components::TextField>(
				this->getIdentifier());

		const std::string &errorText = textField.getErrorText();
		const std::string &support =
			(_error && !errorText.empty()) ? errorText : _supportingText;

		if (_supportingEntity == nullptr && !support.empty()) {
			_supportingEntity = buildText(this->getComponentRegistry(), *this,
										  this->shared_from_this(), support,
										  12.0f, supportingColor);
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (_leadingIconEntity != nullptr) {
			children.push_back(_leadingIconEntity);
		}

		if (_textEntity != nullptr) {
			children.push_back(_textEntity);
		}

		if (_trailingIconEntity != nullptr) {
			children.push_back(_trailingIconEntity);
		}

		if (_supportingEntity != nullptr) {
			children.push_back(_supportingEntity);
		}

		setChildren(children);
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
		applyComponent();
		return *this;
	}

	TextField &TextField::setText(const std::string &text)
	{
		_text = text;

		if (_textEntity != nullptr) {
			_textEntity->setContent(text);
		}

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

	TextField &TextField::setSupportingText(const std::string &supportingText)
	{
		_supportingText = supportingText;
		applyComponent();

		if (_supportingEntity != nullptr) {
			_supportingEntity->setContent(supportingText);
		}

		return *this;
	}

	void TextField::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyComponent();
		buildContent();
	}

	void TextField::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyComponent();
		buildContent();
	}

}	 // namespace guillaume::entities
