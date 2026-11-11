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
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/radio_group.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/radio_button.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Group of mutually exclusive radio buttons (Paper
	 * `RadioButton.Group`).
	 *
	 * The group owns one RadioButton per option, stacked vertically on 40 dp
	 * rows, keeps at most one of them selected and exposes the
	 * `RadioGroup` accessibility role. Selecting an option through user
	 * activation calls the value-change handler with its index.
	 *
	 * The class is a facade: its state lives in components::RadioGroup and
	 * its rules in systems::RadioGroup.
	 */
	class RadioGroup: public SurfaceBase<components::RadioGroup>
	{
		public:
		/**
		 * @brief Builder for RadioGroup entities.
		 */
		class Builder: public EntityBuilderBase<RadioGroup>
		{
			private:
			SurfaceConfig _config;					 ///< Surface configuration.
			std::vector<std::string> _options {};	 ///< Option labels.
			std::optional<std::size_t> _value {};	 ///< Selected option.
			std::function<void(std::size_t)>
				_onValueChange {};	  ///< Value-change handler.

			public:
			/**
			 * @brief Construct a builder.
			 * @param componentRegistry Component registry.
			 * @param entityRegistry Entity registry receiving the group.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the group.
			 * @return The group entity.
			 */
			std::shared_ptr<RadioGroup> buildEntity(void) override;

			/**
			 * @brief Reset the builder to its defaults.
			 */
			void reset(void) override;

			/**
			 * @brief Set the group pose.
			 * @param pose Pose.
			 * @return *this.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Append an option.
			 * @param label Option label.
			 * @return *this.
			 */
			Builder &addOption(const std::string &label);

			/**
			 * @brief Select an option initially.
			 * @param index Option index.
			 * @return *this.
			 */
			Builder &withValue(std::size_t index);

			/**
			 * @brief Set the value-change handler.
			 * @param onValueChange Handler receiving the selected index.
			 * @return *this.
			 */
			Builder &withOnValueChange(
				std::function<void(std::size_t)> onValueChange);

			/**
			 * @brief Build a group of labelled options.
			 * @param parent Parent entity.
			 * @param options Option labels.
			 * @return The registered group.
			 */
			std::shared_ptr<RadioGroup>
				makeRadioGroup(std::shared_ptr<ecs::Entity> parent,
							   const std::vector<std::string> &options);
		};

		/**
		 * @brief Create the entity, fully built.
		 *
		 * Constructors are private: an entity must be owned before it can
		 * parent its children, so construction and child creation happen
		 * together here.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments.
		 * @return The new entity.
		 */
		template<typename... Arguments>
		static std::shared_ptr<RadioGroup> create(Arguments &&...arguments)
		{
			std::shared_ptr<RadioGroup> entity(
				new RadioGroup(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~RadioGroup(void) override = default;

		/**
		 * @brief Select an option programmatically (no handler call).
		 * @param index Option index.
		 * @return *this.
		 * @throws std::out_of_range If @p index is not an option.
		 */
		RadioGroup &setValue(std::size_t index);

		/**
		 * @brief Get the selected option.
		 * @return The selected index, or none.
		 */
		std::optional<std::size_t> getValue(void) const;

		/**
		 * @brief Get the number of options.
		 * @return The option count.
		 */
		std::size_t getOptionCount(void) const;

		/**
		 * @brief Get an option radio button.
		 * @param index Option index.
		 * @return The radio button, or nullptr before the group is built.
		 */
		std::shared_ptr<RadioButton> getOption(std::size_t index) const;

		/**
		 * @brief Set the value-change handler.
		 * @param onValueChange Handler receiving the selected index.
		 * @return *this.
		 */
		RadioGroup &
			setOnValueChange(std::function<void(std::size_t)> onValueChange);

		/**
		 * @brief Vertical space between 20 dp rings so their 40 dp targets
		 * do not overlap.
		 */
		static constexpr float RowSpacing = components::RadioGroup::RowSpacing;

		private:
		/**
		 * @brief Construct a radio group.
		 * @param registry Component registry.
		 * @param config Surface configuration.
		 * @param options Option labels.
		 * @param value Initially selected option.
		 * @param onValueChange Value-change handler.
		 */
		RadioGroup(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config,
				   const std::vector<std::string> &options,
				   std::optional<std::size_t> value,
				   std::function<void(std::size_t)> onValueChange);

		/**
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
