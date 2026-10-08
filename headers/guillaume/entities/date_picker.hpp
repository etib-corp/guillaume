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

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/overlay.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief DatePicker entity: a calendar overlay selecting a day or range.
	 *
	 * The picker reuses `SurfaceBase` for the panel surface and
	 * `components::Overlay` for the transient overlay behaviour, plus
	 * `components::SelectionGroup`/`components::Selectable` for the day grid.
	 * The variants are:
	 * - `SingleDate`: selecting a day replaces the current selection.
	 * - `DateRange`: two successive selections define a start and end day.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 */
	class DatePicker:
		public SurfaceBase<components::Overlay, components::SelectionGroup>
	{
		public:
		/**
		 * @brief A calendar date.
		 */
		struct Date {
			int year { 2026 };	  ///< Year.
			int month { 1 };	  ///< Month (1-12).
			int day { 1 };		  ///< Day (1-31).
		};

		/**
		 * @brief DatePicker variant.
		 */
		enum class Variant { SingleDate, DateRange };

		/**
		 * @brief Builder used to configure and create `DatePicker` entities.
		 */
		class Builder: public EntityBuilderBase<DatePicker>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SingleDate };	 ///< Picker variant.
			int _year { 2026 };							 ///< Initial year.
			int _month { 1 };							 ///< Initial month.
			std::function<void(int)> _onSelect {};		 ///< Selection handler.
			std::function<void(int, int)> _onRange {};	  ///< Range handler.

			public:
			/**
			 * @brief Construct a new DatePicker Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the DatePicker entity from the current
			 * configuration.
			 * @return The newly created DatePicker.
			 */
			std::shared_ptr<DatePicker> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the picker pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the picker variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the initial year.
			 * @param year The new year.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withYear(int year);

			/**
			 * @brief Set the initial month.
			 * @param month The new month.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMonth(int month);

			/**
			 * @brief Set the single date selection handler.
			 * @param onSelect The handler invoked with the selected day.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnSelect(const std::function<void(int)> &onSelect);

			/**
			 * @brief Set the range selection handler.
			 * @param onRange The handler invoked with the start and end days.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnRange(const std::function<void(int, int)> &onRange);
		};

		/**
		 * @brief Director that orchestrates `DatePicker::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a date picker using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The picker variant.
			 * @param year The initial year.
			 * @param month The initial month.
			 * @return The newly created date picker.
			 */
			std::shared_ptr<DatePicker>
				makeDatePicker(Builder &builder,
							   std::shared_ptr<ecs::Entity> parent,
							   Variant variant, int year, int month);
		};

		private:
		/**
		 * @brief Internal selectable day cell.
		 */
		class DayCell;

		private:
		Variant _variant { Variant::SingleDate };	  ///< Picker variant.
		Date _start {};								  ///< Start date.
		Date _end {};								  ///< End date.
		int _year { 2026 };							  ///< Displayed year.
		int _month { 1 };							  ///< Displayed month.
		std::function<void(int)> _onSelect {};		  ///< Selection handler.
		std::function<void(int, int)> _onRange {};	  ///< Range handler.
		std::vector<std::shared_ptr<ecs::Entity>>
			_dayCells {};					 ///< Day cell children.
		std::shared_ptr<Text> _header {};	 ///< Month/year header.
		std::vector<std::shared_ptr<ecs::Entity>> _rows {};	   ///< Week rows.
		bool _built { false };			 ///< Content build flag.
		bool _rangeStarted { false };	 ///< Range in progress.

		private:
		/**
		 * @brief Apply the variant-specific surface configuration and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for the picker.
		 */
		void applyOverlay(void);

		/**
		 * @brief Build the header and the day cell grid.
		 */
		void buildCalendar(void);

		/**
		 * @brief Select a day cell through the selection system.
		 * @param identifier The day cell entity identifier to select.
		 */
		void selectCell(ecs::Entity::Identifier identifier);

		/**
		 * @brief Refresh the month/year header label.
		 */
		void refreshHeader(void);

		public:
		/**
		 * @brief Construct a DatePicker entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The picker variant.
		 * @param year The initial year.
		 * @param month The initial month.
		 * @param onSelect The single date selection handler.
		 * @param onRange The range selection handler.
		 */
		DatePicker(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant, int year,
				   int month, const std::function<void(int)> &onSelect,
				   const std::function<void(int, int)> &onRange);

		/**
		 * @brief Default destructor.
		 */
		~DatePicker(void) override;

		/**
		 * @brief Set the picker variant.
		 * @param variant The new variant.
		 * @return Reference to this picker for chaining.
		 */
		DatePicker &setVariant(Variant variant);

		/**
		 * @brief Get the picker variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the displayed year.
		 * @param year The new year.
		 * @return Reference to this picker for chaining.
		 */
		DatePicker &setYear(int year);

		/**
		 * @brief Get the displayed year.
		 * @return The year.
		 */
		int getYear(void) const;

		/**
		 * @brief Set the displayed month.
		 * @param month The new month (1-12).
		 * @return Reference to this picker for chaining.
		 */
		DatePicker &setMonth(int month);

		/**
		 * @brief Get the displayed month.
		 * @return The month.
		 */
		int getMonth(void) const;

		/**
		 * @brief Get the currently selected day.
		 * @return The selected day.
		 */
		int getSelectedDay(void) const;

		/**
		 * @brief Get the selected range start day.
		 * @return The start day.
		 */
		int getSelectedStartDay(void) const;

		/**
		 * @brief Get the selected range end day.
		 * @return The end day.
		 */
		int getSelectedEndDay(void) const;

		/**
		 * @brief Get the number of day cells.
		 * @return The day cell count.
		 */
		std::size_t getDayCellCount(void) const;

		/**
		 * @brief Get the identifier of a day cell.
		 * @param index The index of the day cell.
		 * @return The day cell identifier, or InvalidIdentifier when out of
		 * range.
		 */
		ecs::Entity::Identifier getDayCellIdentifier(std::size_t index) const;

		/**
		 * @brief Select a day programmatically.
		 * @param day The day to select.
		 * @return Reference to this picker for chaining.
		 */
		DatePicker &select(int day);

		/**
		 * @brief Show the picker.
		 * @return Reference to this picker for chaining.
		 */
		DatePicker &show(void);

		/**
		 * @brief Hide the picker.
		 * @return Reference to this picker for chaining.
		 */
		DatePicker &hide(void);

		/**
		 * @brief Whether the picker is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Open (show) the entity.
		 * @return Reference to this entity for chaining.
		 */
		DatePicker &open(void);

		/**
		 * @brief Close (hide) the entity.
		 * @return Reference to this entity for chaining.
		 */
		DatePicker &close(void);

		/**
		 * @brief Whether the entity is open.
		 * @return True when visible.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Initialize the picker derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the picker derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
