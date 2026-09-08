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

#include "guillaume/entities/floating_action_button.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	FloatingActionButton::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	FloatingActionButton::Builder::~Builder(void)
	{
	}

	std::shared_ptr<FloatingActionButton>
		FloatingActionButton::Builder::registerEntity(
			std::shared_ptr<Entity> parent)
	{
		_fab = std::make_shared<FloatingActionButton>(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _variant, _size, _lowered, _isDisabled,
			_accessibilityLabel, _onClick);
		_fab->setParent(parent);

		this->getEntityRegistry().addEntity(_fab);

		auto fabCopy = _fab;

		reset();

		return fabCopy;
	}

	void FloatingActionButton::Builder::reset(void)
	{
		_fab.reset();
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

	FloatingActionButton::Builder &FloatingActionButton::Builder::withIcon(
		const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;
		return *this;
	}

	FloatingActionButton::Builder &FloatingActionButton::Builder::withIconStyle(
		const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;
		return *this;
	}

	FloatingActionButton::Builder &FloatingActionButton::Builder::withLabel(
		const std::string &labelContent)
	{
		_labelContent = labelContent;
		return *this;
	}

	FloatingActionButton::Builder &FloatingActionButton::Builder::withOnClick(
		std::function<void(void)> onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	FloatingActionButton::Builder &
		FloatingActionButton::Builder::withVariant(const Variant &variant)
	{
		_variant = variant;
		return *this;
	}

	FloatingActionButton::Builder &
		FloatingActionButton::Builder::withSize(const Size &size)
	{
		_size = size;
		return *this;
	}

	FloatingActionButton::Builder &
		FloatingActionButton::Builder::withLowered(const bool &lowered)
	{
		_lowered = lowered;
		return *this;
	}

	FloatingActionButton::Builder &
		FloatingActionButton::Builder::withDisabled(const bool &isDisabled)
	{
		_isDisabled = isDisabled;
		return *this;
	}

	FloatingActionButton::Builder &
		FloatingActionButton::Builder::withAccessibilityLabel(
			const std::string &accessibilityLabel)
	{
		_accessibilityLabel = accessibilityLabel;
		return *this;
	}

	FloatingActionButton::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	FloatingActionButton::Director::~Director(void)
	{
	}

	std::shared_ptr<FloatingActionButton>
		FloatingActionButton::Director::makeFloatingActionButton(
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

	static float getContainerSize(FloatingActionButton::Size size)
	{
		switch (size) {
			case FloatingActionButton::Size::Small:
				return 40.0f;
			case FloatingActionButton::Size::Medium:
				return 56.0f;
			case FloatingActionButton::Size::Large:
				return 96.0f;
			default:
				throw std::runtime_error("Invalid FAB size");
		}
	}

	static float getIconFontSize(FloatingActionButton::Size size)
	{
		switch (size) {
			case FloatingActionButton::Size::Small:
				return 24.0f;
			case FloatingActionButton::Size::Medium:
				return 24.0f;
			case FloatingActionButton::Size::Large:
				return 36.0f;
			default:
				throw std::runtime_error("Invalid FAB size");
		}
	}

	static float getBorderRadius(FloatingActionButton::Size size)
	{
		switch (size) {
			case FloatingActionButton::Size::Small:
				return 12.0f;
			case FloatingActionButton::Size::Medium:
				return 16.0f;
			case FloatingActionButton::Size::Large:
				return 28.0f;
			default:
				throw std::runtime_error("Invalid FAB size");
		}
	}

	static utility::graphic::Color32Bit
		getContainerColor(FloatingActionButton::Variant variant, bool lowered)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (variant) {
			case FloatingActionButton::Variant::Surface:
				return scheme
					.getColor(lowered ? SchemeColorRole::SurfaceContainerLow
									  : SchemeColorRole::SurfaceContainerHigh)
					.getColor();
			case FloatingActionButton::Variant::Primary:
				return scheme.getColor(SchemeColorRole::PrimaryContainer)
					.getColor();
			case FloatingActionButton::Variant::Secondary:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case FloatingActionButton::Variant::Tertiary:
				return scheme.getColor(SchemeColorRole::TertiaryContainer)
					.getColor();
			default:
				throw std::runtime_error("Invalid FAB variant");
		}
	}

	static utility::graphic::Color32Bit
		getContentColor(FloatingActionButton::Variant variant)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (variant) {
			case FloatingActionButton::Variant::Surface:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case FloatingActionButton::Variant::Primary:
				return scheme.getColor(SchemeColorRole::OnPrimaryContainer)
					.getColor();
			case FloatingActionButton::Variant::Secondary:
				return scheme.getColor(SchemeColorRole::OnSecondaryContainer)
					.getColor();
			case FloatingActionButton::Variant::Tertiary:
				return scheme.getColor(SchemeColorRole::OnTertiaryContainer)
					.getColor();
			default:
				throw std::runtime_error("Invalid FAB variant");
		}
	}

	utility::graphic::Color32Bit
		FloatingActionButton::getInitialContentColor(void) const
	{
		return getContentColor(_variant);
	}

	float FloatingActionButton::getIconFontSize(void) const
	{
		return ::guillaume::entities::getIconFontSize(_size);
	}

	void FloatingActionButton::applyStyleState(void)
	{
		if (_isDisabled) {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(getDisabledContainerColor());

			if (_icon) {
				_icon->setColor(getDisabledContentColor());
			}

			if (_label) {
				_label->setColor(getDisabledContentColor());
			}

			return;
		}

		const auto baseContainerColor = getContainerColor(_variant, _lowered);

		utility::graphic::Color32Bit containerColor = baseContainerColor;

		if (isPressed()) {
			containerColor = ButtonBase::applyStateLayer(
				baseContainerColor,
				guillaume::getActiveScheme()
					.getColor(SchemeColorRole::OnPrimaryContainer)
					.getColor(),
				31U);
		} else if (isHovered()) {
			containerColor = ButtonBase::applyStateLayer(
				baseContainerColor,
				guillaume::getActiveScheme()
					.getColor(SchemeColorRole::OnPrimaryContainer)
					.getColor(),
				20U);
		}

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(containerColor);

		if (_icon) {
			_icon->setColor(getContentColor(_variant));
		}

		if (_label) {
			_label->setColor(getContentColor(_variant));
		}
	}

	void FloatingActionButton::applyGeometry(void)
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

	FloatingActionButton::FloatingActionButton(
		ecs::ComponentRegistry &registry, const std::string &iconGlyphName,
		const components::Glyph::Style &iconStyle,
		const std::string &labelContent, Variant variant, Size size,
		bool lowered, bool isDisabled, const std::string &accessibilityLabel,
		std::function<void(void)> onClick)
		: ButtonBase(registry, iconGlyphName, iconStyle, labelContent, false,
					 false, isDisabled, false, accessibilityLabel,
					 std::move(onClick))
		, _variant(variant)
		, _size(size)
		, _lowered(lowered)
	{
	}

	FloatingActionButton::~FloatingActionButton(void)
	{
	}

	FloatingActionButton &
		FloatingActionButton::setVariant(const Variant &variant)
	{
		_variant = variant;

		applyStyleState();

		return *this;
	}

	FloatingActionButton &FloatingActionButton::setSize(const Size &size)
	{
		_size = size;

		applyGeometry();

		return *this;
	}

	FloatingActionButton &FloatingActionButton::setLowered(const bool &lowered)
	{
		_lowered = lowered;

		applyStyleState();

		return *this;
	}

	FloatingActionButton::Variant FloatingActionButton::getVariant(void) const
	{
		return _variant;
	}

	FloatingActionButton::Size FloatingActionButton::getSize(void) const
	{
		return _size;
	}

	bool FloatingActionButton::isLowered(void) const
	{
		return _lowered;
	}
}	 // namespace guillaume::entities
