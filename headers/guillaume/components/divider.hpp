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

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief Configuration of a divider: a thin line separating content.
	 *
	 * This component is the whole state of a divider. systems::Divider turns
	 * it into the entity's Transform, Bound, Line and Color.
	 */
	class Divider: public ecs::Component
	{
		public:
		/**
		 * @brief Which edges of the divider are inset.
		 */
		enum class Variant { FullWidth, Inset, Middle };

		/// Paper's default hairline stroke.
		static constexpr float HairlineThickness = 0.5f;

		/// Paper's bold stroke.
		static constexpr float BoldThickness = 1.0f;

		/**
		 * @brief Default constructor.
		 */
		Divider(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Divider(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const;

		/** @brief Set the variant. @param variant The variant. @return It. */
		Divider &setVariant(Variant variant);

		/** @brief Get the total length. @return The length in dp. */
		float getLength(void) const;

		/** @brief Set the total length. @param length In dp. @return It. */
		Divider &setLength(float length);

		/** @brief Get the stroke thickness. @return The thickness in dp. */
		float getThickness(void) const;

		/** @brief Set the thickness. @param thickness In dp. @return It. */
		Divider &setThickness(float thickness);

		/** @brief Get the inset of an inset edge. @return The inset in dp. */
		float getInset(void) const;

		/** @brief Set the inset. @param inset In dp. @return It. */
		Divider &setInset(float inset);

		/** @brief Get the stroke color. @return The color. */
		const utility::graphic::Color32Bit &getColor(void) const;

		/** @brief Set the stroke color. @param color The color. @return It. */
		Divider &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Get the pose of the divider before the leading inset.
		 * @return The origin pose.
		 */
		const utility::graphic::PoseF &getOriginPose(void) const;

		/**
		 * @brief Set the pose of the divider before the leading inset.
		 * @param pose The origin pose.
		 * @return This component.
		 */
		Divider &setOriginPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Get the pose systems::Divider last wrote to the Transform.
		 * @return The applied pose.
		 */
		const utility::graphic::PoseF &getAppliedPose(void) const;

		/**
		 * @brief Record the pose written to the Transform.
		 *
		 * This is bookkeeping used to detect an external move; it does not
		 * mark the component as changed.
		 * @param pose The applied pose.
		 * @return This component.
		 */
		Divider &setAppliedPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Inset applied on the leading edge for the current variant.
		 * @return The inset in dp, or 0.
		 */
		float getLeadingInset(void) const;

		/**
		 * @brief Inset applied on the trailing edge for the current variant.
		 * @return The inset in dp, or 0.
		 */
		float getTrailingInset(void) const;

		private:
		Variant _variant { Variant::FullWidth };	///< Inset edges.
		float _length { 100.0f };					///< Total length.
		float _thickness { HairlineThickness };		///< Stroke thickness.
		float _inset { 16.0f };						///< Edge inset.
		utility::graphic::Color32Bit _color {};		///< Stroke color.
		utility::graphic::PoseF _originPose {};		///< Pose before inset.
		utility::graphic::PoseF _appliedPose {};	///< Last pose written.
	};

}	 // namespace guillaume::components
