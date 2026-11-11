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

#include <string>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief A picture drawn in the entity's rectangle: the texture to
	 * show, a fallback texture, and whether loading succeeded.
	 *
	 * entities::Image (or the application) writes the two paths.
	 * systems::ImageRender (Render phase) loads the texture through the
	 * resource provider, draws it stretched over the entity's
	 * components::Bound at its components::Transform, and records the
	 * outcome with setLoadState().
	 *
	 * @code
	 * registry.getComponent<components::Image>(id)
	 * 	.setTexturePath("images/avatar.png")
	 * 	.setFallbackPath("images/placeholder.png");
	 * @endcode
	 *
	 * @note The image has no size of its own: set the Bound, or the image
	 * is drawn with a size of zero. The aspect ratio of the texture is not
	 * preserved.
	 * @note The load state belongs to systems::ImageRender, which rewrites
	 * it every frame. Read it; do not set it.
	 *
	 * @see systems::ImageRender, entities::Image, components::Bound
	 */
	class Image: public ecs::Component
	{
		public:
		/**
		 * @brief Whether the texture could be loaded, as recorded by
		 * systems::ImageRender.
		 */
		enum class LoadState {
			Pending,	///< Not attempted since the path was set.
			Loaded,		///< The texture (or the fallback) is drawn.
			Failed		///< Neither loaded; nothing is drawn.
		};

		private:
		std::string _texturePath {};	 ///< Texture to draw, resource path
		std::string _fallbackPath {};	 ///< Texture drawn when the first fails
		LoadState _loadState { LoadState::Pending };	///< Loading state.
		bool _usingFallback { false };	  ///< Whether the fallback is drawn.

		public:
		/**
		 * @brief Default constructor for the Image component.
		 */
		Image(void) = default;

		/**
		 * @brief Default destructor for the Image component.
		 */
		~Image(void) = default;

		/**
		 * @brief Choose the texture to draw.
		 *
		 * When the path differs from the stored one, the load state goes
		 * back to Pending, the fallback flag is cleared and the component
		 * is marked changed. The same path does nothing.
		 *
		 * @param path The path of the texture, as understood by the
		 * application's resource provider.
		 * @return This component, for chaining.
		 */
		Image &setTexturePath(const std::string &path);

		/**
		 * @brief Get the path of the texture to draw.
		 * @return The path given to setTexturePath(); empty by default.
		 */
		const std::string &getTexturePath(void) const;

		/**
		 * @brief Set the texture drawn when the main texture fails to load.
		 *
		 * When the path differs from the stored one, a Failed load state
		 * goes back to Pending and the component is marked changed.
		 *
		 * @param path The path of the fallback texture; empty for no
		 * fallback.
		 * @return This component, for chaining.
		 */
		Image &setFallbackPath(const std::string &path);

		/**
		 * @brief Get the path of the fallback texture.
		 * @return The path; empty (the default) when there is no fallback.
		 */
		const std::string &getFallbackPath(void) const;

		/**
		 * @brief Get the outcome of the last load attempt.
		 * @return Pending until systems::ImageRender has processed the
		 * entity, then Loaded or Failed.
		 */
		LoadState getLoadState(void) const;

		/**
		 * @brief Record the outcome of a load attempt. Called by
		 * systems::ImageRender.
		 *
		 * Unlike the other setters it never marks the component changed:
		 * the render system calls it every frame, and flagging a change
		 * there would keep the entity dirty for ever.
		 *
		 * @param state The outcome of the attempt.
		 * @param usingFallback True when the fallback texture is the one
		 * drawn.
		 * @return This component, for chaining.
		 */
		Image &setLoadState(LoadState state, bool usingFallback = false);

		/**
		 * @brief Check whether the fallback texture is drawn.
		 * @return True when the main texture failed to load and the
		 * fallback is drawn in its place.
		 */
		bool isUsingFallback(void) const;
	};
}	 // namespace guillaume::components
