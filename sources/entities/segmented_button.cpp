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
#include <utility>

#include "guillaume/entities/segmented_button.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/button.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::entities
{
	SegmentedButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<SegmentedButton>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<SegmentedButton> SegmentedButton::Builder::buildEntity(void)
	{
		if (!_segmentConfigs.empty()) {
			return std::make_shared<SegmentedButton>(
				this->getComponentRegistry(), _segmentConfigs, _selectionMode,
				_onChange);
		}
		auto entity = std::make_shared<SegmentedButton>(
			this->getComponentRegistry(), _labels, _selectionMode, _onChange);
		return entity;
	}

	void SegmentedButton::Builder::reset(void)
	{
		_labels.clear();
		_segmentConfigs.clear();
		_selectionMode = SelectionMode::Single;
		_onChange	   = {};
	}

	SegmentedButton::Builder &SegmentedButton::Builder::withLabels(
		const std::vector<std::string> &labels)
	{
		_labels = labels;
		return *this;
	}

	SegmentedButton::Builder &SegmentedButton::Builder::withSelectionMode(
		const SelectionMode &selectionMode)
	{
		_selectionMode = selectionMode;
		return *this;
	}

	SegmentedButton::Builder &SegmentedButton::Builder::withSegments(
		const std::vector<Segment> &segments)
	{
		_segmentConfigs = segments;
		return *this;
	}

	SegmentedButton::Builder &SegmentedButton::Builder::withOnChange(
		std::function<void(std::size_t)> onChange)
	{
		_onChange = std::move(onChange);
		return *this;
	}

	std::shared_ptr<SegmentedButton>
		SegmentedButton::Builder::makeSegmentedButton(
			std::shared_ptr<Entity> parent,
			const std::vector<std::string> &labels, SelectionMode selectionMode,
			std::function<void(std::size_t)> onChange)
	{
		return withLabels(labels)
			.withSelectionMode(selectionMode)
			.withOnChange(onChange)
			.registerEntity(parent);
	}

	void SegmentedButton::segmentChangeHandler(std::size_t index)
	{
		if (index >= _segments.size()) {
			return;
		}

		bool nowSelected = _segments[index]->isSelected();

		// Paper single-select commits the pressed value: pressing the
		// selected segment keeps it selected instead of clearing it.
		if (_selectionMode == SelectionMode::Single && !nowSelected) {
			_segments[index]->setSelected(true);
			nowSelected = true;
		}

		if (_selectionMode == SelectionMode::Single && nowSelected) {
			for (std::size_t i = 0; i < _segments.size(); ++i) {
				if (i != index) {
					_segments[i]->setSelected(false);
				}
			}
		}

		applySegmentIcons();
		layoutSegments();

		if (_onChange) {
			_onChange(index);
		}
	}

	void SegmentedButton::applySegmentIcons(void)
	{
		for (std::size_t i = 0; i < _segments.size(); ++i) {
			const std::string icon =
				i < _segmentConfigs.size() ? _segmentConfigs[i].icon : "";
			_segments[i]->setIconGlyphName(_segments[i]->isSelected() ? "check"
																	  : icon);
		}
	}

	void SegmentedButton::layoutSegments(void)
	{
		// Equalize the segment widths (at least MinSegmentWidth).
		float width = MinSegmentWidth;
		for (const auto &segment: _segments) {
			width = std::max(
				width,
				getComponentRegistry()
					.getComponent<components::Bound>(segment->getIdentifier())
					.getWidth());
		}
		for (auto &segment: _segments) {
			if (auto button = std::dynamic_pointer_cast<Button>(segment)) {
				button->setMinWidth(width);
			}
		}

		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_segments.size());

		for (const auto &segment: _segments) {
			childIdentifiers.push_back(segment->getIdentifier());
		}

		systems::Layout::apply(getComponentRegistry(), getIdentifier(),
							   childIdentifiers,
							   static_cast<std::uint32_t>(getLayer()));

		if (_segments.empty()) {
			return;
		}

		const float height =
			getComponentRegistry()
				.getComponent<components::Bound>(getIdentifier())
				.getHeight();
		const float radius		= height * 0.5f;
		const std::size_t count = _segments.size();

		for (std::size_t i = 0; i < count; ++i) {
			const auto segmentId = _segments[i]->getIdentifier();
			auto &borders =
				getComponentRegistry().getComponent<components::Borders>(
					segmentId);

			const bool isFirst = (i == 0);
			const bool isLast  = (i == count - 1);

			borders.setTopLeftRadius(isFirst ? radius : 0.0f);
			borders.setBottomLeftRadius(isFirst ? radius : 0.0f);
			borders.setTopRightRadius(isLast ? radius : 0.0f);
			borders.setBottomRightRadius(isLast ? radius : 0.0f);
		}
	}

	SegmentedButton::SegmentedButton(ecs::ComponentRegistry &registry,
									 const std::vector<std::string> &labels,
									 SelectionMode selectionMode,
									 std::function<void(std::size_t)> onChange)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Layout>(registry)
		, _labels(labels)
		, _selectionMode(selectionMode)
		, _segments()
		, _onChange(std::move(onChange))
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(components::Layout::Axis::Horizontal)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(0.0f)
			.setPadding(0.0f);
	}

	SegmentedButton::SegmentedButton(ecs::ComponentRegistry &registry,
									 const std::vector<Segment> &segments,
									 SelectionMode selectionMode,
									 std::function<void(std::size_t)> onChange)
		: SegmentedButton(registry, std::vector<std::string> {}, selectionMode,
						  std::move(onChange))
	{
		_segmentConfigs = segments;
		for (const auto &segment: segments) {
			_labels.push_back(segment.label);
		}
	}

	SegmentedButton &SegmentedButton::setSegmentDisabled(std::size_t index,
														 bool disabled)
	{
		if (index < _segments.size()) {
			_segments[index]->setDisabled(disabled);
		}
		if (index < _segmentConfigs.size()) {
			_segmentConfigs[index].disabled = disabled;
		}
		return *this;
	}

	SegmentedButton::~SegmentedButton(void)
	{
	}

	SegmentedButton &SegmentedButton::select(std::size_t index)
	{
		if (index >= _segments.size()) {
			return *this;
		}

		if (_selectionMode == SelectionMode::Single) {
			for (std::size_t i = 0; i < _segments.size(); ++i) {
				_segments[i]->setSelected(i == index);
			}
		} else {
			_segments[index]->setSelected(true);
		}

		applySegmentIcons();
		layoutSegments();

		return *this;
	}

	SegmentedButton &SegmentedButton::deselect(std::size_t index)
	{
		if (index >= _segments.size()) {
			return *this;
		}

		_segments[index]->setSelected(false);

		applySegmentIcons();
		layoutSegments();

		return *this;
	}

	bool SegmentedButton::isSelected(std::size_t index) const
	{
		if (index >= _segments.size()) {
			return false;
		}

		return _segments[index]->isSelected();
	}

	std::vector<std::size_t> SegmentedButton::getSelectedIndices(void) const
	{
		std::vector<std::size_t> indices;

		for (std::size_t i = 0; i < _segments.size(); ++i) {
			if (_segments[i]->isSelected()) {
				indices.push_back(i);
			}
		}

		return indices;
	}

	std::size_t SegmentedButton::getSegmentCount(void) const
	{
		return _segments.size();
	}

	ecs::Entity::Identifier
		SegmentedButton::getSegmentIdentifier(std::size_t index) const
	{
		if (index >= _segments.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _segments[index]->getIdentifier();
	}

	void SegmentedButton::initialize(void)
	{
		Button::Builder buttonBuilder(getComponentRegistry(), *this);

		for (std::size_t i = 0; i < _labels.size(); ++i) {
			auto segment = buttonBuilder.makeButton(
				shared_from_this(), _labels[i],
				std::bind(&SegmentedButton::segmentChangeHandler, this, i),
				Button::Color::Segmented, Button::Shape::Square,
				Button::Size::Medium, false);

			segment->setIsToggle(true);

			if (i < _segmentConfigs.size()) {
				const auto &config = _segmentConfigs[i];
				segment->setIconGlyphName(config.icon);
				segment->setDisabled(config.disabled);
				if (!config.accessibilityLabel.empty()) {
					segment->setAccessibilityLabel(config.accessibilityLabel);
				}
			}

			_segments.push_back(segment);
		}

		applySegmentIcons();
	}

	void SegmentedButton::update(void)
	{
		for (auto &segment: _segments) {
			segment->update();
		}

		layoutSegments();
	}
}	 // namespace guillaume::entities
