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
#include <vector>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/segmented_button.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A row of joined toggle buttons (segments) from which the user
	 * picks one option, or several.
	 *
	 * Its state lives in components::SegmentedButton (segment options,
	 * selection mode, callback, child identifiers). Each segment is a Button
	 * child, and whether a segment is selected is the selection of that
	 * button. systems::SegmentedButton gives a selected segment a check
	 * icon, gives every segment the same width, fills the row and joins the
	 * corners each frame, in the Layout phase. The class itself holds no
	 * field: its methods call the system.
	 *
	 * @code
	 * SegmentedButton::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto view = builder.makeSegmentedButton(parent,
	 * 	{ "Day", "Week", "Month" }, SegmentedButton::SelectionMode::Single,
	 * 	[](std::size_t index) {
	 * 		// called when the user presses the segment at index
	 * 	});
	 * view->select(0);
	 * @endcode
	 *
	 * `SegmentedButton::create(registry, labels, selectionMode, onChange)`,
	 * or the same call with a `std::vector<Segment>` in place of the
	 * labels, builds the same widget without registering it in an entity
	 * registry.
	 *
	 * The segmented button owns its segment buttons: it is the entity
	 * registry they are registered in. The segments are fixed at creation
	 * and none is selected at first. select() and deselect() do not call the
	 * change handler; only a user press does. The click handler, the
	 * minimum width and the corners of the segment buttons, and the
	 * Layout configuration of the row, are set by the system; do not
	 * replace them.
	 *
	 * @see components::SegmentedButton, systems::SegmentedButton, Button
	 */
	class SegmentedButton:
		public std::enable_shared_from_this<SegmentedButton>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound,
									   components::Layout,
									   components::SegmentedButton>
	{
		public:
		/// Selection mode: Single (one segment at most) or Multi (see
		/// components::SegmentedButton::SelectionMode).
		using SelectionMode = components::SegmentedButton::SelectionMode;

		/// Options of one segment: label, icon, disabled flag and
		/// accessibility label (Paper `buttons[]` entry).
		using Segment = components::SegmentedButton::Segment;

		/**
		 * @brief Minimum width of a segment, in dp (Paper SegmentedButtons:
		 * 76 dp).
		 */
		static constexpr float MinSegmentWidth =
			components::SegmentedButton::MinSegmentWidth;

		/**
		 * @brief Configures, creates and registers SegmentedButton widgets.
		 *
		 * Describe the segments with withLabels() or withSegments(), then
		 * call registerEntity(), or use makeSegmentedButton() to do both in
		 * one call.
		 */
		class Builder: public EntityBuilderBase<SegmentedButton>
		{
			private:
			std::shared_ptr<SegmentedButton>
				_segmentedButton;	 ///< Not used by the builder
			std::vector<std::string>
				_labels;	///< Labels of the segments, used when
							///< _segmentConfigs is empty
			SelectionMode _selectionMode;	 ///< Selection mode
			std::vector<Segment>
				_segmentConfigs;	///< Per-segment options; when not empty
									///< they are used in place of _labels
			std::function<void(std::size_t)>
				_onChange;	  ///< Handler called with the index of the
							  ///< segment the user pressed

			public:
			/**
			 * @brief Construct a builder with the default configuration (see
			 * reset()).
			 * @param componentRegistry The registry that stores the
			 * components of the segmented buttons to build.
			 * @param entityRegistry The registry the built segmented buttons
			 * are added to (usually the scene's).
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a SegmentedButton, with one button per segment,
			 * from the current configuration, without registering it.
			 *
			 * The segments come from withSegments() when it was given a
			 * non-empty list, from withLabels() otherwise. registerEntity()
			 * calls this function, then sets the parent, registers the
			 * segmented button and resets the builder.
			 * @return The new segmented button.
			 */
			std::shared_ptr<SegmentedButton> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: no label, no segment configuration,
			 * SelectionMode::Single, no change handler.
			 */
			void reset(void) override;

			/**
			 * @brief Describe the segments by their labels only.
			 * @param labels One label per segment, in display order. Each
			 * segment has no icon and is enabled. Default: empty. Ignored
			 * when withSegments() was given a non-empty list.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabels(const std::vector<std::string> &labels);

			/**
			 * @brief Set the selection mode.
			 * @param selectionMode SelectionMode::Single to keep at most one
			 * segment selected, SelectionMode::Multi to let each segment
			 * toggle on its own. Default: SelectionMode::Single.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelectionMode(const SelectionMode &selectionMode);

			/**
			 * @brief Set the change handler.
			 * @param onChange The function called with the index of the
			 * segment the user pressed, after the selection was updated.
			 * Default: empty, nothing is called.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnChange(std::function<void(std::size_t)> onChange);

			/**
			 * @brief Describe the segments one by one (label, icon, disabled,
			 * accessibility label).
			 * @param segments One configuration per segment, in display
			 * order. When not empty, it is used in place of the labels given
			 * to withLabels(). Default: empty.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSegments(const std::vector<Segment> &segments);

			/**
			 * @brief Build a segmented button from labels, register it under
			 * `parent` and reset the builder.
			 *
			 * Segments given earlier to withSegments() still take the place
			 * of @p labels.
			 * @param parent The entity the segmented button becomes a child
			 * of.
			 * @param labels One label per segment, in display order.
			 * @param selectionMode Single or Multi selection.
			 * @param onChange The function called with the index of the
			 * segment the user pressed (may be empty).
			 * @return The new segmented button, with no segment selected,
			 * registered in the entity registry.
			 */
			std::shared_ptr<SegmentedButton>
				makeSegmentedButton(std::shared_ptr<Entity> parent,
									const std::vector<std::string> &labels,
									SelectionMode selectionMode,
									std::function<void(std::size_t)> onChange);
		};

		/**
		 * @brief Enable or disable one segment. Restyles at once.
		 *
		 * A disabled segment ignores input and keeps its selection.
		 * @param index The index of the segment (0 is the first). Nothing
		 * happens when it is out of range.
		 * @param disabled True to disable the segment, false to enable it.
		 * @return Reference to this segmented button for chaining.
		 */
		SegmentedButton &setSegmentDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Create a segmented button and one button per segment,
		 * without registering it in an entity registry.
		 *
		 * The constructors are private because the segment buttons need a
		 * parent already held by a `std::shared_ptr`: create() constructs
		 * the segmented button then calls build(), which creates them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to a constructor:
		 * (registry, labels, selectionMode, onChange) with labels a
		 * `std::vector<std::string>`, or (registry, segments, selectionMode,
		 * onChange) with segments a `std::vector<Segment>`.
		 * @return The new segmented button, with no segment selected.
		 */
		template<typename... Arguments>
		static std::shared_ptr<SegmentedButton> create(Arguments &&...arguments)
		{
			std::shared_ptr<SegmentedButton> entity(
				new SegmentedButton(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~SegmentedButton(void);

		/**
		 * @brief Select a segment. Restyles at once.
		 *
		 * In single selection mode the other segments are deselected. The
		 * change handler is not called.
		 * @param index The index of the segment to select (0 is the first).
		 * Nothing happens when it is out of range.
		 * @return Reference to this segmented button for chaining.
		 */
		SegmentedButton &select(std::size_t index);

		/**
		 * @brief Deselect a segment. Restyles at once.
		 *
		 * The change handler is not called.
		 * @param index The index of the segment to deselect (0 is the
		 * first). Nothing happens when it is out of range.
		 * @return Reference to this segmented button for chaining.
		 */
		SegmentedButton &deselect(std::size_t index);

		/**
		 * @brief Get whether a segment is selected.
		 * @param index The index of the segment (0 is the first).
		 * @return True when the segment is selected, false when it is not or
		 * when @p index is out of range.
		 */
		bool isSelected(std::size_t index) const;

		/**
		 * @brief Get the indices of all selected segments.
		 * @return The indices in ascending order; empty when no segment is
		 * selected. At most one index in single selection mode.
		 */
		std::vector<std::size_t> getSelectedIndices(void) const;

		/**
		 * @brief Get the number of segments.
		 * @return The number of segment buttons.
		 */
		std::size_t getSegmentCount(void) const;

		/**
		 * @brief Get the identifier of the Button child of one segment.
		 * @param index The index of the segment (0 is the first).
		 * @return The identifier of the segment button, or
		 * ecs::Entity::InvalidIdentifier when @p index is out of range.
		 */
		ecs::Entity::Identifier getSegmentIdentifier(std::size_t index) const;

		private:
		/**
		 * @brief Construct a segmented button from labels, without its
		 * segment buttons; create() calls it then build().
		 * @param registry The registry that stores the widget's components.
		 * @param labels One label per segment, in display order. Each
		 * segment has no icon and is enabled.
		 * @param selectionMode Single or Multi selection.
		 * @param onChange The function called with the index of the segment
		 * the user pressed (may be empty).
		 */
		SegmentedButton(ecs::ComponentRegistry &registry,
						const std::vector<std::string> &labels,
						SelectionMode selectionMode,
						std::function<void(std::size_t)> onChange);

		/**
		 * @brief Construct a segmented button from segment configurations,
		 * without its segment buttons; create() calls it then build().
		 * @param registry The registry that stores the widget's components.
		 * @param segments One configuration per segment, in display order.
		 * @param selectionMode Single or Multi selection.
		 * @param onChange The function called with the index of the segment
		 * the user pressed (may be empty).
		 */
		SegmentedButton(ecs::ComponentRegistry &registry,
						const std::vector<Segment> &segments,
						SelectionMode selectionMode,
						std::function<void(std::size_t)> onChange);

		/**
		 * @brief Create one Button per segment, then wire their clicks and
		 * apply the segment options through
		 * systems::SegmentedButton::install.
		 *
		 * Called once by create(), when the segmented button is held by a
		 * `std::shared_ptr` and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
