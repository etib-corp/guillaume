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

#include <memory>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the focus indicator: an outline around the entity that
	 * has the keyboard focus.
	 *
	 * Runs in the Render phase on entities with Transform, Bound and
	 * StyleState. It writes no component. An entity gets a ring only while
	 * components::StyleState::isFocused is true; the ring is removed on the
	 * first frame the flag is false.
	 *
	 * The ring is `theme::FocusRing::Width` dp thick and starts
	 * `theme::FocusRing::Offset` dp outside the bound, in the Secondary color
	 * of the active scheme. When the entity has a components::Borders with a
	 * radius, the ring follows it (the four radii are averaged); otherwise
	 * its corners are square. It is lifted by RectangleRender::StrokeLift
	 * towards the viewer.
	 *
	 * Registered after systems::Focus and systems::StyleState, which decide
	 * in the Event phase which entity is focused.
	 *
	 * @see components::StyleState, systems::Focus
	 * @see RenderHandleMap, shape::buildRoundedStrokeMesh
	 */
	class FocusRingRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::StyleState>
	{
		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param engine The engine the rings are uploaded to. It must outlive
		 * the system.
		 */
		FocusRingRender(utility::Engine &engine);

		/**
		 * @brief Destroy the system. The engine objects it created are left
		 * in the engine.
		 */
		~FocusRingRender(void) override = default;

		/**
		 * @brief Mark every ring as not drawn yet. Called once per frame
		 * before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the rings that were not drawn this frame, which is
		 * how an entity that lost the focus loses its ring. Called once per
		 * frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Draw the ring of one entity if it is focused, creating the
		 * engine object on the first frame and updating it afterwards.
		 * @param entityIdentifier The entity to consider. It is skipped when
		 * it is not focused, and skipped with a log message when one of the
		 * three components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		private:
		utility::Engine &_engine;	 ///< Engine the rings are uploaded to
	};

}	 // namespace guillaume::systems
