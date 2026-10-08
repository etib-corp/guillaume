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

#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/builder_base.hpp"

#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Divider::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Divider>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Divider> Divider::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Divider>(this->getComponentRegistry(),
												_pose, _color, _length,
												_thickness, _inset, _variant);
		return entity;
	}

	void Divider::Builder::reset(void)
	{
		_pose  = utility::graphic::PoseF();
		_color = guillaume::getActiveScheme()
					 .getColor(SchemeColorRole::OutlineVariant)
					 .getColor();
		_length	   = 100.0f;
		_thickness = 1.0f;
		_inset	   = 16.0f;
		_variant   = Variant::FullWidth;
	}

	Divider::Builder &
		Divider::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_pose = pose;
		return *this;
	}

	Divider::Builder &
		Divider::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	Divider::Builder &Divider::Builder::withLength(float length)
	{
		_length = length;
		return *this;
	}

	Divider::Builder &Divider::Builder::withThickness(float thickness)
	{
		_thickness = thickness;
		return *this;
	}

	Divider::Builder &Divider::Builder::withInset(float inset)
	{
		_inset = inset;
		return *this;
	}

	Divider::Builder &Divider::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	std::shared_ptr<Divider>
		Divider::Director::makeDivider(Builder &builder,
									   std::shared_ptr<Entity> parent,
									   float length, Variant variant)
	{
		return builder.withLength(length).withVariant(variant).registerEntity(
			parent);
	}

	Divider::Divider(ecs::ComponentRegistry &registry,
					 const utility::graphic::PoseF &pose,
					 const utility::graphic::Color32Bit &color, float length,
					 float thickness, float inset, Variant variant)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Color, components::Line>(registry)
		, _pose(pose)
		, _color(color)
		, _length(length)
		, _thickness(thickness)
		, _inset(inset)
		, _variant(variant)
	{
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(_pose);

		applyGeometry();
	}

	Divider::~Divider(void)
	{
	}

	float Divider::getLeadingInset(void) const
	{
		switch (_variant) {
			case Variant::Inset:
			case Variant::Middle:
				return _inset;
			case Variant::FullWidth:
			default:
				return 0.0f;
		}
	}

	float Divider::getTrailingInset(void) const
	{
		switch (_variant) {
			case Variant::Middle:
				return _inset;
			case Variant::Inset:
			case Variant::FullWidth:
			default:
				return 0.0f;
		}
	}

	void Divider::applyGeometry(void)
	{
		const float leading	 = getLeadingInset();
		const float trailing = getTrailingInset();
		const float width	 = _length - leading - trailing;

		auto dividerPose = _pose;
		dividerPose.setPosition(utility::graphic::PositionF(
			_pose.getPosition().getX() + leading, _pose.getPosition().getY(),
			_pose.getPosition().getZ()));

		_appliedPose = dividerPose;

		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(dividerPose);

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(width > 0.0f ? width : 0.0f)
			.setHeight(_thickness);

		getComponentRegistry()
			.getComponent<components::Line>(getIdentifier())
			.setThickness(_thickness);

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(_color);
	}

	Divider &Divider::setVariant(Variant variant)
	{
		_variant = variant;
		applyGeometry();
		return *this;
	}

	Divider &Divider::setLength(float length)
	{
		_length = length;
		applyGeometry();
		return *this;
	}

	Divider &Divider::setThickness(float thickness)
	{
		_thickness = thickness;
		applyGeometry();
		return *this;
	}

	Divider &Divider::setInset(float inset)
	{
		_inset = inset;
		applyGeometry();
		return *this;
	}

	Divider &Divider::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	Divider::Variant Divider::getVariant(void) const
	{
		return _variant;
	}

	void Divider::initialize(void)
	{
		update();
	}

	void Divider::update(void)
	{
		const auto registryPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		// Detect an external move (e.g. Scene placing the entity) and
		// recompute the origin pose, since `applyGeometry` shifts the entity
		// by the leading inset.
		if (registryPose != _appliedPose) {
			const float leading = getLeadingInset();
			_pose				= registryPose;
			_pose.setPosition(utility::graphic::PositionF(
				registryPose.getPosition().getX() - leading,
				registryPose.getPosition().getY(),
				registryPose.getPosition().getZ()));
		}

		applyGeometry();
	}

}	 // namespace guillaume::entities
