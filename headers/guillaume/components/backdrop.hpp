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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief State of a modal backdrop, the dimming layer behind a dialog
	 * or a drawer: whether it is shown and how far its fade-in has
	 * progressed.
	 *
	 * Written through systems::Backdrop::setVisible, which also starts the
	 * entity's components::Animation; that animation writes the progress
	 * while it plays. systems::Backdrop reads the state in the Layout
	 * phase and writes the color of the entity's components::Scrim: the
	 * theme's backdrop color with its alpha multiplied by the progress, or
	 * fully transparent while hidden.
	 *
	 * @code
	 * systems::Backdrop::setVisible(registry, backdropId, true);
	 * @endcode
	 *
	 * @note Prefer systems::Backdrop::setVisible to the setters of this
	 * class: setVisible() here stores the flag only, it neither resets the
	 * progress nor starts the fade.
	 *
	 * @see systems::Backdrop, components::Scrim, entities::Backdrop
	 */
	class Backdrop: public ecs::Component
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		Backdrop(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Backdrop(void) = default;

		/**
		 * @brief Check whether the backdrop is shown.
		 * @return True when shown; false (the default) when hidden, in
		 * which case the scrim is transparent whatever the progress.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Store whether the backdrop is shown.
		 *
		 * Marks the component changed only when the value differs. The
		 * scrim color follows the next time systems::Backdrop runs.
		 *
		 * @param visible True to show the backdrop, false to hide it.
		 * @return This component, for chaining.
		 */
		Backdrop &setVisible(bool visible);

		/**
		 * @brief Get the fade-in progress.
		 * @return 0 (the default) for a transparent backdrop, 1 for the
		 * full backdrop color.
		 */
		float getProgress(void) const;

		/**
		 * @brief Set the fade-in progress. Called by the entity's fade
		 * animation on every sample.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param progress The progress, from 0 (transparent) to 1 (full
		 * backdrop color). The value is stored as given, not clamped.
		 * @return This component, for chaining.
		 */
		Backdrop &setProgress(float progress);

		private:
		bool _visible { false };	 ///< Whether the backdrop is shown.
		float _progress { 0.0f };	 ///< Fade-in progress, 0 to 1.
	};

}	 // namespace guillaume::components
