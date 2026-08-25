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

#include <tuple>
#include <type_traits>

#include "guillaume/ecs/entity_tree_traveler.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/ecs/reverse_level_order_traveler.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Enumeration of system update phases.
	 *
	 * Defines the different phases during which systems can be updated,
	 * allowing for organized and efficient processing of entities based on
	 * their components.
	 */
	enum Phase : size_t {
		Event,		///< Main update phase for application logic
		Measure,	///< Measurement phase for calculating layout and text sizes
		Layout,		///< Layout phase for arranging entities based on
					///< measurements
		Render,		///< Render phase for drawing entities
		Count		///< Sentinel value representing the number of phases
	};

	/**
	 * @brief SystemPhase is a template class that associates a system update
	 * phase with a specific entity tree traversal strategy.
	 * @tparam Value The system update phase.
	 * @tparam TravelerType The type of EntityTreeTraveler to use for this
	 * phase.
	 */
	template<Phase Value, InheritFromEntityTreeTraveler TravelerType>
	class SystemPhase
	{
		public:
		static constexpr Phase phase =
			Value;	  ///< The system update phase associated with this class

		private:
		TravelerType
			_traveler;	  ///< Entity tree traveler for this system phase

		public:
		/**
		 * @brief Default constructor for SystemPhase.
		 */
		SystemPhase(void) = default;

		/**
		 * @brief Default destructor for SystemPhase.
		 */
		~SystemPhase(void) = default;

		/**
		 * @brief Get the phase value associated with this system phase.
		 * @return The system phase value.
		 */
		constexpr Phase getPhase(void) const;

		/**
		 * @brief Get a const reference to the associated traveler.
		 * @return Const traveler reference.
		 */
		const EntityTreeTraveler &getTraveler(void) const;
	};

	/**
	 * @brief Type alias for event handling systems.
	 */
	using EventPhase = SystemPhase<Phase::Event, ReverseLevelOrderTraveler>;

	/**
	 * @brief Type alias for measurement systems.
	 */
	using MeasurePhase = SystemPhase<Phase::Measure, ReverseLevelOrderTraveler>;

	/**
	 * @brief Type alias for layout systems.
	 */
	using LayoutPhase = SystemPhase<Phase::Layout, LevelOrderTraveler>;

	/**
	 * @brief Type alias for rendering systems.
	 */
	using RenderPhase = SystemPhase<Phase::Render, LevelOrderTraveler>;

	/**
	 * @brief Tuple containing all system phases in execution order.
	 */
	using SystemPhaseList =
		std::tuple<EventPhase, MeasurePhase, LayoutPhase, RenderPhase>;

}	 // namespace guillaume::ecs

// Include the implementation of the SystemPhase template class
#include "guillaume/ecs/system_phase.tpp"