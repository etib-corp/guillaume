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

#include "guillaume/entities/toolbar.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	Toolbar::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Toolbar>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Toolbar> Toolbar::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Toolbar>(this->getComponentRegistry(),
												_config, _variant, _children);
		return entity;
	}

	void Toolbar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Docked;

		_config.axis = components::Layout::Axis::Horizontal;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.spacing = 8.0f;
		_config.padding = 16.0f;

		_children.clear();
	}

	Toolbar::Builder &
		Toolbar::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Toolbar::Builder &Toolbar::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Toolbar::Builder &Toolbar::Builder::withSpacing(float spacing)
	{
		_config.spacing = spacing;
		return *this;
	}

	Toolbar::Builder &Toolbar::Builder::withPadding(float padding)
	{
		_config.padding = padding;
		return *this;
	}

	Toolbar::Builder &Toolbar::Builder::withFixedWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	Toolbar::Builder &Toolbar::Builder::withHeight(float height)
	{
		_config.hasFixedHeight = true;
		_config.fixedHeight	   = height;
		return *this;
	}

	Toolbar::Builder &Toolbar::Builder::withChildren(
		const std::vector<std::shared_ptr<ecs::Entity>> &children)
	{
		_children = children;
		return *this;
	}

	std::shared_ptr<Toolbar> Toolbar::Director::makeToolbar(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant)
	{
		return builder.withVariant(variant).registerEntity(parent);
	}

	Toolbar::Toolbar(ecs::ComponentRegistry &registry,
					 const SurfaceConfig &config, Variant variant,
					 const std::vector<std::shared_ptr<ecs::Entity>> &children)
		: SurfaceBase<>(registry, config)
		, _variant(variant)
		, _children(children)
	{
		applyVariant();
	}

	Toolbar::~Toolbar(void)
	{
	}

	void Toolbar::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis = components::Layout::Axis::Horizontal;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;

		if (_variant == Variant::Floating) {
			config.borderRadius = 28.0f;
			setSurfaceConfig(config);
			setColor(schemeColor(SchemeColorRole::SurfaceContainerHigh));
		} else {
			config.borderRadius = 0.0f;
			setSurfaceConfig(config);
			setColor(schemeColor(SchemeColorRole::SurfaceContainer));
		}
	}

	Toolbar &Toolbar::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	Toolbar::Variant Toolbar::getVariant(void) const
	{
		return _variant;
	}

	Toolbar &Toolbar::setChildren(
		const std::vector<std::shared_ptr<ecs::Entity>> &children)
	{
		_children = children;
		SurfaceBase::setChildren(_children);
		return *this;
	}

	void Toolbar::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		setChildren(_children);
	}

	void Toolbar::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		setChildren(_children);
	}

}	 // namespace guillaume::entities
