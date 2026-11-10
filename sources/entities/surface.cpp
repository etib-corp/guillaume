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

#include "guillaume/theme.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{
	SurfaceConfig &SurfaceConfig::setPose(const utility::graphic::PoseF &value)
	{
		pose = value;
		return *this;
	}

	SurfaceConfig &
		SurfaceConfig::setColor(const utility::graphic::Color32Bit &value)
	{
		color = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setBorderRadius(float value)
	{
		borderRadius = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setAxis(components::Layout::Axis value)
	{
		axis = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setMainAxisAlignment(
		components::Layout::MainAxisAlignment value)
	{
		mainAxisAlignment = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setCrossAxisAlignment(
		components::Layout::CrossAxisAlignment value)
	{
		crossAxisAlignment = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setSpacing(float value)
	{
		spacing = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setPadding(float value)
	{
		padding = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setFixedWidth(float value)
	{
		hasFixedWidth = true;
		fixedWidth	  = value;
		return *this;
	}

	SurfaceConfig &SurfaceConfig::setFixedHeight(float value)
	{
		hasFixedHeight = true;
		fixedHeight	   = value;
		return *this;
	}

	SurfaceConfig SurfaceConfig::panel(float width,
									   components::Layout::Axis axis,
									   float spacing)
	{
		SurfaceConfig config;

		config.axis = axis;
		config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Center;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		config.spacing		 = spacing;
		config.padding		 = 24.0f;
		config.borderRadius	 = getShapeRadius(theme::ShapeRole::ExtraLarge);
		config.hasFixedWidth = true;
		config.fixedWidth	 = width;

		return config;
	}

}	 // namespace guillaume::entities

namespace guillaume::entities
{
	Surface::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							  ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Surface>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Surface> Surface::Builder::buildEntity(void)
	{
		return std::make_shared<Surface>(this->getComponentRegistry(), _config,
										 _elevation, _mode);
	}

	void Surface::Builder::reset(void)
	{
		_config				 = SurfaceConfig();
		_elevation			 = theme::ElevationLevel::Level1;
		_mode				 = Mode::Elevated;
		_config.borderRadius = getShapeRadius(theme::ShapeRole::None);
		_config.axis		 = components::Layout::Axis::Vertical;
	}

	Surface::Builder &
		Surface::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Surface::Builder &
		Surface::Builder::withElevation(theme::ElevationLevel elevation)
	{
		_elevation = elevation;
		return *this;
	}

	Surface::Builder &Surface::Builder::withMode(Mode mode)
	{
		_mode = mode;
		return *this;
	}

	Surface::Builder &Surface::Builder::withShape(theme::ShapeRole role)
	{
		_config.borderRadius = getShapeRadius(role);
		return *this;
	}

	Surface::Builder &Surface::Builder::withConfig(const SurfaceConfig &config)
	{
		const auto pose = _config.pose;
		_config			= config;
		_config.pose	= pose;
		return *this;
	}

	std::shared_ptr<Surface> Surface::Director::makeSurface(
		Builder &builder, std::shared_ptr<ecs::Entity> parent,
		theme::ElevationLevel elevation, Mode mode)
	{
		return builder.withElevation(elevation).withMode(mode).registerEntity(
			parent);
	}

	Surface::Surface(ecs::ComponentRegistry &registry,
					 const SurfaceConfig &config,
					 theme::ElevationLevel elevation, Mode mode)
		: SurfaceBase<components::Elevation>(registry, config)
		, _elevation(elevation)
		, _mode(mode)
	{
		applyElevation();
	}

	void Surface::applyElevation(void)
	{
		setColor(getElevatedSurfaceColor(_elevation));
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(_mode == Mode::Elevated
								   ? _elevation
								   : theme::ElevationLevel::Level0);
	}

	Surface &Surface::setElevation(theme::ElevationLevel elevation)
	{
		_elevation = elevation;
		applyElevation();
		return *this;
	}

	theme::ElevationLevel Surface::getElevation(void) const
	{
		return _elevation;
	}

	Surface &Surface::setMode(Mode mode)
	{
		_mode = mode;
		applyElevation();
		return *this;
	}

	Surface::Mode Surface::getMode(void) const
	{
		return _mode;
	}

	void Surface::update(void)
	{
		SurfaceBase::update();
		applyElevation();
	}

}	 // namespace guillaume::entities
