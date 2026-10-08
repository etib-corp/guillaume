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
		, _surfaceConfig(config)
		, _children()
	{
		this->getComponentRegistry()
			.template getComponent<components::Transform>(this->getIdentifier())
			.setPose(_surfaceConfig.pose);
		setSurfaceConfig(_surfaceConfig);
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &SurfaceBase<ExtraComponents...>::setPose(
		const utility::graphic::PoseF &pose)
	{
		_surfaceConfig.pose = pose;
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
		_surfaceConfig.color = color;
		this->getComponentRegistry()
			.template getComponent<components::Color>(this->getIdentifier())
			.setColor(color);
		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &
		SurfaceBase<ExtraComponents...>::setBorderRadius(float radius)
	{
		_surfaceConfig.borderRadius = radius;
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
		_surfaceConfig = config;

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

	template<ecs::InheritFromComponent... ExtraComponents> const SurfaceConfig &
		SurfaceBase<ExtraComponents...>::getSurfaceConfig(void) const
	{
		return _surfaceConfig;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	SurfaceBase<ExtraComponents...> &
		SurfaceBase<ExtraComponents...>::setChildren(const Children &children)
	{
		_children = children;

		for (const auto &child: _children) {
			if (child != nullptr) {
				child->setParent(this->shared_from_this());
			}
		}

		applyGeometry();

		return *this;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	const typename SurfaceBase<ExtraComponents...>::Children &
		SurfaceBase<ExtraComponents...>::getChildren(void) const
	{
		return _children;
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	void SurfaceBase<ExtraComponents...>::initialize(void)
	{
		update();
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	void SurfaceBase<ExtraComponents...>::update(void)
	{
		// The framework may move the surface through the Transform component
		// directly (Scene::placeEntitiesInFrontOfView), so the registry pose is
		// adopted instead of restoring the stale private copy.
		_surfaceConfig.pose = this->getComponentRegistry()
								  .template getComponent<components::Transform>(
									  this->getIdentifier())
								  .getPose();

		setSurfaceConfig(_surfaceConfig);
		setChildren(_children);
	}

	template<ecs::InheritFromComponent... ExtraComponents>
	void SurfaceBase<ExtraComponents...>::applyGeometry(void)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_children.size());

		for (const auto &child: _children) {
			if (child == nullptr
				|| child->getIdentifier() == ecs::Entity::InvalidIdentifier) {
				continue;
			}
			childIdentifiers.push_back(child->getIdentifier());
		}

		systems::Layout::apply(this->getComponentRegistry(),
							   this->getIdentifier(), childIdentifiers,
							   static_cast<std::uint32_t>(this->getLayer()));
	}

}	 // namespace guillaume::entities
