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

#include "guillaume/entities/panel.hpp"

namespace guillaume::entities
{

	Panel::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Panel::Builder::~Builder(void)
	{
	}

	std::shared_ptr<Panel>
		Panel::Builder::registerEntity(std::shared_ptr<Entity> parent)
	{
		_panel =
			std::make_shared<Panel>(this->getComponentRegistry(), _pose, _color,
									_borderRadius, _padding, _entities);
		_panel->setParent(parent);

		this->getEntityRegistry().addEntity(_panel);

		auto panelCopy =
			_panel;	   // Create a copy of the shared pointer to return

		reset();

		return panelCopy;
	}

	void Panel::Builder::reset(void)
	{
		_panel.reset();
		_pose		  = utility::graphic::PoseF();
		_color		  = { 255, 255, 255, 255 };
		_borderRadius = 16.0f;
		_padding	  = 16.0f;
		_entities.clear();
	}

	Panel::Builder &
		Panel::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		return *this;
	}

	Panel::Builder &
		Panel::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Panel::Builder &Panel::Builder::withBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		return *this;
	}

	Panel::Builder &Panel::Builder::withPadding(float padding)
	{
		_padding = padding;
		return *this;
	}

	Panel::Builder &Panel::Builder::withEntities(
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		_entities = entities;
		return *this;
	}

	Panel::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Panel::Director::~Director(void)
	{
	}

	std::shared_ptr<Panel> Panel::Director::makeDefaultPanel(
		Builder &builder, std::shared_ptr<Entity> parent,
		const utility::graphic::PoseF &pose,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		return builder.withPose(pose).withEntities(entities).registerEntity(
			parent);
	}

	std::shared_ptr<Panel> Panel::Director::makeColorPanel(
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

	Panel::Panel(ecs::ComponentRegistry &registry,
				 const utility::graphic::PoseF &pose,
				 const utility::graphic::Color32Bit &color, float borderRadius,
				 float padding,
				 const std::vector<std::shared_ptr<ecs::Entity>> &entities)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Color, components::Borders>(
			  registry)
		, _pose(pose)
		, _color(color)
		, _borderRadius(borderRadius)
		, _padding(padding)
		, _entities(entities)
	{
	}

	Panel::~Panel()
	{
	}

	Panel &Panel::setPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(pose);

		applyGeometry();

		return *this;
	}

	Panel &Panel::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	Panel &Panel::setBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(borderRadius);
		return *this;
	}

	Panel &Panel::setPadding(float padding)
	{
		_padding = padding;

		applyGeometry();

		return *this;
	}

	Panel &Panel::setEntities(
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

	void Panel::initialize(void)
	{
		for (const auto &entity: _entities) {
			if (entity != nullptr) {
				entity->setParent(shared_from_this());
			}
		}
	}

	void Panel::update(void)
	{
		setPose(_pose);
		setColor(_color);
		setBorderRadius(_borderRadius);
		setPadding(_padding);
		setEntities(_entities);
	}

	const utility::graphic::PoseF Panel::applyLayerToPosition(
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

	void Panel::applyGeometry(void)
	{
		const auto &panelPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();
		const auto &panelOrientation = panelPose.getOrientation();

		const float panelX = panelPose.getPosition().getX();
		const float panelY = panelPose.getPosition().getY();
		const float panelZ = panelPose.getPosition().getZ();

		float maxRight	= 0.0f;
		float maxBottom = 0.0f;

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

			// Keep the child's own placement but anchor its depth to the panel
			// plane and lift it toward the camera by its layer depth step so it
			// never z-fights with the panel surface.
			const utility::graphic::PositionF childBasePosition(
				childPose.getPosition().getX(), childPose.getPosition().getY(),
				panelZ);

			const auto layeredPose = applyLayerToPosition(
				childBasePosition, panelOrientation, getLayer());

			utility::graphic::PoseF positionedPose = childPose;
			positionedPose.setPosition(layeredPose.getPosition());
			childTransform.setPose(positionedPose);

			// Measure the panel size from the children sizes.
			const float right =
				childPose.getPosition().getX() - panelX + childBound.getWidth();
			const float bottom = childPose.getPosition().getY() - panelY
				+ childBound.getHeight();

			maxRight  = std::max(maxRight, right);
			maxBottom = std::max(maxBottom, bottom);
		}

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(maxRight + _padding)
			.setHeight(maxBottom + _padding);
	}

}	 // namespace guillaume::entities
