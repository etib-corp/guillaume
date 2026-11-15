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

#include <type_traits>

namespace guillaume::ecs
{

	/**
	 * @brief Base class of every component: a piece of state attached to an
	 * entity, plus a "changed" flag.
	 *
	 * A component holds data only (a size, a color, a checkbox
	 * configuration); the rules that read and write it live in systems. One
	 * instance is stored per entity and per component type in a
	 * ComponentRegistry, and is reached by entity identifier.
	 *
	 * The base class adds one thing: the changed flag. A derived component
	 * raises it from its setters when a value really changes; the frame loop
	 * lowers every flag at the end of the frame
	 * (ComponentRegistry::resetChangedFlags). A component starts unchanged:
	 * adding it to an entity does not raise the flag.
	 *
	 * @code
	 * class Opacity: public guillaume::ecs::Component
	 * {
	 * 	public:
	 * 	float getValue(void) const { return _value; }
	 *
	 * 	Opacity &setValue(float value)
	 * 	{
	 * 		if (_value != value) {
	 * 			_value = value;
	 * 			setHasChanged(true);
	 * 		}
	 * 		return *this;
	 * 	}
	 *
	 * 	private:
	 * 	float _value { 1.0f };	 // 0 to 1
	 * };
	 * @endcode
	 *
	 * @note Components are stored by value and replaced by assignment, so a
	 * component type must be movable and constructible from the arguments
	 * given to ComponentRegistry::addComponent.
	 * @see ComponentRegistry
	 * @see System
	 */
	class Component
	{
		private:
		bool _hasChanged {
			false
		};	  ///< True when a value changed since the last reset

		public:
		/**
		 * @brief Tell whether a value of the component changed since the flags
		 * were last reset (the end of the previous frame).
		 * @return True when the component is marked as changed.
		 * @see ComponentRegistry::hasChanged
		 */
		bool hasChanged(void) const;

		/**
		 * @brief Raise or lower the changed flag.
		 *
		 * Setters of derived components call `setHasChanged(true)` after
		 * storing a different value. ComponentRegistry::resetChangedFlags
		 * calls `setHasChanged(false)` on every component.
		 * @param hasChanged True to mark the component as changed, false to
		 * mark it as unchanged.
		 */
		void setHasChanged(bool hasChanged);

		/**
		 * @brief Create a component that is not marked as changed.
		 */
		Component(void) = default;

		/**
		 * @brief Virtual destructor, so components can be destroyed through
		 * the base class.
		 */
		virtual ~Component(void) = default;
	};

	/**
	 * @brief Constrains a template parameter to a type derived from Component
	 * (or Component itself).
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromComponent = std::is_base_of_v<Component, Type>;

}	 // namespace guillaume::ecs