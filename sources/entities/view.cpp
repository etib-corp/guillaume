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
#include <vector>

#include "guillaume/entities/view.hpp"
#include "guillaume/entities/builder_base.hpp"

#include "guillaume/systems/layout.hpp"

namespace guillaume::entities
{

	View::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<View>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<View> View::Builder::buildEntity(void)
	{
		auto entity = View::create(this->getComponentRegistry(), _pose, _color,
								   _borderRadius, _axis, _mainAxisAlignment,
								   _crossAxisAlignment, _spacing, _padding,
								   _hasFixedWidth, _fixedWidth, _hasFixedHeight,
								   _fixedHeight, _entities);
		return entity;
	}

	void View::Builder::reset(void)
	{
		_pose			   = utility::graphic::PoseF();
		_color			   = { 0, 0, 0, 0 };	// Transparent, like an RN View.
		_borderRadius	   = 16.0f;
		_axis			   = Axis::Horizontal;
		_mainAxisAlignment = MainAxisAlignment::Start;
		_crossAxisAlignment = CrossAxisAlignment::Center;
		_spacing			= 8.0f;
		_padding			= 16.0f;
		_hasFixedWidth		= false;
		_hasFixedHeight		= false;
		_fixedWidth			= 0.0f;
		_fixedHeight		= 0.0f;
		_entities.clear();
	}

	View::Builder &View::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		return *this;
	}

	View::Builder &
		View::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	View::Builder &View::Builder::withBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		return *this;
	}

	View::Builder &View::Builder::withAxis(Axis axis)
	{
		_axis = axis;
		return *this;
	}

	View::Builder &
		View::Builder::withMainAxisAlignment(MainAxisAlignment alignment)
	{
		_mainAxisAlignment = alignment;
		return *this;
	}

	View::Builder &
		View::Builder::withCrossAxisAlignment(CrossAxisAlignment alignment)
	{
		_crossAxisAlignment = alignment;
		return *this;
	}

	View::Builder &View::Builder::withSpacing(float spacing)
	{
		_spacing = spacing;
		return *this;
	}

	View::Builder &View::Builder::withPadding(float padding)
	{
		_padding = padding;
		return *this;
	}

	View::Builder &View::Builder::withFixedWidth(float width)
	{
		_hasFixedWidth = true;
		_fixedWidth	   = width;
		return *this;
	}

	View::Builder &View::Builder::withFixedHeight(float height)
	{
		_hasFixedHeight = true;
		_fixedHeight	= height;
		return *this;
	}

	View::Builder &View::Builder::withEntities(
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		_entities = entities;
		return *this;
	}

	std::shared_ptr<View> View::Builder::makeDefaultView(
		std::shared_ptr<Entity> parent, const utility::graphic::PoseF &pose,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		return withPose(pose).withEntities(entities).registerEntity(parent);
	}

	std::shared_ptr<View> View::Builder::makeColorView(
		std::shared_ptr<Entity> parent, const utility::graphic::PoseF &pose,
		const utility::graphic::Color32Bit &color,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		return withPose(pose)
			.withColor(color)
			.withEntities(entities)
			.registerEntity(parent);
	}

	std::shared_ptr<View> View::create(
		ecs::ComponentRegistry &registry, const utility::graphic::PoseF &pose,
		const utility::graphic::Color32Bit &color, float borderRadius,
		Axis axis, MainAxisAlignment mainAxisAlignment,
		CrossAxisAlignment crossAxisAlignment, float spacing, float padding,
		bool hasFixedWidth, float fixedWidth, bool hasFixedHeight,
		float fixedHeight,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		std::shared_ptr<View> entity(
			new View(registry, pose, color, borderRadius, axis,
					 mainAxisAlignment, crossAxisAlignment, spacing, padding,
					 hasFixedWidth, fixedWidth, hasFixedHeight, fixedHeight));

		// The view is now held by a shared pointer, so it can be given as
		// parent: setEntities() parents the entities and arranges them.
		entity->setEntities(entities);
		return entity;
	}

	View::View(ecs::ComponentRegistry &registry,
			   const utility::graphic::PoseF &pose,
			   const utility::graphic::Color32Bit &color, float borderRadius,
			   Axis axis, MainAxisAlignment mainAxisAlignment,
			   CrossAxisAlignment crossAxisAlignment, float spacing,
			   float padding, bool hasFixedWidth, float fixedWidth,
			   bool hasFixedHeight, float fixedHeight)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders>(registry)
	{
		const auto &identifier = getIdentifier();

		registry.getComponent<components::Transform>(identifier).setPose(pose);
		registry.getComponent<components::Color>(identifier).setColor(color);
		registry.getComponent<components::Borders>(identifier)
			.setBorderRadius(borderRadius);

		auto &layout = registry.getComponent<components::Layout>(identifier);
		layout.setAxis(axis)
			.setMainAxisAlignment(mainAxisAlignment)
			.setCrossAxisAlignment(crossAxisAlignment)
			.setSpacing(spacing)
			.setPadding(padding);

		if (hasFixedWidth) {
			layout.setFixedWidth(fixedWidth);
		}
		if (hasFixedHeight) {
			layout.setFixedHeight(fixedHeight);
		}
	}

	View::~View()
	{
	}

	View &View::setPose(const utility::graphic::PoseF &pose)
	{
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(pose);

		applyGeometry();

		return *this;
	}

	View &View::setColor(const utility::graphic::Color32Bit &color)
	{
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	View &View::setBorderRadius(float borderRadius)
	{
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(borderRadius);
		return *this;
	}

	View &View::setAxis(Axis axis)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(axis);

		applyGeometry();

		return *this;
	}

	View &View::setMainAxisAlignment(MainAxisAlignment alignment)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setMainAxisAlignment(alignment);

		applyGeometry();

		return *this;
	}

	View &View::setCrossAxisAlignment(CrossAxisAlignment alignment)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setCrossAxisAlignment(alignment);

		applyGeometry();

		return *this;
	}

	View &View::setSpacing(float spacing)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setSpacing(spacing);

		applyGeometry();

		return *this;
	}

	View &View::setPadding(float padding)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setPadding(padding);

		applyGeometry();

		return *this;
	}

	View &View::setFixedWidth(float width)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setFixedWidth(width);

		applyGeometry();

		return *this;
	}

	View &View::clearFixedWidth(void)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.clearFixedWidth();

		applyGeometry();

		return *this;
	}

	View &View::setFixedHeight(float height)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setFixedHeight(height);

		applyGeometry();

		return *this;
	}

	View &View::clearFixedHeight(void)
	{
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.clearFixedHeight();

		applyGeometry();

		return *this;
	}

	View &View::setEntities(
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(entities.size());

		for (const auto &entity: entities) {
			if (entity != nullptr
				&& entity->getIdentifier() != ecs::Entity::InvalidIdentifier) {
				entity->setParent(shared_from_this());
				childIdentifiers.push_back(entity->getIdentifier());
			}
		}

		// Record the layout children on the component so `systems::Layout`
		// arranges exactly these.
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setChildren(childIdentifiers);

		applyGeometry();

		return *this;
	}

	void View::applyGeometry(void)
	{
		auto &registry		   = getComponentRegistry();
		const auto &identifier = getIdentifier();

		systems::Layout::apply(
			registry, identifier,
			registry.getComponent<components::Layout>(identifier).getChildren(),
			systems::Layout::layerOf(registry, identifier));
	}

}	 // namespace guillaume::entities
