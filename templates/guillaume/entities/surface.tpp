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

#pragma once

#include "guillaume/entities/surface.hpp"
#include "guillaume/systems/layout.hpp"

namespace guillaume::entities
{

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...>::SurfaceBase(
		ecs::ComponentRegistry &registry, const SurfaceConfig &config)
		: ecs::ParentEntityFiller<components::Layout, components::Transform,
								  components::Bound, components::Color,
								  components::Borders, ExtraComponents...>(
			  registry)
	{
		setSurfaceConfig(config);
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &SurfaceBase<ExtraComponents...>::setPose(
		const utility::graphic::PoseF &pose)
	{
		this->getComponentRegistry()
			.template getComponent<components::Transform>(this->getIdentifier())
			.setPose(pose);
		applyGeometry();
		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &SurfaceBase<ExtraComponents...>::setColor(
		const utility::graphic::Color32Bit &color)
	{
		this->getComponentRegistry()
			.template getComponent<components::Color>(this->getIdentifier())
			.setColor(color);
		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &
		SurfaceBase<ExtraComponents...>::setBorderRadius(float radius)
	{
		this->getComponentRegistry()
			.template getComponent<components::Borders>(this->getIdentifier())
			.setBorderRadius(radius);
		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &
		SurfaceBase<ExtraComponents...>::setSurfaceConfig(
			const SurfaceConfig &config)
	{
		this->getComponentRegistry()
			.template getComponent<components::Transform>(this->getIdentifier())
			.setPose(config.pose);

		this->getComponentRegistry()
			.template getComponent<components::Color>(this->getIdentifier())
			.setColor(config.color);

		this->getComponentRegistry()
			.template getComponent<components::Borders>(this->getIdentifier())
			.setBorderRadius(config.borderRadius);

		auto &layout = this->getComponentRegistry()
						   .template getComponent<components::Layout>(
							   this->getIdentifier());
		layout.setAxis(config.axis)
			.setMainAxisAlignment(config.mainAxisAlignment)
			.setCrossAxisAlignment(config.crossAxisAlignment)
			.setSpacing(config.spacing)
			.setPadding(config.padding);

		if (config.hasFixedWidth) {
			layout.setFixedWidth(config.fixedWidth);
		} else {
			layout.clearFixedWidth();
		}

		if (config.hasFixedHeight) {
			layout.setFixedHeight(config.fixedHeight);
		} else {
			layout.clearFixedHeight();
		}

		applyGeometry();

		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceConfig SurfaceBase<ExtraComponents...>::getSurfaceConfig(void) const
	{
		const auto &registry   = this->getComponentRegistry();
		const auto &identifier = this->getIdentifier();
		const auto &layout =
			registry.template getComponent<components::Layout>(identifier);
		SurfaceConfig config;

		config.pose =
			registry.template getComponent<components::Transform>(identifier)
				.getPose();
		config.color =
			registry.template getComponent<components::Color>(identifier)
				.getColor();
		config.borderRadius =
			registry.template getComponent<components::Borders>(identifier)
				.getTopLeftRadius();
		config.axis				  = layout.getAxis();
		config.mainAxisAlignment  = layout.getMainAxisAlignment();
		config.crossAxisAlignment = layout.getCrossAxisAlignment();
		config.spacing			  = layout.getSpacing();
		config.padding			  = layout.getPadding();
		config.hasFixedWidth	  = layout.hasFixedWidth();
		config.fixedWidth		  = layout.getFixedWidth();
		config.hasFixedHeight	  = layout.hasFixedHeight();
		config.fixedHeight		  = layout.getFixedHeight();
		return config;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &
		SurfaceBase<ExtraComponents...>::setChildren(const Children &children)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(children.size());

		for (const auto &child: children) {
			if (child != nullptr
				&& child->getIdentifier() != ecs::Entity::InvalidIdentifier) {
				child->setParent(this->shared_from_this());
				childIdentifiers.push_back(child->getIdentifier());
			}
		}

		// Record the layout children on the component so `systems::Layout`
		// arranges exactly these, leaving manually positioned children alone.
		this->getComponentRegistry()
			.template getComponent<components::Layout>(this->getIdentifier())
			.setChildren(childIdentifiers);

		applyGeometry();

		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	std::vector<ecs::Entity::Identifier>
		SurfaceBase<ExtraComponents...>::getChildren(void) const
	{
		return this->getComponentRegistry()
			.template getComponent<components::Layout>(this->getIdentifier())
			.getChildren();
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	void SurfaceBase<ExtraComponents...>::applyGeometry(void)
	{
		const auto &identifier = this->getIdentifier();

		systems::Layout::apply(
			this->getComponentRegistry(), identifier,
			this->getComponentRegistry()
				.template getComponent<components::Layout>(identifier)
				.getChildren(),
			static_cast<std::uint32_t>(this->getLayer()));
	}

}	 // namespace guillaume::entities
