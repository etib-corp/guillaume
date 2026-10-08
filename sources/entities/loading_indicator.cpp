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

#include "guillaume/entities/loading_indicator.hpp"

#include <memory>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/animation.hpp"
#include "guillaume/components/arc.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
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
		constexpr float LinearHeight = 4.0f;
		constexpr float LinearWidth	 = 200.0f;
		constexpr float LinearRadius = 2.0f;
		constexpr float CircularSize = 48.0f;
		constexpr float ArcThickness = 4.0f;
		constexpr float TwoPi		 = 6.28318530717958647692f;
		constexpr float HalfPi		 = 1.57079632679489661923f;
		constexpr float ArcSweep	 = 4.71238898038468985769f;	   // 1.5 * pi

		/**
		 * @brief Linear sliding bar child of a loading indicator.
		 */
		class LinearBar:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders,
									 components::Animation>
		{
			public:
			/**
			 * @brief Construct a linear bar.
			 * @param registry The component registry.
			 */
			explicit LinearBar(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders,
									components::Animation>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~LinearBar(void) override = default;
		};

		/**
		 * @brief Rotating arc child of a loading indicator.
		 */
		class RotatingArc:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Arc,
									 components::Animation>
		{
			public:
			/**
			 * @brief Construct a rotating arc.
			 * @param registry The component registry.
			 */
			explicit RotatingArc(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Arc,
									components::Animation>(registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~RotatingArc(void) override = default;
		};
	}	 // namespace

	LoadingIndicator::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<LoadingIndicator>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<LoadingIndicator>
		LoadingIndicator::Builder::buildEntity(void)
	{
		if (_variant == Variant::Circular) {
			_config.setFixedWidth(_size).setFixedHeight(_size);
			_config.borderRadius = _size * 0.5f;
		} else {
			_config.setFixedWidth(LinearWidth).setFixedHeight(LinearHeight);
			_config.borderRadius = LinearRadius;
		}

		auto entity = std::make_shared<LoadingIndicator>(
			this->getComponentRegistry(), _config, _variant);
		return entity;
	}

	void LoadingIndicator::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Linear;
		_size	 = CircularSize;

		_config.borderRadius = LinearRadius;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 0.0f;
		_config.spacing = 0.0f;
		_config.setFixedWidth(LinearWidth);
		_config.setFixedHeight(LinearHeight);
	}

	LoadingIndicator::Builder &
		LoadingIndicator::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	LoadingIndicator::Builder &
		LoadingIndicator::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	LoadingIndicator::Builder &LoadingIndicator::Builder::withSize(float size)
	{
		_size = size;
		return *this;
	}

	std::shared_ptr<LoadingIndicator>
		LoadingIndicator::Director::makeLoadingIndicator(
			Builder &builder, std::shared_ptr<ecs::Entity> parent,
			Variant variant)
	{
		return builder.withVariant(variant).registerEntity(parent);
	}

	LoadingIndicator::LoadingIndicator(ecs::ComponentRegistry &registry,
									   const SurfaceConfig &config,
									   Variant variant)
		: SurfaceBase<components::Animation>(registry, config)
		, _variant(variant)
		, _diameter(config.hasFixedHeight ? config.fixedHeight : CircularSize)
		, _animation()
		, _built(false)
	{
	}

	LoadingIndicator::~LoadingIndicator(void)
	{
	}

	void LoadingIndicator::buildChild(void)
	{
		if (_built) {
			return;
		}

		// Remove any child left over from a previous variant.
		this->accessDirectEntities().clear();

		auto config = getSurfaceConfig();

		config.padding = 0.0f;
		config.spacing = 0.0f;

		if (_variant == Variant::Circular) {
			config.borderRadius	  = _diameter * 0.5f;
			config.hasFixedWidth  = true;
			config.fixedWidth	  = _diameter;
			config.hasFixedHeight = true;
			config.fixedHeight	  = _diameter;
		} else {
			config.borderRadius	  = LinearRadius;
			config.hasFixedWidth  = true;
			config.fixedWidth	  = LinearWidth;
			config.hasFixedHeight = true;
			config.fixedHeight	  = LinearHeight;
		}

		setSurfaceConfig(config);
		setColor(transparentColor());

		if (_variant == Variant::Circular) {
			auto arc = std::make_shared<RotatingArc>(getComponentRegistry());
			arc->setParent(shared_from_this());
			this->addEntity(arc);
			_animation = arc;

			getComponentRegistry()
				.getComponent<components::Arc>(_animation->getIdentifier())
				.setThickness(ArcThickness)
				.setStartAngle(-HalfPi)
				.setSweepAngle(ArcSweep);
			getComponentRegistry()
				.getComponent<components::Color>(_animation->getIdentifier())
				.setColor(schemeColor(SchemeColorRole::Primary));
		} else {
			auto bar = std::make_shared<LinearBar>(getComponentRegistry());
			bar->setParent(shared_from_this());
			this->addEntity(bar);
			_animation = bar;

			getComponentRegistry()
				.getComponent<components::Bound>(_animation->getIdentifier())
				.setWidth(LinearWidth * 0.4f)
				.setHeight(LinearHeight);
			getComponentRegistry()
				.getComponent<components::Borders>(_animation->getIdentifier())
				.setBorderRadius(LinearRadius);
			getComponentRegistry()
				.getComponent<components::Color>(_animation->getIdentifier())
				.setColor(schemeColor(SchemeColorRole::Primary));
		}

		auto &animation =
			getComponentRegistry().getComponent<components::Animation>(
				_animation->getIdentifier());
		animation.setDuration(1.5f)
			.setEasing(motion::Easing::Standard)
			.setLooping(true)
			.setOnSampleHandler([this](float progress) {
				_progress = progress;
				applyState();
			});
		animation.play();

		// The animated child is positioned manually, so it is not registered
		// as a layout child of the surface.
		setChildren({});

		_built = true;
	}

	void LoadingIndicator::applyState(void)
	{
		if (!_built || _animation == nullptr) {
			return;
		}

		const float progress = _progress;

		if (_variant == Variant::Circular) {
			auto &arc = getComponentRegistry().getComponent<components::Arc>(
				_animation->getIdentifier());
			arc.setStartAngle(-HalfPi + progress * TwoPi);
			arc.setSweepAngle(ArcSweep);

			placeChild(getComponentRegistry(), getIdentifier(),
					   _animation->getIdentifier(),
					   utility::graphic::PositionF(0.0f, 0.0f, 0.0f));
			getComponentRegistry()
				.getComponent<components::Bound>(_animation->getIdentifier())
				.setWidth(_diameter)
				.setHeight(_diameter);
			return;
		}

		const float segment = LinearWidth * 0.4f;

		placeChild(getComponentRegistry(), getIdentifier(),
				   _animation->getIdentifier(),
				   utility::graphic::PositionF(
					   progress * (LinearWidth - segment), 0.0f, 0.0f));

		getComponentRegistry()
			.getComponent<components::Bound>(_animation->getIdentifier())
			.setWidth(segment)
			.setHeight(LinearHeight);
	}

	LoadingIndicator &LoadingIndicator::setVariant(Variant variant)
	{
		if (_variant == variant) {
			return *this;
		}

		_variant = variant;
		_built	 = false;

		buildChild();
		applyState();

		return *this;
	}

	LoadingIndicator::Variant LoadingIndicator::getVariant(void) const
	{
		return _variant;
	}

	LoadingIndicator &LoadingIndicator::start(void)
	{
		if (_animation != nullptr) {
			getComponentRegistry()
				.getComponent<components::Animation>(
					_animation->getIdentifier())
				.play();
		}
		return *this;
	}

	LoadingIndicator &LoadingIndicator::stop(void)
	{
		if (_animation != nullptr) {
			getComponentRegistry()
				.getComponent<components::Animation>(
					_animation->getIdentifier())
				.stop();
		}
		return *this;
	}

	bool LoadingIndicator::isRunning(void) const
	{
		if (_animation == nullptr) {
			return false;
		}
		return getComponentRegistry()
			.getComponent<components::Animation>(_animation->getIdentifier())
			.isPlaying();
	}

	void LoadingIndicator::initialize(void)
	{
		SurfaceBase::initialize();
		buildChild();
		applyState();
	}

	void LoadingIndicator::update(void)
	{
		SurfaceBase::update();
		buildChild();
	}

}	 // namespace guillaume::entities
