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

#include "guillaume/entities/slider.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include <utility/math/vector.hpp>

#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/ellipse.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/placement_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float TrackHeight = 4.0f;
		constexpr float TrackRadius = 2.0f;
		constexpr float ThumbSize	= 20.0f;

		/**
		 * @brief Rounded track child of a slider.
		 */
		class SliderTrack:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders>
		{
			public:
			/**
			 * @brief Construct a slider track.
			 * @param registry The component registry.
			 */
			explicit SliderTrack(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders>(
					  registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~SliderTrack(void) override = default;
		};

		/**
		 * @brief Active fill child of a slider.
		 */
		class SliderFill:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders>
		{
			public:
			/**
			 * @brief Construct a slider fill.
			 * @param registry The component registry.
			 */
			explicit SliderFill(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Borders>(
					  registry)
			{
			}

			/**
			 * @brief Default destructor.
			 */
			~SliderFill(void) override = default;
		};

		/**
		 * @brief Circular handle child of a slider.
		 */
		class SliderThumb:
			public ecs::EntityFiller<components::Transform, components::Bound,
									 components::Color, components::Ellipse>
		{
			public:
			/**
			 * @brief Construct a slider handle.
			 * @param registry The component registry.
			 */
			explicit SliderThumb(ecs::ComponentRegistry &registry)
				: ecs::EntityFiller<components::Transform, components::Bound,
									components::Color, components::Ellipse>(
					  registry)
			{
				getComponentRegistry()
					.getComponent<components::Bound>(getIdentifier())
					.setWidth(ThumbSize)
					.setHeight(ThumbSize);
			}

			/**
			 * @brief Default destructor.
			 */
			~SliderThumb(void) override = default;
		};

		/**
		 * @brief Extract the origin pose of an entity.
		 * @param registry The component registry.
		 * @param identifier The entity identifier.
		 * @return The entity pose, or a default pose when absent.
		 */
	}	 // namespace

	Slider::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
							 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Slider>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Slider> Slider::Builder::buildEntity(void)
	{
		_config.setFixedWidth(_width);
		_config.setFixedHeight(ThumbSize);

		auto entity = std::make_shared<Slider>(this->getComponentRegistry(),
											   _config, _variant, _min, _max,
											   _value, _step, _onChanged);
		return entity;
	}

	void Slider::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Continuous;
		_min	 = 0.0f;
		_max	 = 100.0f;
		_value	 = 0.0f;
		_step	 = 0.0f;
		_width	 = 200.0f;

		_config.borderRadius = TrackRadius;
		_config.axis		 = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 0.0f;
		_config.spacing = 0.0f;
		_config.setFixedWidth(_width);
		_config.setFixedHeight(ThumbSize);
	}

	Slider::Builder &
		Slider::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Slider::Builder &Slider::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Slider::Builder &Slider::Builder::withRange(float min, float max)
	{
		_min = min;
		_max = max;
		return *this;
	}

	Slider::Builder &Slider::Builder::withValue(float value)
	{
		_value = value;
		return *this;
	}

	Slider::Builder &Slider::Builder::withStep(float step)
	{
		_step = step;
		return *this;
	}

	Slider::Builder &Slider::Builder::withWidth(float width)
	{
		_width = width;
		return *this;
	}

	Slider::Builder &Slider::Builder::withOnChanged(
		const std::function<void(float)> &onChanged)
	{
		_onChanged = onChanged;
		return *this;
	}

	std::shared_ptr<Slider> Slider::Director::makeSlider(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		float min, float max, float value)
	{
		return builder.withVariant(variant)
			.withRange(min, max)
			.withValue(value)
			.registerEntity(parent);
	}

	Slider::Slider(ecs::ComponentRegistry &registry,
				   const SurfaceConfig &config, Variant variant, float min,
				   float max, float value, float step,
				   const std::function<void(float)> &onChanged)
		: SurfaceBase<components::Value, components::Range,
					  components::DragInteraction,
					  components::MouseButtonInteraction,
					  components::HandButtonInteraction>(registry, config)
		, _variant(variant)
		, _min(min)
		, _max(max)
		, _step(step)
		, _trackLength(config.hasFixedWidth ? config.fixedWidth : 200.0f)
		, _onChanged(onChanged)
		, _track()
		, _fill()
		, _lowThumb()
		, _highThumb()
		, _built(false)
	{
		auto &valueComponent =
			getComponentRegistry().getComponent<components::Value>(
				getIdentifier());
		valueComponent.setMin(min).setMax(max).setStep(
			_variant == Variant::Discrete && step <= 0.0f ? (max - min) * 0.1f
														  : step);
		valueComponent.setValue(value);

		auto &range = getComponentRegistry().getComponent<components::Range>(
			getIdentifier());
		range.setMin(min).setMax(max).setStep(valueComponent.getStep());
		range.setLow(min).setHigh(max);

		// Handlers are installed last so that construction does not invoke the
		// user callback before the entity is fully configured.
		valueComponent.setOnChangedHandler([this](float changed) {
			applyState();
			if (_onChanged) {
				_onChanged(changed);
			}
		});
		range.setOnChangedHandler([this](float, float) {
			applyState();
			if (_onChanged) {
				_onChanged(getValue());
			}
		});
	}

	Slider::~Slider(void)
	{
	}

	float Slider::normalize(float value) const
	{
		const float range = _max - _min;
		if (range <= 0.0f) {
			return 0.0f;
		}
		return std::clamp((value - _min) / range, 0.0f, 1.0f);
	}

	void Slider::placeThumb(const std::shared_ptr<ecs::Entity> &thumb,
							float normalized)
	{
		if (thumb == nullptr) {
			return;
		}

		const float x = normalized * _trackLength - ThumbSize * 0.5f;
		const float y = (TrackHeight - ThumbSize) * 0.5f;

		placeChild(getComponentRegistry(), getIdentifier(),
				   thumb->getIdentifier(),
				   utility::graphic::PositionF(x, y, 0.0f));
	}

	void Slider::buildChildren(void)
	{
		if (_built) {
			return;
		}

		// Remove any children left over from a previous variant.
		this->accessDirectEntities().clear();

		auto config			  = getSurfaceConfig();
		config.padding		  = 0.0f;
		config.spacing		  = 0.0f;
		config.borderRadius	  = TrackRadius;
		config.hasFixedWidth  = true;
		config.fixedWidth	  = _trackLength;
		config.hasFixedHeight = true;
		config.fixedHeight	  = ThumbSize;

		setSurfaceConfig(config);
		setColor(transparentColor());

		auto track = std::make_shared<SliderTrack>(getComponentRegistry());
		track->setParent(shared_from_this());
		this->addEntity(track);
		_track = track;

		getComponentRegistry()
			.getComponent<components::Borders>(_track->getIdentifier())
			.setBorderRadius(TrackRadius);
		getComponentRegistry()
			.getComponent<components::Color>(_track->getIdentifier())
			.setColor(schemeColor(SchemeColorRole::SurfaceVariant));

		auto fill = std::make_shared<SliderFill>(getComponentRegistry());
		fill->setParent(shared_from_this());
		this->addEntity(fill);
		_fill = fill;

		getComponentRegistry()
			.getComponent<components::Borders>(_fill->getIdentifier())
			.setBorderRadius(TrackRadius);
		getComponentRegistry()
			.getComponent<components::Color>(_fill->getIdentifier())
			.setColor(schemeColor(SchemeColorRole::Primary));

		auto lowThumb = std::make_shared<SliderThumb>(getComponentRegistry());
		lowThumb->setParent(shared_from_this());
		this->addEntity(lowThumb);
		_lowThumb = lowThumb;

		getComponentRegistry()
			.getComponent<components::Color>(_lowThumb->getIdentifier())
			.setColor(schemeColor(SchemeColorRole::Primary));

		if (_variant == Variant::Range) {
			auto highThumb =
				std::make_shared<SliderThumb>(getComponentRegistry());
			highThumb->setParent(shared_from_this());
			this->addEntity(highThumb);
			_highThumb = highThumb;

			getComponentRegistry()
				.getComponent<components::Color>(_highThumb->getIdentifier())
				.setColor(schemeColor(SchemeColorRole::Primary));
		}

		// Children are positioned manually; the surface layout is unused.
		setChildren({});

		_built = true;
	}

	void Slider::applyState(void)
	{
		if (!_built || _track == nullptr || _fill == nullptr
			|| _lowThumb == nullptr) {
			return;
		}

		const float trackOffsetY = (ThumbSize - TrackHeight) * 0.5f;

		placeChild(getComponentRegistry(), getIdentifier(),
				   _track->getIdentifier(),
				   utility::graphic::PositionF(0.0f, trackOffsetY, 0.0f));

		getComponentRegistry()
			.getComponent<components::Bound>(_track->getIdentifier())
			.setWidth(_trackLength)
			.setHeight(TrackHeight);

		const float normalized =
			getComponentRegistry()
				.getComponent<components::Value>(getIdentifier())
				.getNormalizedValue();

		if (_variant == Variant::Range) {
			auto &range =
				getComponentRegistry().getComponent<components::Range>(
					getIdentifier());
			const float lowNorm	 = normalize(range.getLow());
			const float highNorm = normalize(range.getHigh());

			placeChild(getComponentRegistry(), getIdentifier(),
					   _fill->getIdentifier(),
					   utility::graphic::PositionF(lowNorm * _trackLength,
												   trackOffsetY, 0.0f));

			getComponentRegistry()
				.getComponent<components::Bound>(_fill->getIdentifier())
				.setWidth((highNorm - lowNorm) * _trackLength)
				.setHeight(TrackHeight);

			placeThumb(_lowThumb, lowNorm);
			placeThumb(_highThumb, highNorm);
		} else {
			placeChild(getComponentRegistry(), getIdentifier(),
					   _fill->getIdentifier(),
					   utility::graphic::PositionF(0.0f, trackOffsetY, 0.0f));

			getComponentRegistry()
				.getComponent<components::Bound>(_fill->getIdentifier())
				.setWidth(normalized * _trackLength)
				.setHeight(TrackHeight);

			placeThumb(_lowThumb, normalized);
		}
	}

	Slider &Slider::setVariant(Variant variant)
	{
		if (_variant == variant) {
			return *this;
		}

		_variant = variant;
		_built	 = false;

		buildChildren();
		applyState();

		return *this;
	}

	Slider::Variant Slider::getVariant(void) const
	{
		return _variant;
	}

	Slider &Slider::setValue(float value)
	{
		getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.setValue(value);
		return *this;
	}

	float Slider::getValue(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.getValue();
	}

	Slider &Slider::setRange(float min, float max)
	{
		_min = min;
		_max = max;

		getComponentRegistry()
			.getComponent<components::Value>(getIdentifier())
			.setMin(min)
			.setMax(max);
		getComponentRegistry()
			.getComponent<components::Range>(getIdentifier())
			.setMin(min)
			.setMax(max);

		applyState();

		return *this;
	}

	float Slider::getLow(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Range>(getIdentifier())
			.getLow();
	}

	float Slider::getHigh(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Range>(getIdentifier())
			.getHigh();
	}

	Slider &
		Slider::setOnChangedHandler(const std::function<void(float)> &onChanged)
	{
		_onChanged = onChanged;
		return *this;
	}

	void Slider::initialize(void)
	{
		SurfaceBase::initialize();
		buildChildren();

		auto &value = getComponentRegistry().getComponent<components::Value>(
			getIdentifier());
		value.setMin(_min).setMax(_max);

		if (_variant == Variant::Discrete && _step <= 0.0f) {
			value.setStep((_max - _min) * 0.1f);
		} else {
			value.setStep(_step);
		}

		auto &range = getComponentRegistry().getComponent<components::Range>(
			getIdentifier());
		range.setMin(_min).setMax(_max).setStep(value.getStep());

		getComponentRegistry()
			.getComponent<components::DragInteraction>(getIdentifier())
			.setOnDragHandler([this](const utility::math::Vector2F &delta) {
				if (_variant == Variant::Range) {
					auto &range =
						getComponentRegistry().getComponent<components::Range>(
							getIdentifier());
					const float stepValue = (_max - _min) > 0.0f
						? delta.x / _trackLength * (_max - _min)
						: 0.0f;
					range.setHigh(range.getHigh() + stepValue);
					if (range.getHigh() <= range.getLow()) {
						range.setLow(range.getHigh() - (_max - _min) * 0.01f);
					}
					return;
				}

				auto &value =
					getComponentRegistry().getComponent<components::Value>(
						getIdentifier());
				const float fraction =
					(_max - _min) > 0.0f ? delta.x / _trackLength : 0.0f;
				value.setNormalizedValue(value.getNormalizedValue() + fraction);
			});

		applyState();
	}

	void Slider::update(void)
	{
		SurfaceBase::update();
		buildChildren();
		applyState();
	}

}	 // namespace guillaume::entities
