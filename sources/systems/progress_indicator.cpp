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

#include "guillaume/systems/progress_indicator.hpp"

#include <algorithm>
#include <cstdint>
#include <optional>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/animation.hpp"
#include "guillaume/components/arc.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/ring.hpp"
#include "guillaume/components/value.hpp"
#include "guillaume/systems/placement_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/motion/easing.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		constexpr float TwoPi  = 6.28318530717958647692f;
		constexpr float HalfPi = 1.57079632679489661923f;

		/**
		 * @brief Give the widget its transparent, fixed-size surface.
		 * @param registry The registry holding the components.
		 * @param identifier The widget entity.
		 * @param width The width of the widget.
		 * @param height The height of the widget.
		 * @param radius The corner radius of the widget.
		 * @param layer The tree depth of the widget.
		 */
		void shape(ecs::ComponentRegistry &registry,
				   const ecs::Entity::Identifier &identifier, float width,
				   float height, float radius, std::uint32_t layer)
		{
			auto &layout =
				registry.getComponent<components::Layout>(identifier);

			layout.setPadding(0.0f)
				.setSpacing(0.0f)
				.setFixedWidth(width)
				.setFixedHeight(height);
			registry.getComponent<components::Borders>(identifier)
				.setBorderRadius(radius);
			registry.getComponent<components::Color>(identifier)
				.setColor(style::transparentColor());
			Layout::apply(registry, identifier, layout.getChildren(), layer);
		}

		/**
		 * @brief Publish the accessibility value, when it differs.
		 *
		 * components::Accessibility::setValue always marks the component
		 * changed, so an unchanged value is not written.
		 * @param registry The registry holding the components.
		 * @param identifier The widget entity.
		 * @param value The value to publish, empty when indeterminate.
		 */
		void publish(ecs::ComponentRegistry &registry,
					 const ecs::Entity::Identifier &identifier,
					 const std::optional<components::AccessibilityValue> &value)
		{
			auto &accessibility =
				registry.getComponent<components::Accessibility>(identifier);
			const auto &current = accessibility.getValue();

			if (current.has_value() == value.has_value()
				&& (!value.has_value()
					|| (current->min == value->min && current->max == value->max
						&& current->now == value->now))) {
				return;
			}
			accessibility.setValue(value);
		}

		/**
		 * @brief Apply the fade opacity to a color.
		 * @param color The color to fade.
		 * @param opacity The opacity, in [0, 1].
		 * @return The faded color.
		 */
		utility::graphic::Color32Bit
			faded(const utility::graphic::Color32Bit &color, float opacity)
		{
			return style::withAlpha(
				color,
				static_cast<std::uint8_t>(
					static_cast<float>(color.getAlpha()) * opacity + 0.5f));
		}
	}	 // namespace

	ProgressIndicator::ProgressIndicator(void)
		: ecs::SystemFiller<components::ProgressIndicator>(ecs::Phase::Layout)
	{
	}

	void ProgressIndicator::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		auto &state =
			componentRegistry.getComponent<components::ProgressIndicator>(
				identifier);
		auto &value =
			componentRegistry.getComponent<components::Value>(identifier);

		value.setMin(0.0f).setMax(1.0f).setStep(0.0f);
		// The value held at installation is drawn as is: only later changes
		// are animated.
		state.setDisplayed(value.getValue()).setValueFrom(value.getValue());
		value.setOnChangedHandler([registry, identifier](float) {
			startTransition(*registry, identifier);
		});

		componentRegistry.getComponent<components::Animation>(identifier)
			.setDuration(getMotionDuration(theme::MotionDuration::Short4))
			.setEasing(theme::MotionScale::standard())
			.setOnSampleHandler([registry, identifier](float progress) {
				auto &current =
					registry->getComponent<components::ProgressIndicator>(
						identifier);
				const float target =
					registry->getComponent<components::Value>(identifier)
						.getValue();
				const float opacityTarget = current.isVisible() ? 1.0f : 0.0f;

				current
					.setDisplayed(current.getValueFrom()
								  + (target - current.getValueFrom())
									  * progress)
					.setOpacity(current.getOpacityFrom()
								+ (opacityTarget - current.getOpacityFrom())
									* progress);
				apply(*registry, identifier);
			});

		componentRegistry.getComponent<components::Accessibility>(identifier)
			.setRole(components::AccessibilityRole::ProgressBar);
	}

	void ProgressIndicator::installChildren(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const auto &state =
			componentRegistry.getComponent<components::ProgressIndicator>(
				identifier);
		const auto fill = state.getFill();

		if (!state.isIndeterminate()
			|| !componentRegistry.hasComponent<components::Animation>(fill)) {
			return;
		}
		componentRegistry.getComponent<components::Animation>(fill)
			.setDuration(theme::MotionScale::IndeterminateCycle)
			.setEasing(motion::Easing::Standard)
			.setLooping(true)
			.setOnSampleHandler([registry, identifier](float progress) {
				registry
					->getComponent<components::ProgressIndicator>(identifier)
					.setCycle(progress);
				apply(*registry, identifier);
			})
			.play();
	}

	void ProgressIndicator::apply(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &state =
			componentRegistry.getComponent<components::ProgressIndicator>(
				entityIdentifier);
		const auto layer = Layout::layerOf(componentRegistry, entityIdentifier);
		const bool circular		 = state.isCircular();
		const bool indeterminate = state.isIndeterminate();
		const float diameter	 = state.getDiameter();
		const float length		 = state.getTrackLength();
		const float height		 = components::ProgressIndicator::LinearHeight;
		const float full		 = getShapeRadius(theme::ShapeRole::Full);
		const float normalized	 = std::clamp(state.getDisplayed(), 0.0f, 1.0f);
		const auto track		 = state.getTrack();
		const auto fill			 = state.getFill();
		const utility::graphic::PositionF origin(0.0f, 0.0f, 0.0f);

		if (circular) {
			shape(componentRegistry, entityIdentifier, diameter, diameter,
				  diameter * 0.5f, layer);
		} else {
			shape(componentRegistry, entityIdentifier, length, height, full,
				  layer);
		}

		if (indeterminate) {
			publish(componentRegistry, entityIdentifier, std::nullopt);
		} else {
			publish(componentRegistry, entityIdentifier,
					components::AccessibilityValue {
						0.0f, 100.0f,
						std::clamp(componentRegistry
									   .getComponent<components::Value>(
										   entityIdentifier)
									   .getValue(),
								   0.0f, 1.0f)
							* 100.0f });
		}

		if (!componentRegistry.hasComponent<components::Bound>(track)
			|| !componentRegistry.hasComponent<components::Color>(track)
			|| !componentRegistry.hasComponent<components::Bound>(fill)
			|| !componentRegistry.hasComponent<components::Color>(fill)) {
			return;
		}

		componentRegistry.getComponent<components::Color>(track).setColor(
			faded(style::schemeColor(SchemeColorRole::SurfaceVariant),
				  state.getOpacity()));
		componentRegistry.getComponent<components::Color>(fill).setColor(
			faded(indicatorColor(componentRegistry, entityIdentifier),
				  state.getOpacity()));
		placement::placeChild(componentRegistry, entityIdentifier, track,
							  origin, layer);
		// The fill covers the track: one step closer to the viewer, or the
		// two would sit at the same depth and the track could hide it.
		const auto fillLayer = layer + 1;

		if (circular) {
			placement::placeChild(componentRegistry, entityIdentifier, fill,
								  origin, fillLayer);
			componentRegistry.getComponent<components::Bound>(track)
				.setWidth(diameter)
				.setHeight(diameter);
			componentRegistry.getComponent<components::Bound>(fill)
				.setWidth(diameter)
				.setHeight(diameter);
			if (componentRegistry.hasComponent<components::Ring>(track)) {
				componentRegistry.getComponent<components::Ring>(track)
					.setThickness(components::ProgressIndicator::RingThickness);
			}
			if (componentRegistry.hasComponent<components::Arc>(fill)) {
				auto &arc =
					componentRegistry.getComponent<components::Arc>(fill);

				arc.setThickness(components::ProgressIndicator::RingThickness);
				if (indeterminate) {
					// y points up: the top is +pi/2 and clockwise is a
					// negative sweep.
					arc.setStartAngle(HalfPi - state.getCycle() * TwoPi)
						.setSweepAngle(-TwoPi
									   * components::ProgressIndicator::
										   IndeterminateSweep);
				} else {
					arc.setStartAngle(HalfPi).setSweepAngle(-normalized
															* TwoPi);
				}
			}
			return;
		}

		componentRegistry.getComponent<components::Bound>(track)
			.setWidth(length)
			.setHeight(height);
		if (componentRegistry.hasComponent<components::Borders>(fill)) {
			componentRegistry.getComponent<components::Borders>(fill)
				.setBorderRadius(full);
		}
		if (indeterminate) {
			const float segment =
				length * components::ProgressIndicator::IndeterminateSegment;

			placement::placeChild(
				componentRegistry, entityIdentifier, fill,
				utility::graphic::PositionF(
					state.getCycle() * (length - segment), 0.0f, 0.0f),
				fillLayer);
			componentRegistry.getComponent<components::Bound>(fill)
				.setWidth(segment)
				.setHeight(height);
		} else {
			placement::placeChild(componentRegistry, entityIdentifier, fill,
								  origin, fillLayer);
			componentRegistry.getComponent<components::Bound>(fill)
				.setWidth(length * normalized)
				.setHeight(height);
		}
	}

	void ProgressIndicator::setValue(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, float value)
	{
		componentRegistry.getComponent<components::Value>(entityIdentifier)
			.setNormalizedValue(std::clamp(value, 0.0f, 1.0f));
	}

	void ProgressIndicator::setVisible(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, bool visible)
	{
		auto &state =
			componentRegistry.getComponent<components::ProgressIndicator>(
				entityIdentifier);

		if (state.isVisible() == visible) {
			return;
		}
		state.setVisible(visible);
		startTransition(componentRegistry, entityIdentifier);
	}

	void ProgressIndicator::startTransition(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &state =
			componentRegistry.getComponent<components::ProgressIndicator>(
				entityIdentifier);

		state.setValueFrom(state.getDisplayed())
			.setOpacityFrom(state.getOpacity());
		componentRegistry.getComponent<components::Animation>(entityIdentifier)
			.play();
		apply(componentRegistry, entityIdentifier);
	}

	utility::graphic::Color32Bit ProgressIndicator::indicatorColor(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
			.getComponent<components::ProgressIndicator>(entityIdentifier)
			.getIndicatorColor()
			.value_or(style::schemeColor(SchemeColorRole::Primary));
	}

	void ProgressIndicator::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::ProgressIndicator>(
				entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
