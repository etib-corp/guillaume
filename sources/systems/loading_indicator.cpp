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

#include "guillaume/systems/loading_indicator.hpp"

#include <cstdint>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/animation.hpp"
#include "guillaume/components/arc.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/motion/easing.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::LoadingIndicator::Variant;

		constexpr float TwoPi	 = 6.28318530717958647692f;
		constexpr float HalfPi	 = 1.57079632679489661923f;
		constexpr float ArcSweep = 4.71238898038468985769f;	   // 1.5 * pi

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
				.setColor(entities::transparentColor());
			Layout::apply(registry, identifier, layout.getChildren(), layer);
		}
	}	 // namespace

	LoadingIndicator::LoadingIndicator(void)
		: ecs::SystemFiller<components::LoadingIndicator>(ecs::Phase::Layout)
	{
	}

	void LoadingIndicator::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::Accessibility>(identifier)
			.setRole(components::AccessibilityRole::ProgressBar);
		componentRegistry.getComponent<components::Animation>(identifier)
			.setDuration(getMotionDuration(theme::MotionDuration::Short4))
			.setEasing(theme::MotionScale::standard())
			.setOnSampleHandler([registry, identifier](float progress) {
				auto &state =
					registry->getComponent<components::LoadingIndicator>(
						identifier);
				state.setOpacity(
					state.getOpacityFrom()
					+ (state.getOpacityTarget() - state.getOpacityFrom())
						* progress);
				apply(*registry, identifier);
			});
	}

	void LoadingIndicator::installChildren(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const auto child =
			componentRegistry
				.getComponent<components::LoadingIndicator>(identifier)
				.getIndicator();

		if (!componentRegistry.hasComponent<components::Animation>(child)) {
			return;
		}
		componentRegistry.getComponent<components::Animation>(child)
			.setDuration(theme::MotionScale::IndeterminateCycle)
			.setEasing(motion::Easing::Standard)
			.setLooping(true)
			.setOnSampleHandler([registry, identifier](float progress) {
				registry->getComponent<components::LoadingIndicator>(identifier)
					.setProgress(progress);
				apply(*registry, identifier);
			})
			.play();
	}

	void
		LoadingIndicator::apply(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &state =
			componentRegistry.getComponent<components::LoadingIndicator>(
				entityIdentifier);
		const auto layer = Layout::layerOf(componentRegistry, entityIdentifier);
		const bool circular = state.getVariant() == Variant::Circular;
		const float size	= state.getSize();
		const float full	= getShapeRadius(theme::ShapeRole::Full);
		const auto child	= state.getIndicator();

		if (circular) {
			shape(componentRegistry, entityIdentifier, size, size, size * 0.5f,
				  layer);
		} else {
			shape(componentRegistry, entityIdentifier,
				  components::LoadingIndicator::LinearWidth,
				  components::LoadingIndicator::LinearHeight, full, layer);
		}

		if (!componentRegistry.hasComponent<components::Bound>(child)
			|| !componentRegistry.hasComponent<components::Color>(child)) {
			return;
		}

		const auto color = indicatorColor(componentRegistry, entityIdentifier);
		componentRegistry.getComponent<components::Color>(child).setColor(
			entities::withAlpha(
				color,
				static_cast<std::uint8_t>(static_cast<float>(color.getAlpha())
											  * state.getOpacity()
										  + 0.5f)));

		if (circular) {
			if (componentRegistry.hasComponent<components::Arc>(child)) {
				componentRegistry.getComponent<components::Arc>(child)
					.setThickness(
						strokeWidth(componentRegistry, entityIdentifier))
					.setStartAngle(-HalfPi + state.getProgress() * TwoPi)
					.setSweepAngle(ArcSweep);
			}
			entities::placeChild(componentRegistry, entityIdentifier, child,
								 utility::graphic::PositionF(0.0f, 0.0f, 0.0f),
								 layer);
			componentRegistry.getComponent<components::Bound>(child)
				.setWidth(size)
				.setHeight(size);
			return;
		}

		const float segment = components::LoadingIndicator::LinearWidth
			* components::LoadingIndicator::LinearSegment;

		if (componentRegistry.hasComponent<components::Borders>(child)) {
			componentRegistry.getComponent<components::Borders>(child)
				.setBorderRadius(full);
		}
		entities::placeChild(
			componentRegistry, entityIdentifier, child,
			utility::graphic::PositionF(
				state.getProgress()
					* (components::LoadingIndicator::LinearWidth - segment),
				0.0f, 0.0f),
			layer);
		componentRegistry.getComponent<components::Bound>(child)
			.setWidth(segment)
			.setHeight(components::LoadingIndicator::LinearHeight);
	}

	void
		LoadingIndicator::start(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto child =
			componentRegistry
				.getComponent<components::LoadingIndicator>(entityIdentifier)
				.getIndicator();

		if (componentRegistry.hasComponent<components::Animation>(child)) {
			componentRegistry.getComponent<components::Animation>(child).play();
		}
		startFade(componentRegistry, entityIdentifier);
	}

	void LoadingIndicator::stop(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto child =
			componentRegistry
				.getComponent<components::LoadingIndicator>(entityIdentifier)
				.getIndicator();

		if (componentRegistry.hasComponent<components::Animation>(child)) {
			componentRegistry.getComponent<components::Animation>(child).stop();
		}
		startFade(componentRegistry, entityIdentifier);
	}

	bool LoadingIndicator::isRunning(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto child =
			componentRegistry
				.getComponent<components::LoadingIndicator>(entityIdentifier)
				.getIndicator();

		return componentRegistry.hasComponent<components::Animation>(child)
			&& componentRegistry.getComponent<components::Animation>(child)
				   .isPlaying();
	}

	void LoadingIndicator::startFade(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &state =
			componentRegistry.getComponent<components::LoadingIndicator>(
				entityIdentifier);
		const float target = (isRunning(componentRegistry, entityIdentifier)
							  || !state.hidesWhenStopped())
			? 1.0f
			: 0.0f;

		if (target == state.getOpacityTarget()
			&& target == state.getOpacity()) {
			return;
		}
		state.setOpacityFrom(state.getOpacity()).setOpacityTarget(target);
		componentRegistry.getComponent<components::Animation>(entityIdentifier)
			.play();
	}

	utility::graphic::Color32Bit LoadingIndicator::indicatorColor(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
			.getComponent<components::LoadingIndicator>(entityIdentifier)
			.getIndicatorColor()
			.value_or(entities::schemeColor(SchemeColorRole::Primary));
	}

	float LoadingIndicator::strokeWidth(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::LoadingIndicator>(entityIdentifier)
				   .getSize()
			* components::LoadingIndicator::StrokeRatio;
	}

	void LoadingIndicator::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::LoadingIndicator>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
