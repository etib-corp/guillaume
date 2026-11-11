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
	 * @brief State of a divider: a thin horizontal line separating content.
	 *
	 * It holds the variant, the length, the thickness, the inset, the color
	 * and the pose of the divider.
	 *
	 * Role in the frame:
	 * - entities::Divider writes the configuration.
	 * - systems::Divider (Measure phase) reads it and writes the entity's
	 *   Transform (the origin pose shifted right by the leading inset),
	 *   Bound (the length minus the insets, by the thickness), Line and
	 *   Color.
	 *
	 * @code
	 * registry.getComponent<components::Divider>(identifier)
	 * 	.setVariant(components::Divider::Variant::Inset)
	 * 	.setLength(320.0f);
	 * // Drawn 304 dp long, starting 16 dp to the right of the origin.
	 * @endcode
	 *
	 * Watch out: the Transform, Bound, Line thickness and Color of the
	 * entity are overwritten every frame from this component, so change the
	 * divider here. Moving the entity through its Transform is supported:
	 * the system notices the move and adopts it as the new origin pose.
	 *
	 * @see systems::Divider, components::Line
	 */
	class Divider: public ecs::Component
	{
		public:
		/**
		 * @brief Which ends of the divider are inset.
		 *
		 * FullWidth: none. Inset: the leading (left) end. Middle: both
		 * ends.
		 */
		enum class Variant { FullWidth, Inset, Middle };

		/// Default stroke thickness, in dp.
		static constexpr float HairlineThickness = 0.5f;

		/// Thickness of a bold stroke, in dp.
		static constexpr float BoldThickness = 1.0f;

		/**
		 * @brief Create the state of a full-width hairline divider, 100 dp
		 * long.
		 */
		Divider(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Divider(void) = default;

		/**
		 * @brief Get which ends are inset.
		 * @return The variant (FullWidth by default).
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Choose which ends are inset.
		 *
		 * Marks the component changed when the variant differs.
		 * @param variant FullWidth, Inset or Middle.
		 * @return This component, for chaining.
		 */
		Divider &setVariant(Variant variant);

		/**
		 * @brief Get the total length, insets included.
		 * @return The length, in dp (100 by default).
		 */
		float getLength(void) const;

		/**
		 * @brief Set the total length, insets included.
		 *
		 * The stroke drawn is this length minus the insets of the variant,
		 * and never negative. Marks the component changed when the length
		 * differs.
		 * @param length The length, in dp.
		 * @return This component, for chaining.
		 */
		Divider &setLength(float length);

		/**
		 * @brief Get the stroke thickness.
		 * @return The thickness, in dp (HairlineThickness by default).
		 */
		float getThickness(void) const;

		/**
		 * @brief Set the stroke thickness.
		 *
		 * Marks the component changed when the thickness differs.
		 * @param thickness The thickness, in dp, for example
		 * HairlineThickness or BoldThickness.
		 * @return This component, for chaining.
		 */
		Divider &setThickness(float thickness);

		/**
		 * @brief Get the inset applied to an inset end.
		 * @return The inset, in dp (16 by default), whatever the variant.
		 */
		float getInset(void) const;

		/**
		 * @brief Set the inset applied to an inset end.
		 *
		 * It has no visible effect on a FullWidth divider. Marks the
		 * component changed when the inset differs.
		 * @param inset The inset, in dp.
		 * @return This component, for chaining.
		 */
		Divider &setInset(float inset);

		/**
		 * @brief Get the stroke color.
		 * @return The color.
		 */
		const utility::graphic::Color32Bit &getColor(void) const;

		/**
		 * @brief Set the stroke color.
		 *
		 * Unlike the other setters, it marks the component changed on every
		 * call, even when the color is the same.
		 * @param color The stroke color.
		 * @return This component, for chaining.
		 */
		Divider &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Get the pose of the divider before the leading inset.
		 * @return The pose of the left end of a full-width divider.
		 */
		const utility::graphic::PoseF &getOriginPose(void) const;

		/**
		 * @brief Set the pose of the divider before the leading inset.
		 *
		 * The system writes this pose, shifted right by the leading inset,
		 * to the Transform. Marks the component changed when the pose
		 * differs.
		 * @param pose The pose of the left end of the full-width divider.
		 * @return This component, for chaining.
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
		 * Bookkeeping of systems::Divider: when the Transform differs from
		 * it, something else moved the entity. It does not mark the
		 * component changed.
		 * @param pose The applied pose.
		 * @return This component, for chaining.
		 */
		Divider &setAppliedPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Get the inset applied on the leading (left) end for the
		 * current variant.
		 * @return The inset, in dp, for Inset and Middle; 0 for FullWidth.
		 */
		float getLeadingInset(void) const;

		/**
		 * @brief Get the inset applied on the trailing (right) end for the
		 * current variant.
		 * @return The inset, in dp, for Middle; 0 otherwise.
		 */
		float getTrailingInset(void) const;

		private:
		Variant _variant { Variant::FullWidth };	///< Inset ends.
		float _length { 100.0f };					///< Total length, in dp.
		float _thickness { HairlineThickness };		///< Thickness, in dp.
		float _inset { 16.0f };						///< End inset, in dp.
		utility::graphic::Color32Bit _color {};		///< Stroke color.
		utility::graphic::PoseF _originPose {};		///< Pose before inset.
		utility::graphic::PoseF _appliedPose {};	///< Last pose written.
	};

}	 // namespace guillaume::components
