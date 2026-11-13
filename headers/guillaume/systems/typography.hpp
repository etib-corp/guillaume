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
	 * @brief Turns the type-scale role of a text (Body Large, Label
	 * Medium, ...) into the font metrics of the active theme.
	 *
	 * Runs in the Measure phase on entities with Typography and Text.
	 * - Reads: the role and the optional font size of
	 *   components::Typography, and the type scale of the active theme.
	 * - Writes: the font size, font weight, line height and letter spacing
	 *   of components::Text (components::Text::setTypeToken).
	 *
	 * A font size set on the Typography replaces the size of the role; the
	 * other metrics still come from the role.
	 *
	 * It is the first system registered, ahead of systems::MeasureText in
	 * the same phase, so a text is measured with the metrics resolved here.
	 * Because it runs every frame, a change of theme reaches every text on
	 * the next frame. Call apply() to resolve a text at once, for example
	 * right after creating it:
	 * @code
	 * registry.getComponent<components::Typography>(label)
	 * 	.setRole(theme::TypeRole::LabelLarge);
	 * systems::Typography::apply(registry, label);
	 * @endcode
	 *
	 * The metrics of a Text that has a Typography are overwritten every
	 * frame: change the role or the Typography font size, not the Text.
	 *
	 * @see components::Typography, components::Text
	 * @see systems::MeasureText
	 */
	class Typography:
		public ecs::SystemFiller<components::Typography, components::Text>
	{
		public:
		/**
		 * @brief Create the system in the Measure phase.
		 */
		Typography(void);

		/**
		 * @brief Destroy the system. It owns no resource.
		 */
		~Typography(void) override = default;

		/**
		 * @brief Resolve the metrics of one text and write them into its
		 * components::Text.
		 *
		 * The Text is marked changed only when a metric differs from the one
		 * it already holds.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The text entity. It must have Typography
		 * and Text.
		 * @throws ecs::EntityComponentNotFoundException When one of the two
		 * components is missing.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Resolve the metrics of one text for this frame.
		 * @param entityIdentifier The text entity. It is skipped, with a log
		 * message, when one of the two components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
