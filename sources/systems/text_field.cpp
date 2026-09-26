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

#include "guillaume/systems/text_field.hpp"

namespace guillaume::systems
{
	TextField::TextField(void)
		: ecs::SystemFiller<components::TextField, components::Text>(
			  ecs::Phase::Measure)
	{
	}

	void TextField::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::TextField>(entityIdentifier)
			|| !requireComponent<components::Text>(entityIdentifier)) {
			return;
		}

		auto &textField = getComponent<components::TextField>(entityIdentifier);
		const auto &text = getComponent<components::Text>(entityIdentifier);

		reconcile(textField, !text.getContent().empty());
	}

	bool TextField::reconcile(components::TextField &textField, bool hasContent)
	{
		const bool floating = hasContent || textField.isError();
		textField.setLabelFloating(floating);
		return floating;
	}

}	 // namespace guillaume::systems
