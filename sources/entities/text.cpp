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
#include "guillaume/systems/typography.hpp"
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
		_color = getActiveColor(SchemeColorRole::OnSurface);
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

	std::shared_ptr<Text> Text::Builder::makeText(
		std::shared_ptr<Entity> parent, const std::string &content,
		const float &fontSize, const utility::graphic::Color32Bit &color)
	{
		return withContent(content)
			.withFontSize(fontSize)
			.withColor(color)
			.registerEntity(parent);
	}

	std::shared_ptr<Text> Text::Builder::makeText(
		std::shared_ptr<Entity> parent, const std::string &content,
		theme::TypeRole role, const utility::graphic::Color32Bit &color)
	{
		return withContent(content)
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
							components::Text, components::Color,
							components::Typography>(registry)
	{
		typography().setRole(role).setFontSize(fontSize);
		setContent(content);
		setColor(color);
		applyTypography();
	}

	Text::~Text()
	{
	}

	components::Typography &Text::typography(void)
	{
		return getComponentRegistry().getComponent<components::Typography>(
			getIdentifier());
	}

	const components::Typography &Text::typography(void) const
	{
		return getComponentRegistry().getComponent<components::Typography>(
			getIdentifier());
	}

	void Text::applyTypography(void)
	{
		systems::Typography::apply(getComponentRegistry(), getIdentifier());
	}

	Text &Text::setContent(const std::string &content)
	{
		getComponentRegistry()
			.getComponent<components::Text>(getIdentifier())
			.setContent(content);
		return *this;
	}

	Text &Text::setFontSize(const float &fontSize)
	{
		typography().setFontSize(fontSize);
		applyTypography();
		return *this;
	}

	Text &Text::setTypeRole(theme::TypeRole role)
	{
		typography().setRole(role).setFontSize(std::nullopt);
		applyTypography();
		return *this;
	}

	theme::TypeRole Text::getTypeRole(void) const
	{
		return typography().getRole();
	}

	Text &Text::setColor(const utility::graphic::Color32Bit &color)
	{
		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(color);
		return *this;
	}

}	 // namespace guillaume::entities