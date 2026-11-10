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
		auto entity = std::make_shared<View>(
			this->getComponentRegistry(), _pose, _color, _borderRadius, _axis,
			_mainAxisAlignment, _crossAxisAlignment, _spacing, _padding,
			_hasFixedWidth, _fixedWidth, _hasFixedHeight, _fixedHeight,
			_entities);
		return entity;
	}

	void View::Builder::reset(void)
	{
		_pose				= utility::graphic::PoseF();
		_color				= { 0, 0, 0, 0 };	 // Transparent, like an RN View.
		_borderRadius		= 16.0f;
		_axis				= Axis::Horizontal;
		_mainAxisAlignment	= MainAxisAlignment::Start;
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

	std::shared_ptr<View> View::Director::makeDefaultView(
		Builder &builder, std::shared_ptr<Entity> parent,
		const utility::graphic::PoseF &pose,
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		return builder.withPose(pose).withEntities(entities).registerEntity(
			parent);
	}

	std::shared_ptr<View> View::Director::makeColorView(
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

	View::View(ecs::ComponentRegistry &registry,
			   const utility::graphic::PoseF &pose,
			   const utility::graphic::Color32Bit &color, float borderRadius,
			   Axis axis, MainAxisAlignment mainAxisAlignment,
			   CrossAxisAlignment crossAxisAlignment, float spacing,
			   float padding, bool hasFixedWidth, float fixedWidth,
			   bool hasFixedHeight, float fixedHeight,
			   const std::vector<std::shared_ptr<ecs::Entity>> &entities)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders>(registry)
		, _pose(pose)
		, _color(color)
		, _borderRadius(borderRadius)
		, _axis(axis)
		, _mainAxisAlignment(mainAxisAlignment)
		, _crossAxisAlignment(crossAxisAlignment)
		, _spacing(spacing)
		, _padding(padding)
		, _hasFixedWidth(hasFixedWidth)
		, _hasFixedHeight(hasFixedHeight)
		, _fixedWidth(fixedWidth)
		, _fixedHeight(fixedHeight)
		, _entities(entities)
	{
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(_pose);
	}

	View::~View()
	{
	}

	View &View::setPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(pose);

		applyGeometry();

		return *this;
	}

	View &View::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	View &View::setBorderRadius(float borderRadius)
	{
		_borderRadius = borderRadius;
		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(borderRadius);
		return *this;
	}

	View &View::setAxis(Axis axis)
	{
		_axis = axis;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setAxis(axis);

		applyGeometry();

		return *this;
	}

	View &View::setMainAxisAlignment(MainAxisAlignment alignment)
	{
		_mainAxisAlignment = alignment;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setMainAxisAlignment(alignment);

		applyGeometry();

		return *this;
	}

	View &View::setCrossAxisAlignment(CrossAxisAlignment alignment)
	{
		_crossAxisAlignment = alignment;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setCrossAxisAlignment(alignment);

		applyGeometry();

		return *this;
	}

	View &View::setSpacing(float spacing)
	{
		_spacing = spacing;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setSpacing(spacing);

		applyGeometry();

		return *this;
	}

	View &View::setPadding(float padding)
	{
		_padding = padding;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setPadding(padding);

		applyGeometry();

		return *this;
	}

	View &View::setFixedWidth(float width)
	{
		_hasFixedWidth = true;
		_fixedWidth	   = width;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setFixedWidth(width);

		applyGeometry();

		return *this;
	}

	View &View::clearFixedWidth(void)
	{
		_hasFixedWidth = false;
		_fixedWidth	   = 0.0f;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.clearFixedWidth();

		applyGeometry();

		return *this;
	}

	View &View::setFixedHeight(float height)
	{
		_hasFixedHeight = true;
		_fixedHeight	= height;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setFixedHeight(height);

		applyGeometry();

		return *this;
	}

	View &View::clearFixedHeight(void)
	{
		_hasFixedHeight = false;
		_fixedHeight	= 0.0f;
		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.clearFixedHeight();

		applyGeometry();

		return *this;
	}

	View &View::setEntities(
		const std::vector<std::shared_ptr<ecs::Entity>> &entities)
	{
		_entities = entities;

		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_entities.size());

		for (const auto &entity: _entities) {
			if (entity != nullptr
				&& entity->getIdentifier() != ecs::Entity::InvalidIdentifier) {
				entity->setParent(shared_from_this());
				childIdentifiers.push_back(entity->getIdentifier());
			}
		}

		getComponentRegistry()
			.getComponent<components::Layout>(getIdentifier())
			.setChildren(childIdentifiers);

		applyGeometry();

		return *this;
	}

	void View::initialize(void)
	{
		update();
	}

	void View::update(void)
	{
		// The framework may move the container through the Transform component
		// directly (Scene::placeEntitiesInFrontOfView), so the registry pose is
		// adopted instead of restoring the stale private copy.
		_pose = getComponentRegistry()
					.getComponent<components::Transform>(getIdentifier())
					.getPose();

		setColor(_color);
		setBorderRadius(_borderRadius);
		setAxis(_axis);
		setMainAxisAlignment(_mainAxisAlignment);
		setCrossAxisAlignment(_crossAxisAlignment);
		setSpacing(_spacing);
		setPadding(_padding);

		if (_hasFixedWidth) {
			setFixedWidth(_fixedWidth);
		} else {
			clearFixedWidth();
		}

		if (_hasFixedHeight) {
			setFixedHeight(_fixedHeight);
		} else {
			clearFixedHeight();
		}

		setEntities(_entities);
	}

	void View::applyGeometry(void)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_entities.size());

		for (const auto &entity: _entities) {
			if (entity == nullptr
				|| entity->getIdentifier() == ecs::Entity::InvalidIdentifier) {
				continue;
			}
			childIdentifiers.push_back(entity->getIdentifier());
		}

		systems::Layout::apply(getComponentRegistry(), getIdentifier(),
							   childIdentifiers,
							   static_cast<std::uint32_t>(getLayer()));
	}

}	 // namespace guillaume::entities
