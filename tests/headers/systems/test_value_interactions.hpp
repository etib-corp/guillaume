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

#include <memory>

#include "guillaume/ecs/component_registry.hpp"

#include "guillaume/event/event_bus.hpp"

#include "guillaume/systems/drag.hpp"
#include "guillaume/systems/scroll.hpp"

namespace guillaume::systems::tests
{

	class TestValueInteractions: public ::testing::Test
	{
		protected:
		event::EventBus _eventBus;
		ecs::ComponentRegistry _componentRegistry;
		std::unique_ptr<Drag> _dragSystem;
		std::unique_ptr<Scroll> _scrollSystem;

		TestValueInteractions(void)			  = default;
		~TestValueInteractions(void) override = default;

		void SetUp(void) override
		{
			_dragSystem = std::make_unique<Drag>(_eventBus);
			_dragSystem->bindComponentRegistry(_componentRegistry);
			_scrollSystem = std::make_unique<Scroll>(_eventBus);
			_scrollSystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_dragSystem->unbindComponentRegistry();
			_scrollSystem->unbindComponentRegistry();
			_scrollSystem.reset();
			_dragSystem.reset();
		}
	};

}	 // namespace guillaume::systems::tests
