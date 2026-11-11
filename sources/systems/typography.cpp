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

#include "guillaume/systems/typography.hpp"

#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	Typography::Typography(void)
		: ecs::SystemFiller<components::Typography, components::Text>(
			  ecs::Phase::Measure)
	{
	}

	void Typography::apply(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &typography =
			componentRegistry.getComponent<components::Typography>(
				entityIdentifier);

		// Fold the explicit size into the token before writing, so the Text
		// is written once and only marked changed when a metric differs.
		theme::TypeToken token = getTypeToken(typography.getRole());
		if (const auto fontSize = typography.getFontSize()) {
			token.fontSize = *fontSize;
		}

		componentRegistry.getComponent<components::Text>(entityIdentifier)
			.setTypeToken(token);
	}

	void Typography::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Typography>(entityIdentifier)
			|| !requireComponent<components::Text>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
