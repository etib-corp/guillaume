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

#include <cstddef>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include <utility/engine.hpp>
#include <utility/event/event.hpp>
#include <utility/graphic/renderable.hpp>
#include <utility/graphic/text/text.hpp>
#include <utility/graphic/view.hpp>

namespace guillaume::tests
{
	/**
	 * @brief Recording test double for `utility::Engine`.
	 *
	 * Captures every engine interaction (object lifecycle, text measurement,
	 * view, and the main-loop calls) so systems and entities can be tested
	 * without a rendering backend. Outcomes of the object methods are
	 * configurable to exercise both the success and failure paths.
	 */
	class EngineMock: public utility::Engine
	{
		public:
		/**
		 * @brief Expose the protected event callback accessor for tests.
		 */
		using utility::Engine::getEventCallback;

		std::size_t createObjectCallCount { 0 };	///< createObject calls
		std::size_t updateObjectCallCount { 0 };	///< updateObject calls
		std::size_t removeObjectCallCount { 0 };	///< removeObject calls

		bool updateObjectResult { true };	 ///< Returned by updateObject
		bool removeObjectResult { true };	 ///< Returned by removeObject

		std::size_t lastUpdatedObject { 0 };	///< Last updateObject id
		std::size_t lastRemovedObject { 0 };	///< Last removeObject id
		std::size_t nextObjectID { 1 };			///< Id returned by createObject

		std::vector<std::shared_ptr<utility::graphic::Renderable>>
			createdObjects;	   ///< Objects passed to createObject
		std::vector<std::shared_ptr<utility::graphic::Renderable>>
			updatedObjects;	   ///< Objects passed to updateObject
		std::vector<std::pair<std::shared_ptr<utility::graphic::Renderable>,
							  std::size_t>>
			removedObjects;	   ///< Objects passed to removeObject

		mutable utility::graphic::SizeF textMeasurement {
			0.0f, 0.0f
		};	  ///< measureText result
		mutable std::size_t measureTextCallCount {
			0
		};	  ///< measureText calls
		mutable std::string
			lastMeasuredContent;	///< Content of the last measured text
		utility::graphic::ViewF view {};	///< Returned by getView

		std::size_t clearCallCount { 0 };		  ///< clear calls
		std::size_t presentCallCount { 0 };		  ///< present calls
		std::size_t pollEventsCallCount { 0 };	  ///< pollEvents calls
		std::size_t updateCallCount { 0 };		  ///< update calls

		/**
		 * @brief Events dispatched by the next pollEvents call.
		 */
		std::queue<std::shared_ptr<utility::event::Event>> events;

		/**
		 * @brief Default constructor.
		 */
		EngineMock(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~EngineMock(void) override = default;

		void clear(void) override
		{
			++clearCallCount;
		}

		void present(void) override
		{
			++presentCallCount;
		}

		size_t createObject(
			std::shared_ptr<utility::graphic::Renderable> object) override
		{
			++createObjectCallCount;
			createdObjects.push_back(object);
			return nextObjectID++;
		}

		bool updateObject(std::shared_ptr<utility::graphic::Renderable> object,
						  size_t objectID) override
		{
			++updateObjectCallCount;
			lastUpdatedObject = objectID;
			updatedObjects.push_back(object);
			return updateObjectResult;
		}

		bool removeObject(std::shared_ptr<utility::graphic::Renderable> object,
						  size_t objectID) override
		{
			++removeObjectCallCount;
			lastRemovedObject = objectID;
			removedObjects.emplace_back(object, objectID);
			return removeObjectResult;
		}

		utility::graphic::SizeF
			measureText(const utility::graphic::Text &text) const override
		{
			++measureTextCallCount;
			lastMeasuredContent = text.getContent();
			return textMeasurement;
		}

		utility::graphic::ViewF getView(void) const override
		{
			return view;
		}

		void pollEvents(void) override
		{
			++pollEventsCallCount;
			auto &callback = getEventCallback();
			while (!events.empty()) {
				auto event = events.front();
				events.pop();
				if (callback) {
					callback(event);
				}
			}
		}

		void update(void) override
		{
			++updateCallCount;
		}

		/**
		 * @brief Get the last object passed to createObject.
		 * @return The last created object, or nullptr.
		 */
		std::shared_ptr<utility::graphic::Renderable> lastCreated(void) const
		{
			if (createdObjects.empty()) {
				return nullptr;
			}
			return createdObjects.back();
		}

		/**
		 * @brief Get the last object passed to updateObject.
		 * @return The last updated object, or nullptr.
		 */
		std::shared_ptr<utility::graphic::Renderable> lastUpdate(void) const
		{
			if (updatedObjects.empty()) {
				return nullptr;
			}
			return updatedObjects.back();
		}

		/**
		 * @brief Count created objects of a given renderable type.
		 * @tparam RenderableType The renderable type to count.
		 * @return Number of matching created objects.
		 */
		template<typename RenderableType> std::size_t countCreated(void) const
		{
			std::size_t count = 0;
			for (const auto &object: createdObjects) {
				if (std::dynamic_pointer_cast<RenderableType>(object)) {
					++count;
				}
			}
			return count;
		}
	};

}	 // namespace guillaume::tests
