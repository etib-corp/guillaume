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
	 * @brief Semantic role of an entity, mirroring react-native-paper's
	 * `accessibilityRole` values.
	 */
	enum class AccessibilityRole {
		None,		   ///< No semantic role (decorative or container).
		Button,		   ///< Pressable button.
		Checkbox,	   ///< Two- or three-state checkbox.
		Radio,		   ///< Radio button.
		RadioGroup,	   ///< Group of radio buttons.
		Switch,		   ///< On/off switch.
		Tab,		   ///< Navigation destination.
		TabList,	   ///< Group of navigation destinations.
		Header,		   ///< Heading (app bar title, list subheader).
		Alert,		   ///< Live announcement (snackbar).
		ProgressBar,   ///< Determinate or indeterminate progress.
		Slider,		   ///< Adjustable value.
		TextField,	   ///< Editable text.
		Search,		   ///< Search field.
		ListItem,	   ///< Row of a list.
		Menu,		   ///< Menu container.
		MenuItem,	   ///< Menu entry.
		Dialog,		   ///< Modal dialog.
		Image,		   ///< Image or icon with meaning.
		Text		   ///< Static text.
	};

	/**
	 * @brief Checked state of a checkbox-like control.
	 */
	enum class CheckedState {
		Unchecked,	  ///< Not checked.
		Checked,	  ///< Checked.
		Mixed		  ///< Indeterminate.
	};

	/**
	 * @brief Live-region politeness of announcements.
	 */
	enum class LiveRegion {
		None,		 ///< No announcement.
		Polite,		 ///< Announce when idle.
		Assertive	 ///< Announce immediately.
	};

	/**
	 * @brief Range value exposed to assistive technologies.
	 */
	struct AccessibilityValue {
		float min { 0.0f };	   ///< Lower bound.
		float max { 1.0f };	   ///< Upper bound.
		float now { 0.0f };	   ///< Current value.
	};

	/**
	 * @brief Semantic accessibility description of an entity.
	 *
	 * This component is the single semantic source of truth of an entity:
	 * label, hint, role, selection/checked/expanded state, range value, live
	 * region and focus order. The disabled state is not duplicated here; it is
	 * owned by `components::StyleState`.
	 */
	class Accessibility: public ecs::Component
	{
		public:
		/**
		 * @brief Construct an empty, roleless description.
		 */
		Accessibility(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Accessibility(void) = default;

		/** @brief Get the label. @return The label. */
		const std::string &getLabel(void) const;

		/** @brief Set the label. @param label New label. @return *this. */
		Accessibility &setLabel(const std::string &label);

		/** @brief Get the hint. @return The hint. */
		const std::string &getHint(void) const;

		/** @brief Set the hint. @param hint New hint. @return *this. */
		Accessibility &setHint(const std::string &hint);

		/** @brief Get the role. @return The role. */
		AccessibilityRole getRole(void) const;

		/** @brief Set the role. @param role New role. @return *this. */
		Accessibility &setRole(AccessibilityRole role);

		/** @brief Check the selected state. @return True when selected. */
		bool isSelected(void) const;

		/**
		 * @brief Set the selected state.
		 * @param selected New state.
		 * @return *this.
		 */
		Accessibility &setSelected(bool selected);

		/**
		 * @brief Get the checked state.
		 * @return The checked state, or none for non-checkable roles.
		 */
		std::optional<CheckedState> getChecked(void) const;

		/**
		 * @brief Set the checked state.
		 * @param checked New state (none for non-checkable roles).
		 * @return *this.
		 */
		Accessibility &setChecked(std::optional<CheckedState> checked);

		/**
		 * @brief Get the expanded state.
		 * @return The expanded state, or none for non-expandable roles.
		 */
		std::optional<bool> getExpanded(void) const;

		/**
		 * @brief Set the expanded state.
		 * @param expanded New state (none for non-expandable roles).
		 * @return *this.
		 */
		Accessibility &setExpanded(std::optional<bool> expanded);

		/**
		 * @brief Get the range value.
		 * @return The value, or none when the entity has no range.
		 */
		const std::optional<AccessibilityValue> &getValue(void) const;

		/**
		 * @brief Set the range value.
		 * @param value New value (none to clear).
		 * @return *this.
		 * @throws std::invalid_argument If min > max or now is out of range.
		 */
		Accessibility &setValue(std::optional<AccessibilityValue> value);

		/** @brief Get the live region. @return The live region. */
		LiveRegion getLiveRegion(void) const;

		/**
		 * @brief Set the live region.
		 * @param liveRegion New live region.
		 * @return *this.
		 */
		Accessibility &setLiveRegion(LiveRegion liveRegion);

		/**
		 * @brief Get the focus order.
		 *
		 * Entities with a positive order are traversed first, ascending;
		 * entities with order 0 follow in tree order.
		 *
		 * @return The focus order.
		 */
		int getFocusOrder(void) const;

		/**
		 * @brief Set the focus order.
		 * @param focusOrder New order, must be non-negative.
		 * @return *this.
		 * @throws std::invalid_argument If @p focusOrder is negative.
		 */
		Accessibility &setFocusOrder(int focusOrder);

		/**
		 * @brief Check whether keyboard traversal may focus the entity.
		 * @return True when focusable.
		 */
		bool isFocusable(void) const;

		/**
		 * @brief Allow or forbid keyboard traversal to focus the entity.
		 * @param focusable New flag.
		 * @return *this.
		 */
		Accessibility &setFocusable(bool focusable);

		private:
		std::string _label {};	  ///< Accessible name.
		std::string _hint {};	  ///< Describes the result of activation.
		AccessibilityRole _role { AccessibilityRole::None };	///< Role.
		bool _selected { false };	 ///< Selected state.
		std::optional<CheckedState> _checked {};	///< Checked state.
		std::optional<bool> _expanded {};			///< Expanded state.
		std::optional<AccessibilityValue> _value {};	///< Range value.
		LiveRegion _liveRegion { LiveRegion::None };	///< Live region.
		int _focusOrder { 0 };		 ///< Explicit focus order (0: tree order).
		bool _focusable { true };	 ///< Reachable by keyboard traversal.
	};

}	 // namespace guillaume::components
