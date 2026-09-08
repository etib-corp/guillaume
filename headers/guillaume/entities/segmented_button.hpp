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
#include <vector>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/borders.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Segmented button entity grouping multiple toggle buttons.
	 */
	class SegmentedButton:
		public std::enable_shared_from_this<SegmentedButton>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound>
	{
		public:
		/**
		 * @brief Selection mode of the segmented button.
		 */
		enum class SelectionMode { Single, Multi };

		/**
		 * @brief Builder used to configure and create `SegmentedButton`
		 * entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<SegmentedButton>
				_segmentedButton;	 ///< Segmented button entity being
									 ///< built
			std::vector<std::string> _labels;	 ///< Labels of the segments
			SelectionMode _selectionMode;		 ///< Selection mode
			std::function<void(std::size_t)>
				_onChange;	  ///< Change event handler (selected index)

			public:
			/**
			 * @brief Construct a new SegmentedButton Builder object.
			 * @param componentRegistry The component registry used to build
			 * entities.
			 * @param entityRegistry The entity registry used to build entities.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the SegmentedButton Builder
			 * class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the segmented button entity.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @return A shared pointer to the newly created connected button
			 * group entity.
			 */
			std::shared_ptr<SegmentedButton>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state.
			 */
			void reset(void) override;

			/**
			 * @brief Set the labels of the segments.
			 * @param labels The labels of the segments.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabels(const std::vector<std::string> &labels);

			/**
			 * @brief Set the selection mode.
			 * @param selectionMode The selection mode to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelectionMode(const SelectionMode &selectionMode);

			/**
			 * @brief Set the change event handler.
			 * @param onChange The change event handler (selected index).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnChange(std::function<void(std::size_t)> onChange);
		};

		/**
		 * @brief Director that orchestrates `SegmentedButton::Builder`.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new SegmentedButton Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the SegmentedButton Director
			 * class.
			 */
			~Director(void);

			/**
			 * @brief Create a segmented button entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the segmented button.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @param labels The labels of the segments.
			 * @param selectionMode The selection mode.
			 * @param onChange The change event handler.
			 * @return A shared pointer to the newly created connected button
			 * group entity.
			 */
			std::shared_ptr<SegmentedButton>
				makeSegmentedButton(Builder &builder,
									std::shared_ptr<Entity> parent,
									const std::vector<std::string> &labels,
									SelectionMode selectionMode,
									std::function<void(std::size_t)> onChange);
		};

		private:
		std::vector<std::string> _labels {};	///< Labels of the segments
		SelectionMode _selectionMode {
			SelectionMode::Single
		};	  ///< Selection mode
		std::vector<std::shared_ptr<ButtonBase>>
			_segments;	  ///< Internal child button entities
		std::function<void(std::size_t)>
			_onChange {};	 ///< Change event handler (selected index)

		private:
		/**
		 * @brief Change event handler for a segment.
		 * @param index The index of the segment that changed.
		 */
		void segmentChangeHandler(std::size_t index);

		/**
		 * @brief Lay out the segments in a connected row and apply connected
		 * corner radii.
		 */
		void layoutSegments(void);

		public:
		/**
		 * @brief Default constructor for the SegmentedButton entity.
		 * @param registry Reference to the component registry.
		 * @param labels The labels of the segments.
		 * @param selectionMode The selection mode.
		 * @param onChange The change event handler.
		 */
		SegmentedButton(ecs::ComponentRegistry &registry,
						const std::vector<std::string> &labels,
						SelectionMode selectionMode,
						std::function<void(std::size_t)> onChange);

		/**
		 * @brief Default destructor for the SegmentedButton entity.
		 */
		~SegmentedButton(void);

		/**
		 * @brief Select a segment by index.
		 * @param index The index of the segment to select.
		 * @return Reference to this SegmentedButton for chaining.
		 */
		SegmentedButton &select(std::size_t index);

		/**
		 * @brief Deselect a segment by index.
		 * @param index The index of the segment to deselect.
		 * @return Reference to this SegmentedButton for chaining.
		 */
		SegmentedButton &deselect(std::size_t index);

		/**
		 * @brief Get whether a segment is selected.
		 * @param index The index of the segment to check.
		 * @return True when the segment is selected, false otherwise.
		 */
		bool isSelected(std::size_t index) const;

		/**
		 * @brief Get the indices of all selected segments.
		 * @return A vector of selected segment indices.
		 */
		std::vector<std::size_t> getSelectedIndices(void) const;

		/**
		 * @brief Get the number of segments.
		 * @return The number of segments.
		 */
		std::size_t getSegmentCount(void) const;

		/**
		 * @brief Get the identifier of a segment child entity.
		 * @param index The index of the segment.
		 * @return The segment entity identifier, or InvalidIdentifier when out
		 * of range.
		 */
		ecs::Entity::Identifier getSegmentIdentifier(std::size_t index) const;

		/**
		 * @brief Initialize the segmented button entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the segmented button entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
