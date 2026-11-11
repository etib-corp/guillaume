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
	 * @brief A column of labelled radio buttons of which at most one is
	 * selected (Paper `RadioButton.Group`).
	 *
	 * Its state lives in components::RadioGroup (option labels, index of
	 * the selected option, value-change handler, identifiers of the radio
	 * buttons); systems::RadioGroup selects the button of the current
	 * value, deselects the others and stacks them each frame, in the
	 * Layout phase. The class itself holds no field. The group builds one
	 * RadioButton per option, owns them, and exposes the `RadioGroup`
	 * accessibility role.
	 *
	 * @code
	 * RadioGroup::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto sizes = builder.withValue(0)
	 * 	.withOnValueChange([](std::size_t index) {
	 * 		// called when the user selects another option
	 * 	})
	 * 	.makeRadioGroup(parent, { "Small", "Medium", "Large" });
	 * @endcode
	 *
	 * `RadioGroup::create(registry, config, options, value, onValueChange)`
	 * builds the same widget and its radio buttons without registering it
	 * in an entity registry.
	 *
	 * Watch out:
	 * - Options are identified by their index, starting at 0, in the order
	 *   they were given. They are fixed at construction: there is no way
	 *   to add or remove one afterwards.
	 * - setValue() does not call the value-change handler. Only a user
	 *   selecting a different option does; pressing the selected option
	 *   again does nothing.
	 * - A group starts with no option selected unless a value is given.
	 *   Once an option is selected, the selection cannot be cleared.
	 * - The group replaces the press handler of each of its radio buttons
	 *   with its own, and overwrites its layout children with the radio
	 *   buttons every frame.
	 *
	 * @see components::RadioGroup, systems::RadioGroup, RadioButton
	 */
	class RadioGroup: public SurfaceBase<components::RadioGroup>
	{
		public:
		/**
		 * @brief Configures RadioGroup widgets and registers them in the
		 * scene's entity registry.
		 */
		class Builder: public EntityBuilderBase<RadioGroup>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and layout of the column.
			std::vector<std::string>
				_options {};	///< Option labels, in display order.
			std::optional<std::size_t>
				_value {};	  ///< Index of the option selected at first
							  ///< (std::nullopt: none).
			std::function<void(std::size_t)>
				_onValueChange {};	  ///< Called with the new index when the
									  ///< user selects another option.

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built groups are stored in.
			 * @param entityRegistry The registry that owns the groups
			 * built by `makeRadioGroup`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a group and its radio buttons from the current
			 * configuration, without registering it or giving it a parent.
			 * @return The new group.
			 */
			std::shared_ptr<RadioGroup> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: default pose, no option, no
			 * selected option, no value-change handler, and a vertical
			 * layout aligned to the start with `RowSpacing` between
			 * options.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the group is.
			 * @param pose The pose of the top-left corner of the group. A
			 * default-constructed pose by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Append an option at the end of the list.
			 * @param label The text shown next to the radio button of the
			 * option.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addOption(const std::string &label);

			/**
			 * @brief Choose the option selected when the group is built.
			 * @param index The index of the option, starting at 0. It is
			 * not checked: with an index that matches no option, no radio
			 * button is selected. By default no option is selected.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withValue(std::size_t index);

			/**
			 * @brief Set the function called when the user selects another
			 * option.
			 * @param onValueChange The function to call, receiving the
			 * index of the newly selected option. Empty by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnValueChange(
				std::function<void(std::size_t)> onValueChange);

			/**
			 * @brief Build a group and its radio buttons, register it under
			 * `parent` and reset the builder.
			 *
			 * The pose, selected option and handler set with the `with...`
			 * methods apply.
			 * @param parent The entity the group is attached to, or
			 * nullptr for a root entity.
			 * @param options The option labels, in display order. They are
			 * appended to the options already given with addOption().
			 * @return The registered group.
			 */
			std::shared_ptr<RadioGroup>
				makeRadioGroup(std::shared_ptr<ecs::Entity> parent,
							   const std::vector<std::string> &options);
		};

		/**
		 * @brief Build a group without registering it in an entity
		 * registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, config, options, value, onValueChange)`. It then
		 * calls build(), which creates one RadioButton per option.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new group. The caller owns it; the group owns its
		 * radio buttons.
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
		 * @brief Destructor.
		 */
		~RadioGroup(void) override = default;

		/**
		 * @brief Select an option from code.
		 *
		 * The radio buttons are updated at once. The value-change handler
		 * is not called.
		 * @param index The index of the option to select, starting at 0.
		 * @return Reference to this group for chaining.
		 * @throws std::out_of_range If @p index is not less than
		 * getOptionCount().
		 */
		RadioGroup &setValue(std::size_t index);

		/**
		 * @brief Get the selected option.
		 * @return The index of the selected option, or std::nullopt when
		 * none is selected.
		 */
		std::optional<std::size_t> getValue(void) const;

		/**
		 * @brief Get the number of options.
		 * @return How many options the group was built with.
		 */
		std::size_t getOptionCount(void) const;

		/**
		 * @brief Get the radio button of an option, for example to disable
		 * it.
		 * @param index The index of the option, starting at 0.
		 * @return The radio button, or nullptr when @p index is not an
		 * option.
		 */
		std::shared_ptr<RadioButton> getOption(std::size_t index) const;

		/**
		 * @brief Set the function called when the user selects another
		 * option.
		 * @param onValueChange The function to call, receiving the index
		 * of the newly selected option. An empty function removes the
		 * handler.
		 * @return Reference to this group for chaining.
		 */
		RadioGroup &
			setOnValueChange(std::function<void(std::size_t)> onValueChange);

		/**
		 * @brief Vertical space between two radio buttons, in dp: enough
		 * for the 40 dp touch targets of their 20 dp rings not to overlap.
		 */
		static constexpr float RowSpacing = components::RadioGroup::RowSpacing;

		private:
		/**
		 * @brief Construct a radio group and give it the `RadioGroup`
		 * accessibility role.
		 * @param registry The registry the components are stored in.
		 * @param config The pose and layout of the column.
		 * @param options The option labels, in display order.
		 * @param value The index of the option selected at first, or
		 * std::nullopt for none.
		 * @param onValueChange The function called with the new index when
		 * the user selects another option. May be empty.
		 */
		RadioGroup(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config,
				   const std::vector<std::string> &options,
				   std::optional<std::size_t> value,
				   std::function<void(std::size_t)> onValueChange);

		/**
		 * @brief Create one RadioButton per option, wire their presses to
		 * the group and select the one of the current value.
		 *
		 * Called once by create(), when the group is held by a shared
		 * pointer and can be given as parent to its radio buttons. The
		 * buttons are registered in the group itself, which owns them.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
