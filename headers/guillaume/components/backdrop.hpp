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
	 * @brief State of a modal backdrop: whether it is shown and how far its
	 * fade-in has progressed.
	 *
	 * systems::Backdrop turns this state into the entity's Scrim color.
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

		/** @brief Whether the backdrop is shown. @return True if shown. */
		bool isVisible(void) const;

		/**
		 * @brief Show or hide the backdrop.
		 * @param visible True to show it.
		 * @return This component.
		 */
		Backdrop &setVisible(bool visible);

		/** @brief Get the fade-in progress. @return A value in [0, 1]. */
		float getProgress(void) const;

		/**
		 * @brief Set the fade-in progress.
		 * @param progress A value in [0, 1].
		 * @return This component.
		 */
		Backdrop &setProgress(float progress);

		private:
		bool _visible { false };	 ///< Whether the backdrop is shown.
		float _progress { 0.0f };	 ///< Fade-in progress in [0, 1].
	};

}	 // namespace guillaume::components
