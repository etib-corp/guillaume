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

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <utility>

#include "guillaume/entities/floating_action_button.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/fab_style.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"
#include "guillaume/systems/floating_action_button.hpp"

namespace guillaume::entities
{
	FloatingActionButton::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<FloatingActionButton>(componentRegistry,
												  entityRegistry)
	{
		reset();
	}

	std::shared_ptr<FloatingActionButton>
		FloatingActionButton::Builder::buildEntity(void)
	{
		auto entity = FloatingActionButton::create(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _variant, _size, _lowered, _isDisabled,
			_accessibilityLabel, _onClick);
		return entity;
	}

	void FloatingActionButton::Builder::reset(void)
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

	std::shared_ptr<FloatingActionButton>
		FloatingActionButton::Builder::makeFloatingActionButton(
			std::shared_ptr<Entity> parent, const std::string &iconGlyphName,
			std::function<void(void)> onClick, Variant variant, Size size)
	{
		return withIcon(iconGlyphName)
			.withOnClick(onClick)
			.withVariant(variant)
			.withSize(size)
			.registerEntity(parent);
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
	{
		getComponentRegistry()
			.addComponent<components::FloatingActionButton>(getIdentifier())
			.setVariant(variant)
			.setSize(size)
			.setLowered(lowered);
		setSignature(
			getSignature()
			| getSignatureFromTypes<components::FloatingActionButton>());
		systems::FloatingActionButton::install(getComponentRegistry(),
											   getIdentifier());
	}

	float FloatingActionButton::getIconFontSize(void) const
	{
		return systems::FloatingActionButton::iconFontSize(
			getComponentRegistry(), getIdentifier());
	}

	utility::graphic::Color32Bit
		FloatingActionButton::getInitialContentColor(void) const
	{
		return systems::FloatingActionButton::contentColor(
			getComponentRegistry(), getIdentifier());
	}

	FloatingActionButton &FloatingActionButton::setMode(Mode mode)
	{
		getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.setMode(mode);
		restyle();
		return *this;
	}

	FloatingActionButton &
		FloatingActionButton::setCustomSize(std::optional<float> size)
	{
		if (size.has_value() && *size <= 0.0f) {
			throw std::invalid_argument("FAB custom size must be positive");
		}
		getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.setCustomSize(size);
		restyle();
		return *this;
	}

	FloatingActionButton &
		FloatingActionButton::setVariant(const Variant &variant)
	{
		getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.setVariant(variant);
		restyle();
		return *this;
	}

	FloatingActionButton &FloatingActionButton::setSize(const Size &size)
	{
		getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.setSize(size);
		restyle();
		return *this;
	}

	FloatingActionButton &FloatingActionButton::setLowered(const bool &lowered)
	{
		getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.setLowered(lowered);
		restyle();
		return *this;
	}

	FloatingActionButton::Mode FloatingActionButton::getMode(void) const
	{
		return getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.getMode();
	}

	FloatingActionButton::Variant FloatingActionButton::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.getVariant();
	}

	FloatingActionButton::Size FloatingActionButton::getSize(void) const
	{
		return getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.getSize();
	}

	bool FloatingActionButton::isLowered(void) const
	{
		return getComponentRegistry()
			.getComponent<components::FloatingActionButton>(getIdentifier())
			.isLowered();
	}

}	 // namespace guillaume::entities
