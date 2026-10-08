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

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/overlay.hpp"
#include "guillaume/components/text_field.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Search entity: an overlay text field with a leading search icon.
	 *
	 * Search reuses `SurfaceBase` for the layout, geometry and background
	 * surface, `components::Overlay` for the transient overlay behaviour and
	 * `components::TextField` to carry the placeholder/text state. The Material
	 * Design variants are:
	 * - `SearchBar`: a pill-shaped inline search field.
	 * - `FullScreen`: a full-height search overlay.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see components::TextField
	 */
	class Search: public SurfaceBase<components::Overlay, components::TextField>
	{
		public:
		/**
		 * @brief Search variant.
		 */
		enum class Variant { SearchBar, FullScreen };

		/**
		 * @brief Builder used to configure and create `Search` entities.
		 */
		class Builder: public EntityBuilderBase<Search>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SearchBar };	///< Search variant.
			std::string _placeholder {};				///< Placeholder text.
			std::string _text {};						///< Search text.

			public:
			/**
			 * @brief Construct a new Search Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Search entity from the current configuration.
			 * @return The newly created Search.
			 */
			std::shared_ptr<Search> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the search pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the search variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the placeholder text.
			 * @param placeholder The new placeholder.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPlaceholder(const std::string &placeholder);

			/**
			 * @brief Set the initial search text.
			 * @param text The new text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withText(const std::string &text);
		};

		/**
		 * @brief Director that orchestrates `Search::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a search using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The search variant.
			 * @param placeholder The placeholder text.
			 * @param text The initial search text.
			 * @return The newly created search.
			 */
			std::shared_ptr<Search>
				makeSearch(Builder &builder,
						   std::shared_ptr<ecs::Entity> parent, Variant variant,
						   const std::string &placeholder,
						   const std::string &text);
		};

		private:
		Variant _variant { Variant::SearchBar };	///< Search variant.
		std::string _placeholder {};				///< Placeholder text.
		std::string _text {};						///< Search text.
		std::shared_ptr<Icon> _searchIcon;			///< Leading search icon.
		std::shared_ptr<Text> _textEntity;			///< Search text child.

		private:
		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for the current variant.
		 */
		void applyOverlay(void);

		/**
		 * @brief Configure the text field component.
		 */
		void applyTextField(void);

		/**
		 * @brief Build the leading search icon and text child entities.
		 */
		void buildContent(void);

		public:
		/**
		 * @brief Construct a Search entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The search variant.
		 * @param placeholder The placeholder text.
		 * @param text The initial search text.
		 */
		Search(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &placeholder,
			   const std::string &text);

		/**
		 * @brief Default destructor.
		 */
		~Search(void) override;

		/**
		 * @brief Set the search variant.
		 * @param variant The new variant.
		 * @return Reference to this search for chaining.
		 */
		Search &setVariant(Variant variant);

		/**
		 * @brief Get the search variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the placeholder text.
		 * @param placeholder The new placeholder.
		 * @return Reference to this search for chaining.
		 */
		Search &setPlaceholder(const std::string &placeholder);

		/**
		 * @brief Set the search text.
		 * @param text The new text.
		 * @return Reference to this search for chaining.
		 */
		Search &setText(const std::string &text);

		/**
		 * @brief Get the search text.
		 * @return The search text.
		 */
		std::string getText(void) const;

		/**
		 * @brief Open the search overlay.
		 * @return Reference to this search for chaining.
		 */
		Search &open(void);

		/**
		 * @brief Close the search overlay.
		 * @return Reference to this search for chaining.
		 */
		Search &close(void);

		/**
		 * @brief Whether the search overlay is open.
		 * @return True when open.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Show the entity.
		 * @return Reference to this entity for chaining.
		 */
		Search &show(void);

		/**
		 * @brief Hide the entity.
		 * @return Reference to this entity for chaining.
		 */
		Search &hide(void);

		/**
		 * @brief Whether the entity is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Initialize the search derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the search derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
