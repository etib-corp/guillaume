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
#include "guillaume/systems/segmented_button.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief Turn plain labels into segment configurations.
		 * @param labels The segment labels.
		 * @return One default configuration per label.
		 */
		std::vector<SegmentedButton::Segment>
			segmentsFromLabels(const std::vector<std::string> &labels)
		{
			std::vector<SegmentedButton::Segment> segments;

			segments.reserve(labels.size());
			for (const auto &label: labels) {
				segments.push_back(
					SegmentedButton::Segment { label, {}, false, {} });
			}
			return segments;
		}
	}	 // namespace

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

	SegmentedButton::SegmentedButton(ecs::ComponentRegistry &registry,
									 const std::vector<std::string> &labels,
									 SelectionMode selectionMode,
									 std::function<void(std::size_t)> onChange)
		: SegmentedButton(registry, segmentsFromLabels(labels), selectionMode,
						  std::move(onChange))
	{
	}

	SegmentedButton::SegmentedButton(ecs::ComponentRegistry &registry,
									 const std::vector<Segment> &segments,
									 SelectionMode selectionMode,
									 std::function<void(std::size_t)> onChange)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Layout,
								  components::SegmentedButton>(registry)
	{
		getComponentRegistry()
			.getComponent<components::SegmentedButton>(getIdentifier())
			.setSegments(segments)
			.setSelectionMode(selectionMode)
			.setOnChange(std::move(onChange));
		systems::SegmentedButton::install(getComponentRegistry(),
										  getIdentifier());
	}

	SegmentedButton &SegmentedButton::setSegmentDisabled(std::size_t index,
														 bool disabled)
	{
		systems::SegmentedButton::setSegmentDisabled(
			getComponentRegistry(), getIdentifier(), index, disabled);
		return *this;
	}

	SegmentedButton::~SegmentedButton(void)
	{
	}

	SegmentedButton &SegmentedButton::select(std::size_t index)
	{
		systems::SegmentedButton::select(getComponentRegistry(),
										 getIdentifier(), index);
		return *this;
	}

	SegmentedButton &SegmentedButton::deselect(std::size_t index)
	{
		systems::SegmentedButton::deselect(getComponentRegistry(),
										   getIdentifier(), index);
		return *this;
	}

	bool SegmentedButton::isSelected(std::size_t index) const
	{
		return systems::SegmentedButton::isSelected(getComponentRegistry(),
													getIdentifier(), index);
	}

	std::vector<std::size_t> SegmentedButton::getSelectedIndices(void) const
	{
		return systems::SegmentedButton::selectedIndices(getComponentRegistry(),
														 getIdentifier());
	}

	std::size_t SegmentedButton::getSegmentCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::SegmentedButton>(getIdentifier())
			.getButtons()
			.size();
	}

	ecs::Entity::Identifier
		SegmentedButton::getSegmentIdentifier(std::size_t index) const
	{
		const auto &buttons =
			getComponentRegistry()
				.getComponent<components::SegmentedButton>(getIdentifier())
				.getButtons();

		return index < buttons.size() ? buttons[index]
									  : ecs::Entity::InvalidIdentifier;
	}

	void SegmentedButton::initialize(void)
	{
		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const std::vector<Segment> segments =
			registry.getComponent<components::SegmentedButton>(getIdentifier())
				.getSegments();

		std::vector<ecs::Entity::Identifier> identifiers;
		Button::Builder buttonBuilder(registry, *this);
		for (const auto &segment: segments) {
			// systems::SegmentedButton::install wires the click and applies
			// the segment options.
			const auto button = buttonBuilder.makeButton(
				shared_from_this(), segment.label, std::function<void(void)>(),
				Button::Color::Segmented, Button::Shape::Square,
				Button::Size::Medium, false);
			identifiers.push_back(button->getIdentifier());
		}

		registry.getComponent<components::SegmentedButton>(getIdentifier())
			.setButtons(identifiers);
		systems::SegmentedButton::install(registry, getIdentifier());
		systems::SegmentedButton::apply(registry, getIdentifier());
	}
}	 // namespace guillaume::entities
