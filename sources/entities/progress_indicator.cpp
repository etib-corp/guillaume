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

#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/motion/easing.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float LinearHeight  = 4.0f;
		constexpr float LinearRadius  = 2.0f;
		constexpr float CircularSize  = 48.0f;
		constexpr float RingThickness = 4.0f;
		constexpr float TwoPi		  = 6.28318530717958647692f;
		constexpr float HalfPi		  = 1.57079632679489661923f;

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

		auto entity = std::make_shared<ProgressIndicator>(
			this->getComponentRegistry(), _config, _variant, _value);
		return entity;
	}

	void ProgressIndicator::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::LinearDeterminate;
		_value	 = 0.0f;
		_width	 = 200.0f;
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
		ProgressIndicator::Director::makeProgressIndicator(
			Builder &builder, std::shared_ptr<ecs::Entity> parent,
			Variant variant, float value)
	{
		return builder.withVariant(variant).withValue(value).registerEntity(
			parent);
	}

	ProgressIndicator::ProgressIndicator(ecs::ComponentRegistry &registry,
										 const SurfaceConfig &config,
										 Variant variant, float value)
		: SurfaceBase<components::Value>(registry, config)
		, _variant(variant)
		, _trackLength((variant == Variant::LinearDeterminate
						|| variant == Variant::LinearIndeterminate)
							   && config.hasFixedWidth
						   ? config.fixedWidth
						   : 200.0f)
		, _diameter((variant == Variant::CircularDeterminate
					 || variant == Variant::CircularIndeterminate)
							&& config.hasFixedHeight
						? config.fixedHeight
						: CircularSize)
		, _track()
		, _fill()
		, _built(false)
	{
		auto &valueComponent =
			getComponentRegistry().getComponent<components::Value>(
				getIdentifier());
		valueComponent.setMin(0.0f).setMax(1.0f).setStep(0.0f);
		valueComponent.setOnChangedHandler([this](float) {
			applyState();
		});
		valueComponent.setValue(std::clamp(value, 0.0f, 1.0f));
	}

	ProgressIndicator::~ProgressIndicator(void)
	{
	}

	bool ProgressIndicator::isCircular(void) const
	{
		return _variant == Variant::CircularDeterminate
			|| _variant == Variant::CircularIndeterminate;
	}

	bool ProgressIndicator::isIndeterminate(void) const
	{
		return _variant == Variant::LinearIndeterminate
			|| _variant == Variant::CircularIndeterminate;
	}

	void ProgressIndicator::buildChildren(void)
	{
		if (_built) {
			return;
		}

		// Remove any children left over from a previous variant.
		this->accessDirectEntities().clear();

		auto config = getSurfaceConfig();

		config.padding = 0.0f;
		config.spacing = 0.0f;

		if (isCircular()) {
			config.borderRadius	  = _diameter * 0.5f;
			config.hasFixedWidth  = true;
			config.fixedWidth	  = _diameter;
			config.hasFixedHeight = true;
			config.fixedHeight	  = _diameter;
		} else {
			config.borderRadius	  = LinearRadius;
			config.hasFixedWidth  = true;
			config.fixedWidth	  = _trackLength;
			config.hasFixedHeight = true;
			config.fixedHeight	  = LinearHeight;
		}

		setSurfaceConfig(config);
		setColor(transparentColor());

		if (isCircular()) {
			auto track =
				std::make_shared<CircularTrack>(getComponentRegistry());
			track->setParent(shared_from_this());
			this->addEntity(track);
			_track = track;

			auto fill = std::make_shared<CircularFill>(getComponentRegistry());
			fill->setParent(shared_from_this());
			this->addEntity(fill);
			_fill = fill;

			getComponentRegistry()
				.getComponent<components::Ring>(_track->getIdentifier())
				.setThickness(RingThickness);

			getComponentRegistry()
				.getComponent<components::Arc>(_fill->getIdentifier())
				.setThickness(RingThickness)
				.setStartAngle(-HalfPi)
				.setSweepAngle(0.0f);

			if (isIndeterminate()) {
				auto &animation =
					getComponentRegistry().getComponent<components::Animation>(
						_fill->getIdentifier());
				animation.setDuration(1.5f)
					.setEasing(motion::Easing::Standard)
					.setLooping(true)
					.setOnSampleHandler([this](float progress) {
						if (_fill == nullptr) {
							return;
						}
						auto &arc = getComponentRegistry()
										.getComponent<components::Arc>(
											_fill->getIdentifier());
						arc.setStartAngle(-HalfPi + progress * TwoPi)
							.setSweepAngle(TwoPi * 0.25f);
					});
				animation.play();
			}
		} else {
			auto track = std::make_shared<LinearTrack>(getComponentRegistry());
			track->setParent(shared_from_this());
			this->addEntity(track);
			_track = track;

			auto fill = std::make_shared<LinearFill>(getComponentRegistry());
			fill->setParent(shared_from_this());
			this->addEntity(fill);
			_fill = fill;

			getComponentRegistry()
				.getComponent<components::Bound>(_fill->getIdentifier())
				.setHeight(LinearHeight);

			getComponentRegistry()
				.getComponent<components::Borders>(_fill->getIdentifier())
				.setBorderRadius(LinearRadius);

			if (isIndeterminate()) {
				auto &animation =
					getComponentRegistry().getComponent<components::Animation>(
						_fill->getIdentifier());
				animation.setDuration(1.5f)
					.setEasing(motion::Easing::Standard)
					.setLooping(true)
					.setOnSampleHandler([this](float progress) {
						if (_fill == nullptr || _track == nullptr) {
							return;
						}
						const float segment = _trackLength * 0.4f;

						placeChild(getComponentRegistry(), getIdentifier(),
								   _fill->getIdentifier(),
								   utility::graphic::PositionF(
									   progress * (_trackLength - segment),
									   0.0f, 0.0f));

						getComponentRegistry()
							.getComponent<components::Bound>(
								_fill->getIdentifier())
							.setWidth(segment)
							.setHeight(LinearHeight);
					});
				animation.play();
			}
		}

		// Track and fill are positioned manually, so they are not registered
		// as layout children of the surface.
		setChildren({});

		_built = true;
	}

	void ProgressIndicator::applyState(void)
	{
		if (!_built || _track == nullptr || _fill == nullptr) {
			return;
		}

		const float normalized =
			std::clamp(getComponentRegistry()
						   .getComponent<components::Value>(getIdentifier())
						   .getValue(),
					   0.0f, 1.0f);

		if (isCircular()) {
			placeChild(getComponentRegistry(), getIdentifier(),
					   _track->getIdentifier(),
					   utility::graphic::PositionF(0.0f, 0.0f, 0.0f));
			placeChild(getComponentRegistry(), getIdentifier(),
					   _fill->getIdentifier(),
					   utility::graphic::PositionF(0.0f, 0.0f, 0.0f));

			getComponentRegistry()
				.getComponent<components::Bound>(_track->getIdentifier())
				.setWidth(_diameter)
				.setHeight(_diameter);
			getComponentRegistry()
				.getComponent<components::Bound>(_fill->getIdentifier())
				.setWidth(_diameter)
				.setHeight(_diameter);

			getComponentRegistry()
				.getComponent<components::Color>(_track->getIdentifier())
				.setColor(schemeColor(SchemeColorRole::SurfaceVariant));
			getComponentRegistry()
				.getComponent<components::Color>(_fill->getIdentifier())
				.setColor(schemeColor(SchemeColorRole::Primary));

			if (!isIndeterminate()) {
				getComponentRegistry()
					.getComponent<components::Arc>(_fill->getIdentifier())
					.setStartAngle(-HalfPi)
					.setSweepAngle(normalized * TwoPi);
			}
			return;
		}

		placeChild(getComponentRegistry(), getIdentifier(),
				   _track->getIdentifier(),
				   utility::graphic::PositionF(0.0f, 0.0f, 0.0f));
		getComponentRegistry()
			.getComponent<components::Bound>(_track->getIdentifier())
			.setWidth(_trackLength)
			.setHeight(LinearHeight);
		getComponentRegistry()
			.getComponent<components::Color>(_track->getIdentifier())
			.setColor(schemeColor(SchemeColorRole::SurfaceVariant));

		placeChild(getComponentRegistry(), getIdentifier(),
				   _fill->getIdentifier(),
				   utility::graphic::PositionF(0.0f, 0.0f, 0.0f));
		getComponentRegistry()
			.getComponent<components::Color>(_fill->getIdentifier())
			.setColor(schemeColor(SchemeColorRole::Primary));

		if (!isIndeterminate()) {
			getComponentRegistry()
				.getComponent<components::Bound>(_fill->getIdentifier())
				.setWidth(_trackLength * normalized)
				.setHeight(LinearHeight);
		}
	}

	ProgressIndicator &ProgressIndicator::setVariant(Variant variant)
	{
		if (_variant == variant) {
			return *this;
		}

		_variant = variant;

		// Children are built lazily from the variant; force a rebuild so the
		// new shape (bar vs. ring) is adopted.
		_built = false;

		buildChildren();
		applyState();

		return *this;
	}

	ProgressIndicator::Variant ProgressIndicator::getVariant(void) const
	{
		return _variant;
	}

	ProgressIndicator &ProgressIndicator::setValue(float value)
	{
		getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.setNormalizedValue(std::clamp(value, 0.0f, 1.0f));
		return *this;
	}

	float ProgressIndicator::getValue(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.getValue();
	}

	void ProgressIndicator::initialize(void)
	{
		SurfaceBase::initialize();
		buildChildren();
		applyState();
	}

	void ProgressIndicator::update(void)
	{
		SurfaceBase::update();
		buildChildren();
		applyState();
	}

}	 // namespace guillaume::entities
