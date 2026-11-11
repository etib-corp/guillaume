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
#include "systems/test_surface_tone.hpp"

#include <guillaume/components/bound.hpp>
#include <guillaume/components/hierarchy.hpp>
#include <guillaume/components/layout.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/systems/layout.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::systems::tests
{
	TEST_F(TestSurfaceToneSystem, RunsInTheMeasurePhase)
	{
		EXPECT_EQ(_system.getPhase(), ecs::Phase::Measure);
	}

	TEST_F(TestSurfaceToneSystem, ResolvesColorAndShadowFromTheElevation)
	{
		_registry.getComponent<components::SurfaceTone>(_entity).setElevation(
			theme::ElevationLevel::Level3);

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level3));
		const auto &elevation =
			_registry.getComponent<components::Elevation>(_entity);
		EXPECT_EQ(elevation.getElevationLevel(), theme::ElevationLevel::Level3);
		EXPECT_TRUE(elevation.hasShadow());
	}

	TEST_F(TestSurfaceToneSystem, WithoutShadowKeepsTheTonalColor)
	{
		_registry.getComponent<components::SurfaceTone>(_entity)
			.setElevation(theme::ElevationLevel::Level3)
			.setShadow(false);

		_system.update(_entity);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  getElevatedSurfaceColor(theme::ElevationLevel::Level3));
		EXPECT_FALSE(
			_registry.getComponent<components::Elevation>(_entity).hasShadow());
	}

	TEST_F(TestSurfaceToneSystem, AnUnchangedSurfaceIsNotMarkedChanged)
	{
		_system.update(_entity);
		_registry.resetChangedFlags();

		_system.update(_entity);

		EXPECT_FALSE(_registry.hasChanged(_entity));
	}

	TEST_F(TestSurfaceToneSystem, FollowsASchemeChange)
	{
		_system.update(_entity);

		setActiveScheme(ThemeSchemeRole::Dark);
		_system.update(_entity);
		const auto expected =
			getElevatedSurfaceColor(theme::ElevationLevel::Level1);
		setActiveScheme(ThemeSchemeRole::Light);

		EXPECT_EQ(_registry.getComponent<components::Color>(_entity).getColor(),
				  expected);
	}

	TEST_F(TestSurfaceToneSystem, LayoutReadsTheLayerFromTheHierarchy)
	{
		const ecs::Entity::Identifier container = 10;
		const ecs::Entity::Identifier child		= 11;
		guillaume::systems::Layout layout;

		_registry.addComponent<components::Layout>(container).setChildren(
			{ child });
		_registry.addComponent<components::Transform>(container);
		_registry.addComponent<components::Bound>(container);
		_registry.addComponent<components::Hierarchy>(container);
		_registry.addComponent<components::Transform>(child);
		_registry.addComponent<components::Bound>(child)
			.setWidth(10.0f)
			.setHeight(10.0f);
		layout.bindComponentRegistry(_registry);

		layout.update(container);
		const auto shallow =
			_registry.getComponent<components::Transform>(child)
				.getPose()
				.getPosition();

		_registry.getComponent<components::Hierarchy>(container).setLayer(3);
		layout.update(container);
		const auto deep = _registry.getComponent<components::Transform>(child)
							  .getPose()
							  .getPosition();
		layout.unbindComponentRegistry();

		EXPECT_NE(shallow.getZ(), deep.getZ());
	}

}	 // namespace guillaume::systems::tests
