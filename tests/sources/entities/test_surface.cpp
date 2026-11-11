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

#include "entities/test_surface.hpp"

#include <cstddef>
#include <memory>

#include <guillaume/components/borders.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/entity_registry_container.hpp>
#include <guillaume/systems/surface_tone.hpp>
#include <guillaume/theme.hpp>

namespace guillaume::entities::tests
{
	namespace
	{
		std::shared_ptr<Surface>
			makeSurface(ecs::ComponentRegistry &registry,
						theme::ElevationLevel level,
						Surface::Mode mode = Surface::Mode::Elevated)
		{
			ecs::EntityRegistryContainer &entityRegistry =
				scratchEntityRegistry();
			Surface::Builder builder(registry, entityRegistry);

			return builder.makeSurface(nullptr, level, mode);
		}
	}	 // namespace

	TEST_F(TestSurface, DefaultsToElevatedLevelOne)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer &entityRegistry = scratchEntityRegistry();
		Surface::Builder builder(registry, entityRegistry);
		auto surface = builder.registerEntity(nullptr);

		EXPECT_EQ(surface->getElevation(), theme::ElevationLevel::Level1);
		EXPECT_EQ(surface->getMode(), Surface::Mode::Elevated);
		EXPECT_EQ(
			registry.getComponent<components::Color>(surface->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level1));
		EXPECT_TRUE(
			registry
				.getComponent<components::Elevation>(surface->getIdentifier())
				.hasShadow());
	}

	TEST_F(TestSurface, BackgroundFollowsEveryElevationToken)
	{
		ecs::ComponentRegistry registry;
		for (std::size_t index = 0;
			 index < static_cast<std::size_t>(theme::ElevationLevel::Count);
			 ++index) {
			const auto level = static_cast<theme::ElevationLevel>(index);
			auto surface	 = makeSurface(registry, level);

			EXPECT_EQ(
				registry
					.getComponent<components::Color>(surface->getIdentifier())
					.getColor(),
				getElevatedSurfaceColor(level))
				<< index;
			const auto &elevation =
				registry.getComponent<components::Elevation>(
					surface->getIdentifier());
			EXPECT_EQ(elevation.getElevationLevel(), level) << index;
			EXPECT_EQ(elevation.hasShadow(),
					  level != theme::ElevationLevel::Level0)
				<< index;
		}
	}

	TEST_F(TestSurface, FlatModeKeepsTintWithoutShadow)
	{
		ecs::ComponentRegistry registry;
		auto surface = makeSurface(registry, theme::ElevationLevel::Level3,
								   Surface::Mode::Flat);

		EXPECT_EQ(
			registry.getComponent<components::Color>(surface->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level3));
		EXPECT_FALSE(
			registry
				.getComponent<components::Elevation>(surface->getIdentifier())
				.hasShadow());

		surface->setMode(Surface::Mode::Elevated);
		EXPECT_EQ(
			registry
				.getComponent<components::Elevation>(surface->getIdentifier())
				.getElevationLevel(),
			theme::ElevationLevel::Level3);
	}

	TEST_F(TestSurface, SetElevationRestylesAndSchemeSwitchUpdates)
	{
		ecs::ComponentRegistry registry;
		auto surface = makeSurface(registry, theme::ElevationLevel::Level1);

		surface->setElevation(theme::ElevationLevel::Level5);
		EXPECT_EQ(
			registry.getComponent<components::Color>(surface->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level5));

		// The entity holds no behavior: the system restyles it each frame.
		setActiveScheme(ThemeSchemeRole::Dark);
		systems::SurfaceTone::apply(registry, surface->getIdentifier());
		EXPECT_EQ(
			registry.getComponent<components::Color>(surface->getIdentifier())
				.getColor(),
			getElevatedSurfaceColor(theme::ElevationLevel::Level5));
		setActiveScheme(ThemeSchemeRole::Light);
	}

	TEST_F(TestSurface, ShapeRoleSetsCornerRadius)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer &entityRegistry = scratchEntityRegistry();
		Surface::Builder builder(registry, entityRegistry);
		auto surface =
			builder.withShape(theme::ShapeRole::Large).registerEntity(nullptr);

		EXPECT_FLOAT_EQ(
			registry.getComponent<components::Borders>(surface->getIdentifier())
				.getTopLeftRadius(),
			getShapeRadius(theme::ShapeRole::Large));
	}

	TEST_F(TestSurface, BuilderPresetBuildsAndRegisters)
	{
		ecs::ComponentRegistry registry;
		ecs::EntityRegistryContainer entityRegistry;
		auto parent = std::make_shared<ecs::Entity>();
		Surface::Builder builder(registry, entityRegistry);

		auto surface =
			builder.makeSurface(parent, theme::ElevationLevel::Level2);
		ASSERT_NE(surface, nullptr);
		EXPECT_EQ(surface->getParent(), parent);
		EXPECT_EQ(surface->getElevation(), theme::ElevationLevel::Level2);
	}

}	 // namespace guillaume::entities::tests
