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
#include "guillaume/components/typography.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Resolves typescale roles into text metrics.
	 *
	 * For every entity carrying a components::Typography it looks the role
	 * up in the active theme and writes the font size, weight, line height
	 * and letter spacing into the entity's components::Text. An explicit
	 * font size on the Typography overrides the role size.
	 *
	 * It runs in the Measure phase and is registered before
	 * systems::MeasureText, so texts are measured with their final metrics.
	 */
	class Typography:
		public ecs::SystemFiller<components::Typography, components::Text>
	{
		public:
		/**
		 * @brief Construct the system.
		 */
		Typography(void);

		/**
		 * @brief Default destructor.
		 */
		~Typography(void) override = default;

		/**
		 * @brief Resolve the typography of one entity right now.
		 *
		 * Writing the same metrics again does not mark the Text as changed.
		 * @param componentRegistry Registry holding the components.
		 * @param entityIdentifier Identifier of the text entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Resolve the typography of one text entity.
		 * @param entityIdentifier Identifier of the entity being updated.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
