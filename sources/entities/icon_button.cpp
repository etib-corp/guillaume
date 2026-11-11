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

#include <array>
#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

#include "guillaume/entities/icon_button.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"
#include "guillaume/systems/icon_button.hpp"

namespace guillaume::entities
{
	IconButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<IconButton>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<IconButton> IconButton::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<IconButton>(
			this->getComponentRegistry(), _iconGlyphName,
			_selectedIconGlyphName, _iconStyle, _isToggle, _variant, _size,
			_isDisabled, _accessibilityLabel, _onClick);
		return entity;
	}

	void IconButton::Builder::reset(void)
	{
		_iconGlyphName.clear();
		_selectedIconGlyphName.clear();
		_iconStyle	= components::Glyph::Style::Outlined;
		_isToggle	= false;
		_variant	= Variant::Standard;
		_size		= Size::Medium;
		_isDisabled = false;
		_accessibilityLabel.clear();
		_onClick = {};
	}

	IconButton::Builder &
		IconButton::Builder::withIcon(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;
		return *this;
	}

	IconButton::Builder &IconButton::Builder::withSelectedIcon(
		const std::string &selectedIconGlyphName)
	{
		_selectedIconGlyphName = selectedIconGlyphName;
		return *this;
	}

	IconButton::Builder &IconButton::Builder::withIconStyle(
		const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;
		return *this;
	}

	IconButton::Builder &
		IconButton::Builder::withOnClick(std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	IconButton::Builder &IconButton::Builder::withToggle(const bool &isToggle)
	{
		_isToggle = isToggle;
		return *this;
	}

	IconButton::Builder &
		IconButton::Builder::withVariant(const Variant &variant)
	{
		_variant = variant;
		return *this;
	}

	IconButton::Builder &IconButton::Builder::withSize(const Size &size)
	{
		_size = size;
		return *this;
	}

	IconButton::Builder &
		IconButton::Builder::withDisabled(const bool &isDisabled)
	{
		_isDisabled = isDisabled;
		return *this;
	}

	IconButton::Builder &IconButton::Builder::withAccessibilityLabel(
		const std::string &accessibilityLabel)
	{
		_accessibilityLabel = accessibilityLabel;
		return *this;
	}

	std::shared_ptr<IconButton> IconButton::Builder::makeIconButton(
		std::shared_ptr<Entity> parent, const std::string &iconGlyphName,
		std::function<void(void)> onClick, Variant variant, Size size)
	{
		return withIcon(iconGlyphName)
			.withOnClick(onClick)
			.withVariant(variant)
			.withSize(size)
			.registerEntity(parent);
	}

	IconButton::IconButton(ecs::ComponentRegistry &registry,
						   const std::string &iconGlyphName,
						   const std::string &selectedIconGlyphName,
						   const components::Glyph::Style &iconStyle,
						   bool isToggle, Variant variant, Size size,
						   bool isDisabled,
						   const std::string &accessibilityLabel,
						   std::function<void(void)> onClick)
		: ButtonBase(registry, iconGlyphName, iconStyle, "", isToggle, false,
					 isDisabled, false, accessibilityLabel, std::move(onClick))
	{
		getComponentRegistry()
			.addComponent<components::IconButton>(getIdentifier())
			.setVariant(variant)
			.setSize(size);
		setSignature(getSignature()
					 | getSignatureFromTypes<components::IconButton>());
		systems::IconButton::install(getComponentRegistry(), getIdentifier());
		setSelectedIconGlyphName(selectedIconGlyphName);
	}

	float IconButton::getIconFontSize(void) const
	{
		return systems::IconButton::iconFontSize(getComponentRegistry(),
												 getIdentifier());
	}

	utility::graphic::Color32Bit IconButton::getInitialContentColor(void) const
	{
		return systems::IconButton::contentColor(getComponentRegistry(),
												 getIdentifier());
	}

	IconButton &IconButton::setVariant(const Variant &variant)
	{
		getComponentRegistry()
			.getComponent<components::IconButton>(getIdentifier())
			.setVariant(variant);
		restyle();
		return *this;
	}

	IconButton &IconButton::setSize(const Size &size)
	{
		getComponentRegistry()
			.getComponent<components::IconButton>(getIdentifier())
			.setSize(size);
		restyle();
		return *this;
	}

	IconButton::Variant IconButton::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::IconButton>(getIdentifier())
			.getVariant();
	}

	IconButton::Size IconButton::getSize(void) const
	{
		return getComponentRegistry()
			.getComponent<components::IconButton>(getIdentifier())
			.getSize();
	}

}	 // namespace guillaume::entities
