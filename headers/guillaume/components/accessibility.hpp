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

#include <optional>
#include <string>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief What an entity is for a screen reader or another assistive
	 * technology. The values mirror react-native-paper's `accessibilityRole`.
	 */
	enum class AccessibilityRole {
		None,			///< No semantic role (decorative or container).
		Button,			///< Pressable button.
		Checkbox,		///< Two- or three-state checkbox.
		Radio,			///< Radio button.
		RadioGroup,		///< Group of radio buttons.
		Switch,			///< On/off switch.
		Tab,			///< Navigation destination.
		TabList,		///< Group of navigation destinations.
		Header,			///< Heading (app bar title, list subheader).
		Alert,			///< Live announcement (snackbar).
		ProgressBar,	///< Determinate or indeterminate progress.
		Slider,			///< Adjustable value.
		TextField,		///< Editable text.
		Search,			///< Search field.
		ListItem,		///< Row of a list.
		Menu,			///< Menu container.
		MenuItem,		///< Menu entry.
		Dialog,			///< Modal dialog.
		Image,			///< Image or icon with meaning.
		Text			///< Static text.
	};

	/**
	 * @brief Checked state of a control that can be ticked (checkbox, radio
	 * button, switch).
	 */
	enum class CheckedState {
		Unchecked,	  ///< Not checked.
		Checked,	  ///< Checked.
		Mixed		  ///< Indeterminate: neither checked nor unchecked.
	};

	/**
	 * @brief How urgently a change of the entity is announced to the user.
	 */
	enum class LiveRegion {
		None,		 ///< Changes are not announced.
		Polite,		 ///< Announced once the current speech is over.
		Assertive	 ///< Announced at once, interrupting the current speech.
	};

	/**
	 * @brief Position of a control within a range, such as a slider or a
	 * progress bar. Accessibility::setValue requires `min <= now <= max`.
	 */
	struct AccessibilityValue {
		float min { 0.0f };	   ///< Lowest value the control can take.
		float max { 1.0f };	   ///< Highest value the control can take.
		float now { 0.0f };	   ///< Current value, between min and max.
	};

	/**
	 * @brief What an entity means to a user who does not see it: its name,
	 * its role, its state and its place in the keyboard traversal.
	 *
	 * Widget systems write it when they install a widget (role) and each time
	 * the widget changes (label, checked, selected, expanded); user code may
	 * set a label, a hint or a focus order. systems::Focus reads the focus
	 * order and the focusable flag to build the Tab traversal. The other
	 * fields describe the entity to an assistive technology.
	 *
	 * @code
	 * registry.getComponent<components::Accessibility>(identifier)
	 * 	.setLabel("Delete the selected file")
	 * 	.setFocusOrder(1);
	 * @endcode
	 *
	 * The disabled state is not stored here: read it from StyleState.
	 *
	 * @see systems::Focus, systems::interaction::makeInteractive, StyleState
	 */
	class Accessibility: public ecs::Component
	{
		public:
		/**
		 * @brief Construct a description with no label, no role, no state, a
		 * focus order of 0 and the focusable flag set.
		 */
		Accessibility(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Accessibility(void) = default;

		/**
		 * @brief Get the name announced for the entity.
		 * @return The label; empty when none was set.
		 */
		const std::string &getLabel(void) const;

		/**
		 * @brief Set the name announced for the entity, such as "Close".
		 *
		 * Marks the component changed only when the label differs.
		 *
		 * @param label The name to announce; empty for none.
		 * @return This component, to chain calls.
		 */
		Accessibility &setLabel(const std::string &label);

		/**
		 * @brief Get the sentence describing what activating the entity does.
		 * @return The hint; empty when none was set.
		 */
		const std::string &getHint(void) const;

		/**
		 * @brief Set the sentence describing what activating the entity does,
		 * such as "Closes the dialog".
		 *
		 * Marks the component changed only when the hint differs.
		 *
		 * @param hint The sentence to announce after the label; empty for
		 * none.
		 * @return This component, to chain calls.
		 */
		Accessibility &setHint(const std::string &hint);

		/**
		 * @brief Get what kind of control the entity is.
		 * @return The role; AccessibilityRole::None by default.
		 */
		AccessibilityRole getRole(void) const;

		/**
		 * @brief Set what kind of control the entity is.
		 *
		 * Marks the component changed only when the role differs.
		 *
		 * @param role The role to announce.
		 * @return This component, to chain calls.
		 */
		Accessibility &setRole(AccessibilityRole role);

		/**
		 * @brief Check whether the entity is the selected one of its group
		 * (the active navigation destination, a selected chip).
		 * @return True when selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Set whether the entity is the selected one of its group.
		 *
		 * Marks the component changed only when the state differs.
		 *
		 * @param selected True when the entity is selected.
		 * @return This component, to chain calls.
		 */
		Accessibility &setSelected(bool selected);

		/**
		 * @brief Get whether the entity is ticked.
		 * @return The checked state, or `std::nullopt` when the entity is not
		 * a control that can be ticked.
		 */
		std::optional<CheckedState> getChecked(void) const;

		/**
		 * @brief Set whether the entity is ticked.
		 *
		 * Marks the component changed only when the state differs.
		 *
		 * @param checked The checked state, or `std::nullopt` for an entity
		 * that cannot be ticked.
		 * @return This component, to chain calls.
		 */
		Accessibility &setChecked(std::optional<CheckedState> checked);

		/**
		 * @brief Get whether the entity shows the content it controls (an
		 * open menu, an unfolded section).
		 * @return True when expanded, false when collapsed, or `std::nullopt`
		 * when the entity does not expand.
		 */
		std::optional<bool> getExpanded(void) const;

		/**
		 * @brief Set whether the entity shows the content it controls.
		 *
		 * Marks the component changed only when the state differs.
		 *
		 * @param expanded True when expanded, false when collapsed, or
		 * `std::nullopt` for an entity that does not expand.
		 * @return This component, to chain calls.
		 */
		Accessibility &setExpanded(std::optional<bool> expanded);

		/**
		 * @brief Get the position of the entity within its range.
		 * @return The range value, or `std::nullopt` when the entity has no
		 * range.
		 */
		const std::optional<AccessibilityValue> &getValue(void) const;

		/**
		 * @brief Set the position of the entity within its range.
		 *
		 * Always marks the component changed.
		 *
		 * @param value The range value, or `std::nullopt` to remove it.
		 * @return This component, to chain calls.
		 * @throws std::invalid_argument If `min > max` or `now` is outside
		 * `[min, max]`. The stored value is then left as it was.
		 */
		Accessibility &setValue(std::optional<AccessibilityValue> value);

		/**
		 * @brief Get how urgently a change of the entity is announced.
		 * @return The live region; LiveRegion::None by default.
		 */
		LiveRegion getLiveRegion(void) const;

		/**
		 * @brief Set how urgently a change of the entity is announced.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param liveRegion The politeness of the announcements.
		 * @return This component, to chain calls.
		 */
		Accessibility &setLiveRegion(LiveRegion liveRegion);

		/**
		 * @brief Get the rank of the entity in the Tab traversal.
		 *
		 * systems::Focus visits the entities with a positive order first, by
		 * ascending order, then the entities with order 0, in the order the
		 * system updates them.
		 *
		 * @return The focus order; 0, the default, means no explicit rank.
		 */
		int getFocusOrder(void) const;

		/**
		 * @brief Set the rank of the entity in the Tab traversal.
		 *
		 * Marks the component changed only when the order differs.
		 *
		 * @param focusOrder The rank: 1 is reached first; 0 removes the
		 * explicit rank.
		 * @return This component, to chain calls.
		 * @throws std::invalid_argument If @p focusOrder is negative.
		 */
		Accessibility &setFocusOrder(int focusOrder);

		/**
		 * @brief Check whether the Tab traversal stops on the entity.
		 * @return True, the default, when Tab can focus the entity.
		 */
		bool isFocusable(void) const;

		/**
		 * @brief Include the entity in the Tab traversal or leave it out.
		 *
		 * An entity left out can still take the focus when it is clicked.
		 * Marks the component changed only when the flag differs.
		 *
		 * @param focusable False to make Tab skip the entity.
		 * @return This component, to chain calls.
		 */
		Accessibility &setFocusable(bool focusable);

		private:
		std::string _label {};	  ///< Accessible name.
		std::string _hint {};	  ///< Describes the result of activation.
		AccessibilityRole _role { AccessibilityRole::None };	///< Role.
		bool _selected { false };						///< Selected state.
		std::optional<CheckedState> _checked {};		///< Checked state.
		std::optional<bool> _expanded {};				///< Expanded state.
		std::optional<AccessibilityValue> _value {};	///< Range value.
		LiveRegion _liveRegion { LiveRegion::None };	///< Live region.
		int _focusOrder { 0 };		 ///< Rank in the Tab traversal (0: none).
		bool _focusable { true };	 ///< Reachable by keyboard traversal.
	};

}	 // namespace guillaume::components
