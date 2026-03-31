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

#include <exception>
#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <SDL3_ttf/SDL_ttf.h>

#include <guillaume/renderer.hpp>

namespace simple_application
{

	class PrimaryDisplayUnavailableException: public std::exception
	{
		private:
		std::string _message;

		public:
		explicit PrimaryDisplayUnavailableException(const std::string &details)
			: _message("SDL_GetPrimaryDisplay failed: " + details)
		{
		}

		const char *what(void) const noexcept override
		{
			return _message.c_str();
		}
	};

	class DisplayBoundsUnavailableException: public std::exception
	{
		private:
		std::string _message;

		public:
		explicit DisplayBoundsUnavailableException(const std::string &details)
			: _message("SDL_GetDisplayBounds failed: " + details)
		{
		}

		const char *what(void) const noexcept override
		{
			return _message.c_str();
		}
	};

	class Renderer: public guillaume::Renderer
	{
		private:
		SDL_Window *_window;		 ///< SDL window pointer
		SDL_GLContext _glContext;	 ///< OpenGL context for the window
		std::unordered_map<std::string, TTF_Font *>
			_fontCache;	   ///< Cache for loaded fonts

		TTF_Font *getOrLoadFont(const std::string &fontPath,
								std::size_t fontSize);

		public:
		/**
		 * @brief Default constructor for the Renderer class.
		 * Initializes SDL, creates a window and OpenGL context, and sets up
		 * font caching.
		 */
		Renderer(void);

		/**
		 * @brief Destructor for the Renderer class.
		 * Cleans up SDL resources and cached fonts.
		 */
		~Renderer(void) override;

		/**
		 * @brief Clears the renderer's drawing surface.
		 */
		void clear(void) override;

		/**
		 * @brief Presents the rendered content to the display.
		 */
		void present(void) override;

		/**
		 * @brief Draw a set of vertices forming a mesh.
		 * @param vertices The list of vertices, each containing position and
		 * any additional attributes (such as texture coordinates, color, etc.)
		 * required by the concrete renderer implementation. The vertices are
		 * interpreted using the renderer's default primitive topology
		 * (typically a triangle list) to form the mesh.
		 */
		void drawVertices(
			const std::vector<utility::graphic::VertexF> &vertices) override;

		/**
		 * @brief Measure the size of the given text using the specified font.
		 * @param text The text to draw.
		 */
		void drawText(const utility::graphic::Text &text) override;

		/**
		 * @brief Measures the pixel dimensions of a given text string when
		 * rendered with a specific font.
		 * @param text The text to measure.
		 * @return A 2D vector containing the width and height of the rendered
		 * text in pixels in the form of utility::math::Vector<std::float_t, 2>.
		 */
		utility::math::Vector<std::float_t, 2>
			measureText(const utility::graphic::Text &text) override;

		/**
		 * @brief Get the renderer viewport size in pixels.
		 * @return Width and height as a 2D vector.
		 */
		ViewportSize getViewportSize(void) const override;
	};

}	 // namespace simple_application