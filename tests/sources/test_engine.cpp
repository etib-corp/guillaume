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

#include "test_engine.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace guillaume::tests
{
	/**
	 * @brief Exposes the protected Engine state so the concrete, device-free
	 * surface of guillaume::Engine can be exercised without a rendering
	 * backend.
	 */
	class EngineProbe: public guillaume::Engine
	{
		public:
		EngineProbe(void)			= default;
		~EngineProbe(void) override = default;

		using guillaume::Engine::getEventCallback;

		void clear(void) override
		{
		}
		void present(void) override
		{
		}
		size_t addMesh(const utility::graphic::Mesh &,
					   const std::string &) override
		{
			return 0u;
		}
		bool removeObject(size_t) override
		{
			return true;
		}
		utility::graphic::SizeF
			measureText(const utility::graphic::Text &) const override
		{
			return {};
		}
		size_t addText(utility::graphic::Text) override
		{
			return 0u;
		}
		size_t addModel(std::shared_ptr<utility::graphic::Model>) override
		{
			return 0u;
		}
		utility::graphic::ViewF getView(void) const override
		{
			return {};
		}
		void addScene(size_t) override
		{
		}
		void pollEvents(void) override
		{
		}
		void update(void) override
		{
		}
	};

	TEST_F(TestEngine, CapturesViewportInputByDefault)
	{
		EngineProbe engine;

		EXPECT_TRUE(engine.shouldCaptureViewportInput());
	}

	TEST_F(TestEngine, DisablingCaptureReflectsOnQuery)
	{
		EngineProbe engine;

		engine.setShouldCaptureViewportInput(false);

		EXPECT_FALSE(engine.shouldCaptureViewportInput());
	}

	TEST_F(TestEngine, ReenablingCaptureReflectsOnQuery)
	{
		EngineProbe engine;

		engine.setShouldCaptureViewportInput(false);
		engine.setShouldCaptureViewportInput(true);

		EXPECT_TRUE(engine.shouldCaptureViewportInput());
	}

	TEST_F(TestEngine, EventCallbackStoresAndReturnsHandler)
	{
		EngineProbe engine;
		bool invoked = false;

		guillaume::Engine::Handler handler =
			[&invoked](std::shared_ptr<utility::event::Event> &) {
				invoked = true;
			};

		engine.setEventCallback(handler);
		auto &stored = engine.getEventCallback();

		EXPECT_TRUE(static_cast<bool>(stored));

		std::shared_ptr<utility::event::Event> event;
		stored(event);
		EXPECT_TRUE(invoked);
	}

	TEST_F(TestEngine, EmptyEventCallbackIsNotInvokable)
	{
		EngineProbe engine;

		auto &stored = engine.getEventCallback();
		EXPECT_FALSE(static_cast<bool>(stored));
	}

	TEST_F(TestEngine, ReplacingCallbackOverwritesPrevious)
	{
		EngineProbe engine;
		bool firstInvoked  = false;
		bool secondInvoked = false;

		engine.setEventCallback(
			[&firstInvoked](std::shared_ptr<utility::event::Event> &) {
				firstInvoked = true;
			});
		engine.setEventCallback(
			[&secondInvoked](std::shared_ptr<utility::event::Event> &) {
				secondInvoked = true;
			});

		auto &stored = engine.getEventCallback();
		std::shared_ptr<utility::event::Event> event;
		stored(event);

		EXPECT_FALSE(firstInvoked);
		EXPECT_TRUE(secondInvoked);
	}
}	 // namespace guillaume::tests
