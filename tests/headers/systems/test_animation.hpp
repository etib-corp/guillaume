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

#include "guillaume/components/animation.hpp"

#include "guillaume/systems/animation.hpp"

#include "mocks/engine_mock.hpp"

#include <utility/engine.hpp>

namespace guillaume::systems::tests
{

	class TestAnimation: public ::testing::Test
	{
		protected:
		std::unique_ptr<utility::Engine> _engine;
		guillaume::tests::EngineMock *_engineMock { nullptr };
		std::unique_ptr<Animation> _animationSystem;
		ecs::ComponentRegistry _componentRegistry;

		TestAnimation(void)			  = default;
		~TestAnimation(void) override = default;

		void SetUp(void) override
		{
			_engine = std::make_unique<guillaume::tests::EngineMock>();
			_engineMock =
				static_cast<guillaume::tests::EngineMock *>(_engine.get());
			_animationSystem = std::make_unique<Animation>(_engine);
			_animationSystem->bindComponentRegistry(_componentRegistry);
		}

		void TearDown(void) override
		{
			_animationSystem->unbindComponentRegistry();
			_animationSystem.reset();
			_engine.reset();
		}
	};

}	 // namespace guillaume::systems::tests
