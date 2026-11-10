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
#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/elevation.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/text_field.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Search entity (Paper `Searchbar`).
	 *
	 * Search reuses `SurfaceBase` for the layout, geometry and background
	 * surface, `components::Overlay` for the transient overlay behaviour and
	 * `components::TextField` to carry the placeholder/text state. It holds a
	 * pressable leading icon button, the editable query (or the placeholder),
	 * a clear button shown while the query is not empty (or a loading glyph),
	 * and an optional trailing icon button. The Material Design variants are:
	 * - `SearchBar`: a 56 dp pill-shaped inline search field (Paper `bar`).
	 * - `FullScreen` (alias `View`): a 72 dp unrounded inline field with a
	 *   bottom divider (Paper `view`).
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see components::TextField
	 */
	class Search:
		public SurfaceBase<components::Overlay, components::TextField,
						   components::Elevation, components::Line>
	{
		public:
		/**
		 * @brief Search variant.
		 */
		enum class Variant {
			SearchBar,			   ///< Pill-shaped 56 dp bar.
			FullScreen,			   ///< 72 dp inline view with a divider.
			View = FullScreen	   ///< Paper `view` mode name.
		};

		/**
		 * @brief Builder used to configure and create search fields.
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
			 * @brief Construct a search builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the search field.
			 * @return The search field.
			 */
			std::shared_ptr<Search> buildEntity(void) override;

			/**
			 * @brief Reset the builder to the M3 defaults.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose.
			 * @param pose The pose.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant The variant.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the placeholder.
			 * @param placeholder The placeholder.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPlaceholder(const std::string &placeholder);

			/**
			 * @brief Set the query.
			 * @param text The query.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withText(const std::string &text);
		};

		/**
		 * @brief Director building search fields.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Build a search field.
			 * @param builder Search builder.
			 * @param parent Parent entity.
			 * @param variant Search variant.
			 * @param placeholder Placeholder text.
			 * @param text Query text.
			 * @return The registered search field.
			 */
			std::shared_ptr<Search>
				makeSearch(Builder &builder,
						   std::shared_ptr<ecs::Entity> parent, Variant variant,
						   const std::string &placeholder,
						   const std::string &text = "");
		};

		/**
		 * @brief Construct a search field.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The search variant.
		 * @param placeholder The placeholder text.
		 * @param text The query text.
		 */
		Search(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &placeholder,
			   const std::string &text);

		/**
		 * @brief Default destructor.
		 */
		~Search(void) override;

		/**
		 * @brief Set the variant.
		 * @param variant The new variant.
		 * @return Reference to this search for chaining.
		 */
		Search &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the placeholder.
		 * @param placeholder The new placeholder.
		 * @return Reference to this search for chaining.
		 */
		Search &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Set the query.
		 * @param text The new query.
		 * @return Reference to this search for chaining.
		 */
		Search &setText(const std::string &text);

		/**
		 * @brief Get the query.
		 * @return The query.
		 */
		std::string getText(void) const;

		/**
		 * @brief Clear the query, as the clear button does, and call the
		 * clear handler.
		 * @return Reference to this search for chaining.
		 */
		Search &clear(void);

		/**
		 * @brief Replace the leading icon glyph (Paper `icon`).
		 * @param iconGlyph The icon glyph.
		 * @return Reference to this search for chaining.
		 */
		Search &setIcon(const std::string &iconGlyph);

		/**
		 * @brief Set the leading icon press handler (Paper `onIconPress`).
		 * @param onIconPress The handler.
		 * @return Reference to this search for chaining.
		 */
		Search &setOnIconPress(std::function<void(void)> onIconPress);

		/**
		 * @brief Set the clear press handler (Paper `onClearIconPress`).
		 * @param onClearIconPress The handler.
		 * @return Reference to this search for chaining.
		 */
		Search &setOnClearIconPress(std::function<void(void)> onClearIconPress);

		/**
		 * @brief Set an optional trailing icon (Paper `right`/`traileringIcon`).
		 * @param iconGlyph Trailing glyph ("" removes it on rebuild).
		 * @param onPress Trailing press handler.
		 * @return Reference to this search for chaining.
		 */
		Search &setTrailingIcon(const std::string &iconGlyph,
								std::function<void(void)> onPress = {});

		/**
		 * @brief Show the loading glyph in place of the clear button.
		 * @param loading Whether a search is in progress.
		 * @return Reference to this search for chaining.
		 */
		Search &setLoading(bool loading);

		/**
		 * @brief Check whether the loading glyph is shown.
		 * @return True while loading.
		 */
		bool isLoading(void) const;

		/**
		 * @brief Set the elevation level (Paper `elevation`, default 0).
		 * @param level The elevation level.
		 * @return Reference to this search for chaining.
		 */
		Search &setElevation(theme::ElevationLevel level);

		/**
		 * @brief Get the leading icon button identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getIconIdentifier(void) const;

		/**
		 * @brief Get the clear button identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getClearIdentifier(void) const;

		/**
		 * @brief Get the placeholder text identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getPlaceholderIdentifier(void) const;

		/**
		 * @brief Get the query text identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getTextIdentifier(void) const;

		/**
		 * @brief Show the search field.
		 * @return Reference to this search for chaining.
		 */
		Search &show(void);

		/**
		 * @brief Hide the search field.
		 * @return Reference to this search for chaining.
		 */
		Search &hide(void);

		/**
		 * @brief Check whether the search field is shown.
		 * @return True when shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Show the search field.
		 * @return Reference to this search for chaining.
		 */
		Search &open(void);

		/**
		 * @brief Hide the search field.
		 * @return Reference to this search for chaining.
		 */
		Search &close(void);

		/**
		 * @brief Check whether the search field is shown.
		 * @return True when shown.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Build the content.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle and re-arrange the search field.
		 */
		void update(void) override;

		private:
		Variant _variant { Variant::SearchBar };	///< Search variant.
		std::string _placeholder {};				///< Placeholder text.
		std::string _text {};						///< Query text.
		std::string _iconGlyph { "search" };		///< Leading glyph.
		std::string _trailingGlyph {};				///< Trailing glyph.
		bool _loading { false };					///< Loading state.
		std::function<void(void)> _onIconPress {};	  ///< Leading handler.
		std::function<void(void)>
			_onClearIconPress {};	 ///< Clear handler.
		std::function<void(void)>
			_onTrailingIconPress {};	///< Trailing handler.
		std::shared_ptr<Icon> _searchIcon;	 ///< Pressable leading icon.
		std::shared_ptr<Text> _textEntity;			///< Query child.
		std::shared_ptr<Text> _placeholderEntity;	 ///< Placeholder child.
		std::shared_ptr<Icon> _clearIcon;	///< Pressable clear icon.
		std::shared_ptr<Icon> _loadingIcon;			 ///< Loading glyph.
		std::shared_ptr<Icon> _trailingIcon;	 ///< Pressable trailing icon.

		/**
		 * @brief Apply the variant geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Apply the overlay flags.
		 */
		void applyOverlay(void);

		/**
		 * @brief Mirror the placeholder and query on the TextField component.
		 */
		void applyTextField(void);

		/**
		 * @brief Build or refresh the children.
		 */
		void buildContent(void);

		/**
		 * @brief Build a pressable icon child.
		 * @param glyph Icon glyph.
		 * @param label Accessibility label.
		 * @param onPress Press handler.
		 * @return The icon.
		 */
		std::shared_ptr<Icon> buildPressableIcon(const std::string &glyph,
												 const std::string &label,
												 std::function<void(void)> onPress);
	};

}	 // namespace guillaume::entities
