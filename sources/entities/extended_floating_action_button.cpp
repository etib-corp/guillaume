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
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

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
		auto entity = std::make_shared<ExtendedFloatingActionButton>(
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
				return getShapeRadius(theme::ShapeRole::Medium);
			case ExtendedFloatingActionButton::Size::Medium:
				return getShapeRadius(theme::ShapeRole::Large);
			case ExtendedFloatingActionButton::Size::Large:
				return getShapeRadius(theme::ShapeRole::ExtraLarge);
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
				// Paper's surface FAB uses the tonal elevation.level3 color
				// (level1 when lowered).
				return getElevatedSurfaceColor(
					lowered ? theme::ElevationLevel::Level1
							: theme::ElevationLevel::Level3);
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
		return resolveContentColor(getContentColor(_variant));
	}

	float ExtendedFloatingActionButton::getIconFontSize(void) const
	{
		return ::guillaume::entities::getFontSize(_size);
	}

	void ExtendedFloatingActionButton::applyElevation(void)
	{
		// M3 FAB: level 3 at rest (1 when lowered), one level up on hover,
		// and flat when disabled.
		auto level = _lowered ? theme::ElevationLevel::Level1
							  : theme::ElevationLevel::Level3;
		if (isDisabled() || !isVisible() || _mode == Mode::Flat) {
			level = theme::ElevationLevel::Level0;
		} else if (isHovered() && !isPressed()) {
			level = static_cast<theme::ElevationLevel>(
				static_cast<std::size_t>(level) + 1);
		}
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(level);
	}

	void ExtendedFloatingActionButton::applyStyleState(void)
	{
		applyElevation();

		if (!isVisible()) {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(transparentColor());
			if (_icon) {
				_icon->setColor(transparentColor());
			}
			if (_label) {
				_label->setColor(transparentColor());
			}
			return;
		}

		if (isDisabled()) {
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

		const auto baseContainerColor =
			resolveContainerColor(getContainerColor(_variant, _lowered));
		const auto contentColor =
			resolveContentColor(getContentColor(_variant));

		utility::graphic::Color32Bit containerColor = baseContainerColor;

		if (isPressed()) {
			containerColor = stateLayer(baseContainerColor, contentColor,
										theme::StateLayer::Pressed);
		} else if (isHovered()) {
			containerColor = stateLayer(baseContainerColor, contentColor,
										theme::StateLayer::Hovered);
		}

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(containerColor);

		if (_icon) {
			_icon->setColor(contentColor);
		}

		if (_label) {
			_label->setColor(contentColor);
		}
	}

	void ExtendedFloatingActionButton::applyGeometry(void)
	{
		const float baseHeight = _customSize.value_or(getContainerSize(_size));
		const float fontHeight = getFontSize(_size);

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(_customSize.has_value() ? *_customSize / 3.5f
													 : getBorderRadius(_size));

		if (!_icon && !_label) {
			return;
		}

		if (_icon) {
			_icon->setFontSize(fontHeight);
		}

		if (_label) {
			_label->setTypeRole(theme::TypeRole::LabelLarge);
		}

		// M3 extended FAB: 16 dp leading, 12 dp icon-label gap, 20 dp
		// trailing; collapsed it is a square icon-only FAB.
		constexpr float leading	 = 16.0f;
		constexpr float gap		 = 12.0f;
		constexpr float trailing = 20.0f;

		const auto &buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		const auto sizeOf = [this](const std::shared_ptr<ecs::Entity> &child) {
			const auto &bound =
				getComponentRegistry().getComponent<components::Bound>(
					child->getIdentifier());
			return std::make_pair(bound.getWidth(), bound.getHeight());
		};

		const auto place = [&](const std::shared_ptr<ecs::Entity> &child,
							   float x, float height) {
			getComponentRegistry()
				.getComponent<components::Transform>(child->getIdentifier())
				.setPose(ButtonBase::applyLayerToPosition(
					utility::graphic::PositionF(buttonPose.getPosition().x + x,
												buttonPose.getPosition().y
													+ (baseHeight - height)
														/ 2.0f,
												buttonPose.getPosition().z),
					buttonPose.getOrientation(), getLayer()));
		};

		const auto [iconWidth, iconHeight] =
			_icon ? sizeOf(_icon) : std::make_pair(0.0f, 0.0f);
		const auto [labelWidth, labelHeight] =
			_label ? sizeOf(_label) : std::make_pair(0.0f, 0.0f);

		const bool showLabel = _isExtended && labelWidth > 0.0f;
		float width			 = baseHeight;

		if (showLabel) {
			width		 = leading + labelWidth + trailing;
			float cursor = leading;
			if (iconWidth > 0.0f) {
				place(_icon, cursor, iconHeight);
				cursor += iconWidth + gap;
				width += iconWidth + gap;
			}
			place(_label, cursor, labelHeight);
		} else {
			if (_icon) {
				place(_icon, (baseHeight - iconWidth) / 2.0f, iconHeight);
			}
			if (_label) {
				place(_label, (baseHeight - labelWidth) / 2.0f, labelHeight);
			}
		}

		if (_label) {
			// A collapsed FAB keeps its label child but hides it.
			const auto contentColor =
				resolveContentColor(getContentColor(_variant));
			if (!showLabel) {
				_label->setColor(transparentColor());
			} else if (!isDisabled() && isVisible()) {
				_label->setColor(contentColor);
			}
		}

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(width)
			.setHeight(baseHeight);
		applyTouchTarget(width, baseHeight);
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setExtended(bool extended)
	{
		_isExtended = extended;
		applyGeometry();
		return *this;
	}

	bool ExtendedFloatingActionButton::isExtended(void) const
	{
		return _isExtended;
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
		if (!getComponentRegistry().hasComponent<components::Elevation>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Elevation>(
				getIdentifier());
		}
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setMode(Mode mode)
	{
		_mode = mode;
		applyStyleState();
		return *this;
	}

	ExtendedFloatingActionButton::Mode
		ExtendedFloatingActionButton::getMode(void) const
	{
		return _mode;
	}

	ExtendedFloatingActionButton &
		ExtendedFloatingActionButton::setCustomSize(std::optional<float> size)
	{
		if (size.has_value() && *size <= 0.0f) {
			throw std::invalid_argument("FAB custom size must be positive");
		}
		_customSize = size;
		applyGeometry();
		return *this;
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
