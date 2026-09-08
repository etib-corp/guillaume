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

#include "guillaume/entities/extended_floating_action_button.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	ExtendedFloatingActionButton::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	ExtendedFloatingActionButton::Builder::~Builder(void)
	{
	}

	std::shared_ptr<ExtendedFloatingActionButton>
		ExtendedFloatingActionButton::Builder::registerEntity(
			std::shared_ptr<Entity> parent)
	{
		_fab = std::make_shared<ExtendedFloatingActionButton>(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _variant, _size, _lowered, _isDisabled,
			_accessibilityLabel, _onClick);
		_fab->setParent(parent);

		this->getEntityRegistry().addEntity(_fab);

		auto fabCopy = _fab;

		reset();

		return fabCopy;
	}

	void ExtendedFloatingActionButton::Builder::reset(void)
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

	ExtendedFloatingActionButton::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	ExtendedFloatingActionButton::Director::~Director(void)
	{
	}

	std::shared_ptr<ExtendedFloatingActionButton> ExtendedFloatingActionButton::
		Director::makeExtendedFloatingActionButton(
			Builder &builder, std::shared_ptr<Entity> parent,
			const std::string &labelContent, std::function<void(void)> onClick)
	{
		return builder.withLabel(labelContent)
			.withOnClick(onClick)
			.registerEntity(parent);
	}

	static float getContainerSize(ExtendedFloatingActionButton::Size size)
	{
		switch (size) {
			case ExtendedFloatingActionButton::Size::Small:
				return 40.0f;
			case ExtendedFloatingActionButton::Size::Medium:
				return 56.0f;
			case ExtendedFloatingActionButton::Size::Large:
				return 96.0f;
			default:
				throw std::runtime_error("Invalid extended FAB size");
		}
	}

	static float getFontSize(ExtendedFloatingActionButton::Size size)
	{
		switch (size) {
			case ExtendedFloatingActionButton::Size::Small:
				return 18.0f;
			case ExtendedFloatingActionButton::Size::Medium:
				return 24.0f;
			case ExtendedFloatingActionButton::Size::Large:
				return 36.0f;
			default:
				throw std::runtime_error("Invalid extended FAB size");
		}
	}

	static float getBorderRadius(ExtendedFloatingActionButton::Size size)
	{
		switch (size) {
			case ExtendedFloatingActionButton::Size::Small:
				return 20.0f;
			case ExtendedFloatingActionButton::Size::Medium:
				return 28.0f;
			case ExtendedFloatingActionButton::Size::Large:
				return 48.0f;
			default:
				throw std::runtime_error("Invalid extended FAB size");
		}
	}

	static utility::graphic::Color32Bit
		getContainerColor(ExtendedFloatingActionButton::Variant variant,
						  bool lowered)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (variant) {
			case ExtendedFloatingActionButton::Variant::Surface:
				return scheme
					.getColor(lowered ? SchemeColorRole::SurfaceContainerLow
									  : SchemeColorRole::SurfaceContainerHigh)
					.getColor();
			case ExtendedFloatingActionButton::Variant::Primary:
				return scheme.getColor(SchemeColorRole::PrimaryContainer)
					.getColor();
			case ExtendedFloatingActionButton::Variant::Secondary:
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case ExtendedFloatingActionButton::Variant::Tertiary:
				return scheme.getColor(SchemeColorRole::TertiaryContainer)
					.getColor();
			default:
				throw std::runtime_error("Invalid extended FAB variant");
		}
	}

	static utility::graphic::Color32Bit
		getContentColor(ExtendedFloatingActionButton::Variant variant)
	{
		const auto &scheme = guillaume::getActiveScheme();

		switch (variant) {
			case ExtendedFloatingActionButton::Variant::Surface:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case ExtendedFloatingActionButton::Variant::Primary:
				return scheme.getColor(SchemeColorRole::OnPrimaryContainer)
					.getColor();
			case ExtendedFloatingActionButton::Variant::Secondary:
				return scheme.getColor(SchemeColorRole::OnSecondaryContainer)
					.getColor();
			case ExtendedFloatingActionButton::Variant::Tertiary:
				return scheme.getColor(SchemeColorRole::OnTertiaryContainer)
					.getColor();
			default:
				throw std::runtime_error("Invalid extended FAB variant");
		}
	}

	utility::graphic::Color32Bit
		ExtendedFloatingActionButton::getInitialContentColor(void) const
	{
		return getContentColor(_variant);
	}

	float ExtendedFloatingActionButton::getIconFontSize(void) const
	{
		return ::guillaume::entities::getFontSize(_size);
	}

	void ExtendedFloatingActionButton::applyStyleState(void)
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

	void ExtendedFloatingActionButton::applyGeometry(void)
	{
		const float baseHeight = getContainerSize(_size);
		const float fontHeight = getFontSize(_size);

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getBorderRadius(_size));

		if (!_icon && !_label) {
			return;
		}

		if (_icon) {
			_icon->setFontSize(fontHeight);
		}

		if (_label) {
			_label->setFontSize(fontHeight);
		}

		const float heightPadding = (baseHeight - fontHeight) / 2.0f;
		const float gap			  = 8.0f;

		const auto &buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		if (_icon) {
			const float iconX = buttonPose.getPosition().x + heightPadding;
			const float iconY = buttonPose.getPosition().y + heightPadding;

			const auto iconPose = ButtonBase::applyLayerToPosition(
				utility::graphic::PositionF(iconX, iconY,
											buttonPose.getPosition().z),
				buttonPose.getOrientation(), getLayer());

			getComponentRegistry()
				.getComponent<components::Transform>(_icon->getIdentifier())
				.setPose(iconPose);
		}

		const float iconWidth = _icon
			? getComponentRegistry()
				  .getComponent<components::Bound>(_icon->getIdentifier())
				  .getWidth()
			: 0.0f;

		if (_label) {
			const float labelX = buttonPose.getPosition().x + heightPadding
				+ (iconWidth > 0.0f ? iconWidth + gap : 0.0f);
			const float labelY = buttonPose.getPosition().y + heightPadding;

			const auto labelPose = ButtonBase::applyLayerToPosition(
				utility::graphic::PositionF(labelX, labelY,
											buttonPose.getPosition().z),
				buttonPose.getOrientation(), getLayer());

			getComponentRegistry()
				.getComponent<components::Transform>(_label->getIdentifier())
				.setPose(labelPose);
		}

		const float labelWidth = _label
			? getComponentRegistry()
				  .getComponent<components::Bound>(_label->getIdentifier())
				  .getWidth()
			: 0.0f;

		float width = heightPadding * 2.0f;
		width += iconWidth;
		if (labelWidth > 0.0f) {
			width += (iconWidth > 0.0f ? gap : 0.0f) + labelWidth;
		}

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(width)
			.setHeight(baseHeight);
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
		, _variant(variant)
		, _size(size)
		, _lowered(lowered)
	{
	}

	ExtendedFloatingActionButton::~ExtendedFloatingActionButton(void)
	{
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setVariant(const Variant &variant)
	{
		_variant = variant;

		applyStyleState();

		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setSize(const Size &size)
	{
		_size = size;

		applyGeometry();

		return *this;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setLowered(const bool &lowered)
	{
		_lowered = lowered;

		applyStyleState();

		return *this;
	}

	ExtendedFloatingActionButton::Variant
		ExtendedFloatingActionButton::getVariant(void) const
	{
		return _variant;
	}

	ExtendedFloatingActionButton::Size
		ExtendedFloatingActionButton::getSize(void) const
	{
		return _size;
	}

	bool ExtendedFloatingActionButton::isLowered(void) const
	{
		return _lowered;
	}
}	 // namespace guillaume::entities
