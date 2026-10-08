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
#include "guillaume/components/value.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief TimePicker entity: an input or clock face overlay selecting a
	 * time.
	 *
	 * The picker reuses `SurfaceBase` for the panel surface,
	 * `components::Overlay` for the transient overlay behaviour and
	 * `components::Value` to hold the time encoded as minutes since midnight.
	 * The variants are:
	 * - `Input`: two text fields showing the hour and minute.
	 * - `Clock`: twelve hour markers arranged around a circle.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see components::Value
	 */
	class TimePicker: public SurfaceBase<components::Overlay, components::Value>
	{
		public:
		/**
		 * @brief TimePicker variant.
		 */
		enum class Variant { Input, Clock };

		/**
		 * @brief Builder used to configure and create `TimePicker` entities.
		 */
		class Builder: public EntityBuilderBase<TimePicker>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Input };	///< Picker variant.
			int _hour { 0 };						///< Initial hour.
			int _minute { 0 };						///< Initial minute.
			std::function<void(int, int)> _onChange {};	   ///< Change handler.

			public:
			/**
			 * @brief Construct a new TimePicker Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the TimePicker entity from the current
			 * configuration.
			 * @return The newly created TimePicker.
			 */
			std::shared_ptr<TimePicker> buildEntity(void) override;

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
			 * @brief Set the initial hour.
			 * @param hour The new hour (0-23).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withHour(int hour);

			/**
			 * @brief Set the initial minute.
			 * @param minute The new minute (0-59).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMinute(int minute);

			/**
			 * @brief Set the time change handler.
			 * @param onChange The handler invoked with the new hour and minute.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withOnChange(const std::function<void(int, int)> &onChange);
		};

		/**
		 * @brief Director that orchestrates `TimePicker::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a time picker using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The picker variant.
			 * @param hour The initial hour.
			 * @param minute The initial minute.
			 * @return The newly created time picker.
			 */
			std::shared_ptr<TimePicker>
				makeTimePicker(Builder &builder,
							   std::shared_ptr<ecs::Entity> parent,
							   Variant variant, int hour, int minute);
		};

		private:
		Variant _variant { Variant::Input };		   ///< Picker variant.
		int _hour { 0 };							   ///< Current hour.
		int _minute { 0 };							   ///< Current minute.
		std::function<void(int, int)> _onChange {};	   ///< Change handler.
		std::vector<std::shared_ptr<ecs::Entity>>
			_markers {};					  ///< Clock markers.
		std::shared_ptr<Text> _hourText;	  ///< Hour text child.
		std::shared_ptr<Text> _minuteText;	  ///< Minute text child.
		std::shared_ptr<Text> _colonText;	  ///< Colon text child.
		bool _built { false };				  ///< Content build flag.
		bool _updating { false };			  ///< Reentrancy guard.

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
		 * @brief Configure the value component range and handler.
		 */
		void applyValue(void);

		/**
		 * @brief Build the input fields or the clock markers.
		 */
		void buildContent(void);

		/**
		 * @brief Place the clock markers around the circle.
		 */
		void applyMarkerPlacement(void);

		/**
		 * @brief Refresh the hour and minute text contents.
		 */
		void refreshTexts(void);

		public:
		/**
		 * @brief Construct a TimePicker entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The picker variant.
		 * @param hour The initial hour.
		 * @param minute The initial minute.
		 * @param onChange The time change handler.
		 */
		TimePicker(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant, int hour,
				   int minute, const std::function<void(int, int)> &onChange);

		/**
		 * @brief Default destructor.
		 */
		~TimePicker(void) override;

		/**
		 * @brief Set the picker variant.
		 * @param variant The new variant.
		 * @return Reference to this picker for chaining.
		 */
		TimePicker &setVariant(Variant variant);

		/**
		 * @brief Get the picker variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the hour.
		 * @param hour The new hour (0-23).
		 * @return Reference to this picker for chaining.
		 */
		TimePicker &setHour(int hour);

		/**
		 * @brief Get the hour.
		 * @return The hour.
		 */
		int getHour(void) const;

		/**
		 * @brief Set the minute.
		 * @param minute The new minute (0-59).
		 * @return Reference to this picker for chaining.
		 */
		TimePicker &setMinute(int minute);

		/**
		 * @brief Get the minute.
		 * @return The minute.
		 */
		int getMinute(void) const;

		/**
		 * @brief Set the time.
		 * @param hour The new hour (0-23).
		 * @param minute The new minute (0-59).
		 * @return Reference to this picker for chaining.
		 */
		TimePicker &setTime(int hour, int minute);

		/**
		 * @brief Get the time as minutes since midnight.
		 * @return The value in minutes.
		 */
		int getValue(void) const;

		/**
		 * @brief Show the picker.
		 * @return Reference to this picker for chaining.
		 */
		TimePicker &show(void);

		/**
		 * @brief Hide the picker.
		 * @return Reference to this picker for chaining.
		 */
		TimePicker &hide(void);

		/**
		 * @brief Whether the picker is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Open (show) the entity.
		 * @return Reference to this entity for chaining.
		 */
		TimePicker &open(void);

		/**
		 * @brief Close (hide) the entity.
		 * @return Reference to this entity for chaining.
		 */
		TimePicker &close(void);

		/**
		 * @brief Whether the entity is open.
		 * @return True when visible.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Get the number of clock markers.
		 * @return The marker count.
		 */
		std::size_t getMarkerCount(void) const;

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
