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

#include <functional>
#include <string>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a search field.
	 *
	 * This component is the whole state of a search field, apart from its
	 * visibility, which lives in components::Overlay, and its elevation,
	 * which lives in components::Elevation. systems::Search turns it into
	 * the field's Layout, Color, Borders, Line, Overlay, TextField and
	 * Accessibility, and into the glyph, color and enabled state of its
	 * children.
	 */
	class Search: public ecs::Component
	{
		public:
		/**
		 * @brief Search variant.
		 */
		enum class Variant {
			SearchBar,			 ///< Pill-shaped 56 dp bar.
			FullScreen,			 ///< 72 dp inline view with a divider.
			View = FullScreen	 ///< Paper `view` mode name.
		};

		/// Default search width, in dp.
		static constexpr float DefaultWidth = 360.0f;

		/// Paper `bar` height, in dp.
		static constexpr float BarHeight = 56.0f;

		/// Paper `view` height, in dp.
		static constexpr float ViewHeight = 72.0f;

		/// Search icon size, in dp.
		static constexpr float IconSize = 24.0f;

		/// Default leading glyph.
		static constexpr const char *SearchGlyph = "search";

		/// Glyph of the clear button.
		static constexpr const char *ClearGlyph = "close";

		/// Material Symbols glyph shown while loading.
		static constexpr const char *LoadingGlyph = "progress_activity";

		private:
		Variant _variant { Variant::SearchBar };		   ///< Search variant
		std::string _placeholder {};					   ///< Placeholder text
		std::string _text {};							   ///< Query text
		std::string _iconGlyph { SearchGlyph };			   ///< Leading glyph
		std::string _trailingGlyph {};					   ///< Trailing glyph
		bool _loading { false };						   ///< Loading state
		std::function<void(void)> _onIconPress {};		   ///< Leading handler
		std::function<void(void)> _onClearIconPress {};	   ///< Clear handler
		std::function<void(void)>
			_onTrailingIconPress {};	///< Trailing handler
		ecs::Entity::Identifier _searchIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Pressable leading icon

		ecs::Entity::Identifier _textEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Query child

		ecs::Entity::Identifier _placeholderEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Placeholder child

		ecs::Entity::Identifier _clearIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Pressable clear icon

		ecs::Entity::Identifier _loadingIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Loading glyph

		ecs::Entity::Identifier _trailingIcon {
			ecs::Entity::InvalidIdentifier
		};	  ///< Pressable trailing icon

		public:
		/**
		 * @brief Default constructor.
		 */
		Search(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Search(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Search &setVariant(Variant variant);

		/** @brief Get the placeholder. @return The placeholder. */
		const std::string &getPlaceholder(void) const noexcept;

		/** @brief Set the placeholder. @param placeholder It. @return It. */
		Search &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Get the query.
		 *
		 * Once the query child exists, its components::Text is what the
		 * user edits; systems::Search copies it back here.
		 * @return The query.
		 */
		const std::string &getText(void) const noexcept;

		/** @brief Set the query. @param text It. @return It. */
		Search &setText(const std::string &text);

		/** @brief Get the leading glyph. @return The glyph name. */
		const std::string &getIconGlyph(void) const noexcept;

		/** @brief Set the leading glyph. @param glyph It. @return It. */
		Search &setIconGlyph(const std::string &glyph);

		/** @brief Get the trailing glyph. @return Its name, or "". */
		const std::string &getTrailingGlyph(void) const noexcept;

		/** @brief Set the trailing glyph. @param glyph It. @return It. */
		Search &setTrailingGlyph(const std::string &glyph);

		/** @brief Check the loading state. @return True while loading. */
		bool isLoading(void) const noexcept;

		/** @brief Set the loading state. @param loading It. @return It. */
		Search &setLoading(bool loading);

		/** @brief Get the leading press handler. @return The handler. */
		const std::function<void(void)> &getOnIconPress(void) const noexcept;

		/** @brief Set the leading press handler. @param handler It. @return
		 * It. */
		Search &setOnIconPress(std::function<void(void)> handler);

		/** @brief Get the clear press handler. @return The handler. */
		const std::function<void(void)> &
			getOnClearIconPress(void) const noexcept;

		/** @brief Set the clear press handler. @param handler It. @return
		 * It. */
		Search &setOnClearIconPress(std::function<void(void)> handler);

		/** @brief Get the trailing press handler. @return The handler. */
		const std::function<void(void)> &
			getOnTrailingIconPress(void) const noexcept;

		/** @brief Set the trailing press handler. @param handler It. @return
		 * It. */
		Search &setOnTrailingIconPress(std::function<void(void)> handler);

		/** @brief Get the leading icon. @return Its identifier, or Invalid. */
		ecs::Entity::Identifier getSearchIcon(void) const noexcept;

		/** @brief Set the leading icon. @param identifier It. @return It. */
		Search &setSearchIcon(ecs::Entity::Identifier identifier);

		/** @brief Get the query child. @return Its identifier, or Invalid. */
		ecs::Entity::Identifier getTextEntity(void) const noexcept;

		/** @brief Set the query child. @param identifier It. @return It. */
		Search &setTextEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the placeholder child. @return Its identifier, or
		 * Invalid. */
		ecs::Entity::Identifier getPlaceholderEntity(void) const noexcept;

		/** @brief Set the placeholder child. @param identifier It. @return
		 * It. */
		Search &setPlaceholderEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the clear icon. @return Its identifier, or Invalid. */
		ecs::Entity::Identifier getClearIcon(void) const noexcept;

		/** @brief Set the clear icon. @param identifier It. @return It. */
		Search &setClearIcon(ecs::Entity::Identifier identifier);

		/** @brief Get the loading glyph. @return Its identifier, or Invalid.
		 */
		ecs::Entity::Identifier getLoadingIcon(void) const noexcept;

		/** @brief Set the loading glyph. @param identifier It. @return It. */
		Search &setLoadingIcon(ecs::Entity::Identifier identifier);

		/** @brief Get the trailing icon. @return Its identifier, or Invalid.
		 */
		ecs::Entity::Identifier getTrailingIcon(void) const noexcept;

		/** @brief Set the trailing icon. @param identifier It. @return It. */
		Search &setTrailingIcon(ecs::Entity::Identifier identifier);

		private:
		/**
		 * @brief Store a value and mark the component changed when it
		 * differs.
		 * @tparam Type Type of the value.
		 * @param field The field to write.
		 * @param value The new value.
		 * @return Reference to this component for chaining.
		 */
		template<typename Type> Search &assign(Type &field, const Type &value)
		{
			if (!(field == value)) {
				field = value;
				setHasChanged(true);
			}
			return *this;
		}
	};

}	 // namespace guillaume::components
