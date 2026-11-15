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
	 * @brief Styles a search field and lays out the children that its
	 * query, loading state and trailing icon call for.
	 *
	 * Runs in the Layout phase on every entity that has a
	 * components::Search. It is registered after systems::Layout, so the
	 * texts and icons are already measured when it arranges them.
	 * - **Reads**: components::Search (variant, placeholder, glyphs,
	 *   loading flag, child identifiers), the Text of the query child, the
	 *   level of the field's components::Elevation and the active theme.
	 * - **Writes** on the field: the query recorded in components::Search,
	 *   components::Layout (a row, fixed height, child list, a default
	 *   width when none is fixed), components::Line, components::Borders,
	 *   components::Color, the flags of components::Overlay, the
	 *   placeholder, label and floating state of components::TextField and
	 *   the label of components::Accessibility.
	 * - **Writes** on the children: the Glyph and Color of the icons, the
	 *   Text and Color of the placeholder, and the enabled state of the
	 *   clear and trailing icons. They are then placed through
	 *   systems::Layout::apply.
	 *
	 * The row holds, in order: the leading icon; the query child once
	 * something is typed, otherwise the placeholder; the loading icon while
	 * loading, otherwise the clear icon; and the trailing icon while it has
	 * a glyph. The clear icon keeps its place in an empty field: it is
	 * painted transparent and disabled.
	 *
	 * The query child is what the user types in. As soon as it exists, its
	 * components::Text is the source of the query, and the copy in
	 * components::Search is overwritten from it on every apply().
	 *
	 * Whether the field is shown is the `visible` flag of its
	 * components::Overlay. A search field is never modal; it closes on
	 * Escape but not on a click outside.
	 *
	 * @see components::Search, components::TextField, entities::Search,
	 * systems::TextField
	 */
	class Search: public ecs::SystemFiller<components::Search>
	{
		public:
		/**
		 * @brief Create the system in the Layout phase.
		 */
		Search(void);

		/**
		 * @brief Default destructor.
		 */
		~Search(void) override = default;

		/**
		 * @brief Give a search field its accessibility role and make its
		 * icons pressable.
		 *
		 * Sets the role of its components::Accessibility to Search, resets
		 * its components::Elevation to level 0, then calls installIcon(),
		 * installClearIcon() and installTrailingIcon(). Call it once, after
		 * the entity has its components::Search, Overlay, TextField,
		 * Elevation, Line, Accessibility, Layout, Transform, Bound, Color
		 * and Borders. Set another elevation level after this call, not
		 * before.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the icon handlers and must outlive the entity.
		 * @param entityIdentifier The search entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * components::Accessibility, Elevation or Search.
		 */
		static void install(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the leading icon a button that calls pressIcon().
		 *
		 * Adds the interaction components to the leading icon recorded in
		 * components::Search and names it "search" for assistive
		 * technology. Call it once the icon entity exists. Does nothing when
		 * no leading icon is recorded.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the press handler and must outlive the entity.
		 * @param entityIdentifier The search entity.
		 */
		static void
			installIcon(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the clear icon a button that calls clear().
		 *
		 * Adds the interaction components to the clear icon recorded in
		 * components::Search and names it "clear search" for assistive
		 * technology. Call it once the icon entity exists. Does nothing when
		 * no clear icon is recorded.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the press handler and must outlive the entity.
		 * @param entityIdentifier The search entity.
		 */
		static void
			installClearIcon(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Make the trailing icon a button that calls
		 * pressTrailingIcon().
		 *
		 * Adds the interaction components to the trailing icon recorded in
		 * components::Search. Its accessible name is the name of the
		 * trailing glyph at the time of the call. Call it once the icon
		 * entity exists. Does nothing when no trailing icon is recorded.
		 * @param componentRegistry The registry holding the components. It
		 * is captured by the press handler and must outlive the entity.
		 * @param entityIdentifier The search entity.
		 */
		static void installTrailingIcon(
			ecs::ComponentRegistry &componentRegistry,
			const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Style one search field and lay out its children.
		 *
		 * This is what update() runs each frame. Call it directly after
		 * changing components::Search to see the result before the next
		 * frame, which is what the setters of entities::Search do.
		 *
		 * The SearchBar variant is a pill of components::Search::BarHeight;
		 * the FullScreen variant is a square-cornered header of
		 * components::Search::ViewHeight with a divider under it. The
		 * container color follows the elevation level of the field.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @throws ecs::EntityComponentNotFoundException When the entity lacks
		 * one of the components listed in install().
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Get the query the field holds.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @return The content of the query child when it exists and has a
		 * components::Text, otherwise the query recorded in
		 * components::Search. Empty when nothing is typed.
		 */
		static std::string
			text(const ecs::ComponentRegistry &componentRegistry,
				 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Replace the query and restyle the field.
		 *
		 * Writes the query to components::Search and to the query child
		 * when it exists, then calls apply() so the placeholder and the
		 * clear icon follow. No handler is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @param text The new query; an empty string shows the placeholder
		 * again.
		 */
		static void setText(ecs::ComponentRegistry &componentRegistry,
							const ecs::Entity::Identifier &entityIdentifier,
							const std::string &text);

		/**
		 * @brief Empty the query as a press on the clear icon would.
		 *
		 * Calls setText() with an empty string, then the clear handler of
		 * components::Search when there is one.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void clear(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the leading icon as a press on it would.
		 *
		 * Calls the icon press handler of components::Search when there is
		 * one.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void pressIcon(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Activate the trailing icon as a press on it would.
		 *
		 * Calls the trailing icon press handler of components::Search when
		 * there is one.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void
			pressTrailingIcon(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Show the search field.
		 *
		 * Sets the `visible` flag of its components::Overlay. The field is
		 * not restyled here; that happens on the next frame.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void show(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Hide the search field.
		 *
		 * Clears the `visible` flag of its components::Overlay. The query is
		 * kept and no handler is called.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 */
		static void hide(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Whether the search field is shown.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The search entity.
		 * @return The `visible` flag of its components::Overlay.
		 */
		static bool isVisible(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Run apply() on one search field; called by the frame loop.
		 *
		 * Logs a warning and does nothing when the entity has no
		 * components::Search.
		 * @param entityIdentifier The search entity.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
