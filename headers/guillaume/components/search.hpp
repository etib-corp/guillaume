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
	 * @brief State of a search field: a leading icon, the query the user
	 * types (or a placeholder), then a clear button or a loading glyph and
	 * an optional trailing icon.
	 *
	 * The entities::Search widget writes it: the texts, glyphs, handlers
	 * and the identifiers of the children it builds. systems::Search reads
	 * it every frame in the Layout phase and turns it into the field's
	 * Layout, Color, Borders, Line, Overlay flags, TextField and
	 * Accessibility, and into the glyph, color and enabled state of its
	 * children.
	 *
	 * Two things are not stored here: whether the field is shown
	 * (components::Overlay of the same entity) and its elevation
	 * (components::Elevation).
	 *
	 * Once the query child exists, the components::Text of that child is
	 * where the user types, and systems::Search copies it back into this
	 * component every frame. So change the query with
	 * systems::Search::setText, which writes both; a value written with
	 * setText() alone is overwritten on the next frame.
	 *
	 * @see systems::Search, components::TextField, entities::Search
	 */
	class Search: public ecs::Component
	{
		public:
		/**
		 * @brief Shape of the search field.
		 */
		enum class Variant {
			SearchBar,			 ///< Pill-shaped bar, 56 dp high.
			FullScreen,			 ///< Square-cornered view, 72 dp high, with a
								 ///< divider line under it.
			View = FullScreen	 ///< Another name for FullScreen.
		};

		/// Width given to a search field that has no fixed width, in dp.
		static constexpr float DefaultWidth = 360.0f;

		/// Height of the SearchBar variant, in dp.
		static constexpr float BarHeight = 56.0f;

		/// Height of the FullScreen variant, in dp.
		static constexpr float ViewHeight = 72.0f;

		/// Size of the icons, in dp.
		static constexpr float IconSize = 24.0f;

		/// Glyph name of the default leading icon.
		static constexpr const char *SearchGlyph = "search";

		/// Glyph name of the clear button.
		static constexpr const char *ClearGlyph = "close";

		/// Glyph name of the icon shown while loading.
		static constexpr const char *LoadingGlyph = "progress_activity";

		private:
		Variant _variant { Variant::SearchBar };		   ///< Field shape
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
		 * @brief Create the configuration of an empty search bar with the
		 * "search" leading icon.
		 */
		Search(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Search(void) = default;

		/**
		 * @brief Get the shape of the field.
		 * @return The variant, Variant::SearchBar by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the shape of the field.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		Search &setVariant(Variant variant);

		/**
		 * @brief Get the hint shown while the query is empty.
		 * @return The placeholder; it is also the field's accessible name.
		 */
		const std::string &getPlaceholder(void) const noexcept;

		/**
		 * @brief Set the hint shown while the query is empty, such as
		 * "Search".
		 *
		 * Marks the component changed only when the text differs.
		 * @param placeholder The new placeholder.
		 * @return This component, for chaining.
		 */
		Search &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Get the query.
		 *
		 * Once the query child exists, its components::Text is what the
		 * user edits; systems::Search copies it back here every frame.
		 * @return The query as of the last pass of systems::Search.
		 */
		const std::string &getText(void) const noexcept;

		/**
		 * @brief Store the query.
		 *
		 * Once the query child exists this value is overwritten every
		 * frame from that child; use systems::Search::setText to change
		 * the query. Marks the component changed only when the text
		 * differs.
		 * @param text The new query.
		 * @return This component, for chaining.
		 */
		Search &setText(const std::string &text);

		/**
		 * @brief Get the icon at the leading edge of the field.
		 * @return The glyph name, "search" by default.
		 */
		const std::string &getIconGlyph(void) const noexcept;

		/**
		 * @brief Set the icon at the leading edge of the field, such as
		 * "arrow_back".
		 *
		 * Marks the component changed only when the name differs.
		 * @param glyph The glyph name.
		 * @return This component, for chaining.
		 */
		Search &setIconGlyph(const std::string &glyph);

		/**
		 * @brief Get the icon at the trailing edge of the field.
		 * @return The glyph name; empty when the field has no trailing
		 * icon.
		 */
		const std::string &getTrailingGlyph(void) const noexcept;

		/**
		 * @brief Set the icon at the trailing edge of the field, such as
		 * "mic".
		 *
		 * The trailing icon is shown and pressable only while this name is
		 * not empty. Marks the component changed only when the name
		 * differs.
		 * @param glyph The glyph name; empty for no trailing icon.
		 * @return This component, for chaining.
		 */
		Search &setTrailingGlyph(const std::string &glyph);

		/**
		 * @brief Tell whether the field shows its loading glyph.
		 * @return True while loading.
		 */
		bool isLoading(void) const noexcept;

		/**
		 * @brief Show or hide the loading glyph.
		 *
		 * While loading, the loading glyph takes the place of the clear
		 * button. Marks the component changed only when the flag differs.
		 * @param loading True while results are being fetched.
		 * @return This component, for chaining.
		 */
		Search &setLoading(bool loading);

		/**
		 * @brief Get the callback run when the leading icon is pressed.
		 * @return The callback; empty when none is set.
		 */
		const std::function<void(void)> &getOnIconPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the leading icon is pressed.
		 *
		 * Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		Search &setOnIconPress(std::function<void(void)> handler);

		/**
		 * @brief Get the callback run when the clear button is pressed.
		 * @return The callback; empty when none is set.
		 */
		const std::function<void(void)> &
			getOnClearIconPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the clear button is pressed.
		 *
		 * It runs after the query has been emptied. Does not mark the
		 * component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		Search &setOnClearIconPress(std::function<void(void)> handler);

		/**
		 * @brief Get the callback run when the trailing icon is pressed.
		 * @return The callback; empty when none is set.
		 */
		const std::function<void(void)> &
			getOnTrailingIconPress(void) const noexcept;

		/**
		 * @brief Set the callback run when the trailing icon is pressed.
		 *
		 * Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		Search &setOnTrailingIconPress(std::function<void(void)> handler);

		/**
		 * @brief Get the pressable child that draws the leading icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getSearchIcon(void) const noexcept;

		/**
		 * @brief Record the pressable child that draws the leading icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Search &setSearchIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child the user types the query in.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getTextEntity(void) const noexcept;

		/**
		 * @brief Record the child the user types the query in.
		 *
		 * From then on its components::Text is the source of the query.
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Search &setTextEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the placeholder.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getPlaceholderEntity(void) const noexcept;

		/**
		 * @brief Record the child that draws the placeholder.
		 *
		 * It takes the place of the query child while the query is empty.
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Search &setPlaceholderEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the pressable child that draws the clear button.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getClearIcon(void) const noexcept;

		/**
		 * @brief Record the pressable child that draws the clear button.
		 *
		 * The button is shown and enabled only while there is a query and
		 * the field is not loading. Marks the component changed only when
		 * the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Search &setClearIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the child that draws the loading glyph.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getLoadingIcon(void) const noexcept;

		/**
		 * @brief Record the child that draws the loading glyph.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Search &setLoadingIcon(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the pressable child that draws the trailing icon.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getTrailingIcon(void) const noexcept;

		/**
		 * @brief Record the pressable child that draws the trailing icon.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		Search &setTrailingIcon(ecs::Entity::Identifier identifier);

		private:
		/**
		 * @brief Store a value and mark the component changed when it
		 * differs from the stored one.
		 * @tparam Type Type of the value; it must be comparable with `==`.
		 * @param field The member to write.
		 * @param value The new value.
		 * @return This component, for chaining.
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
