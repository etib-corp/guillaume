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
#include "guillaume/systems/divider.hpp"

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
		auto entity =
			Divider::create(this->getComponentRegistry(), _pose, _color,
							_length, _thickness, _inset, _variant);
		return entity;
	}

	void Divider::Builder::reset(void)
	{
		_pose	   = utility::graphic::PoseF();
		_color	   = getActiveColor(SchemeColorRole::OutlineVariant);
		_length	   = 100.0f;
		_thickness = HairlineThickness;
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
		Divider::Builder::makeDivider(std::shared_ptr<Entity> parent,
									  float length, Variant variant)
	{
		return withLength(length).withVariant(variant).registerEntity(parent);
	}

	Divider::Divider(ecs::ComponentRegistry &registry,
					 const utility::graphic::PoseF &pose,
					 const utility::graphic::Color32Bit &color, float length,
					 float thickness, float inset, Variant variant)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Color, components::Line,
							components::Divider>(registry)
	{
		getComponentRegistry()
			.getComponent<components::Transform>(getIdentifier())
			.setPose(pose);

		config()
			.setOriginPose(pose)
			.setAppliedPose(pose)
			.setColor(color)
			.setLength(length)
			.setThickness(thickness)
			.setInset(inset)
			.setVariant(variant);

		apply();
	}

	Divider::~Divider(void)
	{
	}

	components::Divider &Divider::config(void)
	{
		return getComponentRegistry().getComponent<components::Divider>(
			getIdentifier());
	}

	const components::Divider &Divider::config(void) const
	{
		return getComponentRegistry().getComponent<components::Divider>(
			getIdentifier());
	}

	void Divider::apply(void)
	{
		systems::Divider::apply(getComponentRegistry(), getIdentifier());
	}

	Divider &Divider::setVariant(Variant variant)
	{
		config().setVariant(variant);
		apply();
		return *this;
	}

	Divider &Divider::setLength(float length)
	{
		config().setLength(length);
		apply();
		return *this;
	}

	Divider &Divider::setThickness(float thickness)
	{
		config().setThickness(thickness);
		apply();
		return *this;
	}

	Divider &Divider::setBold(bool bold)
	{
		return setThickness(bold ? BoldThickness : HairlineThickness);
	}

	bool Divider::isBold(void) const
	{
		return config().getThickness() >= BoldThickness;
	}

	Divider &Divider::setInset(float inset)
	{
		config().setInset(inset);
		apply();
		return *this;
	}

	Divider &Divider::setColor(const utility::graphic::Color32Bit &color)
	{
		config().setColor(color);
		apply();
		return *this;
	}

	Divider::Variant Divider::getVariant(void) const
	{
		return config().getVariant();
	}

}	 // namespace guillaume::entities
