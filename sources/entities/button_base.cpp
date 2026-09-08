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

#include <utility>

#include "guillaume/entities/button_base.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	ButtonBase::ButtonBase(ecs::ComponentRegistry &registry,
						   const std::string &iconGlyphName,
						   const components::Glyph::Style &iconStyle,
						   const std::string &labelContent, bool isToggle,
						   bool isMorph, bool isDisabled, bool isTrailingIcon,
						   const std::string &accessibilityLabel,
						   std::function<void(void)> onClick)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Color, components::Borders,
								  components::Focus,
								  components::HandButtonInteraction,
								  components::HandHoverInteraction,
								  components::MouseHoverInteraction,
								  components::MouseButtonInteraction>(registry)
		, _iconGlyphName(iconGlyphName)
		, _iconStyle(iconStyle)
		, _labelContent(labelContent)
		, _icon()
		, _label()
		, _isToggle(isToggle)
		, _isSelected(false)
		, _isMorph(isMorph)
		, _isDisabled(isDisabled)
		, _isTrailingIcon(isTrailingIcon)
		, _accessibilityLabel(accessibilityLabel)
		, _onClick(std::move(onClick))
	{
	}

	ButtonBase &ButtonBase::setIconGlyphName(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;

		if (_icon) {
			_icon->setGlyphName(getActiveIconGlyphName());
		}

		return *this;
	}

	ButtonBase &
		ButtonBase::setIconStyle(const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;

		if (_icon) {
			_icon->setStyle(_iconStyle);
		}

		return *this;
	}

	ButtonBase &ButtonBase::setLabelContent(const std::string &labelContent)
	{
		_labelContent = labelContent;

		if (_label) {
			_label->setContent(_labelContent);
		}

		return *this;
	}

	ButtonBase &ButtonBase::setIsToggle(const bool &isToggle)
	{
		_isToggle = isToggle;

		if (!_isToggle) {
			_isSelected = false;
		}

		return *this;
	}

	bool ButtonBase::isSelected(void) const
	{
		return _isSelected;
	}

	ButtonBase &ButtonBase::setSelected(const bool &selected)
	{
		if (!_isToggle) {
			return *this;
		}

		_isSelected = selected;

		if (_icon && !_selectedIconGlyphName.empty()) {
			_icon->setGlyphName(getActiveIconGlyphName());
		}

		applyStyleState();
		applyGeometry();

		return *this;
	}

	ButtonBase &ButtonBase::setSelectedIconGlyphName(
		const std::string &selectedIconGlyphName)
	{
		_selectedIconGlyphName = selectedIconGlyphName;

		if (_icon && !_selectedIconGlyphName.empty() && _isSelected) {
			_icon->setGlyphName(_selectedIconGlyphName);
		}

		return *this;
	}

	ButtonBase &ButtonBase::setMorph(const bool &isMorph)
	{
		_isMorph = isMorph;

		applyStyleState();
		applyGeometry();

		return *this;
	}

	ButtonBase &ButtonBase::setTrailingIcon(const bool &isTrailingIcon)
	{
		_isTrailingIcon = isTrailingIcon;

		applyGeometry();

		return *this;
	}

	ButtonBase &ButtonBase::setDisabled(const bool &isDisabled)
	{
		_isDisabled = isDisabled;

		applyStyleState();
		applyGeometry();

		return *this;
	}

	bool ButtonBase::isDisabled(void) const
	{
		return _isDisabled;
	}

	ButtonBase &
		ButtonBase::setAccessibilityLabel(const std::string &accessibilityLabel)
	{
		_accessibilityLabel = accessibilityLabel;
		return *this;
	}

	const std::string &ButtonBase::getAccessibilityLabel(void) const
	{
		return _accessibilityLabel;
	}

	ButtonBase &ButtonBase::setOnClick(std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	ecs::Entity::Identifier ButtonBase::getIconIdentifier(void) const
	{
		return _icon != nullptr ? _icon->getIdentifier()
								: ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier ButtonBase::getLabelIdentifier(void) const
	{
		return _label != nullptr ? _label->getIdentifier()
								 : ecs::Entity::InvalidIdentifier;
	}

	bool ButtonBase::isHovered(void)
	{
		bool hovered = false;

		hovered =
			getComponentRegistry()
				.getComponent<components::HandHoverInteraction>(getIdentifier())
				.isHovered()
			? true
			: hovered;

		hovered = getComponentRegistry()
					  .getComponent<components::MouseHoverInteraction>(
						  getIdentifier())
					  .isHovered()
			? true
			: hovered;

		return hovered;
	}

	bool ButtonBase::isPressed(void)
	{
		bool pressed = false;

		pressed =
			getComponentRegistry()
				.getComponent<components::HandButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::HandButtonEvent::Button::A)
			? true
			: pressed;

		pressed =
			getComponentRegistry()
				.getComponent<components::MouseButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::MouseButtonEvent::Button::Left)
			? true
			: pressed;

		return pressed;
	}

	const std::string &ButtonBase::getActiveIconGlyphName(void) const
	{
		if (_isSelected && !_selectedIconGlyphName.empty()) {
			return _selectedIconGlyphName;
		}

		return _iconGlyphName;
	}

	const utility::graphic::PoseF ButtonBase::applyLayerToPosition(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation,
		const std::uint32_t &layer)
	{
		const auto forwardVector					= orientation.getForward();
		utility::graphic::PositionF forwardPosition = position;

		forwardPosition.translate(utility::graphic::PositionF(
			-forwardVector * static_cast<float>(layer + 1) * 1.0f));

		return utility::graphic::PoseF(forwardPosition, orientation);
	}

	utility::graphic::Color32Bit
		ButtonBase::applyStateAlpha(const utility::graphic::Color32Bit &color,
									std::uint8_t alpha)
	{
		return utility::graphic::Color32Bit(color.getRed(), color.getGreen(),
											color.getBlue(), alpha);
	}

	utility::graphic::Color32Bit
		ButtonBase::applyStateLayer(const utility::graphic::Color32Bit &base,
									const utility::graphic::Color32Bit &overlay,
									std::uint8_t alpha)
	{
		return overlay.withAlpha(alpha).blendOver(base);
	}

	utility::graphic::Color32Bit ButtonBase::getDisabledContainerColor(void)
	{
		const auto &scheme = guillaume::getActiveScheme();

		return applyStateAlpha(
			scheme.getColor(SchemeColorRole::OnSurface).getColor(), 31U);
	}

	utility::graphic::Color32Bit ButtonBase::getDisabledContentColor(void)
	{
		const auto &scheme = guillaume::getActiveScheme();

		return applyStateAlpha(
			scheme.getColor(SchemeColorRole::OnSurface).getColor(), 97U);
	}

	utility::graphic::Color32Bit ButtonBase::getDisabledBorderColor(void)
	{
		const auto &scheme = guillaume::getActiveScheme();

		return applyStateAlpha(
			scheme.getColor(SchemeColorRole::OnSurface).getColor(), 31U);
	}

	void ButtonBase::hoverHandler(void)
	{
		if (_isDisabled) {
			return;
		}

		applyStyleState();
	}

	void ButtonBase::unHoverHandler(void)
	{
		if (_isDisabled) {
			return;
		}

		applyStyleState();
	}

	void ButtonBase::buttonPressHandler()
	{
		if (_isDisabled) {
			return;
		}

		applyStyleState();
	}

	void ButtonBase::buttonReleaseHandler()
	{
		if (_isDisabled) {
			return;
		}

		if (_isToggle) {
			_isSelected = !_isSelected;
		}

		if (_icon && !_selectedIconGlyphName.empty()) {
			_icon->setGlyphName(getActiveIconGlyphName());
		}

		applyStyleState();
		applyGeometry();

		if (_onClick) {
			_onClick();
		}
	}

	void ButtonBase::initialize(void)
	{
		if (usesIcon()) {
			Icon::Builder iconBuilder(getComponentRegistry(), *this);
			Icon::Director iconDirector;

			_icon = iconDirector.makeIcon(iconBuilder, shared_from_this(),
										  _iconGlyphName, getIconFontSize(),
										  getInitialContentColor(), _iconStyle);
		}

		if (usesLabel()) {
			Text::Builder labelBuilder(getComponentRegistry(), *this);
			Text::Director labelDirector;

			_label = labelDirector.makeText(labelBuilder, shared_from_this(),
											_labelContent, getIconFontSize(),
											getInitialContentColor());
		}

		getComponentRegistry()
			.getComponent<components::HandButtonInteraction>(getIdentifier())
			.setOnButtonPressHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&ButtonBase::buttonPressHandler, this))
			.setOnButtonReleaseHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&ButtonBase::buttonReleaseHandler, this));

		getComponentRegistry()
			.getComponent<components::MouseButtonInteraction>(getIdentifier())
			.setOnButtonPressHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&ButtonBase::buttonPressHandler, this))
			.setOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&ButtonBase::buttonReleaseHandler, this));

		getComponentRegistry()
			.getComponent<components::HandHoverInteraction>(getIdentifier())
			.setOnHoverHandler(std::bind(&ButtonBase::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&ButtonBase::unHoverHandler, this));

		getComponentRegistry()
			.getComponent<components::MouseHoverInteraction>(getIdentifier())
			.setOnHoverHandler(std::bind(&ButtonBase::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&ButtonBase::unHoverHandler, this));
	}

	void ButtonBase::update(void)
	{
		setIconGlyphName(_iconGlyphName);
		setIconStyle(_iconStyle);
		setLabelContent(_labelContent);
		setIsToggle(_isToggle);
		setMorph(_isMorph);
		setDisabled(_isDisabled);
		setTrailingIcon(_isTrailingIcon);
		setSelectedIconGlyphName(_selectedIconGlyphName);
		setAccessibilityLabel(_accessibilityLabel);
		setOnClick(_onClick);

		applyStyleState();
		applyGeometry();
	}
}	 // namespace guillaume::entities
