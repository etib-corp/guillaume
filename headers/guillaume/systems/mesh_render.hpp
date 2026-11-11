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

#include "guillaume/components/mesh.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the 3D model of every entity that has a
	 * components::Mesh.
	 *
	 * Runs in the Render phase on entities with Transform, Bound and Mesh.
	 * It writes no component: its output is one engine object per entity,
	 * rebuilt and uploaded every frame.
	 * - components::Transform gives the pose the model is loaded at.
	 * - components::Mesh gives the path of the model file and, optionally,
	 *   the path of a texture applied to it.
	 * - components::Bound is required but not read here: it is filled by
	 *   systems::MeasureMesh for the layout.
	 *
	 * Each frame the model is requested from the resource provider with the
	 * current pose. When the provider returns no model (missing or invalid
	 * file) nothing is drawn and a model drawn on an earlier frame is
	 * removed.
	 *
	 * @see components::Mesh, systems::MeasureMesh
	 * @see RenderHandleMap
	 */
	class MeshRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Mesh>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;		 ///< Provider the models are loaded from
		utility::Engine &_engine;	 ///< Engine the models are uploaded to

		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param ressourceProvider Provider used to load the models. Must
		 * not be null.
		 * @param engine The engine the models are uploaded to. It must
		 * outlive the system.
		 */
		MeshRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Forget the engine objects without removing them from the
		 * engine.
		 */
		~MeshRender(void);

		/**
		 * @brief Mark every model as not drawn yet. Called once per frame
		 * before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the engine objects of the entities that were not
		 * drawn this frame, including those whose model failed to load.
		 * Called once per frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Load the model of one entity at its current pose and upload
		 * it, creating the engine object on the first frame.
		 * @param entityIdentifier The entity to draw. It is skipped when the
		 * model cannot be loaded, and skipped with a log message when one of
		 * the three components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
