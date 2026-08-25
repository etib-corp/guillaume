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

#include <concepts>
#include <functional>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief Concept to check if an EventType has an associated Factory type.
	 *
	 * This concept checks whether a given EventType has a nested type named
	 * Factory. It is used to determine which constructor to use for the
	 * Interaction class template, allowing for different initialization logic
	 * based on the presence of a Factory type.
	 *
	 * @tparam EventType The type to check for the presence of a Factory type.
	 * @return True if EventType has a nested Factory type, false otherwise.
	 */
	template<typename EventType>
	concept HasFactoryType = requires { typename EventType::Factory; };

	/**
	 * @brief Base class for interaction components that handle events of a
	 * specific type.
	 *
	 * This class provides common functionality for interaction components, such
	 * as storing an event handler function that can be called when the
	 * interaction event occurs. It is designed to be inherited by more specific
	 * interaction component classes that handle particular types of events.
	 *
	 * @tparam EventType The type of event that this interaction component will
	 * handle. This should be a type that represents an event in the system,
	 * such as a user input event or a game event.
	 */
	template<typename EventType> class InteractionBase: public ecs::Component
	{
		public:
		using EventHandler = std::function<void(
			void)>;	   ///< Type alias for the event handler function.

		/**
		 * @brief Constructor for the InteractionBase class.
		 */
		InteractionBase(void);

		/**
		 * @brief Set the On Event Handler object
		 *
		 * @param handler The event handler function to be called when the
		 * interaction event occurs.
		 */
		void setOnEventHandler(const EventHandler &handler);

		/**
		 * @brief Get the On Event Handler object
		 *
		 * @return The current event handler function, which will be called when
		 * the interaction event occurs.
		 */
		EventHandler getOnEventHandler(void) const;

		EventHandler _onEventHandler {
			nullptr
		};	  ///< Handler function to be called when
			  ///< the interaction event occurs.
	};

	/**
	 * @brief Template class for interaction components that handle events of a
	 * specific type, with optional Factory type support.
	 *
	 * This class inherits from both the specified EventType and the
	 * InteractionBase class, allowing it to handle events of the given type
	 * while also providing common interaction functionality. The template
	 * parameter HasFactory determines which constructor is used, allowing for
	 * different initialization logic based on whether the EventType has an
	 * associated Factory type.
	 *
	 * @tparam EventType The type of event that this interaction component will
	 * handle. This should be a type that represents an event in the system,
	 * such as a user input event or a game event.
	 * @tparam HasFactory A boolean template parameter that indicates whether
	 * the EventType has an associated Factory type. This is determined using
	 * the HasFactoryType concept.
	 */
	template<typename EventType, bool HasFactory = HasFactoryType<EventType>>
	class Interaction;

	/**
	 * @brief Specialization of the Interaction class template for EventTypes
	 * that do not have an associated Factory type. This specialization is used
	 * when the EventType does not have a nested Factory type. It inherits from
	 * both the EventType and the InteractionBase class, allowing it to handle
	 * events of the given type while also providing common interaction
	 * functionality.
	 * @tparam EventType The type of event that this interaction component will
	 * handle. This should be a type that represents an event in the system,
	 * such as a user input event or a game event, and it should not have an
	 * associated Factory type.
	 */
	template<typename EventType> class Interaction<EventType, false>:
		public EventType,
		public InteractionBase<EventType>
	{
		public:
		/**
		 * @brief Constructor for the Interaction class when the EventType does
		 * not have a Factory type.
		 *
		 * This constructor initializes the Interaction component and ensures
		 * that it is properly set up to handle events of the specified
		 * EventType, which does not include an associated Factory for event
		 * creation.
		 */
		Interaction(void);
	};

	/**
	 * @brief Specialization of the Interaction class template for EventTypes
	 * that have an associated Factory type. This specialization is used when
	 * the EventType has a nested Factory type. It inherits from both the
	 * EventType and the InteractionBase class, allowing it to handle events of
	 * the given type while also providing common interaction functionality.
	 * Additionally, it defines a type alias for the Factory type associated
	 * with the EventType.
	 * @tparam EventType The type of event that this interaction component will
	 * handle. This should be a type that represents an event in the system,
	 * such as a user input event or a game event, and it should have an
	 * associated Factory type.
	 */
	template<typename EventType> class Interaction<EventType, true>:
		public EventType,
		public InteractionBase<EventType>
	{
		public:
		/**
		 * @brief Constructor for the Interaction class when the EventType has a
		 * Factory type.
		 *
		 * This constructor initializes the Interaction component and ensures
		 * that it is properly set up to handle events of the specified
		 * EventType, which includes having an associated Factory for event
		 * creation.
		 */
		Interaction(void);

		private:
		using Factory =
			typename EventType::Factory;	///< Type alias for the
											///< event's factory type.
	};
}	 // namespace guillaume::components

#include "guillaume/components/interaction.tpp"