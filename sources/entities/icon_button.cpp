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

	namespace
	{
		using Role = SchemeColorRole;

		/// Metrics of one icon button size.
		struct SizeMetrics {
			float containerSize;	///< Container extent.
			float iconFontSize;		///< Icon font size.
		};

		/// Indexed by IconButton::Size.
		constexpr std::array<SizeMetrics, 3> sizeTable { {
			{ 32.0f, 18.0f },	 // Small
			{ 40.0f, 24.0f },	 // Medium
			{ 48.0f, 30.0f },	 // Large
		} };

		/// Paper v5 IconButton/utils.ts colors of one variant. No container
		/// role means transparent.
		struct Palette {
			std::optional<Role> activeContainer;	///< Container when active.
			std::optional<Role> idleContainer;		///< Container otherwise.
			Role activeContent;						///< Content when active.
			Role idleContent;						///< Content otherwise.
			/// True when a plain (non-toggle) button uses the active palette;
			/// otherwise only a selected button does.
			bool plainIsActive;
		};

		/// Indexed by IconButton::Variant.
		constexpr std::array<Palette, 4> paletteTable { {
			// Standard
			{ std::nullopt, std::nullopt, Role::Primary, Role::OnSurfaceVariant,
			  false },
			// Filled
			{ Role::Primary, Role::SurfaceVariant, Role::OnPrimary,
			  Role::Primary, true },
			// Tonal
			{ Role::SecondaryContainer, Role::SurfaceVariant,
			  Role::OnSecondaryContainer, Role::OnSurfaceVariant, true },
			// Outlined
			{ Role::InverseSurface, std::nullopt, Role::InverseOnSurface,
			  Role::OnSurfaceVariant, false },
		} };

		const SizeMetrics &metrics(IconButton::Size size)
		{
			const auto index = static_cast<std::size_t>(size);
			if (index >= sizeTable.size()) {
				throw std::runtime_error("Invalid icon button size");
			}
			return sizeTable[index];
		}

		const Palette &palette(IconButton::Variant variant)
		{
			const auto index = static_cast<std::size_t>(variant);
			if (index >= paletteTable.size()) {
				throw std::runtime_error("Invalid icon button variant");
			}
			return paletteTable[index];
		}

		bool isActive(const Palette &entry, bool isToggle, bool isSelected)
		{
			return isSelected || (entry.plainIsActive && !isToggle);
		}
	}	 // namespace

	static float getContainerSize(IconButton::Size size)
	{
		return metrics(size).containerSize;
	}

	static float getIconFontSize(IconButton::Size size)
	{
		return metrics(size).iconFontSize;
	}

	static float getBorderRadius(IconButton::Size size)
	{
		// Paper/M3 icon buttons are circular at every size.
		static_cast<void>(metrics(size));
		return getShapeRadius(theme::ShapeRole::Full);
	}

	static utility::graphic::Color32Bit
		getContainerColorFor(IconButton::Variant variant, bool isToggle,
							 bool isSelected)
	{
		const Palette &entry = palette(variant);
		const auto role		 = isActive(entry, isToggle, isSelected)
				 ? entry.activeContainer
				 : entry.idleContainer;

		return role ? getActiveColor(*role)
					: utility::graphic::Color32Bit(0, 0, 0, 0);
	}

	static utility::graphic::Color32Bit
		getContentColor(IconButton::Variant variant, bool isToggle,
						bool isSelected)
	{
		const Palette &entry = palette(variant);

		return getActiveColor(isActive(entry, isToggle, isSelected)
								  ? entry.activeContent
								  : entry.idleContent);
	}

	/**
	 * Blend a state layer; over a transparent container the layer is the
	 * content color at the layer opacity.
	 */
	static utility::graphic::Color32Bit
		stateLayerOver(const utility::graphic::Color32Bit &base,
					   const utility::graphic::Color32Bit &content,
					   theme::StateLayer layer)
	{
		if (base.getAlpha() == 0) {
			return withAlpha(content, getStateLayerAlpha(layer));
		}
		return stateLayer(base, content, layer);
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
		return resolveContentColor(
			getContentColor(_variant, _isToggle, _isSelected));
	}

	float IconButton::getIconFontSize(void) const
	{
		return ::guillaume::entities::getIconFontSize(_size);
	}

	void IconButton::applyStyleState(void)
	{
		if (isDisabled()) {
			getComponentRegistry()
				.getComponent<components::Color>(getIdentifier())
				.setColor(
					(_variant == Variant::Filled || _variant == Variant::Tonal)
						? getDisabledContainerColor()
						: transparentColor());

			getComponentRegistry()
				.getComponent<components::Borders>(getIdentifier())
				.setColor(_variant == Variant::Outlined
							  ? getDisabledContainerColor()
							  : transparentColor())
				.setWidth(1.0f);

			if (_icon) {
				_icon->setColor(getDisabledContentColor());
			}

			return;
		}

		const auto baseContainerColor = resolveContainerColor(
			getContainerColorFor(_variant, _isToggle, _isSelected));
		const auto contentColor = resolveContentColor(
			getContentColor(_variant, _isToggle, _isSelected));

		utility::graphic::Color32Bit containerColor = baseContainerColor;

		if (isPressed()) {
			containerColor = stateLayerOver(baseContainerColor, contentColor,
											theme::StateLayer::Pressed);
		} else if (isHovered()) {
			containerColor = stateLayerOver(baseContainerColor, contentColor,
											theme::StateLayer::Hovered);
		}

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(containerColor);

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setColor(getBorderColor(_variant, _isSelected))
			.setWidth(1.0f);

		if (_icon) {
			_icon->setColor(contentColor);
		}
	}

	void IconButton::applyGeometry(void)
	{
		const float containerSize = getContainerSize(_size);

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(containerSize)
			.setHeight(containerSize);
		applyTouchTarget(containerSize, containerSize);

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getBorderRadius(_size));

		if (_icon) {
			_icon->setFontSize(getIconFontSize());

			const auto &buttonPose =
				getComponentRegistry()
					.getComponent<components::Transform>(getIdentifier())
					.getPose();
			const auto &iconBound =
				getComponentRegistry().getComponent<components::Bound>(
					_icon->getIdentifier());

			const float iconX = buttonPose.getPosition().x
				+ (containerSize - iconBound.getWidth()) / 2.0f;
			const float iconY = buttonPose.getPosition().y
				+ (containerSize - iconBound.getHeight()) / 2.0f;

			const auto iconPose = ButtonBase::applyLayerToPosition(
				utility::graphic::PositionF(iconX, iconY,
											buttonPose.getPosition().z),
				buttonPose.getOrientation(),
				static_cast<std::uint32_t>(getLayer()));

			getComponentRegistry()
				.getComponent<components::Transform>(_icon->getIdentifier())
				.setPose(iconPose);
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
