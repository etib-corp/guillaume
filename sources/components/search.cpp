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

#include "guillaume/components/search.hpp"

#include <utility>

namespace guillaume::components
{
	Search::Variant Search::getVariant(void) const noexcept
	{
		return _variant;
	}

	Search &Search::setVariant(Variant variant)
	{
		return assign(_variant, variant);
	}

	const std::string &Search::getPlaceholder(void) const noexcept
	{
		return _placeholder;
	}

	Search &Search::setPlaceholder(const std::string &placeholder)
	{
		return assign(_placeholder, placeholder);
	}

	const std::string &Search::getText(void) const noexcept
	{
		return _text;
	}

	Search &Search::setText(const std::string &text)
	{
		return assign(_text, text);
	}

	const std::string &Search::getIconGlyph(void) const noexcept
	{
		return _iconGlyph;
	}

	Search &Search::setIconGlyph(const std::string &glyph)
	{
		return assign(_iconGlyph, glyph);
	}

	const std::string &Search::getTrailingGlyph(void) const noexcept
	{
		return _trailingGlyph;
	}

	Search &Search::setTrailingGlyph(const std::string &glyph)
	{
		return assign(_trailingGlyph, glyph);
	}

	bool Search::isLoading(void) const noexcept
	{
		return _loading;
	}

	Search &Search::setLoading(bool loading)
	{
		return assign(_loading, loading);
	}

	const std::function<void(void)> &Search::getOnIconPress(void) const noexcept
	{
		return _onIconPress;
	}

	Search &Search::setOnIconPress(std::function<void(void)> handler)
	{
		_onIconPress = std::move(handler);
		return *this;
	}

	const std::function<void(void)> &
		Search::getOnClearIconPress(void) const noexcept
	{
		return _onClearIconPress;
	}

	Search &Search::setOnClearIconPress(std::function<void(void)> handler)
	{
		_onClearIconPress = std::move(handler);
		return *this;
	}

	const std::function<void(void)> &
		Search::getOnTrailingIconPress(void) const noexcept
	{
		return _onTrailingIconPress;
	}

	Search &Search::setOnTrailingIconPress(std::function<void(void)> handler)
	{
		_onTrailingIconPress = std::move(handler);
		return *this;
	}

	ecs::Entity::Identifier Search::getSearchIcon(void) const noexcept
	{
		return _searchIcon;
	}

	Search &Search::setSearchIcon(ecs::Entity::Identifier identifier)
	{
		return assign(_searchIcon, identifier);
	}

	ecs::Entity::Identifier Search::getTextEntity(void) const noexcept
	{
		return _textEntity;
	}

	Search &Search::setTextEntity(ecs::Entity::Identifier identifier)
	{
		return assign(_textEntity, identifier);
	}

	ecs::Entity::Identifier Search::getPlaceholderEntity(void) const noexcept
	{
		return _placeholderEntity;
	}

	Search &Search::setPlaceholderEntity(ecs::Entity::Identifier identifier)
	{
		return assign(_placeholderEntity, identifier);
	}

	ecs::Entity::Identifier Search::getClearIcon(void) const noexcept
	{
		return _clearIcon;
	}

	Search &Search::setClearIcon(ecs::Entity::Identifier identifier)
	{
		return assign(_clearIcon, identifier);
	}

	ecs::Entity::Identifier Search::getLoadingIcon(void) const noexcept
	{
		return _loadingIcon;
	}

	Search &Search::setLoadingIcon(ecs::Entity::Identifier identifier)
	{
		return assign(_loadingIcon, identifier);
	}

	ecs::Entity::Identifier Search::getTrailingIcon(void) const noexcept
	{
		return _trailingIcon;
	}

	Search &Search::setTrailingIcon(ecs::Entity::Identifier identifier)
	{
		return assign(_trailingIcon, identifier);
	}
}	 // namespace guillaume::components
