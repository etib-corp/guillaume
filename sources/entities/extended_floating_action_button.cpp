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
#include <stdexcept>
#include <utility>

#include "guillaume/entities/extended_floating_action_button.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/fab_style.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"
#include "guillaume/systems/extended_floating_action_button.hpp"

namespace guillaume::entities
{
	ExtendedFloatingActionButton::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<ExtendedFloatingActionButton>(componentRegistry,
														  entityRegistry)
	{
		reset();
	}

	std::shared_ptr<ExtendedFloatingActionButton>
		ExtendedFloatingActionButton::Builder::buildEntity(void)
	{
		auto entity = ExtendedFloatingActionButton::create(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _variant, _size, _lowered, _isDisabled,
			_accessibilityLabel, _onClick);
		return entity;
	}

	void ExtendedFloatingActionButton::Builder::reset(void)
	{
		_iconGlyphName.clear();
		_iconStyle = components::Glyph::Style::Outlined;
		_labelContent.clear();
		_variant	= Variant::Primary;
		_size		= Size::Medium;
		_lowered	= false;
		_isDisabled = false;
		_accessibilityLabel.clear();
		_onClick = {};
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withIcon(
			const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withIconStyle(
			const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withLabel(
			const std::string &labelContent)
	{
		_labelContent = labelContent;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withOnClick(
			std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withVariant(
			const Variant &variant)
	{
		_variant = variant;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withSize(const Size &size)
	{
		_size = size;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withLowered(const bool &lowered)
	{
		_lowered = lowered;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withDisabled(
			const bool &isDisabled)
	{
		_isDisabled = isDisabled;
		return *this;
	}

	ExtendedFloatingActionButton::Builder &
		ExtendedFloatingActionButton::Builder::withAccessibilityLabel(
			const std::string &accessibilityLabel)
	{
		_accessibilityLabel = accessibilityLabel;
		return *this;
	}

	std::shared_ptr<ExtendedFloatingActionButton>
		ExtendedFloatingActionButton::Builder::makeExtendedFloatingActionButton(
			std::shared_ptr<Entity> parent, const std::string &labelContent,
			std::function<void(void)> onClick)
	{
		return withLabel(labelContent)
			.withOnClick(onClick)
			.registerEntity(parent);
	}

	ExtendedFloatingActionButton::ExtendedFloatingActionButton(
		ecs::ComponentRegistry &registry, const std::string &iconGlyphName,
		const components::Glyph::Style &iconStyle,
		const std::string &labelContent, Variant variant, Size size,
		bool lowered, bool isDisabled, const std::string &accessibilityLabel,
		std::function<void(void)> onClick)
		: ButtonBase(registry, iconGlyphName, iconStyle, labelContent, false,
					 false, isDisabled, false, accessibilityLabel,
					 std::move(onClick))
	{
		getComponentRegistry()
			.addComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setVariant(variant)
			.setSize(size)
			.setLowered(lowered);
		setSignature(getSignature()
					 | getSignatureFromTypes<
						 components::ExtendedFloatingActionButton>());
		systems::ExtendedFloatingActionButton::install(getComponentRegistry(),
													   getIdentifier());
	}

	float ExtendedFloatingActionButton::getIconFontSize(void) const
	{
		return systems::ExtendedFloatingActionButton::iconFontSize(
			getComponentRegistry(), getIdentifier());
	}

	utility::graphic::Color32Bit
		ExtendedFloatingActionButton::getInitialContentColor(void) const
	{
		return systems::ExtendedFloatingActionButton::contentColor(
			getComponentRegistry(), getIdentifier());
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setMode(Mode mode)
	{
		getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setMode(mode);
		restyle();
		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setCustomSize(std::optional<float> size)
	{
		if (size.has_value() && *size <= 0.0f) {
			throw std::invalid_argument("FAB custom size must be positive");
		}
		getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setCustomSize(size);
		restyle();
		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setVariant(const Variant &variant)
	{
		getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setVariant(variant);
		restyle();
		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setSize(const Size &size)
	{
		getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setSize(size);
		restyle();
		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setLowered(const bool &lowered)
	{
		getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setLowered(lowered);
		restyle();
		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setExtended(bool extended)
	{
		getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.setExtended(extended);
		restyle();
		return *this;
	}

	ExtendedFloatingActionButton::Mode
		ExtendedFloatingActionButton::getMode(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.getMode();
	}

	ExtendedFloatingActionButton::Variant
		ExtendedFloatingActionButton::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.getVariant();
	}

	ExtendedFloatingActionButton::Size
		ExtendedFloatingActionButton::getSize(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.getSize();
	}

	bool ExtendedFloatingActionButton::isLowered(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.isLowered();
	}

	bool ExtendedFloatingActionButton::isExtended(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ExtendedFloatingActionButton>(
				getIdentifier())
			.isExtended();
	}

}	 // namespace guillaume::entities
