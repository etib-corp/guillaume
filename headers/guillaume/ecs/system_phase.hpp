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
#include <tuple>
#include <type_traits>

#include "guillaume/ecs/entity_tree_traveler.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/ecs/reverse_level_order_traveler.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief The four steps of a frame. Every system belongs to one of them.
	 *
	 * Application::routine runs the phases in the order of the enumerators,
	 * once per frame. Within a phase, the systems run one after the other in
	 * the order they were registered, each one over the whole scene. A
	 * system therefore sees what the systems of the earlier phases wrote in
	 * the same frame, and what the later ones wrote in the previous frame.
	 *
	 * Pick the phase of a system by what it produces: state driven by input
	 * goes in Event, an intrinsic size in Measure, positions and style in
	 * Layout, engine calls in Render.
	 *
	 * @see System
	 * @see SystemPhaseList
	 */
	enum class Phase : std::size_t {
		Event,	  ///< Turn input (mouse, hands, keyboard) into component
				  ///< state and call the user's handlers
		Measure,	///< Compute intrinsic sizes (text, glyphs), children
					///< before parents
		Layout,		///< Place children and style widgets, parents before
					///< children
		Render,		///< Hand the geometry to the engine, parents before
					///< children
		Count		///< Number of phases; not a phase
	};

	/**
	 * @brief Ties a Phase to the order in which its systems visit the
	 * entities.
	 *
	 * Application keeps one instance per phase (SystemPhaseList) and, for
	 * each, runs the systems registered for getPhase() with getTraveler().
	 * Use the aliases EventPhase, MeasurePhase, LayoutPhase and RenderPhase
	 * rather than this template.
	 * @tparam Value The phase.
	 * @tparam TravelerType The traveler that orders the entities for the
	 * systems of this phase.
	 * @see EntityTreeTraveler
	 */
	template<Phase Value, InheritFromEntityTreeTraveler TravelerType>
	class SystemPhase
	{
		public:
		static constexpr Phase phase = Value;	 ///< The phase, known at
												 ///< compile time

		private:
		TravelerType
			_traveler;	  ///< The traveler handed to the systems of the phase

		public:
		/**
		 * @brief Create the phase definition and its traveler.
		 */
		SystemPhase(void) = default;

		/**
		 * @brief Destroy the phase definition and its traveler.
		 */
		~SystemPhase(void) = default;

		/**
		 * @brief Get the phase this definition describes.
		 * @return The phase, equal to SystemPhase::phase.
		 */
		constexpr Phase getPhase(void) const;

		/**
		 * @brief Get the traveler that orders the entities for this phase.
		 * @return Reference to the traveler, valid as long as this object;
		 * pass it to System::routine.
		 */
		const EntityTreeTraveler &getTraveler(void) const;
	};

	/**
	 * @brief The Event phase: entities are visited children before parents.
	 */
	using EventPhase = SystemPhase<Phase::Event, ReverseLevelOrderTraveler>;

	/**
	 * @brief The Measure phase: entities are visited children before
	 * parents, so a parent is sized after its children.
	 */
	using MeasurePhase = SystemPhase<Phase::Measure, ReverseLevelOrderTraveler>;

	/**
	 * @brief The Layout phase: entities are visited parents before children,
	 * so a parent is placed before it places its children.
	 */
	using LayoutPhase = SystemPhase<Phase::Layout, LevelOrderTraveler>;

	/**
	 * @brief The Render phase: entities are visited parents before children.
	 */
	using RenderPhase = SystemPhase<Phase::Render, LevelOrderTraveler>;

	/**
	 * @brief Every phase, in the order a frame runs them: Event, Measure,
	 * Layout, Render.
	 */
	using SystemPhaseList =
		std::tuple<EventPhase, MeasurePhase, LayoutPhase, RenderPhase>;

}	 // namespace guillaume::ecs

// Include the implementation of the SystemPhase template class
#include "guillaume/ecs/system_phase.tpp"