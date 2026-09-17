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

#include "guillaume/entities/container.hpp"

namespace guillaume::entities
{

	Container::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Container::Builder::~Builder(void)
	{
	}

	std::shared_ptr<Container>
		Container::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_container = std::make_shared<Container>(
			this->getComponentRegistry(), _pose, _color, _borderRadius,
			_padding, _spacing, _margin, _direction, _entities);
		_container->setParent(parent);

		this->getEntityRegistry().addEntity(_container);

		auto containerCopy =
			_container;	   // Create a copy of the shared pointer to return

		reset();

		return containerCopy;
	}

	void Container::Builder::reset(void)
	{
		_container.reset();
		_pose		  = utility::graphic::PoseF();
		_color		  = { 255, 255, 255, 255 };
		_borderRadius = 16.0f;
		_padding	  = 16.0f;
		_spacing	  = 8.0f;
		_margin		  = 0.0f;
		_direction	  = Direction::Row;
		_entities.clear();
	}

	Container::Builder &
		Container::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		return *this;
	}

	Container::Builder &
		Container::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Container::Builder &Container::Builder::withBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		return *this;
	}

	Container::Builder &Container::Builder::withPadding(float padding)
	{
		_padding = padding;
		return *this;
	}

	Container::Builder &Container::Builder::withSpacing(float spacing)
	{
		_spacing = spacing;
		return *this;
	}

	Container::Builder &Container::Builder::withMargin(float margin)
	{
		_margin = margin;
		return *this;
	}

	Container::Builder &Container::Builder::withDirection(Direction direction)
	{
		_direction = direction;
		return *this;
	}

	Container::Builder &Container::Builder::withEntities(
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		_entities = entities;
		return *this;
	}

	Container::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Container::Director::~Director(void)
	{
	}

	std::shared_ptr<Container> Container::Director::makeDefaultContainer(
		Builder &builder, std::shared_ptr<Entity> parent,
		const utility::graphic::PoseF &pose,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		return builder.withPose(pose).withEntities(entities).registerEntity(
			parent);
	}

	std::shared_ptr<Container> Container::Director::makeColorContainer(
		Builder &builder, std::shared_ptr<Entity> parent,
		const utility::graphic::PoseF &pose,
		const utility::graphic::Color32Bit &color,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		return builder.withPose(pose)
			.withColor(color)
			.withEntities(entities)
			.registerEntity(parent);
	}

	Container::Container(
		ecs::ComponentRegistry &registry, const utility::graphic::PoseF &pose,
		const utility::graphic::Color32Bit &color, float borderRadius,
		float padding, float spacing, float margin, Direction direction,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Color, components::Borders>(
			  registry)
		, _pose(pose)
		, _color(color)
		, _borderRadius(borderRadius)
		, _padding(padding)
		, _spacing(spacing)
		, _margin(margin)
		, _direction(direction)
		, _entities(entities)
	{
	}

	Container::~Container()
	{
	}

	Container &Container::setPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(pose);

		applyGeometry();

		return *this;
	}

	Container &Container::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	Container &Container::setBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(borderRadius);
		return *this;
	}

	Container &Container::setPadding(float padding)
	{
		_padding = padding;

		applyGeometry();

		return *this;
	}

	Container &Container::setSpacing(float spacing)
	{
		_spacing = spacing;

		applyGeometry();

		return *this;
	}

	Container &Container::setMargin(float margin)
	{
		_margin = margin;

		applyGeometry();

		return *this;
	}

	Container &Container::setDirection(Direction direction)
	{
		_direction = direction;

		applyGeometry();

		return *this;
	}

	Container &Container::setEntities(
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		_entities = entities;

		for (const auto &entity: _entities) {
			if (entity != nullptr) {
				entity->setParent(shared_from_this());
			}
		}

		applyGeometry();

		return *this;
	}

	void Container::initialize(void)
	{
		for (const auto &entity: _entities) {
			if (entity != nullptr) {
				entity->setParent(shared_from_this());
			}
		}
	}

	void Container::update(void)
	{
		// The framework may move the container through the Transform component
		// directly (Scene::placeEntitiesInFrontOfView), so the registry pose is
		// adopted instead of restoring the stale private copy. Restoring it
		// would snap the container back to its construction pose and re-lift
		// its children from a wrong depth every time a component changes.
		_pose = getComponentRegistry()
					.getComponent<components::Transform>(getIdentifier())
					.getPose();

		setColor(_color);
		setBorderRadius(_borderRadius);
		setPadding(_padding);
		setSpacing(_spacing);
		setMargin(_margin);
		setDirection(_direction);
		setEntities(_entities);
	}

	const utility::graphic::PoseF Container::applyLayerToPosition(
		const utility::graphic::PositionF &position,
		const utility::graphic::OrientationF &orientation,
		const std::uint32_t &layer)
	{
		const auto forwardVector					= orientation.getForward();
		utility::graphic::PositionF forwardPosition = position;

		forwardPosition.translate(utility::graphic::PositionF(
			-forwardVector * static_cast<float>(layer + 1) * _layerDepthStep));

		return utility::graphic::PoseF(forwardPosition, orientation);
	}

	void Container::applyGeometry(void)
	{
		const auto &containerPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();
		const auto &containerOrientation = containerPose.getOrientation();

		const float containerX = containerPose.getPosition().getX();
		const float containerY = containerPose.getPosition().getY();
		const float containerZ = containerPose.getPosition().getZ();

		// Cursor starts at the container origin plus the padding along the
		// main axis. The cross axis is centered within the padding.
		float cursor = _padding;

		float contentWidth	= 0.0f;
		float contentHeight = 0.0f;

		std::size_t placedCount = 0;

		for (const auto &entity: _entities) {
			if (entity == nullptr
				|| entity->getIdentifier() == ecs::Entity::InvalidIdentifier) {
				continue;
			}

			const auto &childBound =
				getComponentRegistry().getComponent<components::Bound>(
					entity->getIdentifier());

			auto &childTransform =
				getComponentRegistry().getComponent<components::Transform>(
					entity->getIdentifier());

			const auto childPose = childTransform.getPose();

			const float childWidth	= childBound.getWidth();
			const float childHeight = childBound.getHeight();

			float childX = containerX;
			float childY = containerY;

			if (_direction == Direction::Row) {
				childX = containerX + cursor;
				childY = containerY + _padding;

				cursor += childWidth + _spacing;

				contentWidth += childWidth;
				contentHeight = std::max(contentHeight, childHeight);
			} else {
				childX = containerX + _padding;
				childY = containerY + cursor;

				cursor += childHeight + _spacing;

				contentWidth = std::max(contentWidth, childWidth);
				contentHeight += childHeight;
			}

			++placedCount;

			// Anchor the child's depth to the container plane and lift it
			// toward the camera by its layer depth step so it never z-fights
			// with the container surface.
			const utility::graphic::PositionF childBasePosition(childX, childY,
																containerZ);

			const auto layeredPose = applyLayerToPosition(
				childBasePosition, containerOrientation, getLayer());

			utility::graphic::PoseF positionedPose = childPose;
			positionedPose.setPosition(layeredPose.getPosition());
			childTransform.setPose(positionedPose);
		}

		if (placedCount > 1) {
			const float totalSpacing =
				_spacing * static_cast<float>(placedCount - 1);
			if (_direction == Direction::Row) {
				contentWidth += totalSpacing;
			} else {
				contentHeight += totalSpacing;
			}
		}

		const float width  = contentWidth + 2.0f * (_padding + _margin);
		const float height = contentHeight + 2.0f * (_padding + _margin);

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(width)
			.setHeight(height);
	}

}	 // namespace guillaume::entities
