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

#include <utility/ressource_provider.hpp>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/image.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the picture of every entity that has a
	 * components::Image, stretched over its bound.
	 *
	 * Runs in the Render phase on entities with Transform, Bound and Image.
	 * - Reads: the position of components::Transform (top-left corner), the
	 *   size of components::Bound, and the texture path and fallback path
	 *   of components::Image.
	 * - Writes: the load state of components::Image
	 *   (components::Image::setLoadState), and one engine object per entity,
	 *   rebuilt and uploaded every frame.
	 *
	 * Each frame the texture is requested from the resource provider. If it
	 * cannot be loaded the fallback path is tried. The Image ends up
	 * `Loaded` (with components::Image::isUsingFallback telling which
	 * texture is shown) or `Failed`; in the `Failed` case nothing is drawn
	 * and a picture drawn on an earlier frame is removed.
	 *
	 * Things to know:
	 * - The picture fills the bound exactly: the aspect ratio of the
	 *   texture is not preserved. Size the Bound accordingly.
	 * - The quad is built along the world X and Y axes: the orientation of
	 *   the Transform is not applied.
	 * - This is the last system registered, so within the Render phase
	 *   images are uploaded after every other shape.
	 *
	 * @see components::Image, RenderHandleMap
	 */
	class ImageRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Image>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;		 ///< Provider the textures are loaded
									 ///< from
		utility::Engine &_engine;	 ///< Engine the pictures are uploaded to

		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param ressourceProvider Provider used to load the textures. Must
		 * not be null.
		 * @param engine The engine the pictures are uploaded to. It must
		 * outlive the system.
		 */
		ImageRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Forget the engine objects without removing them from the
		 * engine.
		 */
		~ImageRender(void);

		/**
		 * @brief Mark every picture as not drawn yet. Called once per frame
		 * before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the engine objects of the entities that were not
		 * drawn this frame, including those whose texture failed to load.
		 * Called once per frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Load the texture of one image, record the outcome in its
		 * components::Image and upload a textured quad covering its bound.
		 * @param entityIdentifier The entity to draw. It is skipped, with a
		 * log message, when one of the three components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
