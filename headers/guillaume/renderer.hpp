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
#include <optional>
#include <string>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include <utility/graphic/view.hpp>
#include <utility/graphic/ray.hpp>
#include <utility/graphic/orientation.hpp>
#include <utility/graphic/text.hpp>
#include <utility/graphic/vertex.hpp>

#include <utility/math/vector.hpp>

namespace guillaume
{

	/**
	 * @brief Renderer interface.
	 * @see Drawable
	 * @see Text
	 * @see Font
	 */
	class Renderer:
		protected utility::logging::Loggable<Renderer,
											 utility::logging::StandardLogger>
	{
		public:
		using ViewportSize =
			utility::math::Vector2F;	///< 2D vector representing viewport
										///< width and height in pixels.

		private:
		utility::graphic::ViewF _view;	  ///< View state

		public:
		/**
		 * @brief Default constructor
		 */
		Renderer(void)
			: Loggable()
		{
		}

		/**
		 * @brief Default destructor
		 */
		virtual ~Renderer(void) = default;

		public:
		/**
		 * @brief Get the current viewport size in pixels.
		 * @return The viewport size vector (width, height).
		 */
		virtual ViewportSize getViewportSize(void) const = 0;

		/**
		 * @brief Clear the current rendering target with the drawing color.
		 */
		virtual void clear(void) = 0;

		/**
		 * @brief Present the composed back buffer to the screen.
		 */
		virtual void present(void) = 0;

		/**
		 * @brief Draw a set of vertices forming a mesh.
		 * @param vertices The list of vertices, each containing position and
		 * any additional attributes (such as texture coordinates, color, etc.)
		 * required by the concrete renderer implementation. The vertices are
		 * interpreted using the renderer's default primitive topology
		 * (typically a triangle list) to form the mesh.
		 */
		virtual void drawVertices(
			const std::vector<utility::graphic::VertexF> &vertices) = 0;

		/**
		 * @brief Measures the pixel dimensions of a given text string when
		 * rendered with a specific font.
		 * @param text The text to measure.
		 * @return A 2D vector containing the width and height of the rendered
		 * text in pixels in the form of utility::math::Vector<std::float_t, 2>.
		 */
		virtual utility::math::Vector<std::float_t, 2>
			measureText(const utility::graphic::Text &text) = 0;

		/**
		 * @brief Measure the size of the given text using the specified font.
		 * @param text The text to draw.
		 * @param pose The pose at which to draw the text.
		 */
		virtual void drawText(const utility::graphic::Text &text,
							  const utility::graphic::PoseF &pose) = 0;

		/**
		 * @brief Set the full view model.
		 * @param view The new view instance.
		 * @note Synchronizes cached orientation and last mouse ray.
		 */
		void setView(const utility::graphic::ViewF &view)
		{
			_view = view;
		}

		/**
		 * @brief Get the full view model.
		 * @return The view instance.
		 */
		utility::graphic::ViewF getView(void) const
		{
			return _view;
		}
	};

	/**
	 * @brief Concept to ensure a type inherits from Renderer.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromRenderer = std::is_base_of_v<Renderer, Type>;

}	 // namespace guillaume
