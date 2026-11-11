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

#include <utility>
#include <functional>
#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/search.hpp"
#include "guillaume/components/text_field.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A search field: a leading icon, an editable query with a
	 * placeholder, a clear button and an optional trailing icon (Paper
	 * `Searchbar`).
	 *
	 * Its state lives in components::Search (variant, placeholder, query,
	 * glyphs, loading flag, handlers, child identifiers); whether it is
	 * shown lives in components::Overlay and its elevation in
	 * components::Elevation. systems::Search styles the field and lays out
	 * the children its state calls for each frame, in the Layout phase.
	 * The class itself holds no field: it builds the child entities, and
	 * its setters write the components and restyle at once.
	 *
	 * @code
	 * Search::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto search = builder.makeSearch(parent, Search::Variant::SearchBar,
	 * 	"Search messages");
	 * search->setOnClearIconPress([] {
	 * 	// called after the query was cleared
	 * });
	 * search->open();
	 * std::string query = search->getText();
	 * @endcode
	 *
	 * `Search::create(registry, config, variant, placeholder, text)` builds
	 * the same widget without registering it in an entity registry.
	 *
	 * Things to know:
	 * - `Variant::SearchBar` is a 56 dp high pill; `Variant::FullScreen`
	 *   (alias `View`) is 72 dp high, without rounded corners, with a
	 *   divider under it. The height, shape, divider, color, padding and
	 *   spacing are overwritten each frame. The width is the fixed width of
	 *   the surface configuration, 360 dp by default.
	 * - The placeholder shows while the query is empty. The clear button
	 *   works while the query is not empty; the loading glyph replaces it
	 *   while isLoading() is true.
	 * - The user edits the query child directly: read the query with
	 *   getText(). The widget has no handler called when the query
	 *   changes; setText() calls no handler either.
	 * - isOpen() is false at creation: the field's components::Overlay
	 *   starts not visible until open() or show().
	 * - The field is the entity registry that owns its children; they are
	 *   built once and kept, so their identifiers stay valid.
	 *
	 * @see components::Search, systems::Search, TextField
	 */
	class Search:
		public SurfaceBase<components::Overlay, components::TextField,
						   components::Elevation, components::Line,
						   components::Focus, components::Accessibility,
						   components::Search>
	{
		public:
		using Variant = components::Search::Variant;	///< Search variant.

		/**
		 * @brief Configures a Search, creates it and registers it in an
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Search>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SearchBar };	///< Search variant.
			std::string _placeholder {};				///< Placeholder text.
			std::string _text {};						///< Query text.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the search fields built.
			 * @param entityRegistry The registry the search fields are
			 * added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a search field and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new search field.
			 */
			std::shared_ptr<Search> buildEntity(void) override;

			/**
			 * @brief Restore the defaults.
			 *
			 * Defaults: `SearchBar` variant, empty placeholder, empty
			 * query, default pose, 360 dp wide
			 * (components::Search::DefaultWidth), 56 dp high
			 * (components::Search::BarHeight).
			 */
			void reset(void) override;

			/**
			 * @brief Set where the search field is placed.
			 * @param pose The pose of the field. Default: a
			 * default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant `SearchBar` (the default) for the 56 dp pill,
			 * `FullScreen` for the 72 dp field with a divider.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the placeholder: the hint shown while the query is
			 * empty.
			 * @param placeholder The placeholder text; it is also the
			 * accessible name of the field. Default: empty.
			 * @return This builder, for chaining.
			 */
			Builder &withPlaceholder(const std::string &placeholder);

			/**
			 * @brief Set the initial query.
			 * @param text The query the field starts with. Default: empty.
			 * @return This builder, for chaining.
			 */
			Builder &withText(const std::string &text);

			/**
			 * @brief Build a search field, register it under `parent` and
			 * reset the builder.
			 * @param parent The entity the field becomes a child of, or
			 * nullptr for a root entity.
			 * @param variant The variant of the field.
			 * @param placeholder The hint shown while the query is empty.
			 * @param text The query the field starts with.
			 * @return The new search field.
			 */
			std::shared_ptr<Search>
				makeSearch(std::shared_ptr<ecs::Entity> parent, Variant variant,
						   const std::string &placeholder,
						   const std::string &text = "");
		};

		/**
		 * @brief Create a search field and its children, without
		 * registering it in an entity registry.
		 *
		 * The constructor is private because the children need the field
		 * to be owned by a shared pointer before they can take it as
		 * parent; create() constructs the field, then builds the icons,
		 * the query text and the placeholder.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to the private constructor:
		 * (registry, config, variant, placeholder, text).
		 * @return The new search field.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Search> create(Arguments &&...arguments)
		{
			std::shared_ptr<Search> entity(
				new Search(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Search(void) override;

		/**
		 * @brief Change the variant. Restyles at once.
		 * @param variant `SearchBar` for the 56 dp pill, `FullScreen` for
		 * the 72 dp field with a divider.
		 * @return This search field, for chaining.
		 */
		Search &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::Search.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the placeholder: the hint shown while the query is
		 * empty.
		 *
		 * Builds the placeholder child the first time the text is not
		 * empty, then restyles at once.
		 * @param placeholder The new placeholder text; it is also the
		 * accessible name of the field.
		 * @return This search field, for chaining.
		 */
		Search &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Replace the query. Restyles at once.
		 *
		 * Calls no handler.
		 * @param text The new query.
		 * @return This search field, for chaining.
		 */
		Search &setText(const std::string &text);

		/**
		 * @brief Get the query, including what the user typed.
		 * @return The content of the query child, or the query stored in
		 * components::Search before that child is built.
		 */
		std::string getText(void) const;

		/**
		 * @brief Empty the query, as the clear button does, then call the
		 * clear handler.
		 * @return This search field, for chaining.
		 */
		Search &clear(void);

		/**
		 * @brief Change the glyph of the leading icon (Paper `icon`).
		 * Restyles at once.
		 * @param iconGlyph The glyph name of the icon. The default is
		 * components::Search::SearchGlyph (`"search"`).
		 * @return This search field, for chaining.
		 */
		Search &setIcon(const std::string &iconGlyph);

		/**
		 * @brief Set the handler called when the user presses the leading
		 * icon (Paper `onIconPress`).
		 * @param onIconPress The handler; an empty function for none.
		 * @return This search field, for chaining.
		 */
		Search &setOnIconPress(std::function<void(void)> onIconPress);

		/**
		 * @brief Set the handler called after the query was cleared, by
		 * the clear button or by clear() (Paper `onClearIconPress`).
		 * @param onClearIconPress The handler; an empty function for none.
		 * @return This search field, for chaining.
		 */
		Search &setOnClearIconPress(std::function<void(void)> onClearIconPress);

		/**
		 * @brief Set or hide the trailing icon (Paper `right` and
		 * `traileringIcon`).
		 *
		 * Builds the trailing icon child the first time the glyph is not
		 * empty, then restyles at once.
		 * @param iconGlyph The glyph name of the icon; an empty string
		 * hides the icon and disables its presses.
		 * @param onPress Called when the user presses the trailing icon;
		 * it replaces the previous handler, even when left empty.
		 * @return This search field, for chaining.
		 */
		Search &setTrailingIcon(const std::string &iconGlyph,
								std::function<void(void)> onPress = {});

		/**
		 * @brief Show the loading glyph in place of the clear button, or
		 * the reverse. Restyles at once.
		 * @param loading True while a search is in progress.
		 * @return This search field, for chaining.
		 */
		Search &setLoading(bool loading);

		/**
		 * @brief Whether the loading glyph replaces the clear button.
		 * @return True while loading.
		 */
		bool isLoading(void) const;

		/**
		 * @brief Set the elevation level (Paper `elevation`). Restyles at
		 * once.
		 *
		 * Writes the field's components::Elevation. The fill color follows:
		 * the surface-container-high color at level 0, the tinted elevated
		 * surface color above.
		 * @param level The elevation level. Default: level 0.
		 * @return This search field, for chaining.
		 */
		Search &setElevation(theme::ElevationLevel level);

		/**
		 * @brief Get the leading icon: a pressable icon entity.
		 * @return The identifier of the icon, or
		 * ecs::Entity::InvalidIdentifier when it was not built (the leading
		 * glyph was empty).
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Get the clear button: a pressable icon entity.
		 * @return The identifier of the icon, or
		 * ecs::Entity::InvalidIdentifier before the children are built.
		 */
		ecs::Entity::Identifier getClearIdentifier(void) const;

		/**
		 * @brief Get the placeholder text entity.
		 * @return The identifier of the text, or
		 * ecs::Entity::InvalidIdentifier when the placeholder has always
		 * been empty.
		 */
		ecs::Entity::Identifier getPlaceholderIdentifier(void) const;

		/**
		 * @brief Get the query text entity: the text the user edits.
		 * @return The identifier of the text, or
		 * ecs::Entity::InvalidIdentifier before the children are built.
		 */
		ecs::Entity::Identifier getTextIdentifier(void) const;

		/**
		 * @brief Show the search field: set its components::Overlay
		 * visible.
		 * @return This search field, for chaining.
		 */
		Search &show(void);

		/**
		 * @brief Hide the search field: set its components::Overlay not
		 * visible.
		 * @return This search field, for chaining.
		 */
		Search &hide(void);

		/**
		 * @brief Whether the search field is shown.
		 * @return True when its components::Overlay is visible; false at
		 * creation.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Open the search field. Same as show().
		 * @return This search field, for chaining.
		 */
		Search &open(void);

		/**
		 * @brief Close the search field. Same as hide().
		 * @return This search field, for chaining.
		 */
		Search &close(void);

		/**
		 * @brief Whether the search field is open. Same as isVisible().
		 * @return True when shown; false at creation.
		 */
		bool isOpen(void) const;

		private:
		/**
		 * @brief Create the icons, the editable text and the placeholder
		 * that the configuration calls for and that do not exist yet, record
		 * their identifiers in components::Search, make the icons pressable
		 * and hand the children to the surface.
		 *
		 * Existing children are kept. Does nothing until the search field
		 * is owned by a shared pointer.
		 */
		void buildChildren(void);

		private:
		/**
		 * @brief Construct a search field.
		 *
		 * Writes the configuration to components::Search, sets the
		 * elevation to level 0 and styles the field. The children are built
		 * by build().
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose, fixed width).
		 * @param variant The variant.
		 * @param placeholder The hint shown while the query is empty.
		 * @param text The query the field starts with.
		 */
		Search(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &placeholder,
			   const std::string &text);

		/**
		 * @brief Build the child entities, then style the field.
		 *
		 * Called once by create(), when the field is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
