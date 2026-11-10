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
	 * @brief Paint the M3 focus indicator around focused entities.
	 *
	 * Entities whose `components::StyleState` is focused get a ring
	 * `theme::FocusRing::Width` thick, `theme::FocusRing::Offset` outside
	 * their bound, following their `components::Borders` radius when present,
	 * in the active scheme's secondary color.
	 */
	class FocusRingRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::StyleState>
	{
		public:
		/**
		 * @brief Construct the focus-ring render system.
		 * @param engine Engine drawing the rings.
		 */
		FocusRingRender(utility::Engine &engine);

		/**
		 * @brief Default destructor.
		 */
		~FocusRingRender(void) override = default;

		/**
		 * @brief Mark every ring unused before the frame.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the rings of entities that lost the focus.
		 */
		void cleanup(void) override;

		/**
		 * @brief Draw or refresh the ring of one entity.
		 * @param entityIdentifier Entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		private:
		utility::Engine &_engine;	 ///< Engine.
	};

}	 // namespace guillaume::systems
