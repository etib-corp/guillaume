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

#include "guillaume/entities/text.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{

	Text::Text::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
								 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Text>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Text> Text::Builder::buildEntity(void)
	{
		auto entity =
			std::make_shared<Text>(this->getComponentRegistry(), _content,
								   _typeRole, _color, _fontSize);
		return entity;
	}

	void Text::Builder::reset(void)
	{
		_content.clear();
		_typeRole = theme::TypeRole::BodyMedium;
		_fontSize.reset();
		_color =
			getActiveScheme().getColor(SchemeColorRole::OnSurface).getColor();
	}

	Text::Builder &Text::Builder::withContent(const std::string &content)
	{
		_content = content;
		return *this;
	}

	Text::Builder &Text::Builder::withFontSize(const float &fontSize)
	{
		_fontSize = fontSize;
		return *this;
	}

	Text::Builder &Text::Builder::withTypeRole(theme::TypeRole role)
	{
		_typeRole = role;
		_fontSize.reset();
		return *this;
	}

	Text::Builder &
		Text::Builder::withColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		return *this;
	}

	std::shared_ptr<Text> Text::Director::makeText(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &content, const float &fontSize,
		const utility::graphic::Color32Bit &color)
	{
		return builder.withContent(content)
			.withFontSize(fontSize)
			.withColor(color)
			.registerEntity(parent);
	}

	std::shared_ptr<Text> Text::Director::makeText(
		Builder &builder, std::shared_ptr<Entity> parent,
		const std::string &content, theme::TypeRole role,
		const utility::graphic::Color32Bit &color)
	{
		return builder.withContent(content)
			.withTypeRole(role)
			.withColor(color)
			.registerEntity(parent);
	}

	Text::Text(ecs::ComponentRegistry &registry, const std::string &content,
			   const float &fontSize, const utility::graphic::Color32Bit &color)
		: Text(registry, content, theme::TypeRole::Default, color, fontSize)
	{
	}

	Text::Text(ecs::ComponentRegistry &registry, const std::string &content,
			   theme::TypeRole role, const utility::graphic::Color32Bit &color,
			   std::optional<float> fontSize)
		: ecs::EntityFiller<components::Transform, components::Bound,
							components::Text, components::Color>(registry)
		, _content(content)
		, _typeRole(role)
		, _fontSize(fontSize)
		, _color(color)
	{
		applyTypography();
	}

	Text::~Text()
	{
	}

	Text &Text::setContent(const std::string &content)
	{
		_content = content;
		getComponentRegistry()
			.getComponent<components::Text>(getIdentifier())
			.setContent(content);
		return *this;
	}

	Text &Text::setFontSize(const float &fontSize)
	{
		_fontSize = fontSize;
		applyTypography();
		return *this;
	}

	Text &Text::setTypeRole(theme::TypeRole role)
	{
		_typeRole = role;
		_fontSize.reset();
		applyTypography();
		return *this;
	}

	theme::TypeRole Text::getTypeRole(void) const
	{
		return _typeRole;
	}

	void Text::applyTypography(void)
	{
		auto &text = getComponentRegistry().getComponent<components::Text>(
			getIdentifier());
		const auto &token = getTypeToken(_typeRole);
		text.setTypeToken(token);
		if (_fontSize.has_value()) {
			text.setFontSize(*_fontSize);
		}
	}

	Text &Text::setColor(const utility::graphic::Color32Bit &color)
	{
		_color = color;
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

	void Text::initialize(void)
	{
	}

	void Text::update(void)
	{
		setContent(_content);
		applyTypography();
		setColor(_color);
	}
}	 // namespace guillaume::entities