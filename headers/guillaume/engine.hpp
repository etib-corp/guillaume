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

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <typeindex>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include <utility/ressource_provider.hpp>

#include <utility/graphic/view.hpp>
#include <utility/graphic/ray.hpp>
#include <utility/graphic/orientation.hpp>
#include <utility/graphic/text/text.hpp>
#include <utility/graphic/mesh.hpp>
#include <utility/graphic/size.hpp>

#include <utility/math/vector.hpp>

#include <utility/event/event.hpp>

namespace guillaume
{

	/**
	 * @brief Engine interface combining rendering and event handling.
	 *
	 * This class provides an abstract interface for the core engine loop in the
	 * Guillaume framework. Implementations should provide platform-specific
	 * rendering and event polling capabilities.
	 *
	 * @code
	 * class MyEngine : public Engine { ... };
	 * Application<MyEngine> app;
	 * return app.run();
	 * @endcode
	 *
	 * @see EventBus
	 */
	class Engine:
		protected utility::logging::Loggable<Engine,
											 utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief 2D vector representing viewport width and height in pixels.
		 */
		using ViewportSize = utility::math::Vector2F;

		/**
		 * @brief Type alias for an event handler callback function.
		 *
		 * Takes a unique pointer to an Event and performs an action.
		 */
		using Handler =
			std::function<void(std::shared_ptr<utility::event::Event> &)>;

		private:
		Handler _callback;	  ///< Event callback function
		bool _shouldCaptureViewportInput {
			true
		};	  ///< Viewport input capture state

		protected:
		/**
		 * @brief Get the current event callback function.
		 * @return Reference to the event callback function.
		 */
		Handler &getEventCallback(void);

		public:
		/**
		 * @brief Check if viewport input capture is enabled.
		 * @return True when viewport input is captured for camera movement.
		 */
		bool shouldCaptureViewportInput(void) const;

		/**
		 * @brief Set the viewport input capture state.
		 * @param capture True to enable viewport input capture, false to
		 * disable. When disabled, input is routed to UI instead of camera
		 * controls.
		 */
		void setShouldCaptureViewportInput(bool capture);

		public:
		/**
		 * @brief Default constructor
		 */
		Engine(void);

		/**
		 * @brief Default destructor
		 */
		virtual ~Engine(void) = default;

		public:
		/**
		 * @brief Clear the current rendering target with the drawing color.
		 */
		virtual void clear(void) = 0;

		/**
		 * @brief Present the composed back buffer to the screen.
		 */
		virtual void present(void) = 0;

		/**
		 * @brief Add a mesh to the engine.
		 *
		 * @param mesh The mesh to add to the engine.
		 * @param materialName The name of the material to use for rendering the
		 * mesh.
		 * @return A unique identifier for the added mesh.
		 */
		virtual size_t addMesh(const utility::graphic::Mesh &mesh,
							   const std::string &materialName) = 0;

		/**
		 * @brief Remove a previously added render object.
		 *
		 * @param objectID The identifier returned by addMesh/addText.
		 * @return True when the object was removed.
		 */
		virtual bool removeObject(size_t objectID) = 0;

		/**
		 * @brief Measures the pixel dimensions of a given text string when
		 * rendered with a specific font.
		 * @param text The text to measure.
		 * @return A 2D vector containing the width and height of the rendered
		 * text in pixels in the form of utility::math::Vector2F.
		 */
		virtual utility::graphic::SizeF
			measureText(const utility::graphic::Text &text) const = 0;

		/**
		 * @brief Add a text element to the engine at a specified pose.
		 * @param text The text to draw.
		 * @return A unique identifier for the added text.
		 */
		virtual size_t addText(utility::graphic::Text text) = 0;

		/**
		 * @brief Add a model to the engine for rendering.
		 * @param model A shared pointer to the model to add.
		 * @return A unique identifier for the added model.
		 */
		virtual size_t
			addModel(std::shared_ptr<utility::graphic::Model> model) = 0;

		/**
		 * @brief Get the full view model.
		 * @return The view instance.
		 */
		virtual utility::graphic::ViewF getView(void) const = 0;

		/**
		 * @brief Add a scene to the engine.
		 * @param sceneIndex The index of the scene to add.
		 */
		virtual void addScene(size_t sceneIndex) = 0;

		/**
		 * @brief Set the event callback function.
		 *
		 * The callback will be invoked for each event when pollEvents() is
		 * called.
		 *
		 * @param callback Function to call when an event is received.
		 */
		void setEventCallback(const Handler &callback);

		/**
		 * @brief Poll for events and dispatch them.
		 *
		 * This method should check for pending events from the underlying
		 * platform, convert them to Event objects, and call the registered
		 * callback for each event.
		 */
		virtual void pollEvents(void) = 0;

		/**
		 * @brief Update the engine state.
		 *
		 * This method should handle logic updates, input processing, and other
		 * non-rendering related tasks. It is typically called once per frame,
		 * allowing the engine to respond to user input and update the state of
		 * objects in the scene.
		 */
		virtual void update(void) = 0;
	};

	/**
	 * @brief Concept to ensure a type inherits from Engine.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEngine = std::is_base_of_v<Engine, Type>;

}	 // namespace guillaume
