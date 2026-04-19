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

#include "renderer.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace simple_application
{

	Renderer::Renderer(void)
		: guillaume::Renderer()
		, _window(nullptr)
		, _glContext(nullptr)
	{
		auto cleanupSdlResources = [this](void) {
			if (_glContext) {
				SDL_GL_DestroyContext(_glContext);
				_glContext = nullptr;
			}
			if (_window) {
				SDL_DestroyWindow(_window);
				_window = nullptr;
			}
			TTF_Quit();
			SDL_Quit();
		};

		if (!SDL_Init(SDL_INIT_VIDEO)) {
			std::string error = SDL_GetError();
			getLogger().error("Failed to initialize SDL: " + error);
			throw std::runtime_error("SDL_Init failed: " + error);
		}

		if (!TTF_Init()) {
			std::string error = SDL_GetError();
			getLogger().error("Failed to initialize SDL_ttf: " + error);
			SDL_Quit();
			throw std::runtime_error("TTF_Init failed: " + error);
		}

		SDL_DisplayID primaryDisplayID = SDL_GetPrimaryDisplay();
		if (primaryDisplayID == 0) {
			getLogger().warning("Failed to get primary display ID: "
								+ std::string(SDL_GetError()));
			throw PrimaryDisplayUnavailableException(SDL_GetError());
		}

		SDL_Rect primaryDisplayBounds {};
		if (!SDL_GetDisplayBounds(primaryDisplayID, &primaryDisplayBounds)) {
			getLogger().warning("Failed to get primary display bounds: "
								+ std::string(SDL_GetError()));
			throw DisplayBoundsUnavailableException(SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
							SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		_window = SDL_CreateWindow("Simple Application", primaryDisplayBounds.w,
								   primaryDisplayBounds.h,
								   SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
		if (!_window) {
			std::string error = SDL_GetError();
			getLogger().error("Failed to create SDL window: " + error);
			cleanupSdlResources();
			throw std::runtime_error("SDL_CreateWindow failed: " + error);
		}

		_glContext = SDL_GL_CreateContext(_window);
		if (!_glContext) {
			std::string error = SDL_GetError();
			getLogger().error("Failed to create OpenGL context: " + error);
			cleanupSdlResources();
			throw std::runtime_error("SDL_GL_CreateContext failed: " + error);
		}

		if (!SDL_GL_MakeCurrent(_window, _glContext)) {
			std::string error = SDL_GetError();
			getLogger().error("Failed to make OpenGL context current: "
							  + error);
			cleanupSdlResources();
			throw std::runtime_error("SDL_GL_MakeCurrent failed: " + error);
		}

		SDL_GL_SetSwapInterval(1);

		if (!SDL_StartTextInput(_window)) {
			getLogger().warning("Failed to start SDL text input: "
								+ std::string(SDL_GetError()));
		}

		int windowWidth	 = 0;
		int windowHeight = 0;
		if (!SDL_GetWindowSize(_window, &windowWidth, &windowHeight)) {
			getLogger().warning("Failed to get window size: "
								+ std::string(SDL_GetError()));
			windowWidth	 = primaryDisplayBounds.w;
			windowHeight = primaryDisplayBounds.h;
		}

		glViewport(0, 0, windowWidth, windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0.0, static_cast<double>(windowWidth),
				static_cast<double>(windowHeight), 0.0, -1000.0, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Renderer::~Renderer(void)
	{
		for (auto &pair: _fontCache) {
			if (pair.second) {
				TTF_CloseFont(pair.second);
			}
		}
		_fontCache.clear();

		if (_window) {
			SDL_StopTextInput(_window);
		}

		if (_glContext) {
			SDL_GL_DestroyContext(_glContext);
		}
		if (_window) {
			SDL_DestroyWindow(_window);
		}
		TTF_Quit();
		SDL_Quit();
	}

	void Renderer::clear(void)
	{
		getLogger().debug("Clearing the screen and depth buffer");
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::present(void)
	{
		getLogger().debug("Presenting the back buffer to the screen");
		SDL_GL_SwapWindow(_window);
	}

	void Renderer::drawVertices(
		const std::vector<utility::graphic::VertexF> &vertices)
	{
		const auto viewPosition = getView().getPose().getPosition();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_TRIANGLE_FAN);
		for (const auto &vertex: vertices) {
			glColor4ub(vertex.getColor().getRed(), vertex.getColor().getGreen(),
					   vertex.getColor().getBlue(),
					   vertex.getColor().getAlpha());
			auto position = vertex.getPosition();
			position -= viewPosition;
			glVertex3f(position[0], position[1], position[2]);
		}
		glEnd();
		glEnable(GL_DEPTH_TEST);
	}

	utility::math::Vector<std::float_t, 2>
		Renderer::measureText(const utility::graphic::Text &text)
	{
		getLogger().debug("Measuring text: " + text.getContent());

		TTF_Font *ttfFont =
			getOrLoadFont(text.getFontFamily(), text.getFontSize());
		if (!ttfFont) {
			getLogger().error("Failed to load font for measurement");
			return { 0.0f, 0.0f };
		}

		int width = 0, height = 0;
		// Pass 0 as length to indicate that the input string is null-terminated
		// (SDL_ttf API).
		if (!TTF_GetStringSize(ttfFont, text.getContent().c_str(), 0, &width,
							   &height)) {
			getLogger().error("Failed to measure text: "
							  + std::string(SDL_GetError()));
			return { 0.0f, 0.0f };
		}

		return { static_cast<float>(width), static_cast<float>(height) };
	}

	guillaume::Renderer::ViewportSize Renderer::getViewportSize(void) const
	{
		if (!_window) {
			return { 1.0f, 1.0f };
		}

		int width  = 0;
		int height = 0;
		if (!SDL_GetWindowSize(_window, &width, &height)) {
			return { 1.0f, 1.0f };
		}

		return { static_cast<float>(width), static_cast<float>(height) };
	}

	void Renderer::drawText(const utility::graphic::Text &text,
							const utility::graphic::PoseF &pose)
	{
		getLogger().debug("Drawing text: " + text.getContent());

		TTF_Font *ttfFont =
			getOrLoadFont(text.getFontFamily(), text.getFontSize());
		if (!ttfFont) {
			getLogger().error("Failed to load font for rendering");
			return;
		}

		auto color		   = text.getColor();
		SDL_Color sdlColor = { color.getRed(), color.getGreen(),
							   color.getBlue(), color.getAlpha() };

		SDL_Surface *surface = TTF_RenderText_Blended(
			ttfFont, text.getContent().c_str(), 0, sdlColor);
		if (!surface) {
			getLogger().error("Failed to render text to surface: "
							  + std::string(SDL_GetError()));
			return;
		}

		SDL_Surface *converted =
			SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
		if (!converted) {
			getLogger().error("Failed to convert surface for OpenGL: "
							  + std::string(SDL_GetError()));
			SDL_DestroySurface(surface);
			return;
		}

		glDisable(GL_DEPTH_TEST);

		GLuint textureId = 0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);

		auto position			= pose.getPosition();
		const auto viewPosition = getView().getPose().getPosition();
		position -= viewPosition;
		auto orientation				 = pose.getOrientation();
		float width						 = static_cast<float>(converted->w);
		float height					 = static_cast<float>(converted->h);
		float z							 = position[2];
		const auto normalizedOrientation = orientation.normalized();
		const float clampedW = std::clamp(normalizedOrientation.w, -1.0f, 1.0f);
		const float angleRadians = 2.0f * std::acos(clampedW);
		const float angleDegrees =
			angleRadians * (180.0f / 3.14159265358979323846f);
		const float sineHalfAngle =
			std::sqrt(std::max(0.0f, 1.0f - (clampedW * clampedW)));

		float axisX = 0.0f;
		float axisY = 0.0f;
		float axisZ = 1.0f;
		if (sineHalfAngle > 1.0e-6f) {
			axisX = normalizedOrientation.x / sineHalfAngle;
			axisY = normalizedOrientation.y / sineHalfAngle;
			axisZ = normalizedOrientation.z / sineHalfAngle;
		}

		glColor4ub(color.getRed(), color.getGreen(), color.getBlue(),
				   color.getAlpha());
		glPushMatrix();
		glTranslatef(position[0], position[1], z);
		glRotatef(angleDegrees, axisX, axisY, axisZ);

		float halfWidth	 = width / 2.0f;
		float halfHeight = height / 2.0f;

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-halfWidth, -halfHeight, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(halfWidth, -halfHeight, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(halfWidth, halfHeight, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-halfWidth, halfHeight, 0.0f);
		glEnd();

		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDeleteTextures(1, &textureId);
		SDL_DestroySurface(converted);
		SDL_DestroySurface(surface);

		glEnable(GL_DEPTH_TEST);
	}

	TTF_Font *Renderer::getOrLoadFont(const std::string &fontPath,
									  std::size_t fontSize)
	{
		if (fontSize == 0) {
			getLogger().warning(
				"Requested font size is 0, using fallback size 24");
			fontSize = 24;
		}

		std::string fontKey = fontPath + ":" + std::to_string(fontSize);

		auto it = _fontCache.find(fontKey);
		if (it != _fontCache.end()) {
			return it->second;
		}

		TTF_Font *ttfFont = TTF_OpenFont(fontPath.c_str(), fontSize);
		if (!ttfFont) {
			getLogger().error("Failed to load font: "
							  + std::string(SDL_GetError()));
			return nullptr;
		}

		_fontCache[fontKey] = ttfFont;
		return ttfFont;
	}

}	 // namespace simple_application