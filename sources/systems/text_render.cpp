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

#include <memory>

#include <utility/graphic/text/text.hpp>

#include "guillaume/systems/text_render.hpp"

namespace guillaume::systems
{
	TextRender::TextRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<utility::Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Text,
							components::Color>(ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath("fonts/Roboto-Regular.ttf")
	{
	}

	TextRender::~TextRender(void)
	{
		clear();
	}

	void TextRender::prepare(void)
	{
		markAllUnused();
	}

	void TextRender::cleanup(void)
	{
		removeUnused(*_engine);
	}

	void TextRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating TextRender system for entity " << entityIdentifier;
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Text>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &textComponent =
			getComponent<components::Text>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		getLogger().debug()
			<< "Rendering text for entity " << entityIdentifier
			<< " (content: '" << textComponent.getContent() << "')";

		auto renderable = std::make_shared<utility::graphic::Text>(
			_ressourceProvider, transformComponent.getPose(),
			colorComponent.getColor(), textComponent.getContent(),
			textComponent.getFontSize(), _defaultFontPath);

		RenderHandle *handle = find(entityIdentifier);
		if (handle == nullptr) {
			size_t objectId = _engine->createObject(renderable);
			insert(entityIdentifier,
				   RenderHandle { objectId, renderable, true });
		} else {
			if (!_engine->updateObject(renderable, handle->objectId)) {
				handle->objectId = _engine->createObject(renderable);
			}
			handle->renderable = renderable;
			handle->used	   = true;
		}
	}

}	 // namespace guillaume::systems
