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

#include "guillaume/entities/icon_button.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	IconButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	IconButton::Builder::~Builder(void)
	{
	}

	std::shared_ptr<IconButton>
		IconButton::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_iconButton = std::make_shared<IconButton>(
			this->getComponentRegistry(), _iconGlyphName,
			_selectedIconGlyphName, _iconStyle, _isToggle, _variant, _size,
			_isDisabled, _accessibilityLabel, _onClick);
		_iconButton->setParent(parent);

		this->getEntityRegistry().addEntity(_iconButton);

		auto iconButtonCopy = _iconButton;

		reset();

		return iconButtonCopy;
	}

	void IconButton::Builder::reset(void)
	{
		_iconButton.reset();
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

	IconButton::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	IconButton::Director::~Director(void)
	{
	}

	std::shared_ptr<IconButton> IconButton::Director::makeIconButton(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &iconGlyphName, std::function<void(void)> onClick,
		Variant variant, Size size)
	{
		return builder.withIcon(iconGlyphName)
			.withOnClick(onClick)
			.withVariant(variant)
			.withSize(size)
			.registerEntity(parent);
	}

	static float getContainerSize(IconButton::Size size)
	{
		switch (size) {
			case IconButton::Size::Small:
				return 32.0f;
			case IconButton::Size::Medium:
				return 40.0f;
			case IconButton::Size::Large:
				return 48.0f;
			default:
				throw std::runtime_error("Invalid icon button size");
		}
	}

	static float getIconFontSize(IconButton::Size size)
	{
		switch (size) {
			case IconButton::Size::Small:
				return 18.0f;
			case IconButton::Size::Medium:
				return 24.0f;
			case IconButton::Size::Large:
				return 30.0f;
			default:
				throw std::runtime_error("Invalid icon button size");
		}
	}

	static float getBorderRadius(IconButton::Size size)
	{
		switch (size) {
			case IconButton::Size::Small:
				return 8.0f;
			case IconButton::Size::Medium:
				return 12.0f;
			case IconButton::Size::Large:
				return 16.0f;
			default:
				throw std::runtime_error("Invalid icon button size");
		}
	}

	static utility::graphic::Color32Bit
		getDefaultContainerColor(IconButton::Variant variant)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (variant) {
			case IconButton::Variant::Standard:
				return utility::graphic::Color32Bit(0, 0, 0, 0);
			case IconButton::Variant::Filled:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case IconButton::Variant::Tonal:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case IconButton::Variant::Outlined:
				return utility::graphic::Color32Bit(0, 0, 0, 0);
			default:
				throw std::runtime_error("Invalid icon button variant");
		}
	}

	static utility::graphic::Color32Bit
		getSelectedContainerColor(IconButton::Variant variant)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (variant) {
			case IconButton::Variant::Standard:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case IconButton::Variant::Filled:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case IconButton::Variant::Tonal:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case IconButton::Variant::Outlined:
				return scheme.getColor(SchemeColorRole::InverseSurface)
					.getColor();
			default:
				throw std::runtime_error("Invalid icon button variant");
		}
	}

	static utility::graphic::Color32Bit
		getContentColor(IconButton::Variant variant, bool isSelected)
	{
		const auto &scheme = guillaume::getActiveScheme();

		if (isSelected) {
			switch (variant) {
				case IconButton::Variant::Standard:
					return scheme
						.getColor(SchemeColorRole::OnSecondaryContainer)
						.getColor();
				case IconButton::Variant::Filled:
					return scheme.getColor(SchemeColorRole::OnPrimary)
						.getColor();
				case IconButton::Variant::Tonal:
					return scheme
						.getColor(SchemeColorRole::OnSecondaryContainer)
						.getColor();
				case IconButton::Variant::Outlined:
					return scheme.getColor(SchemeColorRole::InverseOnSurface)
						.getColor();
				default:
					throw std::runtime_error("Invalid icon button variant");
			}
		}

		switch (variant) {
			case IconButton::Variant::Standard:
				return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
					.getColor();
			case IconButton::Variant::Filled:
				return scheme.getColor(SchemeColorRole::OnPrimary).getColor();
			case IconButton::Variant::Tonal:
				return scheme.getColor(SchemeColorRole::OnSecondaryContainer)
					.getColor();
			case IconButton::Variant::Outlined:
				return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
					.getColor();
			default:
				throw std::runtime_error("Invalid icon button variant");
		}
	}

	static utility::graphic::Color32Bit
		getBorderColor(IconButton::Variant variant, bool isSelected)
	{
		const auto &scheme = guillaume::getActiveScheme();

		if (variant != IconButton::Variant::Outlined) {
			return utility::graphic::Color32Bit(0, 0, 0, 0);
		}

		if (isSelected) {
			return utility::graphic::Color32Bit(0, 0, 0, 0);
		}

		return scheme.getColor(SchemeColorRole::Outline).getColor();
	}

	utility::graphic::Color32Bit IconButton::getInitialContentColor(void) const
	{
		return getContentColor(_variant, _isSelected);
	}

	float IconButton::getIconFontSize(void) const
	{
		return ::guillaume::entities::getIconFontSize(_size);
	}

	void IconButton::applyStyleState(void)
	{
		if (_isDisabled) {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(getDisabledContainerColor());

			getComponentRegistry()
				.getComponent<components::Borders>(getIdentifier())
				.setColor(getDisabledContainerColor());

			if (_icon) {
				_icon->setColor(getDisabledContentColor());
			}

			return;
		}

		const auto baseContainerColor = _isSelected
			? getSelectedContainerColor(_variant)
			: getDefaultContainerColor(_variant);

		utility::graphic::Color32Bit containerColor = baseContainerColor;

		if (isPressed()) {
			containerColor = ButtonBase::applyStateLayer(
				baseContainerColor, getContentColor(_variant, _isSelected),
				31U);
		} else if (isHovered()) {
			containerColor = ButtonBase::applyStateLayer(
				baseContainerColor, getContentColor(_variant, _isSelected),
				20U);
		}

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(containerColor);

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(getBorderColor(_variant, _isSelected));

		if (_icon) {
			_icon->setColor(getContentColor(_variant, _isSelected));
		}
	}

	void IconButton::applyGeometry(void)
	{
		const float containerSize = getContainerSize(_size);

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(containerSize)
			.setHeight(containerSize);

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getBorderRadius(_size));

		if (_icon) {
			_icon->setFontSize(getIconFontSize());
		}
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
		, _variant(variant)
		, _size(size)
	{
		setSelectedIconGlyphName(selectedIconGlyphName);
	}

	IconButton::~IconButton(void)
	{
	}

	IconButton &IconButton::setVariant(const Variant &variant)
	{
		_variant = variant;

		applyStyleState();

		return *this;
	}

	IconButton &IconButton::setSize(const Size &size)
	{
		_size = size;

		applyGeometry();

		return *this;
	}

	IconButton::Variant IconButton::getVariant(void) const
	{
		return _variant;
	}

	IconButton::Size IconButton::getSize(void) const
	{
		return _size;
	}
}	 // namespace guillaume::entities
