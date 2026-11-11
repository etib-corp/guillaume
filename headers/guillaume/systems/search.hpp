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

#include <string>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/search.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Styles a search field and shows the children its state calls
	 * for.
	 *
	 * Reads components::Search, the query typed in the query child, the
	 * field's components::Elevation and the active theme, and writes the
	 * field's Layout, Color, Borders, Line, Overlay, TextField and
	 * Accessibility, and the glyph, text, color and enabled state of its
	 * children. Runs in the Layout phase, after the content has been
	 * measured.
	 */
	class Search: public ecs::SystemFiller<components::Search>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Search(void);

		/**
		 * @brief Default destructor.
		 */
		~Search(void) override = default;

		/**
		 * @brief Make an entity behave as a search field.
		 *
		 * Sets the accessibility semantics and the default elevation
		 * (level 0) of the field and makes the icons already recorded in
		 * components::Search pressable. Call once, after the entity has its
		 * components::Search, Overlay, TextField, Elevation, Line,
		 * Accessibility, Layout, Transform, Bound, Color and Borders.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the leading icon recorded in components::Search
		 * pressable: a press calls `pressIcon`. Call once the icon exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void
			installIcon(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the clear icon recorded in components::Search
		 * pressable: a press calls `clear`. Call once the icon exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void
			installClearIcon(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the trailing icon recorded in components::Search
		 * pressable: a press calls `pressTrailingIcon`. Call once the icon
		 * exists.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void installTrailingIcon(
			ecs::ComponentRegistry &componentRegistry,
			const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one search field and lay out the children its query,
		 * loading state and trailing glyph call for.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the query.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @return The content of the query child when it exists, else the
		 * query recorded in components::Search.
		 */
		static std::string
			text(const ecs::ComponentRegistry &componentRegistry,
				 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Replace the query and restyle the field.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @param text The new query.
		 */
		static void setText(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							const std::string &text);

		/**
		 * @brief Clear the query as the clear button does, then call the
		 * clear handler.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void clear(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Call the leading icon press handler.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void pressIcon(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Call the trailing icon press handler.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void
			pressTrailingIcon(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show the search field.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the search field.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Check whether the search field is shown.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @return True when shown.
		 */
		static bool isVisible(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style the search field with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
