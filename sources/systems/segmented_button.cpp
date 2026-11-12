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

#include "guillaume/systems/segmented_button.hpp"

#include <algorithm>
#include <string>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/button.hpp"
#include "guillaume/components/common_button.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/systems/button.hpp"
#include "guillaume/systems/common_button.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::systems
{
	namespace
	{
		using SelectionMode = components::SegmentedButton::SelectionMode;

		/**
		 * @brief Whether a segment child is a button this system can drive.
		 * @param registry The registry holding the components.
		 * @param button The child identifier (may be invalid).
		 * @return True when it has a components::Button.
		 */
		bool isButton(const ecs::ComponentRegistry &registry,
					  ecs::Entity::Identifier button)
		{
			return registry.hasComponent<components::Button>(button);
		}
	}	 // namespace

	SegmentedButton::SegmentedButton(void)
		: ecs::SystemFiller<components::SegmentedButton>(ecs::Phase::Layout)
	{
	}

	void SegmentedButton::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Layout>(identifier)
			.setAxis(components::Layout::Axis::Horizontal)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(0.0f)
			.setPadding(0.0f);

		const auto &config =
			componentRegistry.getComponent<components::SegmentedButton>(
				identifier);
		const auto &buttons	 = config.getButtons();
		const auto &segments = config.getSegments();

		for (std::size_t index = 0; index < buttons.size(); ++index) {
			const auto button = buttons[index];
			if (!isButton(componentRegistry, button)) {
				continue;
			}

			componentRegistry.getComponent<components::Button>(button)
				.setToggle(true)
				.setOnClick([registry, identifier, index]() {
					segmentChanged(*registry, identifier, index);
				});

			if (index < segments.size()) {
				const auto &segment = segments[index];
				if (componentRegistry.hasComponent<components::StyleState>(
						button)) {
					componentRegistry
						.getComponent<components::StyleState>(button)
						.setEnabled(!segment.disabled);
				}
				if (!segment.accessibilityLabel.empty()
					&& componentRegistry
						   .hasComponent<components::Accessibility>(button)) {
					componentRegistry
						.getComponent<components::Accessibility>(button)
						.setLabel(segment.accessibilityLabel);
				}
			}

			Button::restyle(componentRegistry, button);
		}
	}

	bool SegmentedButton::isSelected(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index)
	{
		const auto &buttons =
			componentRegistry
				.getComponent<components::SegmentedButton>(entityIdentifier)
				.getButtons();

		return index < buttons.size()
			&& isButton(componentRegistry, buttons[index])
			&& componentRegistry
				   .getComponent<components::Button>(buttons[index])
				   .isSelected();
	}

	std::vector<std::size_t> SegmentedButton::selectedIndices(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const std::size_t count =
			componentRegistry
				.getComponent<components::SegmentedButton>(entityIdentifier)
				.getButtons()
				.size();
		std::vector<std::size_t> indices;

		for (std::size_t index = 0; index < count; ++index) {
			if (isSelected(componentRegistry, entityIdentifier, index)) {
				indices.push_back(index);
			}
		}
		return indices;
	}

	void
		SegmentedButton::select(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index)
	{
		const auto &config =
			componentRegistry.getComponent<components::SegmentedButton>(
				entityIdentifier);
		const auto &buttons = config.getButtons();

		if (index >= buttons.size()) {
			return;
		}

		if (config.getSelectionMode() == SelectionMode::Single) {
			for (std::size_t other = 0; other < buttons.size(); ++other) {
				if (isButton(componentRegistry, buttons[other])) {
					Button::setSelected(componentRegistry, buttons[other],
										other == index);
				}
			}
		} else if (isButton(componentRegistry, buttons[index])) {
			Button::setSelected(componentRegistry, buttons[index], true);
		}

		apply(componentRegistry, entityIdentifier);
	}

	void SegmentedButton::deselect(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index)
	{
		const auto &buttons =
			componentRegistry
				.getComponent<components::SegmentedButton>(entityIdentifier)
				.getButtons();

		if (index >= buttons.size()) {
			return;
		}

		if (isButton(componentRegistry, buttons[index])) {
			Button::setSelected(componentRegistry, buttons[index], false);
		}

		apply(componentRegistry, entityIdentifier);
	}

	void SegmentedButton::segmentChanged(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index)
	{
		const auto &config =
			componentRegistry.getComponent<components::SegmentedButton>(
				entityIdentifier);
		const auto &buttons = config.getButtons();

		if (index >= buttons.size()
			|| !isButton(componentRegistry, buttons[index])) {
			return;
		}

		if (config.getSelectionMode() == SelectionMode::Single) {
			// Paper single-select commits the pressed value: pressing the
			// selected segment keeps it selected instead of clearing it.
			if (!isSelected(componentRegistry, entityIdentifier, index)) {
				Button::setSelected(componentRegistry, buttons[index], true);
			}
			for (std::size_t other = 0; other < buttons.size(); ++other) {
				if (other != index
					&& isButton(componentRegistry, buttons[other])) {
					Button::setSelected(componentRegistry, buttons[other],
										false);
				}
			}
		}

		apply(componentRegistry, entityIdentifier);

		// Copy: the callback may replace itself.
		const auto onChange = config.getOnChange();
		if (onChange) {
			onChange(index);
		}
	}

	void SegmentedButton::setSegmentDisabled(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index,
		bool disabled)
	{
		auto &config =
			componentRegistry.getComponent<components::SegmentedButton>(
				entityIdentifier);
		const auto &buttons = config.getButtons();

		config.setSegmentDisabled(index, disabled);
		if (index < buttons.size()
			&& componentRegistry.hasComponent<components::StyleState>(
				buttons[index])) {
			componentRegistry
				.getComponent<components::StyleState>(buttons[index])
				.setEnabled(!disabled);
			if (isButton(componentRegistry, buttons[index])) {
				Button::restyle(componentRegistry, buttons[index]);
			}
		}

		// Restyling a button resets its corners: connect them again.
		apply(componentRegistry, entityIdentifier);
	}

	void SegmentedButton::apply(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &config =
			componentRegistry.getComponent<components::SegmentedButton>(
				entityIdentifier);
		const auto &buttons	 = config.getButtons();
		const auto &segments = config.getSegments();

		// A selected segment shows the check glyph, the others their icon.
		for (std::size_t index = 0; index < buttons.size(); ++index) {
			if (!isButton(componentRegistry, buttons[index])) {
				continue;
			}

			auto &button = componentRegistry.getComponent<components::Button>(
				buttons[index]);
			const std::string icon = button.isSelected()
				? std::string(SelectedGlyph)
				: (index < segments.size() ? segments[index].icon
										   : std::string());
			if (button.getIconGlyphName() != icon) {
				button.setIconGlyphName(icon);
				Button::syncContent(componentRegistry, buttons[index]);
				Button::restyle(componentRegistry, buttons[index]);
			}
		}

		// Equalize the segment widths (at least MinSegmentWidth).
		float width = components::SegmentedButton::MinSegmentWidth;
		for (const auto button: buttons) {
			if (componentRegistry.hasComponent<components::Bound>(button)) {
				width = std::max(
					width,
					componentRegistry.getComponent<components::Bound>(button)
						.getWidth());
			}
		}
		for (const auto button: buttons) {
			if (!componentRegistry.hasComponent<components::CommonButton>(
					button)) {
				continue;
			}

			auto &common =
				componentRegistry.getComponent<components::CommonButton>(
					button);
			if (common.getMinWidth() != width) {
				common.setMinWidth(width);
				Button::restyle(componentRegistry, button);
			}
		}

		// The segments are the children the Layout system places in a row.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setChildren(buttons);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));

		if (buttons.empty()) {
			return;
		}

		// Connected corners: only the outer corners of the row are rounded.
		const float radius =
			componentRegistry.getComponent<components::Bound>(entityIdentifier)
				.getHeight()
			* 0.5f;
		const std::size_t count = buttons.size();

		for (std::size_t index = 0; index < count; ++index) {
			if (!componentRegistry.hasComponent<components::CommonButton>(
					buttons[index])) {
				continue;
			}

			const bool isFirst = (index == 0);
			const bool isLast  = (index == count - 1);
			const components::CommonButton::Corners corners {
				isFirst ? radius : 0.0f, isLast ? radius : 0.0f,
				isLast ? radius : 0.0f, isFirst ? radius : 0.0f
			};

			// The segment's own system writes these to Borders; apply it now
			// so the result is visible at once.
			componentRegistry
				.getComponent<components::CommonButton>(buttons[index])
				.setCorners(corners);
			CommonButton::geometry(componentRegistry, buttons[index]);
		}
	}

	void
		SegmentedButton::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::SegmentedButton>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
