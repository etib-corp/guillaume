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

#include <cmath>
#include <utility>

#include "guillaume/entities/button_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
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
		: ecs::ParentEntityFiller<
			  components::Transform, components::Bound, components::Color,
			  components::Borders, components::Focus,
			  components::HandButtonInteraction,
			  components::HandHoverInteraction,
			  components::MouseHoverInteraction,
			  components::MouseButtonInteraction, components::StyleState,
			  components::Accessibility, components::Button>(registry)
	{
		state()
			.setIconGlyphName(iconGlyphName)
			.setIconStyle(iconStyle)
			.setLabelContent(labelContent)
			.setToggle(isToggle)
			.setMorph(isMorph)
			.setTrailingIcon(isTrailingIcon)
			.setOnClick(std::move(onClick));
		getComponentRegistry()
			.getComponent<components::StyleState>(getIdentifier())
			.setEnabled(!isDisabled);
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::Button)
			.setLabel(accessibilityLabel.empty() ? labelContent
												 : accessibilityLabel);
	}

	components::Button &ButtonBase::state(void)
	{
		return getComponentRegistry().getComponent<components::Button>(
			getIdentifier());
	}

	const components::Button &ButtonBase::state(void) const
	{
		return getComponentRegistry().getComponent<components::Button>(
			getIdentifier());
	}

	void ButtonBase::restyle(void)
	{
		systems::Button::restyle(getComponentRegistry(), getIdentifier());
	}

	ButtonBase &ButtonBase::setIconGlyphName(const std::string &iconGlyphName)
	{
		state().setIconGlyphName(iconGlyphName);
		systems::Button::syncContent(getComponentRegistry(), getIdentifier());
		return *this;
	}

	ButtonBase &
		ButtonBase::setIconStyle(const components::Glyph::Style &iconStyle)
	{
		state().setIconStyle(iconStyle);
		systems::Button::syncContent(getComponentRegistry(), getIdentifier());
		return *this;
	}

	ButtonBase &ButtonBase::setLabelContent(const std::string &labelContent)
	{
		state().setLabelContent(labelContent);
		systems::Button::Child(getComponentRegistry(), state().getLabel())
			.setContent(labelContent);
		return *this;
	}

	void ButtonBase::syncAccessibilityState(void)
	{
		systems::Button::syncContent(getComponentRegistry(), getIdentifier());
	}

	ButtonBase &ButtonBase::setIsToggle(const bool &isToggle)
	{
		state().setToggle(isToggle);
		if (!isToggle) {
			state().setSelected(false);
		}
		return *this;
	}

	bool ButtonBase::isSelected(void) const
	{
		return state().isSelected();
	}

	ButtonBase &ButtonBase::setSelected(const bool &selected)
	{
		systems::Button::setSelected(getComponentRegistry(), getIdentifier(),
									 selected);
		return *this;
	}

	ButtonBase &ButtonBase::setSelectedIconGlyphName(
		const std::string &selectedIconGlyphName)
	{
		state().setSelectedIconGlyphName(selectedIconGlyphName);
		systems::Button::syncContent(getComponentRegistry(), getIdentifier());
		return *this;
	}

	ButtonBase &ButtonBase::setMorph(const bool &isMorph)
	{
		state().setMorph(isMorph);
		restyle();
		return *this;
	}

	ButtonBase &ButtonBase::setTrailingIcon(const bool &isTrailingIcon)
	{
		state().setTrailingIcon(isTrailingIcon);
		restyle();
		return *this;
	}

	ButtonBase &ButtonBase::setDisabled(const bool &isDisabled)
	{
		getComponentRegistry()
			.getComponent<components::StyleState>(getIdentifier())
			.setEnabled(!isDisabled);
		restyle();
		return *this;
	}

	bool ButtonBase::isDisabled(void) const
	{
		return !getComponentRegistry()
					.getComponent<components::StyleState>(getIdentifier())
					.isEnabled();
	}

	ButtonBase &
		ButtonBase::setAccessibilityLabel(const std::string &accessibilityLabel)
	{
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setLabel(accessibilityLabel);
		return *this;
	}

	const std::string &ButtonBase::getAccessibilityLabel(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.getLabel();
	}

	ButtonBase &
		ButtonBase::setAccessibilityHint(const std::string &accessibilityHint)
	{
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setHint(accessibilityHint);
		return *this;
	}

	const std::string &ButtonBase::getAccessibilityHint(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.getHint();
	}

	ButtonBase &
		ButtonBase::setAccessibilityRole(components::AccessibilityRole role)
	{
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(role);
		return *this;
	}

	const utility::graphic::PoseF ButtonBase::applyLayerToPosition(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation,
		const std::uint32_t &layer)
	{
		return systems::Layout::contentLayerPose(position, orientation, layer);
	}

	utility::graphic::Color32Bit
		ButtonBase::applyStateAlpha(const utility::graphic::Color32Bit &color,
									std::uint8_t alpha)
	{
		return withAlpha(color, alpha);
	}

	utility::graphic::Color32Bit
		ButtonBase::applyStateLayer(const utility::graphic::Color32Bit &base,
									const utility::graphic::Color32Bit &overlay,
									std::uint8_t alpha)
	{
		return stateLayer(base, overlay, alpha);
	}

	utility::graphic::Color32Bit ButtonBase::getDisabledContainerColor(void)
	{
		const auto &scheme = guillaume::getActiveScheme();

		return applyStateAlpha(
			scheme.getColor(SchemeColorRole::OnSurface).getColor(),
			getStateLayerAlpha(theme::StateLayer::DisabledContainer));
	}

	utility::graphic::Color32Bit ButtonBase::getDisabledContentColor(void)
	{
		const auto &scheme = guillaume::getActiveScheme();

		return applyStateAlpha(
			scheme.getColor(SchemeColorRole::OnSurface).getColor(),
			getStateLayerAlpha(theme::StateLayer::DisabledContent));
	}

	utility::graphic::Color32Bit ButtonBase::getDisabledBorderColor(void)
	{
		const auto &scheme = guillaume::getActiveScheme();

		return applyStateAlpha(
			scheme.getColor(SchemeColorRole::OnSurface).getColor(),
			getStateLayerAlpha(theme::StateLayer::DisabledContainer));
	}

	ButtonBase &
		ButtonBase::setOnLongPress(std::function<void(void)> onLongPress)
	{
		state().setOnLongPress(std::move(onLongPress));
		return *this;
	}

	ButtonBase &ButtonBase::setOnClick(std::function<void(void)> onClick)
	{
		state().setOnClick(std::move(onClick));
		return *this;
	}

	ButtonBase &ButtonBase::setLoading(bool loading)
	{
		state().setLoading(loading);
		systems::Button::syncContent(getComponentRegistry(), getIdentifier());
		return *this;
	}

	bool ButtonBase::isLoading(void) const
	{
		return state().isLoading();
	}

	ButtonBase &ButtonBase::setVisible(bool visible)
	{
		state().setVisible(visible);
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setFocusable(visible);
		restyle();
		return *this;
	}

	bool ButtonBase::isVisible(void) const
	{
		return state().isVisible();
	}

	ButtonBase &ButtonBase::setContainerColorOverride(
		std::optional<utility::graphic::Color32Bit> color)
	{
		state().setContainerColorOverride(color);
		restyle();
		return *this;
	}

	ButtonBase &ButtonBase::setContentColorOverride(
		std::optional<utility::graphic::Color32Bit> color)
	{
		state().setContentColorOverride(color);
		restyle();
		return *this;
	}

	utility::graphic::Color32Bit ButtonBase::resolveContainerColor(
		const utility::graphic::Color32Bit &color) const
	{
		return systems::Button::resolveContainerColor(state(), color);
	}

	utility::graphic::Color32Bit ButtonBase::resolveContentColor(
		const utility::graphic::Color32Bit &color) const
	{
		return systems::Button::resolveContentColor(state(), color);
	}

	ecs::Entity::Identifier ButtonBase::getIconIdentifier(void) const
	{
		return state().getIcon();
	}

	ecs::Entity::Identifier ButtonBase::getLabelIdentifier(void) const
	{
		return state().getLabel();
	}

	bool ButtonBase::isHovered(void)
	{
		return systems::Button::isHovered(getComponentRegistry(),
										  getIdentifier());
	}

	bool ButtonBase::isPressed(void)
	{
		return systems::Button::isPressed(getComponentRegistry(),
										  getIdentifier());
	}

	const std::string &ButtonBase::getActiveIconGlyphName(void) const
	{
		return systems::Button::activeIconGlyphName(state());
	}

	void ButtonBase::applyTouchTarget(float width, float height)
	{
		systems::Button::applyTouchTarget(getComponentRegistry(),
										  getIdentifier(), width, height);
	}

	void ButtonBase::hoverHandler(void)
	{
		systems::Button::hoverChanged(getComponentRegistry(), getIdentifier());
	}

	void ButtonBase::unHoverHandler(void)
	{
		systems::Button::hoverChanged(getComponentRegistry(), getIdentifier());
	}

	void ButtonBase::buttonPressHandler()
	{
		systems::Button::press(getComponentRegistry(), getIdentifier());
	}

	void ButtonBase::buttonReleaseHandler()
	{
		systems::Button::release(getComponentRegistry(), getIdentifier());
	}

	void ButtonBase::build(void)
	{
		if (usesIcon()) {
			Icon::Builder iconBuilder(getComponentRegistry(), *this);
			const auto icon = iconBuilder.makeIcon(
				shared_from_this(), state().getIconGlyphName(),
				getIconFontSize(), getInitialContentColor(),
				state().getIconStyle());
			if (icon != nullptr) {
				state().setIcon(icon->getIdentifier());
			}
		}

		if (usesLabel()) {
			Text::Builder labelBuilder(getComponentRegistry(), *this);
			const auto label = labelBuilder.makeText(
				shared_from_this(), state().getLabelContent(),
				theme::TypeRole::LabelLarge, getInitialContentColor());
			if (label != nullptr) {
				state().setLabel(label->getIdentifier());
			}
		}

		systems::Button::syncContent(getComponentRegistry(), getIdentifier());
		restyle();
	}

}	 // namespace guillaume::entities
