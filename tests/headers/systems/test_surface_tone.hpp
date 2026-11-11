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

#include <gtest/gtest.h>

#include <guillaume/components/color.hpp>
#include <guillaume/components/elevation.hpp>
#include <guillaume/components/surface_tone.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/systems/surface_tone.hpp>

namespace guillaume::systems::tests
{
	/**
	 * @brief Fixture driving systems::SurfaceTone on a bare entity
	 * identifier, without any entity object.
	 */
	class TestSurfaceToneSystem: public ::testing::Test
	{
		protected:
		ecs::ComponentRegistry _registry;			///< Component storage.
		ecs::Entity::Identifier _entity { 1 };		///< Entity under test.
		guillaume::systems::SurfaceTone _system;	///< System under test.

		void SetUp(void) override
		{
			_registry.addComponent<components::SurfaceTone>(_entity);
			_registry.addComponent<components::Color>(_entity);
			_registry.addComponent<components::Elevation>(_entity);
			_system.bindComponentRegistry(_registry);
		}

		void TearDown(void) override
		{
			_system.unbindComponentRegistry();
		}
	};

}	 // namespace guillaume::systems::tests
