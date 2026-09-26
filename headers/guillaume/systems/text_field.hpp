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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/text.hpp"
#include "guillaume/components/text_field.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System maintaining the derived state of text fields.
	 *
	 * Floats the label when the field has content or is in an error state, so
	 * `entities::TextField` and `entities::Search` stay thin composites.
	 *
	 * @see components::TextField
	 */
	class TextField:
		public ecs::SystemFiller<components::TextField, components::Text>
	{
		public:
		/**
		 * @brief Construct a text field system in the Measure phase.
		 */
		TextField(void);

		/**
		 * @brief Default destructor.
		 */
		~TextField(void) override = default;

		/**
		 * @brief Update the derived state of one text field.
		 * @param entityIdentifier The target entity identifier.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;

		/**
		 * @brief Reconcile the label floating state with the field content.
		 * @param textField The text field component.
		 * @param hasContent Whether the field currently has content.
		 * @return The updated label floating state.
		 */
		static bool reconcile(components::TextField &textField,
							  bool hasContent);
	};

}	 // namespace guillaume::systems
