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

#include "guillaume/entities/progress_indicator.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/animation.hpp"
#include "guillaume/components/arc.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ring.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/progress_indicator.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float LinearHeight =
			components::ProgressIndicator::LinearHeight;
		const float LinearRadius = getShapeRadius(theme::ShapeRole::Full);
		constexpr float CircularSize =
			components::ProgressIndicator::DefaultDiameter;
		constexpr float TrackLength =
			components::ProgressIndicator::DefaultTrackLength;

		/**
		 * @brief Linear track child of a progress indicator.
		 */
		class LinearTrack:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders>
		{
			public:
			/**
			 * @brief Construct a linear track.
			 * @param registry The component registry.
			 */
			explicit LinearTrack(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders>(
					  registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~LinearTrack(void) override = default;
		};

		/**
		 * @brief Linear progress fill child of a progress indicator.
		 */
		class LinearFill:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders,
									 components::Animation>
		{
			public:
			/**
			 * @brief Construct a linear fill.
			 * @param registry The component registry.
			 */
			explicit LinearFill(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders,
									components::Animation>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~LinearFill(void) override = default;
		};

		/**
		 * @brief Circular track child of a progress indicator.
		 */
		class CircularTrack:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Ring>
		{
			public:
			/**
			 * @brief Construct a circular track.
			 * @param registry The component registry.
			 */
			explicit CircularTrack(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Ring>(
					  registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~CircularTrack(void) override = default;
		};

		/**
		 * @brief Circular progress fill child of a progress indicator.
		 */
		class CircularFill:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Arc,
									 components::Animation>
		{
			public:
			/**
			 * @brief Construct a circular fill.
			 * @param registry The component registry.
			 */
			explicit CircularFill(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Arc,
									components::Animation>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~CircularFill(void) override = default;
		};
	}	 // namespace

	ProgressIndicator::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<ProgressIndicator>(componentRegistry,
											   entityRegistry)
	{
		reset();
	}

	std::shared_ptr<ProgressIndicator>
		ProgressIndicator::Builder::buildEntity(void)
	{
		if (_variant == Variant::CircularDeterminate
			|| _variant == Variant::CircularIndeterminate) {
			_config.setFixedWidth(_size).setFixedHeight(_size);
			_config.borderRadius = _size * 0.5f;
		} else {
			_config.setFixedWidth(_width).setFixedHeight(LinearHeight);
			_config.borderRadius = LinearRadius;
		}

		auto entity = ProgressIndicator::create(this->getComponentRegistry(),
												_config, _variant, _value);
		return entity;
	}

	void ProgressIndicator::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::LinearDeterminate;
		_value	 = 0.0f;
		_width	 = TrackLength;
		_size	 = CircularSize;

		_config.borderRadius = LinearRadius;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 0.0f;
		_config.spacing = 0.0f;
		_config.setFixedWidth(_width);
		_config.setFixedHeight(LinearHeight);
	}

	ProgressIndicator::Builder &ProgressIndicator::Builder::withPose(
		const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	ProgressIndicator::Builder &
		ProgressIndicator::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	ProgressIndicator::Builder &
		ProgressIndicator::Builder::withValue(float value)
	{
		_value = std::clamp(value, 0.0f, 1.0f);
		return *this;
	}

	ProgressIndicator::Builder &
		ProgressIndicator::Builder::withWidth(float width)
	{
		_width = width;
		return *this;
	}

	ProgressIndicator::Builder &ProgressIndicator::Builder::withSize(float size)
	{
		_size = size;
		return *this;
	}

	std::shared_ptr<ProgressIndicator>
		ProgressIndicator::Builder::makeProgressIndicator(
			std::shared_ptr<ecs::Entity> parent, Variant variant, float value)
	{
		return withVariant(variant).withValue(value).registerEntity(parent);
	}

	ProgressIndicator::ProgressIndicator(ecs::ComponentRegistry &registry,
										 const SurfaceConfig &config,
										 Variant variant, float value)
		: SurfaceBase<components::Value, components::Animation,
					  components::Accessibility, components::ProgressIndicator>(
			  registry, config)
	{
		auto &state =
			getComponentRegistry().getComponent<components::ProgressIndicator>(
				getIdentifier());

		state.setVariant(variant);
		state
			.setTrackLength(!state.isCircular() && config.hasFixedWidth
								? config.fixedWidth
								: TrackLength)
			.setDiameter(state.isCircular() && config.hasFixedHeight
							 ? config.fixedHeight
							 : CircularSize);
		// Set before the system is installed, so the initial value is drawn
		// as is instead of being animated from zero.
		getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.setValue(std::clamp(value, 0.0f, 1.0f));
		systems::ProgressIndicator::install(getComponentRegistry(),
											getIdentifier());
		systems::ProgressIndicator::apply(getComponentRegistry(),
										  getIdentifier());
	}

	float ProgressIndicator::getDisplayedValue(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ProgressIndicator>(getIdentifier())
			.getDisplayed();
	}

	ProgressIndicator &ProgressIndicator::setVisible(bool visible)
	{
		systems::ProgressIndicator::setVisible(getComponentRegistry(),
											   getIdentifier(), visible);
		return *this;
	}

	bool ProgressIndicator::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ProgressIndicator>(getIdentifier())
			.isVisible();
	}

	float ProgressIndicator::getOpacity(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ProgressIndicator>(getIdentifier())
			.getOpacity();
	}

	ProgressIndicator &ProgressIndicator::setIndicatorColor(
		const utility::graphic::Color32Bit &color)
	{
		getComponentRegistry()
			.getComponent<components::ProgressIndicator>(getIdentifier())
			.setIndicatorColor(color);
		systems::ProgressIndicator::apply(getComponentRegistry(),
										  getIdentifier());
		return *this;
	}

	utility::graphic::Color32Bit
		ProgressIndicator::getIndicatorColor(void) const
	{
		return systems::ProgressIndicator::indicatorColor(
			getComponentRegistry(), getIdentifier());
	}

	ProgressIndicator::~ProgressIndicator(void)
	{
	}

	void ProgressIndicator::buildChildren(void)
	{
		auto &registry = getComponentRegistry();
		auto &state	   = registry.getComponent<components::ProgressIndicator>(
			   getIdentifier());

		if (state.isBuilt()) {
			return;
		}

		// Remove any children left over from a previous variant, and silence
		// the fill animation so it no longer drives this indicator.
		if (registry.hasComponent<components::Animation>(state.getFill())) {
			registry.getComponent<components::Animation>(state.getFill())
				.setOnSampleHandler(components::Animation::SampleHandler {})
				.stop();
		}
		this->clearEntities();

		std::shared_ptr<ecs::Entity> track;
		std::shared_ptr<ecs::Entity> fill;
		if (state.isCircular()) {
			track = std::make_shared<CircularTrack>(registry);
			fill  = std::make_shared<CircularFill>(registry);
		} else {
			track = std::make_shared<LinearTrack>(registry);
			fill  = std::make_shared<LinearFill>(registry);
		}
		track->setParent(shared_from_this());
		this->addEntity(track);
		fill->setParent(shared_from_this());
		this->addEntity(fill);
		state.setTrack(track->getIdentifier())
			.setFill(fill->getIdentifier())
			.setCycle(0.0f)
			.setBuilt(true);
		systems::ProgressIndicator::installChildren(registry, getIdentifier());

		// Track and fill are positioned by systems::ProgressIndicator, so
		// they are not registered as layout children of the surface.
		setChildren({});
	}

	ProgressIndicator &ProgressIndicator::setVariant(Variant variant)
	{
		auto &state =
			getComponentRegistry().getComponent<components::ProgressIndicator>(
				getIdentifier());

		if (state.getVariant() == variant) {
			return *this;
		}

		// Children are built from the variant: rebuild them so the new shape
		// (bar vs. ring) is adopted.
		state.setVariant(variant).setBuilt(false);
		buildChildren();
		systems::ProgressIndicator::apply(getComponentRegistry(),
										  getIdentifier());

		return *this;
	}

	ProgressIndicator::Variant ProgressIndicator::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::ProgressIndicator>(getIdentifier())
			.getVariant();
	}

	ProgressIndicator &ProgressIndicator::setValue(float value)
	{
		systems::ProgressIndicator::setValue(getComponentRegistry(),
											 getIdentifier(), value);
		return *this;
	}

	float ProgressIndicator::getValue(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.getValue();
	}

	void ProgressIndicator::build(void)
	{
		buildChildren();
		systems::ProgressIndicator::apply(getComponentRegistry(),
										  getIdentifier());
	}

}	 // namespace guillaume::entities
