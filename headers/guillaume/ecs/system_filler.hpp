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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/system.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief The base class to derive a system from: a System whose
	 * signature is the list of component types given as template arguments.
	 *
	 * The list answers "which entities does this system run on?": update()
	 * is called for every entity of the scene that declares all the listed
	 * components. List the components the system cannot work without;
	 * check the optional ones in update() with hasComponent().
	 *
	 * @code
	 * // Runs in the Measure phase on every entity that has both a
	 * // Transform and a Bound.
	 * class Snap:
	 * 	public ecs::SystemFiller<components::Transform, components::Bound>
	 * {
	 * 	public:
	 * 	Snap(void)
	 * 		: ecs::SystemFiller<components::Transform, components::Bound>(
	 * 			  ecs::Phase::Measure)
	 * 	{
	 * 	}
	 *
	 * 	void update(const ecs::Entity::Identifier &identifier) override;
	 * };
	 * @endcode
	 *
	 * @tparam ComponentTypes The component types an entity must declare to
	 * be visited. An empty list makes the system visit every entity.
	 * @see System
	 * @see Phase
	 */
	template<InheritFromComponent... ComponentTypes> class SystemFiller:
		public System
	{
		public:
		/**
		 * @brief Create the system and set its signature from
		 * `ComponentTypes`.
		 * @param phase The phase of the frame in which the system runs.
		 */
		SystemFiller(Phase phase);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~SystemFiller(void) = default;
	};

}	 // namespace guillaume::ecs

// Include the implementation of the SystemFiller template class
#include "guillaume/ecs/system_filler.tpp"