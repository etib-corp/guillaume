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

#include "guillaume/entities/segmented_button.hpp"
#include "guillaume/entities/button.hpp"

namespace guillaume::entities
{
	SegmentedButton::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									  ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	SegmentedButton::Builder::~Builder(void)
	{
	}

	std::shared_ptr<SegmentedButton>
		SegmentedButton::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_segmentedButton = std::make_shared<SegmentedButton>(
			this->getComponentRegistry(), _labels, _selectionMode, _onChange);
		_segmentedButton->setParent(parent);

		this->getEntityRegistry().addEntity(_segmentedButton);

		auto segmentedButtonCopy = _segmentedButton;

		reset();

		return segmentedButtonCopy;
	}

	void SegmentedButton::Builder::reset(void)
	{
		_segmentedButton.reset();
		_labels.clear();
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

	SegmentedButton::Builder &SegmentedButton::Builder::withOnChange(
		std::function<void(std::size_t)> onChange)
	{
		_onChange = std::move(onChange);
		return *this;
	}

	SegmentedButton::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	SegmentedButton::Director::~Director(void)
	{
	}

	std::shared_ptr<SegmentedButton>
		SegmentedButton::Director::makeSegmentedButton(
			Builder &builder, std::shared_ptr<Entity> parent,
			const std::vector<std::string> &labels, SelectionMode selectionMode,
			std::function<void(std::size_t)> onChange)
	{
		return builder.withLabels(labels)
			.withSelectionMode(selectionMode)
			.withOnChange(onChange)
			.registerEntity(parent);
	}

	void SegmentedButton::segmentChangeHandler(std::size_t index)
	{
		if (index >= _segments.size()) {
			return;
		}

		const bool nowSelected = _segments[index]->isSelected();

		if (_selectionMode == SelectionMode::Single && nowSelected) {
			for (std::size_t i = 0; i < _segments.size(); ++i) {
				if (i != index) {
					_segments[i]->setSelected(false);
				}
			}
		}

		layoutSegments();

		if (_onChange) {
			_onChange(index);
		}
	}

	void SegmentedButton::layoutSegments(void)
	{
		const auto segmentedPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		auto &selfBound =
			getComponentRegistry().getComponent<components::Bound>(
				getIdentifier());

		float offsetX = 0.0f;
		float height  = 0.0f;

		for (std::size_t i = 0; i < _segments.size(); ++i) {
			const auto segmentId = _segments[i]->getIdentifier();

			const auto &bound =
				getComponentRegistry().getComponent<components::Bound>(
					segmentId);
			const float width = bound.getWidth();

			height = bound.getHeight() > height ? bound.getHeight() : height;

			utility::graphic::PoseF segmentPose = segmentedPose;
			segmentPose.setPosition(utility::graphic::PositionF(
				segmentedPose.getPosition().getX() + offsetX,
				segmentedPose.getPosition().getY(),
				segmentedPose.getPosition().getZ()));

			getComponentRegistry()
				.getComponent<components::Transform>(segmentId)
				.setPose(segmentPose);

			offsetX += width;
		}

		selfBound.setWidth(offsetX).setHeight(height);

		if (_segments.empty()) {
			return;
		}

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
		: ecs::ParentEntityFiller<components::Transform, components::Bound>(
			  registry)
		, _labels(labels)
		, _selectionMode(selectionMode)
		, _segments()
		, _onChange(std::move(onChange))
	{
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

		layoutSegments();

		return *this;
	}

	SegmentedButton &SegmentedButton::deselect(std::size_t index)
	{
		if (index >= _segments.size()) {
			return *this;
		}

		_segments[index]->setSelected(false);

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
		Button::Director buttonDirector;

		for (std::size_t i = 0; i < _labels.size(); ++i) {
			auto segment = buttonDirector.makeButton(
				buttonBuilder, shared_from_this(), _labels[i],
				std::bind(&SegmentedButton::segmentChangeHandler, this, i),
				Button::Color::Segmented, Button::Shape::Square,
				Button::Size::Medium, false);

			segment->setIsToggle(true);

			_segments.push_back(segment);
		}
	}

	void SegmentedButton::update(void)
	{
		for (auto &segment: _segments) {
			segment->update();
		}

		layoutSegments();
	}
}	 // namespace guillaume::entities
